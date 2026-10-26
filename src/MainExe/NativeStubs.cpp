// NativeStubs.cpp — inert stand-ins for the native MSVC build only (see CMakeLists.txt).
//
// The internet lobby (GSC rooms, chat, game reporting) lives in the IChat and IntExplorer modules
// and, in the web build, in the JavaScript relay (web/cos_net.js, cos_gsc.js). The native build does
// not compile those, so their entry points are stubbed here: the internet lobby is unavailable, while
// single-player, LAN and direct-IP multiplayer (CommCore, MPlayerStub.cpp) are unaffected.
//
// The game declares the IChat functions __declspec(dllimport); MSVC's linker resolves those
// references against these local definitions (warning LNK4217).

struct RoomInfo;
struct OnePlayerReport;

// ---- IChat (GSC lobby + chat) ----
int Process_GSC_ChatWindow(bool, RoomInfo*) { return 0; }
void ChatProcess() {}
void GetGameID(char* id) { if (id) id[0] = 0; }
void GoHomeAnyway() {}
void LeaveGSCRoom() {}
void ReportAliveState(int, int*) {}
void ReportGSCGame(int, int, OnePlayerReport*) {}
void SendPrivateMessage(char*, char*) {}
void SendVictoryState(int, unsigned char) {}
void StartGSCGame(char*, char*, int, int*, char**, int*, int*) {}

// ---- IntExplorer access keys ----
static char g_emptyAccessKey[1] = "";
extern "C" char* SXP_GetAccessKey(int) { return g_emptyAccessKey; }
extern "C" void SXP_SetAccessKeyDirect(int, char*) {}

// ---- web relay (web/cos_net.js); native peers always use real UDP ----
extern "C" int cosn_has_native_udp(void) { return 1; }
extern "C" void cosn_set_relay_mode(int) {}
extern "C" void cosn_gsc_room(int, int, const char*, int) {}
extern "C" int cosn_my_relay_id(void) { return 0; }
extern "C" void cosn_gsc_leave(void) {}
