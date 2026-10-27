// NativeData.cpp - game-data backend for the native MSVC build only (see CMakeLists.txt).
//
// The web build serves game files through virtual paths provided by JavaScript (web/cos_data.js):
// /game/<canon>, /local/<canon>, /manifest.txt. Natively the same files live on disk:
//   /game/X        -> <root>\<lang>\X, falling back to <root>\en\X
//   /local/X       -> <exe dir>\X  (as in the original game; Gopen writes, FindFirstFile("*.sav")
//                     and other plain-fopen code paths also use the exe folder)
//   /manifest.txt  -> <root>\<lang>\manifest.txt (fallback en)
// The data root comes from cossacks_native.ini ([Data] DataDir), else <exe dir>\data, else a folder
// picker. It is resolved lazily on first use because files are opened from static initialisers,
// before WinMain runs.
//
// Mods: cossacks_native.ini [Mods] Active=name1,name2 activates the folders <exe dir>\mods\<name>\.
// A mod folder holds loose data files at the same paths as in data\en (no gsc packing). /game/X is
// looked up in the active mods first (in the listed order, so name1 wins over name2), then in data.

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shobjidl.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <algorithm>
#include "NativeData.h"

#pragma comment(lib, "ole32.lib")

static char g_exeDir[MAX_PATH];
static char g_ini[MAX_PATH];
static char g_root[MAX_PATH];
static char g_lang[32];
static bool g_ready = false;

static std::vector<std::string> g_mods;      // active mod folders, highest priority first
static std::vector<std::string> g_modFiles;  // every file in the active mods, canonical (UPPER, '/')
static unsigned int g_modsHash = 0;          // 0 = no active mod

static bool file_exists(const char* p)
{
    DWORD a = GetFileAttributesA(p);
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
}

static bool dir_exists(const char* p)
{
    DWORD a = GetFileAttributesA(p);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
}

static bool root_valid(const char* root)
{
    char p[MAX_PATH * 2];
    if (!root[0]) return false;
    snprintf(p, sizeof p, "%s\\en\\manifest.txt", root);
    return file_exists(p);
}

static bool pick_folder(char* out, int n)
{
    bool ok = false;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    bool uninit = SUCCEEDED(hr);
    IFileOpenDialog* dlg = NULL;
    if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&dlg)))) {
        DWORD opts = 0;
        dlg->GetOptions(&opts);
        dlg->SetOptions(opts | FOS_PICKFOLDERS | FOS_FORCEFILESYSTEM);
        dlg->SetTitle(L"Select the Cossacks data folder (the one containing en\\)");
        if (SUCCEEDED(dlg->Show(NULL))) {
            IShellItem* it = NULL;
            if (SUCCEEDED(dlg->GetResult(&it))) {
                PWSTR w = NULL;
                if (SUCCEEDED(it->GetDisplayName(SIGDN_FILESYSPATH, &w))) {
                    ok = WideCharToMultiByte(CP_ACP, 0, w, -1, out, n, NULL, NULL) > 0;
                    CoTaskMemFree(w);
                }
                it->Release();
            }
        }
        dlg->Release();
    }
    if (uninit) CoUninitialize();
    return ok;
}

// FNV-1a, continued from h
static unsigned int fnv1a(unsigned int h, const void* data, size_t n)
{
    const unsigned char* p = (const unsigned char*)data;
    for (size_t i = 0; i < n; i++) { h ^= p[i]; h *= 16777619u; }
    return h;
}

// collects the files below dir as paths relative to the mod root (prefix), canonical form
static void list_mod_files(const std::string& dir, const std::string& prefix, std::vector<std::string>& out)
{
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA((dir + "\\*").c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        if (!strcmp(fd.cFileName, ".") || !strcmp(fd.cFileName, "..")) continue;
        std::string rel = prefix + fd.cFileName;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            list_mod_files(dir + "\\" + fd.cFileName, rel + "/", out);
        } else {
            for (char& c : rel) c = (char)toupper((unsigned char)c);
            out.push_back(rel);
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
}

// reads [Mods] Active, lists the mod files and hashes their paths and contents
static void mods_resolve()
{
    char active[1024];
    GetPrivateProfileStringA("Mods", "Active", "", active, sizeof active, g_ini);
    unsigned int h = 2166136261u;
    for (char* name = strtok(active, ",;"); name; name = strtok(NULL, ",;")) {
        while (*name == ' ' || *name == '\t') name++;
        size_t l = strlen(name);
        while (l && (name[l - 1] == ' ' || name[l - 1] == '\t')) name[--l] = 0;
        if (!*name) continue;
        std::string dir = std::string(g_exeDir) + "\\mods\\" + name;
        if (!dir_exists(dir.c_str())) {
            std::string msg = "Mod folder not found, the mod is skipped:\n" + dir;
            MessageBoxA(NULL, msg.c_str(), "Cossacks", MB_OK | MB_ICONWARNING);
            continue;
        }
        g_mods.push_back(dir);
        std::vector<std::string> files;
        list_mod_files(dir, "", files);
        std::sort(files.begin(), files.end());
        h = fnv1a(h, name, strlen(name) + 1);
        for (const std::string& f : files) {
            h = fnv1a(h, f.c_str(), f.size() + 1);
            FILE* fp = fopen((dir + "\\" + f).c_str(), "rb");
            if (fp) {
                unsigned char buf[65536];
                size_t n;
                while ((n = fread(buf, 1, sizeof buf, fp)) > 0) h = fnv1a(h, buf, n);
                fclose(fp);
            }
            g_modFiles.push_back(f);
        }
    }
    if (!g_mods.empty()) g_modsHash = h ? h : 1;
}

static void data_resolve()
{
    if (g_ready) return;
    g_ready = true;
    char* slash;
    GetModuleFileNameA(NULL, g_exeDir, MAX_PATH);
    slash = strrchr(g_exeDir, '\\');
    if (slash) *slash = 0;
    snprintf(g_ini, sizeof g_ini, "%s\\cossacks_native.ini", g_exeDir);

    GetPrivateProfileStringA("Data", "DataDir", "", g_root, sizeof g_root, g_ini);
    if (!root_valid(g_root)) {
        snprintf(g_root, sizeof g_root, "%s\\data", g_exeDir);
        if (!root_valid(g_root)) {
            for (;;) {
                if (!pick_folder(g_root, sizeof g_root)) {
                    MessageBoxA(NULL, "No data folder selected. Cossacks cannot start.", "Cossacks", MB_OK | MB_ICONERROR);
                    ExitProcess(1);
                }
                if (root_valid(g_root)) break;
                MessageBoxA(NULL, "That folder does not contain en\\manifest.txt.\nPick the data folder that contains the en\\ subfolder.",
                            "Cossacks", MB_OK | MB_ICONWARNING);
            }
            WritePrivateProfileStringA("Data", "DataDir", g_root, g_ini);
        }
    }
    size_t l = strlen(g_root);
    while (l > 3 && (g_root[l - 1] == '\\' || g_root[l - 1] == '/')) g_root[--l] = 0;

    GetPrivateProfileStringA("Data", "Language", "en", g_lang, sizeof g_lang, g_ini);
    char p[MAX_PATH * 2];
    snprintf(p, sizeof p, "%s\\%s", g_root, g_lang);
    if (!g_lang[0] || !dir_exists(p)) strcpy(g_lang, "en");

    mods_resolve();
}

// real path of a game-data file: active mods first, then the selected language, then en
static bool game_path(const char* rel, char* out, int n)
{
    for (const std::string& mod : g_mods) {
        snprintf(out, n, "%s\\%s", mod.c_str(), rel);
        if (file_exists(out)) return true;
    }
    snprintf(out, n, "%s\\%s\\%s", g_root, g_lang, rel);
    if (file_exists(out)) return true;
    snprintf(out, n, "%s\\en\\%s", g_root, rel);
    return file_exists(out);
}

extern "C" void cos_data_init(void) { data_resolve(); }

extern "C" int cos_fetch_file(const char* canon)
{
    char p[MAX_PATH * 2];
    data_resolve();
    return game_path(canon, p, sizeof p) ? 0 : -1;
}

extern "C" void cos_persist_soon(void) {}

extern "C" int cos_ini_int(const char* section, const char* key, int def)
{
    data_resolve();
    return (int)GetPrivateProfileIntA(section, key, def, g_ini);
}

extern "C" unsigned int cos_mods_hash(void)
{
    data_resolve();
    return g_modsHash;
}

extern "C" int cos_mod_file_count(void)
{
    data_resolve();
    return (int)g_modFiles.size();
}

extern "C" const char* cos_mod_file(int i)
{
    return (i >= 0 && i < (int)g_modFiles.size()) ? g_modFiles[i].c_str() : NULL;
}

extern "C" const char* cos_native_path(const char* vpath, char* out, int n)
{
    if (!vpath) return vpath;
    if (strncmp(vpath, "/game/", 6) == 0) {
        data_resolve();
        if (!game_path(vpath + 6, out, n))   // absent: still return the preferred location
            snprintf(out, n, "%s\\%s\\%s", g_root, g_lang, vpath + 6);
        return out;
    }
    if (strncmp(vpath, "/local/", 7) == 0) {
        data_resolve();
        snprintf(out, n, "%s\\%s", g_exeDir, vpath + 7);
        return out;
    }
    if (strcmp(vpath, "/manifest.txt") == 0) {
        data_resolve();
        game_path("manifest.txt", out, n);
        return out;
    }
    return vpath;
}
