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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shobjidl.h>
#include <stdio.h>
#include <string.h>
#include "NativeData.h"

#pragma comment(lib, "ole32.lib")

static char g_exeDir[MAX_PATH];
static char g_ini[MAX_PATH];
static char g_root[MAX_PATH];
static char g_lang[32];
static bool g_ready = false;

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
}

// real path of a game-data file: selected language first, then en
static bool game_path(const char* rel, char* out, int n)
{
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
