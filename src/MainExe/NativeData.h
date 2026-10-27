// NativeData.h - virtual-path to Windows-path mapping for the native MSVC build (see NativeData.cpp).
#ifndef NATIVEDATA_H
#define NATIVEDATA_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
/* Maps "/game/X", "/local/X" and "/manifest.txt" to real paths in out[n]; any other path is returned unchanged. */
const char* cos_native_path(const char* vpath, char* out, int n);

/* Integer setting from cossacks_native.ini, def when missing. */
int cos_ini_int(const char* section, const char* key, int def);

/* Active mods ([Mods] Active in cossacks_native.ini): hash of their names, file paths and contents, 0 = none. */
unsigned int cos_mods_hash(void);
/* Files in the active mods, canonical (UPPER, '/'), for enumeration next to manifest.txt. */
int cos_mod_file_count(void);
const char* cos_mod_file(int i);
#endif

#ifdef __cplusplus
}
#endif

#ifndef _MSC_VER
/* The web build has no mods folder. */
static inline unsigned int cos_mods_hash(void) { return 0; }
#endif
#endif
