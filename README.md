# Cossacks: Back to War 1.52 — Web Edition

The 2002 real-time-strategy game running in the browser via WebAssembly, with native desktop clients
for Windows, macOS and Linux and a self-contained online-multiplayer server. It preserves the full
stability of classic 1.52 — battles of up to 40,000 units — and adds single-player cheat codes.

> **About this fork.** This fork adds a native 32-bit Windows executable built with MSVC, so the
> game runs as real x86 code with all the 1.52 fixes, without the WebAssembly/Electron layer. The
> work is in progress; see [Fork: native Windows build](#fork-native-windows-build) for the status
> and a log of every change made to the upstream code.

---

## Packages

| Package | Contents | Online multiplayer |
|---|---|---|
| Desktop client — Windows / macOS / Linux | The full game as a native app | Yes, against a server |
| Web archive (`Cossacks152-Web-SinglePlayer.zip`) | The browser game as static files, for any web host | No |
| Docker image (`cossacks152-image.tar.gz`) | The browser game **and** the multiplayer server | Yes |

Static web hosting serves files only and therefore runs single-player. Online multiplayer (rooms,
lobby, chat, NAT traversal) requires the server, which is included in the Docker image — deploy that
on any host with Docker to run the game and the online stack together.

**Downloads:**

| | Link |
|---|---|
| Windows | [`<<< download link >>>`](https://ia800504.us.archive.org/25/items/cossacks-back-to-war-v-1.52/Cossacks152-Windows.zip) |
| macOS Intel| [`<<< download link >>>`](https://ia800504.us.archive.org/25/items/cossacks-back-to-war-v-1.52/Cossacks152-macOS.zip) |
| macOS Apple Silicon| [`<<< download link >>>`](https://ia800504.us.archive.org/25/items/cossacks-back-to-war-v-1.52/Cossacks152-macOS-arm64.zip) |
| Linux | [`<<< download link >>> `](https://ia800504.us.archive.org/25/items/cossacks-back-to-war-v-1.52/Cossacks152-Linux.tar.gz) |
| Web archive (single-player) | [`<<< download link >>>`](https://ia800504.us.archive.org/25/items/cossacks-back-to-war-v-1.52/Cossacks152-Web-SinglePlayer.zip) |
| Docker image (server + web) | [`<<< download link >>>`](https://ia800504.us.archive.org/25/items/cossacks-back-to-war-v-1.52/cossacks152-image.tar.gz) |

Source code, build recipes and server code are in this repository.

## Desktop clients

Unpack and run. Single-player, LAN discovery and direct-IP games require no server or configuration.

| Platform | Run |
|---|---|
| Windows | `Cossacks152-Windows.zip` → `Cossacks152.exe` |
| Linux | `Cossacks152-Linux.tar.gz` → `./cossacks152` |
| macOS | `Cossacks152-macOS.zip` → `Cossacks152.app` (unsigned; first launch: right-click → Open) |

For internet rooms on a server, configure `relay.json` — see [Connecting a desktop client](#connecting-a-desktop-client-to-a-server).

## Browser

If a server is running, open `http://<server>:8790/`. Nothing to install.

The web build caches aggressively: the `data/` asset tree (~1.5 GB) is served with a one-year
`immutable` cache and is fetched only on first load; only the engine core (`dmcr.wasm`, ~63 MB) is
revalidated so fixes are always picked up. Repeat visits do not re-download the game.

## Cheat codes (single-player)

Entered in the in-game chat:

| Code | Effect |
|---|---|
| `king` | God mode; millions of resources; unit cap raised to 15,000; artillery build limits and rising unit/building costs removed. |
| `house` | AI players build many houses. Enter again to stop. |
| `nocost` | Disables the rising cost of units and buildings. |
| `peace` | Toggles a truce at any point in the match. |
| `ai` | Hands your nation to the AI. Enter again to reclaim it. |

## Deploying the server (Docker)

```sh
docker load -i cossacks152-image.tar.gz
docker run -d --name cossacks152 \
  -p 8790:8790 -p 8792:8792/udp \
  -v "$PWD/config.json:/config/config.json:ro" \
  cossacks152:latest
```

The container serves the game and the full online stack (lobby, rooms, chat, NAT rendezvous). Players
open `http://<server>:8790/`.

Configuration is a single file, `config.json`:

```json
{ "host": "203.0.113.10" }
```

`host` is the public IP or domain the server is reached at; behind NAT, use the external IP. A public
IPv4 enables NAT hole-punching for direct peer-to-peer between desktop clients; a domain or empty value
routes them through the relay. Behind a TLS reverse proxy, forward the domain to 8790 with WebSocket
upgrade enabled and use `wss://` in `relay.json`.

| Port | Proto | Purpose |
|---|---|---|
| 8790 | tcp | Game (HTTP) and WebSocket (relay, chat, lobby) |
| 8792 | udp | NAT hole-punch rendezvous (desktop peer-to-peer) |

### Connecting a desktop client to a server

Each desktop build ships `relay.json.example`, inert until configured. For internet play, rename it to
`relay.json` and set the server address in all three entries:

```json
{
  "relay": "ws://your-server:8790/cossacks",
  "gsc":   "ws://your-server:8790/cossacks-gsc",
  "ircd":  "ws://your-server:8790/cossacks-irc"
}
```

It belongs next to the executable — `Cossacks152.exe` (Windows), `cossacks152` (Linux), or
`Cossacks152.app/Contents/MacOS/` (macOS). LAN and direct-IP play do not use it. Behind a TLS proxy,
use `wss://`.

## Web-only deployment (shared hosting)

Upload the contents of `Cossacks152-Web-SinglePlayer.zip` to any web host and open `index.html`. The
host only needs to serve files over HTTP/HTTPS (with `.wasm` as `application/wasm`). This is
single-player; multiplayer requires the server and is not available on static hosting.

## Deploying without Docker

Two processes on a Linux host — the Perl lobby and the Node master:

```sh
cpanm EV Coro AnyEvent AnyEvent::HTTP Coro::LWP Mouse JSON LWP Config::Simple \
      Getopt::Compact Getopt::Long::Descriptive String::Escape Template Template::Plugin::POSIX URI
cd Prod/server && npm install --omit=dev && cd ..
HOST=<public-ipv4> ./Prod/nodocker/start.sh
```

# Fork: native Windows build

## Goal

Upstream ships the Windows desktop client as Electron wrapping the WebAssembly build, which is slow:
mission DLLs run in an x86 interpreter and everything passes through the browser layer. The engine
source still contains the original Windows code paths (SDL2 + Win32) next to the WebAssembly ones, so
this fork compiles that source natively with MSVC into a 32-bit `.exe`. It has to be 32-bit because the
exe loads the original x86 AI and mission DLLs directly.

## Building (work in progress)

Requirements: Visual Studio 2022 with the C++ desktop workload (provides MSVC and CMake).

```sh
cd src
cmake -S . -B out/build/win32-release -G "Visual Studio 17 2022" -A Win32
cmake --build out/build/win32-release --config Release
```

The `.sln` files are not used: they target the VS 2015/2019 toolsets and do not include `platform/`.

**Status:** `dmcr.exe` builds and links. It has not been tested in play yet.

## Running

The build puts the exe and its runtime files in `src/out/build/win32-release/MainExe/Release/`:

| File(s) | Source | Copied |
|---|---|---|
| `dmcr.exe` | the build | — |
| `SDL2.dll`, `SDL2_mixer.dll`, `libogg-0.dll`, `libopus*.dll`, ... | `src/MainExe/sdl`, `src/MainExe/SDL2_mixer` | every build |
| `ai\*.dll` (nation AI) | `ai/` | every build |
| `cew.dll`, `agew_1.pal`, `*.opt`, `mode.dat`, `rec.dat`, `MuliExOpt.dat` | `data_min/` | only when missing (the game rewrites the settings files) |

The one thing to add by hand is the **game data**, which is not in this repository: the `data` folder
(containing `en\`, `ru\`, ...) from the full source archive linked under
[Building from Source](#building-from-source). Either copy it next to the exe:

```
Release\
├── dmcr.exe
├── SDL2.dll, SDL2_mixer.dll, ...
├── ai\
├── cew.dll, *.opt, *.dat, agew_1.pal
└── data\
    ├── en\        (contains manifest.txt)
    └── ru\ ...
```

or keep it anywhere and pick it in the folder dialog on first run. Then start `dmcr.exe`. The whole
`Release` folder can be copied elsewhere as a portable install, as long as it stays writable.

**Logs and crashes.** The exe writes these next to itself:

| File | Contents |
|---|---|
| `dmcr_early.log` | Written at startup, before any game code runs; confirms the crash handlers are installed |
| `dmcr_debug.log` | Startup steps (window, sound, video init, menus) |
| `dmcr_stderr.log` | Runtime messages (sound loading, AI, clicks, ...) |
| `dmcr_crash.log` | Written on a crash: exception, registers, and a stack trace with module offsets |

`dmcr.pdb` (debug symbols) is built next to the exe. To see a crash at source level, open `dmcr.exe`
in Visual Studio (File → Open → Project/Solution, select the exe), start it with F5, and reproduce
the crash: the debugger stops at the faulting line with the call stack.

**Game data lookup.** The exe reads the unpacked `data/<lang>/` tree of the web build. On first use it
looks for:

1. `[Data] DataDir` in `cossacks_native.ini` next to the exe;
2. `data\` next to the exe;
3. otherwise a folder picker asks for the data folder (the one containing `en\`) and saves the choice
   to `cossacks_native.ini`.

A folder is accepted when it contains `en\manifest.txt`. The language is `[Data] Language` in the same
file (default `en`); files missing from that language fall back to `en`, as in the web build.

**User files.** As in the original game, saves (`*.sav`, including `AUTO.SAV`), settings (`mode2.dat`,
`rec.dat`, `*.opt`), the player profile (`players.txt`) and generated random maps (`*.m3d`) are kept
next to the exe, so the exe folder must be writable. Files there also take precedence over the data
folder.

## Nation AI sources

`src/AI/` holds the source of the computer opponents: the community **AI Pack v0.995** (2002,
originally Visual C++ 6), covering 17 nations. There is no source for Algeria, Turkey and Ukraine.
The 20 shipped DLLs in `ai/` are byte-identical to the original Back to War ones, and every game
identifier in them appears in this source, so they were very likely built from it. Its readme is in
`src/AI/AI/AIPack Readme.txt`; `src/AI/variants/` keeps alternative Bavaria/Russia versions and a
newer `Extraction.h` for reference (not built).

The MSVC build compiles one DLL per nation (`src/AI/CMakeLists.txt`) into
`src/out/build/win32-release/MainExe/Release/ai_rebuilt/`. With the CMake option
`COS_USE_REBUILT_AI` (ON by default) the `AI_Deploy` step then copies them into `ai\` next to the
exe, replacing the shipped DLLs for those 17 nations; Algeria, Turkey and Ukraine keep the shipped
ones. To play against the shipped AI, configure with `-DCOS_USE_REBUILT_AI=OFF` (the shipped DLLs
come back on the exe's next relink). The AI finds engine functions at runtime by name
(`GetProcAddress(GetModuleHandle("DMCR.EXE"), ...)` in `Ai.h`), so the exe must keep the name
`dmcr.exe`.

The sources are Windows-1251 encoded; keep that encoding when editing them.

### AI changes

On land maps the AI runs `Extr00()` in `src/AI/Extraction.h` (the `LandScript…` functions in
`Scripts.h` are never called). Its `Buildings()` is a fixed build order where each `return` stops
everything below it until a condition is met; `TryUnit(type, max, costPercent, probability)` only
proposes a building, which the engine accepts if the AI can pay in full and the price is at most
`costPercent` of its current stockpile. A `TryUnit` without a following `return` is therefore
optional: the AI builds it when it can afford it and otherwise carries on with the plan.

| Change | Values (constants at the top of `Extraction.h`) |
|---|---|
| Town Centres 3–5 are optional right after the "2 Town Centres finished" gate (before: the 4th came late behind a gate, the 5th only after the 18th century and the Academy building upgrade) | up to `AI_OPTIONAL_CENTERS` = 5, at most `AI_OPTIONAL_CENTERS_COST_PERCENT` = 75% of the stockpile |
| Storehouses 2–4 are optional right after the same gate (before: 1 until 4 Town Centres were finished) | up to `AI_OPTIONAL_STOREHOUSES` = 4, at most `AI_OPTIONAL_STOREHOUSES_COST_PERCENT` = 50% of the stockpile |
| 18c barracks: no count limit beyond the original first 3 (before: max 3, later 6) | `AI_KAZ18_MAX` = 1000, at most `AI_KAZ18_COST_PERCENT` = 90% of the stockpile; each barracks costs twice the previous one, which limits them naturally |
| 18th century first: it is proposed at the top of `Buildings()`, ahead of every building and upgrade (before: at the end of `Upgrades()`), and the market starts saving food, gold and iron for it once the Academy's AKAWOOD upgrade runs (before: only after the later AKASTN2) | `AI_XVIII_COST_PERCENT` = 98% (unchanged) |

Houses are built by the engine, not the AI script (`City` logic in `src/MainExe/Megapolis.cpp`): a
computer player adds a house when its free population capacity drops below a buffer that grows with
population (7 places above 10 population … 60 above 80). The fork adds: above 500 population, keep
at least 80 places free. This applies to every computer player, including the three nations
without AI source.

**Debug chat.** Configure with `-DCOS_AI_DEBUG_CHAT=ON` (default OFF) to make the rebuilt AI announce
its building decisions in the in-game chat, in its player colour: `AI: building <name> | food .. wood .. stone .. gold .. iron .. coal .. | pop <current>/<cap>`
(its stockpile and population at that moment), and likewise `AI: researching <upgrade>` for upgrades, at most once
per building or upgrade type and computer player every 20 seconds. It also proves the rebuilt DLLs are the ones
in use: the shipped DLLs never post these messages. The DLL calls the engine export
`AIReportProject` whenever `TryUnit` accepts a building project or `TryUpgrade` accepts an upgrade; the message is display only and
does not affect the game.

**AI activity log.** Configure with `-DCOS_AI_LOG=ON` (default OFF, MSVC only) to write one file per
game, `ai_logs\ai_<YYYYMMDD-HHMMSS>.log` next to `dmcr.exe`, flushed after every line. Every line starts
with the game time `mm:ss`, the nation index and `AI` or `HUMAN`. Events (computer players only):
`BUILD` / `BUILD-FAIL` (building foundation placed / project abandoned), `UPGRADE` and `MINE-UPGRADE`
(started), `TRADE` (market exchange, with the amount received). Every 60 s of game time each player,
human included, gets a snapshot: stockpile (`RES`), gathered so far (`GATH`), population, houses,
peasants, military and 18th century state (`POP`), gold/iron/coal mines with count, miners inside /
capacity and upgrade levels (`MINE`), key buildings (`BLD`) and all finished buildings by name (`BLDN`),
so the AI economy can be compared with the human's.

```
00:00 P1 AI    BUILD Town Centre
03:12 P1 AI    BUILD Gold Mine
04:05 P1 AI    MINE-UPGRADE Mine upgrade 1
05:00 P1 AI    TRADE 500 wood -> 410 gold
05:00 P1 AI    SNAP nation=Poland name=Computer
05:00 P1 AI    MINE gold: mines=2 building=0 inside=11/20 levels=1,1,0,0,0,0,0,0 iron: mines=1 building=0 inside=4/10 levels=1,0,0,0,0,0,0,0 coal: mines=0 building=0 inside=0/0 levels=0,0,0,0,0,0,0,0
05:00 P0 HUMAN RES  food=900 wood=1200 stone=300 gold=2100 iron=800 coal=100
```

## Changes from upstream

| Change | Why |
|---|---|
| Added `.gitignore`; stopped tracking `.vs/`, `build/obj`, `build/x86obj`, `web/dmcr.*`, `node_modules`, `__pycache__`, `x86/dll/M_*`, `x86/dll/England`, `x86/dll/*/*.bin`, `*.pdb` | Keep only sources; all of these are build output, caches, or generated files no build uses. The AI objects in `build/x86obj` are compiled from `x86/dll/AI_*/recomp_src` by `x86/build_multitest.sh`; the `.bin` section dumps are re-extracted from `ai/*.dll` by `x86/cos_prep_dll.py`. |
| Fixed folder paths in `src/CMakeLists.txt`, `src/Cossacks.sln` and the IChat/IntExplorer `.vcxproj` files | They still used the old folder names (`Main executable`, `CommCore library`, ...). |
| `src/platform/` is no longer on the include path for MSVC | Its stand-in headers (`windows.h`, `crtdbg.h`, `Winsock2.h`, ...) forward with `#include_next`, which MSVC lacks, and they hid the real Windows SDK headers. The WebAssembly and GCC/Clang builds are unchanged. |
| `platform_types.h` and `MPlayerStub.cpp` include `winsock2.h` before `windows.h` | Otherwise `windows.h` pulls in the legacy `winsock.h`, which clashes with `winsock2.h`. |
| Renamed `MainExe/Process.h` to `ProcessInfo.h` | It hid the CRT `<process.h>` that the C++ `<thread>` header needs. |
| 32-bit builds target Windows 7 instead of XP | MSVC v143 cannot target XP, and the XP target hid `RtlCaptureStackBackTrace` used by the crash logger. |
| Small MSVC equivalents in `Ddex1.cpp` (early-init hook via `.CRT$XCT` instead of `__attribute__((constructor))`, kept with `/include` so the linker does not drop it), `Ddini.cpp` (`F_OK`), `GSCset.cpp` (`mkdir` → `_mkdir`) | GCC-only constructs. The early-init hook installs the crash handlers that write `dmcr_crash.log`. |
| MSVC Release builds also produce `dmcr.pdb` (`/Zi`, `/DEBUG /OPT:REF /OPT:ICF`) | Lets Visual Studio show crashes at source level without a separate debug build. |
| MSVC builds use `MPlayerStub.cpp` instead of `MPlayer.cpp`; `dplayx.lib` is no longer linked; `DirectPlayLobbyCreateA` stub declared `WINAPI` | `MPlayerStub.cpp` is the CommCore multiplayer with the 1.52 direct-IP rework used by every other build. `MPlayer.cpp` uses DirectPlay, a legacy Windows component players would have to enable by hand. |
| `MPlayerStub.cpp`: `DPL_CreatePlayer` stub returns `bool` | `Interface.cpp` declares it `bool`; MSVC includes the return type in the mangled name, so the `void` stub did not link. |
| New `MainExe/NativeStubs.cpp` (MSVC only): inert IChat lobby/chat functions, IntExplorer access keys, and the web relay `cosn_*` functions | The native build does not compile IChat/IntExplorer or the JavaScript relay. The internet lobby is therefore unavailable natively; single-player, LAN and direct-IP play are unaffected. |
| Windows builds no longer link `SDL2main`; MSVC builds a GUI-subsystem exe (`WIN32_EXECUTABLE`) | The game defines its own `WinMain` (`Ddex1.cpp`); `SDL2main` replaced it with a `main` that calls a nonexistent `SDL_main`. |
| New `MainExe/NativeData.cpp`/`.h` (MSVC only): native `cos_data_init`, `cos_fetch_file`, `cos_persist_soon` and `cos_native_path`; `Arc/GSCset.cpp` and `DeviceCD.cpp` route their `/game/...`, `/local/...` and `/manifest.txt` paths through `cos_native_path` under `_MSC_VER` | In the web build those virtual paths and functions come from JavaScript (`web/cos_data.js`). The native loader maps them to the data folder and the exe folder as described under **Game data lookup** and **User files** above. Web and GCC/Clang code paths are unchanged. |
| `ActiveScenary.cpp`: mission DLLs are loaded from the data folder (via `cos_native_path`) under `_MSC_VER` | The game passes data-relative names such as `Missions\...\x.dll`; natively `LoadLibrary` would look for them next to the exe. |
| `Cwave.cpp`: sound effects are opened from the exe folder, then the data folder (via `cos_native_path`) under `_MSC_VER` | The real Windows `mmioOpen` resolved `Sound\*.wav` against the exe folder only, so no effect loaded (music, loaded through `DeviceCD.cpp`, was unaffected). Non-Windows builds already did this inside the `mmioOpen` stand-in in `platform/mmsystem.h`. |
| `/local/...` (user files) maps to the exe folder instead of a `UserData\` subfolder | With `UserData\`, user files were split: `Gopen` (settings, `players.txt`) wrote to the exe folder but read from `UserData\`, and saves were written to `UserData\` but listed with `FindFirstFile("*.sav")` in the exe folder. So saves, options and the profile seemed lost on restart. The exe folder is also where the original game kept them. |
| MSVC post-build step in `src/MainExe/CMakeLists.txt` (+ `src/cmake/copy_missing.cmake`) copies the SDL DLLs, `ai/` and `data_min/` next to the exe | The game switches to the exe folder at startup and opens these by relative path. See [Running](#running). |
| Added the nation AI sources (`src/AI/`, AI Pack v0.995) and a CMake subproject building 17 AI DLLs into `ai_rebuilt/` (MSVC only; flags `/Zc:forScope-` for VC6 loop scoping, static CRT as originally, `/NODEFAULTLIB:..\dmcr.lib` for an obsolete pragma in `Ai.h`). No source edits. | Lets the AI be changed and rebuilt like the rest of the game. See [Nation AI sources](#nation-ai-sources). |
| `COS_USE_REBUILT_AI` CMake option (ON) with an `AI_Deploy` target copying the rebuilt AI DLLs into `ai\` next to the exe | Plays against the AI built from source; it runs after `dmcr`'s post-build step, which copies the shipped `ai/` folder. |
| `src/AI/Extraction.h`: optional Town Centres 3–5 (≤75% of stockpile), optional Storehouses 2–4 (≤50%), unlimited 18c barracks (≤90%), 18th century first and saved for earlier | The AI's economy stayed far behind a human's in long games; see [AI changes](#ai-changes). |
| `Megapolis.cpp`: computer players above 500 population keep at least 80 free population places (house building) | The original buffer stops at 60 free places, too little for large late-game populations. Applies to all builds and all computer players. |
| `COS_AI_DEBUG_CHAT` CMake option (OFF): `src/AI/Ai.h` reports accepted `TryUnit` building projects through the new engine export `AIReportProject` (`ActiveScenary.cpp`), bound optionally so DLLs still load with an exe without it | Shows what the AI decides to build and confirms the rebuilt AI is running. See **Debug chat** above. |
| `COS_AI_LOG` CMake option (OFF): `AILog.cpp` writes `ai_logsi_<time>.log` with the AI's builds, upgrades, trades and a per-minute economy snapshot of every player (hooks in `Megapolis.cpp`, `Upgrade.cpp`, `Economy.cpp`, `Ddex1.cpp`) | Answers why the AI gathers less gold and iron than the human. See **AI activity log** above. |
| Chat messages stay 20 s instead of 10 s (`kChatMessageDisplayTime`, `Hint.cpp`) | They disappeared too fast to read. |
| Chat messages start with the game time (hours:minutes:seconds), e.g. `[0:12:34] Player: text` (`CreateChatHint`, `Hint.cpp`; used in `Interface.cpp`, `mapa.cpp`, `ActiveScenary.cpp`). Replays show the time stored in the recording; replays recorded before this change have no time | Easier to follow what happened when. |
| The minimap "under attack" marker flashes 240 frames instead of 60 (`kAttackAlarmFrames`, `Nation.cpp`; used in `Nation.cpp`, `Mine.cpp`, `NewMon.cpp`, `ActiveScenary.cpp`, `mapa.cpp`) | Easier to notice. |

## Possible future work

| Idea | Notes |
|---|---|
| Read an original Cossacks: Back to War installation (`all.gsc` archive + loose files) | The native data loader targets the unpacked `data/<lang>/` tree of the web build. Supporting an original install may need the original GSC archive reader restored in `Arc/GSCset.cpp`. |
| Native internet lobby | Build IChat/IntExplorer natively (or reimplement their transport) so the exe can use the GSC lobby, chat and relay on the multiplayer server, replacing the stubs in `NativeStubs.cpp`. |

# Building from Source

**Important:** This repository contains **only the build code for the engine**. It is provided solely for reference.

If you want to build **exactly the same version** as the original (with the full set of game assets), you need to download a separate version of the repository/archive from this [link](https://ia800504.us.archive.org/25/items/cossacks-back-to-war-v-1.52/src-cossacks-1.52-platform.zip), which includes all game resources.

The repository is self-contained: a clean checkout builds the web game, Docker image, and desktop clients without files from the original game and without hardcoded paths (scripts self-detect via `emenv.sh`).

The only external requirement is the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html). Docker and Node.js are needed only for the image and desktop clients respectively.

```sh
# engine → WebAssembly (creates web/dmcr.*)
. emenv.sh
bash build/build_dmcr.sh

# Docker image (server + web)
bash Prod/docker/build.sh
docker save cossacks152:latest | gzip > cossacks152-image.tar.gz

# desktop clients (each OS builds its own target)
bash Prod/make_game.sh
cd Prod/electron && npm install && npm run dist
```

If `em++` is not in `PATH`, first source your `emsdk_env.sh` or set `EMSDK` before running `emenv.sh`.

AI runtime data is embedded in `ai/`. The `Prod/game/` directory is regenerated by `Prod/make_game.sh` from `web/`.

On Windows, the optional electron-builder signing step requires Developer Mode and may throw a symlink error `winCodeSign` at the end — the build is already ready in `dist/win-unpacked/` — just archive that folder.


## Architecture

**Multiplayer.** The lobby is the rloger Perl stack (`gscserver/`) speaking the game's native binary
protocol; a Node master bridges browser WebSocket connections to it, provides a WebSocket virtual-LAN
relay (each peer gets a virtual IP; unicast/broadcast datagrams are forwarded so LAN discovery works
over the internet), runs the NAT hole-punch rendezvous on UDP 8792 with automatic relay fallback, and
carries an IRC chat server on the same origin. Desktop clients use real UDP for LAN and direct-IP.
Server state is ephemeral — online peers, current rooms and chat live in memory and reset on restart;
match stats are a rolling last-1000 record. Nothing accumulates over time.

**Engine.** The original Cossacks C++ source compiled to WebAssembly with Emscripten (`em++`) — not an
emulated or recompiled x86 binary. The Win32/DirectX/MFC surface is replaced by shims: DirectDraw is
compiled out (`-DNODDRAW`), SDL2 supplies compile-time types only, `src/platform/*` provides Win32
stand-ins. Runtime is a native web layer — WebGL2, HTML5 input, Web Audio.

**x86 subsystem.** AI and mission logic are 240 original x86 PE DLLs loaded by a minimal in-WASM PE
loader. The 20 nation-AI DLLs are statically recompiled x86→C and linked into `dmcr.wasm`; the 220
mission DLLs are fetched on demand and run through a decode-on-demand x86-32 + x87 interpreter.

**Video.** One 8-bit palette-indexed offscreen frame, presented by a WebGL2 palette-lookup shader
(`R8` index texture + a 256×1 RGBA8 palette, resolved on the GPU). Fullscreen composes at the
monitor's native resolution (`screen.width × devicePixelRatio`).

**Audio.** DirectSound and MCI/CD replaced by Web Audio: per-channel
`AudioBufferSourceNode → StereoPanner → GainNode`, effects read raw PCM from the WASM heap, music
decodes off-thread with an LRU cache.

**Input.** HTML5 events feed a synthesized Win32 message ring buffer drained by the engine's own
`PeekMessage()`; coordinates are transformed through the letterbox; pointer lock engages in-game.

**Data & caching.** The original single ~240 MB archive is unpacked into per-language files under
`data/<lang>/`, served over HTTP and fetched one file at a time (a manifest backs lookup and directory
enumeration). Saves and options persist to IndexedDB via IDBFS. `data/` files are `immutable`-cached;
manifests and `dmcr.*` are `no-cache` so rebuilds are picked up.

## Project history

The C++ engine descends from the 2017 revamp of 1.42, carried forward as 1.52:

```
  1.42  · 2017 · ereb-thanatos ───────►  original C++ revamp (cossacks-revamp-2017)
                       │
                       ▼
  1.52  · 2025 · MR.CODERMAN ─────────►  C++ / SDL2 rebuild (cossacks-1.52)
                       │
                       ▼
  1.52  · "The Last Stand" · Ildar (JIEgOKOJI) ──►  first cross-platform port attempt (Windows + macOS),
                       │                             never fully finished but the groundwork this edition grew from
                       ▼
  1.52  · platform · (this project) ──►  WebAssembly engine, online multiplayer, desktop clients
```

| Stage | Author | Year | Repository |
|---|---|---|---|
| 1.42 revamp | ereb-thanatos | 2017 | [cossacks-revamp-2017](https://github.com/ereb-thanatos/cossacks-revamp-2017) |
| 1.52 | MR.CODERMAN | June 11, 2025 | [cossacks-1.52](https://github.com/MisterCoderman/cossacks-1.52) |
| 1.52 "The Last Stand" | Ildar (JIEgOKOJI) | Mar 14, 2026 | [cossacks-1.52](https://github.com/JIEgOKOJI/cossacks-1.52) |
| 1.52 platform | MR.CODERMAN | July 14, 2026 | WebAssembly + multiplayer + desktop |

## Acknowledgements

- **[ereb-thanatos](https://github.com/ereb-thanatos)** — author of the project and of the code base it grew from.
- **[Ildar](https://github.com/JIEgOKOJI)** — the first cross-platform build, *Cossacks: The Last Stand (v1.52)*, on which this WebAssembly edition is based.
- **[emilekm2142](https://github.com/emilekm2142)** — reworking of direct-IP multiplayer onto CommCore.
- **[rloger](https://github.com/rloger)** — the Cossacks game-server code the online multiplayer is built on: [SimpleCossacksServer](https://github.com/rloger/SimpleCossacksServer), [GSC-Server](https://github.com/rloger/GSC-Server), [GSC-Streamer](https://github.com/rloger/GSC-Streamer).
- **[Ivan Petkov](https://github.com/IvanPetkov555)** — mission and campaign development.
- **Slovnoslon** ([GitHub](https://github.com/slovnoslon) · [YouTube](https://www.youtube.com/channel/UC3PtKudP4TgewAMTWkI6dMw)) — finding and fixing bugs in the original game so 1.52 ships without them.

## Screenshots
<img width="1279" height="797" alt="screenshot" src="https://github.com/user-attachments/assets/5b967e7d-edb4-44dd-85e7-90eea64edcd4" />
<img width="1279" height="795" alt="screenshot" src="https://github.com/user-attachments/assets/095d3f16-87b2-4aaf-a5d7-5c0ede8e780c" />
<img width="1275" height="800" alt="screenshot" src="https://github.com/user-attachments/assets/360c70d6-180a-4c0a-a582-c583dc374fde" />
<img width="1847" height="902" alt="screenshot" src="https://github.com/user-attachments/assets/4a0d3772-38fc-46f6-829d-16afe9611599" />
<img width="1845" height="952" alt="screenshot" src="https://github.com/user-attachments/assets/a9e8f935-f568-495c-abcc-33a23a86eac8" />
<img width="1630" height="1289" alt="screenshot" src="https://github.com/user-attachments/assets/a6a5e8c8-b01f-4397-9479-3464be8b27f5" />






