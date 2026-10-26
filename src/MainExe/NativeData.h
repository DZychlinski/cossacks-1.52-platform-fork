// NativeData.h - virtual-path to Windows-path mapping for the native MSVC build (see NativeData.cpp).
#ifndef NATIVEDATA_H
#define NATIVEDATA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Maps "/game/X", "/local/X" and "/manifest.txt" to real paths in out[n]; any other path is returned unchanged. */
const char* cos_native_path(const char* vpath, char* out, int n);

#ifdef __cplusplus
}
#endif
#endif
