// AILog.h - opt-in AI activity log (CMake option COS_AI_LOG, MSVC builds only).
// Writes ai_logs\ai_<YYYYMMDD-HHMMSS>.log next to the exe: what each computer player built,
// researched and traded, plus a per-minute economy snapshot of every player (human included).
#pragma once

#if defined(_MSC_VER) && defined(COS_AI_LOG)
#define AILOG_ENABLED 1

// One event line for nation NI. Ignored unless NI is a computer player (AI_Enabled).
void AILog_Event(int NI, const char* fmt, ...);

// Called every game frame; writes the snapshot of all players once per minute of game time.
void AILog_Tick();

// Resource names by engine resource id (as used by XRESRC / ResTotal).
const char* AILog_ResName(int ResID);
#endif
