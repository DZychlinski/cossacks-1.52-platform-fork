# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

Cossacks: Back to War 1.52 ported to WebAssembly. The original C++ engine (`src/`) is compiled with
Emscripten into `web/dmcr.{html,js,wasm,data}`, and the same web build is shipped three ways: as a static
site, inside a Docker image with the multiplayer server, and wrapped in Electron desktop clients.
`README.md` covers end-user deployment and has a fuller architecture summary.

This is a fork whose main goal is a native 32-bit Windows exe (MSVC/CMake) with all the 1.52 fixes;
the "Fork changes" section of `README.md` logs every divergence from upstream and must be updated with
each such change. Commits use the local identity `Cossacks152Fork` with no email and no co-author lines.
Code comments are a mix of English and Russian.

## Build commands

Run all build scripts from Git Bash. On Windows, `emenv.sh` falls back to `C:/emsdk` when `em++` is not on PATH.

```sh
. ./emenv.sh                     # sets COS_ROOT/COS_SRC/COS_DATA and puts emsdk on PATH
bash build/build_dmcr.sh         # full build: all engine TUs + x86 subsystem + link → web/dmcr.*
bash build/build_x86.sh          # x86 harness/interpreter objects only → build/x86obj
bash build/build_all.sh          # compile-only pass over engine TUs, prints OK/FAIL counts per file
bash build/rebuild_gsc_debug.sh  # fast path: recompiles a fixed list of TUs, then relinks
```

- Per-TU compiler output goes to `build/logs/<name>.log`; the link output goes to `build/logs/link.log`.
  `build_dmcr.sh` prints only `CC FAIL <file>`, so read the log to see the actual error.
- All compiler flags live in `build/flags.sh`. Key points: `-DNODDRAW`, `-include src/platform/prelude.h`,
  `-I src/platform` comes first so the Win32 shim headers (`windows.h`, `Winsock2.h`, …) shadow the real
  ones, `-fms-extensions`, and gnu++14. SDL2 headers are used at compile time only, and SDL is not linked.
- **Adding an engine source file:** the MainExe source list comes from the `set(MAIN_SOURCES …)` block in
  `src/MainExe/CMakeLists.txt`. `build_dmcr.sh` parses that block with sed. Web-only files are appended
  separately in `build_dmcr.sh` (`MPlayerStub SdlEventBridge WebInput WebRender WebSound SDLStubs WebNet`).
  The CommCore, IChat and IntExplorer lists are also hardcoded in `build_dmcr.sh`.
- IChat `.c` files are 1999–2001 GameSpy SDK code and must compile as C (`emcc`), not C++.
  `GSC_ChatWindow.cpp` and the IntExplorer files need the `-include cos_*_rename.h` symbol-rename shims
  to avoid collisions with `Interface.cpp`.
- The recompiled AI objects (`build/x86obj/AI_*_recomp_*.o`) are linked as-is: `build_dmcr.sh` does
  not build them. They are not tracked in git; compile them from `x86/dll/AI_*/recomp_src` with
  `x86/build_multitest.sh`, which hardcodes `/c/cossacks_wasm` paths.
- The native CMake/MSVC build (`src/CMakeLists.txt`, `src/Cossacks.sln`) comes from the earlier
  "Last Stand" port. The WASM build does not use it, except to read the source list.

## Running locally

```sh
python web/serve.py                       # static only, http://localhost:8791/dmcr.html (single-player, no-store caching)
cd server && npm install && node cos_master.js 8790 8792   # master: serves STATIC_DIR + WS relay/lobby
```

For full online play you also need the Perl GSC lobby on :34001; see `Prod/nodocker/start.sh` for how
the two processes are wired (`GSC_HOST`, `GSC_PORT`, `GSC_PROXY_KEY`, `PUBLIC_UDP_HOST`, `STATIC_DIR`).

## Packaging

```sh
bash Prod/make_game.sh                    # copy web/dmcr.* + index.html → Prod/game/ (data/ only on first run)
bash Prod/docker/build.sh                 # make_game + docker build → cossacks152:latest
cd Prod/electron && npm install && npm run dist
```

`Prod/game/` is build output. `Prod/electron/` is the desktop client that ships, with `lanlink.js` and
relay/internet support. The top-level `electron/` folder is an older offline-only wrapper.
`server/cos_master.js` and `Prod/server/cos_master.js` are duplicate copies, so keep them in sync.

## Tests

There is no test framework. `tools/*.cjs` are standalone Node smoke and unit scripts (`node tools/<name>.cjs`).
Most of them expect a running master on :8790 and the Perl lobby. Several hardcode `C:/cossacks_wasm/...`
paths (for example the path to `ws` in `server/node_modules`), so fix that path before running them.
`tools/cosnet_route_test.cjs` loads `web/cos_net.js` directly with a mocked HEAP and needs no server.

## Architecture

**Platform replacement layer.** The engine still believes it is a Win32/DirectX game. The browser
runtime is provided by:
- `src/platform/*`: Win32 API stand-ins (types, threads, winsock, CRT compat).
- `src/MainExe/Web*.cpp`, which use `web/cos_*.js` (linked with `--js-library`) for their JavaScript side:
  - `WebRender` uses WebGL2 to present an 8-bit indexed frame through a palette shader.
  - `WebInput` turns HTML5 events into a synthetic Win32 message queue that the engine drains with `PeekMessage`.
  - `WebSound` sends audio to `cos_audio.js`, which uses Web Audio.
  - `WebNet` provides a virtual UDP layer for CommCore through `cos_net.js`.
- `cos_data.js` loads game files lazily from `data/<lang>/` using **synchronous** XHR. This is
  deliberate: files are opened from C++ static initializers, where ASYNCIFY cannot help.
  Saves go to IDBFS.
- The link uses `-sASYNCIFY`, `INITIAL_MEMORY=1GB`, and `--preload-file ai@/ai`. The page template is
  `web/shell.html`.

**x86 subsystem (`x86/`).** AI and mission logic are original x86 PE DLLs. `cos_load.c` is a PE loader
and `cos_engine.cpp` provides `LoadLibrary`/`GetProcAddress` with trampolines into the engine.
`cos_gameapi_gen.cpp` contains the generated marshalers for the game API. The 20 nation-AI DLLs are
statically recompiled x86→C by the Python pipeline (`transpile.py`, `cos_translate.py`,
`cos_gen_*.py`; output in `x86/dll/AI_*/recomp_src`). Mission DLLs run in the decode-on-demand
interpreter (`interp.c`, `decode_x86.c`, `ops_cache.c`). If an instruction cannot be translated, the
pipeline emits a loud `x86_unimpl` stub. Fix that by adding the mnemonic to `transpile.py`.

**Networking.** The game has three independent transports, each with a browser JS library and a
WebSocket path on the master server (`server/cos_master.js`, one port: 8790):
- `cos_net.js` uses `/cossacks` for CommCore game data. This is a virtual LAN where each peer gets a
  `10.x.y.z` address and broadcast is forwarded, so LAN discovery works over the internet. In Electron,
  `window.cosElectron` provides real UDP instead, and NAT hole-punching uses rendezvous on UDP 8792
  with fallback to the relay. Binary frames on this socket carry game data; text frames carry JSON room
  control (`cos_master.js` on the client side).
- `cos_gsc.js` uses `/cossacks-gsc`, which leads to the native GSC lobby protocol (ParsedRQ::Compact
  payloads; the codec is in `src/IntExplorer/ParseRQ.cpp`). The master adds GSC-Stream framing
  (12-byte header + zlib) and forwards the data over TCP to the rloger Perl server (`gscserver/`, :34001).
- `cos_chat.js` uses `/cossacks-irc`, a virtual TCP byte stream for the GameSpy PeerChat SDK in `src/IChat`.

Desktop clients find these endpoints through `relay.json` (keys `relay`, `gsc`, `ircd`). The web build
connects to the same origin it was served from. All server state lives in memory.
