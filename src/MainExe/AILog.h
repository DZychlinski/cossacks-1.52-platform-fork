// AILog.h - opt-in AI activity log (CMake option COS_AI_LOG, MSVC builds only).
// Writes ai_logs\ai_<YYYYMMDD-HHMMSS>.log next to the exe: what every player (human included) actually
// built, researched and traded, per-minute snapshots (economy, peasant jobs, units made/lost), and what
// the AI scripts wanted to do (proposals and why they were skipped). Only written in games with an AI.
#pragma once

#if defined(_MSC_VER) && defined(COS_AI_LOG)
#define AILOG_ENABLED 1

// One event line for nation NI (tagged AI or HUMAN). Ignored in games without a computer player.
void AILog_Event(int NI, const char* fmt, ...);

// Called every game frame; writes the snapshot of all players once per minute of game time.
void AILog_Tick();

// Called when a game ends: writes the final summary (length, every player's state and score) and closes the file.
void AILog_GameOver();

// Resource names by engine resource id (as used by XRESRC / ResTotal).
const char* AILog_ResName(int ResID);

// Outcome of an AI proposal (TryUnit / TryUpgrade). Probability rejections are random noise and never reported.
enum AILogWant
{
	AILOG_WANT_ACCEPTED,
	AILOG_WANT_MAX_REACHED,    // A = have, B = max
	AILOG_WANT_TOO_EXPENSIVE,  // A = project cost in percent of stock, B = allowed percent
	AILOG_WANT_UNAVAILABLE,    // no proposed project: no producer, locked or already done
	AILOG_WANT_BUSY            // another building project is in progress
};

// Reports a proposal outcome; written only when it differs from the last one for (NI, Upgrade, Index).
void AILog_Want(int NI, bool Upgrade, int Index, const char* Name, AILogWant Outcome, int A, int B);

// AI peasant distribution weights (SetPDistribution); written on change.
void AILog_PDist(int NI, int OnFood, int OnWood, int OnStone);

// AI mine upgrade rules (SetMinesUpgradeRules, 27 values); written on change.
void AILog_MineRules(int NI, const int* Table);
#endif
