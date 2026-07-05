#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unordered_map>

#include "platform.h"
#include "so_util.h"
#include "libyoyo.h"
#include "configuration.h"

ABI_ATTR create_async_event_with_ds_map_t CreateAsynEventWithDSMap = NULL;
ABI_ATTR create_ds_map_t CreateDsMap = NULL;
ABI_ATTR bool (*dsMapAddDouble)(int, const char*, double) = NULL;
ABI_ATTR bool (*dsMapAddInt)(int, const char*, int) = NULL;
ABI_ATTR bool (*dsMapAddString)(int, const char*, const char*) = NULL;
ABI_ATTR fct_add_t Function_Add = NULL;
ABI_ATTR float (*Audio_GetTrackPos)(int sound_id) = NULL;
ABI_ATTR int (*Audio_WAVs)(uint8_t*, uint32_t, uint8_t*, int) = NULL;
ABI_ATTR int32_t (*Graphics_DisplayHeight)() = NULL;
ABI_ATTR int32_t (*Graphics_DisplayWidth)() = NULL;
ABI_ATTR int32_t (*YYGetInt32)(RValue *val, int idx) = NULL;
ABI_ATTR int64_t (*YYGetInt64)(RValue *val, int idx) = NULL;
ABI_ATTR double (*YYGetReal)(RValue *val, int idx) = NULL;
ABI_ATTR void (*YYCreateString)(RValue *val, const char *str) = NULL;
ABI_ATTR routine_t F_YoYo_DrawTextureFlush = NULL;
ABI_ATTR uint32_t *(*ReadPNGFile)(void *a1, int a2, int *a3, int *a4, char a5) = NULL;
ABI_ATTR void (*Audio_PrepareGroup)(void*) = NULL;
ABI_ATTR void (*CThread__start_NS)(CThread *, void *, void *) = NULL;
ABI_ATTR void (*CThread__start_PcNS)(CThread *, void *, void *, char *) = NULL;
ABI_ATTR void (*FreePNGFile)() = NULL;
ABI_ATTR void (*GamepadUpdate)() = NULL;
ABI_ATTR void (*InitLLVM)(LLVMVars *) = NULL;
ABI_ATTR void (*InvalidateTextureState)() = NULL;
ABI_ATTR void (*MemoryManager__Free)(void *) = NULL;
ABI_ATTR void (*MemoryManager__Free_2)(void *) = NULL;
ABI_ATTR void (*Mutex__ctor)(void*, char*) = NULL;
ABI_ATTR void (*Mutex__dtor)(void *) = NULL;
ABI_ATTR void (*SetWorkingDirectory_ptr)() = NULL;
ABI_ATTR void (*surface_depth_disable)(RValue *ret, void *self, void *other, int argc, RValue *args) = NULL;
ABI_ATTR char (*Variable_GetValue_Direct)(void* inst, int var_slot, int array_idx, RValue *val, char unk1, char unk2) = NULL;
ABI_ATTR char (*Variable_SetValue_Direct)(void* inst, int var_slot, int array_idx, RValue *val) = NULL;
ABI_ATTR char (*Variable_GetBuiltIn_Direct)(void* inst, int var_slot, int array_idx, RValue *val) = NULL;
ABI_ATTR char (*Variable_SetBuiltIn_Direct)(void* inst, int var_slot, int array_idx, RValue *val) = NULL;
ABI_ATTR void (*Code_Function_GET_the_function)(int numb,char **name,void **code,int *args, int *unk) = NULL;
ABI_ATTR void (*_RefThing__dec)(void *ref) = NULL;
ABI_ATTR int (*Variable_BuiltIn_Find)(const char *name) = NULL;
ABI_ATTR long (*PrepareGame)(void) = NULL;
ABI_ATTR int (*Code_Variable_Find_Slot_From_Name)(void *instance, const char *name) = NULL;
ABI_ATTR int (*Variable_FindName)(const char *name) = NULL; /* Unavailable in GMS 1.4+, very old symbol */
ABI_ATTR long (*ExecuteIt)(void *self, void *other, void *code, RValue *args) = NULL;
ABI_ATTR long (*ExecuteIt_flags)(void *self, void *other, void *code, RValue *args, int argc) = NULL;

bionic_off_t *g_GameFileLength = NULL; //android had 32bit off_t???
char **g_pWorkingDirectory = NULL;
char *g_fNoAudio = NULL;
char *g_fYYC = NULL;
int *g_fVMTrace = NULL;
int *g_TextureScale = NULL;
int g_fdGameFileBuffer = -1;
int32_t *g_MousePosX = NULL;
int32_t *g_MousePosY = NULL;
LLVMVars **g_pLLVMVars = NULL;
long long *g_GML_DeltaTime = NULL;
int *the_numb = NULL;
int *g_nInstanceVariables = NULL;
void **g_pGlobal = NULL;
struct CCode **g_pFirstCode = NULL;
int *g_TotalCodeBlocks = NULL;
int *g_ArgumentCount = NULL;
RValue **Argument = NULL;
FunctionEntry **the_functions = NULL;
uint32_t *g_IOFrameCount = NULL;
uint8_t *_IO_ButtonDown = NULL;
uint8_t *_IO_ButtonPressed = NULL;
uint8_t *_IO_ButtonReleased = NULL;
uint8_t *_IO_CurrentKey = NULL;
uint8_t *_IO_KeyDown = NULL;
uint8_t *_IO_KeyPressed = NULL;
uint8_t *_IO_KeyReleased = NULL;
uint8_t *_IO_LastKey = NULL;
uintptr_t *Current_Room = NULL;
uintptr_t *New_Room = NULL;
void **g_nYYCode = NULL;
void **g_pGameFileBuffer = NULL;
void **g_ppYYStackTrace = NULL;
int *Extension_Main_number = NULL;
uint8_t prev_kbd_state[N_KEYS] = {};
uint8_t cur_keys[N_KEYS] = {};
static const char *fake_functs[] = {
    "object_set_collisions",
};

/* Risk of Rain Returns: steam funcs not covered by gmloader */
static const char *rrr_steam_real0[] = {
    "steam_deck", "steam_get_friends", "steam_input_activate_action_set",
    "steam_input_activate_action_set_layer", "steam_input_deactivate_action_set_layer", "steam_input_deactivate_all_action_set_layers",
    "steam_input_enable_action_event_callbacks", "steam_input_enable_device_callbacks", "steam_input_get_action_origin_from_xbox_origin",
    "steam_input_get_action_set_handle", "steam_input_get_active_action_set_layers", "steam_input_get_analog_action_data",
    "steam_input_get_analog_action_handle", "steam_input_get_analog_action_origins", "steam_input_get_connected_controllers",
    "steam_input_get_controller_for_gamepad_index", "steam_input_get_current_action_set", "steam_input_get_device_binding_revision",
    "steam_input_get_digital_action_data", "steam_input_get_digital_action_handle", "steam_input_get_digital_action_origins",
    "steam_input_get_gamepad_index_for_controller", "steam_input_get_input_type_for_handle", "steam_input_get_motion_data",
    "steam_input_get_remote_play_session_id", "steam_input_get_session_input_configuration_settings", "steam_input_init",
    "steam_input_new_data_available", "steam_input_run_frame", "steam_input_set_dualsense_trigger_effect",
    "steam_input_set_input_action_manifest_file_path", "steam_input_set_led_color", "steam_input_show_binding_panel",
    "steam_input_shutdown", "steam_input_stop_analog_action_momentum", "steam_input_translate_action_origin",
    "steam_input_trigger_haptic_pulse_legacy", "steam_input_trigger_repeated_haptic_pulse_legacy", "steam_input_trigger_simple_haptic_event",
    "steam_input_trigger_vibration", "steam_input_trigger_vibration_extended", "steam_input_wait_for_data",
    "steam_inventory_add_promo_item", "steam_inventory_add_promo_items", "steam_inventory_consume_item",
    "steam_inventory_exchange_items", "steam_inventory_generate_items", "steam_inventory_get_all_items",
    "steam_inventory_get_item_definition_property", "steam_inventory_get_item_price", "steam_inventory_get_items_by_id",
    "steam_inventory_get_items_with_prices", "steam_inventory_load_item_definitions", "steam_inventory_remove_property",
    "steam_inventory_request_eligible_promo_item_defs", "steam_inventory_request_prices", "steam_inventory_result_destroy",
    "steam_inventory_result_get_item_property", "steam_inventory_result_get_items", "steam_inventory_result_get_status",
    "steam_inventory_result_get_unix_timestamp", "steam_inventory_set_property_bool", "steam_inventory_set_property_float",
    "steam_inventory_set_property_int", "steam_inventory_start_purchase", "steam_inventory_start_update_properties",
    "steam_inventory_submit_update_properties", "steam_inventory_transfer_item_quantity", "steam_inventory_trigger_item_drop",
    "steam_lobby_activate_invite_overlay", "steam_lobby_button", "steam_lobby_create",
    "steam_lobby_get_chat_message_data", "steam_lobby_get_chat_message_size", "steam_lobby_get_chat_message_text",
    "steam_lobby_get_data", "steam_lobby_get_lobby_id", "steam_lobby_get_member_count",
    "steam_lobby_get_member_id", "steam_lobby_get_owner_id", "steam_lobby_is_owner",
    "steam_lobby_join_id", "steam_lobby_leave", "steam_lobby_list_add_distance_filter",
    "steam_lobby_list_add_near_filter", "steam_lobby_list_add_numerical_filter", "steam_lobby_list_get_count",
    "steam_lobby_list_get_data", "steam_lobby_list_get_lobby_id", "steam_lobby_list_get_lobby_member_count",
    "steam_lobby_list_get_lobby_member_id", "steam_lobby_list_get_lobby_owner_id", "steam_lobby_list_is_loading",
    "steam_lobby_list_join", "steam_lobby_list_request", "steam_lobby_send_chat_message",
    "steam_lobby_send_chat_message_buffer", "steam_lobby_set_data", "steam_lobby_set_joinable",
    "steam_lobby_set_member_limit", "steam_lobby_set_owner_id", "steam_lobby_set_type",
    "steam_music_get_status", "steam_music_is_enabled", "steam_music_is_playing",
    "steam_music_pause", "steam_music_play", "steam_music_play_next",
    "steam_music_play_previous", "steam_music_set_volume", "steam_net_accept_p",
    "steam_net_close_p", "steam_net_packet_get_data", "steam_net_packet_get_sender_id",
    "steam_net_packet_get_size", "steam_net_packet_receive", "steam_net_packet_send",
    "steam_net_packet_set_type", "steam_net_set_auto_accept_p", "steam_timeline_add_event",
    "steam_timeline_add_instantaneous_event", "steam_timeline_clear_state_description", "steam_timeline_end_event",
    "steam_timeline_event_open_overlay", "steam_timeline_event_recording_exists", "steam_timeline_game_phase_add_tag",
    "steam_timeline_game_phase_end", "steam_timeline_game_phase_open_overlay", "steam_timeline_game_phase_recording_exists",
    "steam_timeline_game_phase_set_attribute", "steam_timeline_game_phase_set_id", "steam_timeline_game_phase_start",
    "steam_timeline_remove_event", "steam_timeline_set_game_mode", "steam_timeline_set_state_description",
    "steam_timeline_start_event", "steam_timeline_update_event", "steam_ugc_add_item_to_favorites",
    "steam_ugc_details", "steam_ugc_download_item", "steam_ugc_get_user_item_vote",
    "steam_ugc_query_add_excluded_tag", "steam_ugc_query_add_required_tag", "steam_ugc_remove_item_from_favorites",
    "steam_ugc_set_user_item_vote",
};

static const char *rrr_steam_str[] = {
    "steam_get_app_ownership_ticket_data", "steam_input_get_glyph_for_action_origin_legacy", "steam_input_get_glyph_for_xbox_origin",
    "steam_input_get_glyph_png_for_action_origin", "steam_input_get_glyph_svg_for_action_origin", "steam_input_get_string_for_action_origin",
    "steam_input_get_string_for_analog_action_name", "steam_input_get_string_for_digital_action_name", "steam_input_get_string_for_xbox_origin",
    "steam_inventory_set_property_string", "steam_lobby_list_add_string_filter", "steam_user_request_encrypted_app_ticket",
};



/* Risk of Rain (2013) extension stubs (non-INI) */
static const char *rr_stub_real0[] = {
    "FS_clean_temporary", "FS_copy_fast", "FS_directory_create", "FS_directory_delete",
    "FS_directory_exists", "FS_export_image", "FS_export_image_adv", "FS_export_raw",
    "FS_export_sound", "FS_file_attributes", "FS_file_bin_bad", "FS_file_bin_clear_bad",
    "FS_file_bin_clear_fail", "FS_file_bin_close", "FS_file_bin_eof", "FS_file_bin_fail",
    "FS_file_bin_good", "FS_file_bin_open", "FS_file_bin_position", "FS_file_bin_read_byte",
    "FS_file_bin_read_dword", "FS_file_bin_read_word", "FS_file_bin_seek", "FS_file_bin_seek_relative",
    "FS_file_bin_size", "FS_file_bin_write_byte", "FS_file_bin_write_dword", "FS_file_bin_write_flush",
    "FS_file_bin_write_word", "FS_file_copy", "FS_file_delete", "FS_file_exists",
    "FS_file_find_close", "FS_file_find_first", "FS_file_find_next", "FS_file_rename",
    "FS_file_text_bad", "FS_file_text_clear_bad", "FS_file_text_clear_fail", "FS_file_text_close",
    "FS_file_text_eof", "FS_file_text_eoln", "FS_file_text_fail", "FS_file_text_good",
    "FS_file_text_open_append", "FS_file_text_open_append_ext", "FS_file_text_open_read", "FS_file_text_open_read_ext",
    "FS_file_text_open_write", "FS_file_text_open_write_ext", "FS_file_text_read_real", "FS_file_text_set_endl",
    "FS_file_text_set_endl_posix", "FS_file_text_set_endl_windows", "FS_file_text_unread", "FS_file_text_write_bom",
    "FS_file_text_write_flush", "FS_file_text_write_real", "FS_file_text_write_string", "FS_file_text_writeln",
    "FS_gmfilesystem_initialize_detail", "FS_import_image", "FS_max_open", "FS_set_gm_save_area",
    "FS_set_locale", "FS_set_working_directory",
};

static const char *rr_stub_str[] = {
    "FS_current_directory", "FS_file_text_read_char", "FS_file_text_read_string", "FS_file_text_readln",
    "FS_unique_fname",
};


double FORCE_PLATFORM = os_android;

size_t g_function_stride = 0;
fct_add_t Original_Function_Add = NULL;
static std::unordered_map<std::string, bool> g_function_map;

ReentrantHook REHPrepareGame = {};
ReentrantHook REHFunctionAdd = {};

/*
* YoYo's Function_Add used to blindly append functions to the function table.
* This allowed duplicate functions which worked since our functions are added to the table before the engine does its own thing.
* As of 2024.14, Function_Add checks for duplicates and fails if a function with the same name already exists.
* Sadly it adds a new index to the table array before this check, so when the check fails there's a null index which is later dereferenced.
* Oops.
*/

void Function_Add_Hook(const char* f_name, routine_t func, int arg_count, char reg)
{
    if (!the_functions || !the_numb) {
        return;
    }

    auto it = g_function_map.find(f_name);
    if (it != g_function_map.end()) {
        return;
    }

    rehook_unhook(&REHFunctionAdd);
    Original_Function_Add(f_name, func, arg_count, reg);
    rehook_hook(&REHFunctionAdd);

    g_function_map[f_name] = true;
}

ABI_ATTR int _dbg_csol_print(void *csol, const char *fmt, ...) {
    char csol_str[2048];
    va_list list;

    va_start(list, fmt);
    int ret = vsnprintf(csol_str, sizeof(csol_str)-1, fmt, list);
    warning("%s", csol_str);
    va_end(list);
    return ret;
}

ABI_ATTR void show_message(const char *c)
{
    printf("%s\n", c);
}

// Crashes the application, don't want to deal with this.
ABI_ATTR static void alNoop()
{
    return;
}

ABI_ATTR static void dont_init_extensions()
{
    //*Extension_Main_number = 0;
    warning("dont_init_extensions!!!\n");
}

ABI_ATTR static void stub_gml(RValue *ret, void *self, void *other, int argc, RValue *args)
{
    ret->kind = VALUE_REAL;
    ret->rvalue.val = 0;
    /* */
}

ABI_ATTR static void rr_stub_string(RValue *ret, void *self, void *other, int argc, RValue *args)
{
    YYCreateString(ret, "");
}
/* ===== Risk of Rain (2013): real INI read/write for d_ini_* / FS_ini_* ===== */
#include <map>
#include <unistd.h>

typedef std::map<std::string, std::map<std::string,std::string>> RRIniData;

static std::string rr_save_base()
{
    std::string sd = gmloader_config.save_dir;
    if (sd.empty()) sd = ".";
    if (sd[0] != '/') {
        char cwd[4096];
        if (getcwd(cwd, sizeof(cwd))) sd = std::string(cwd) + "/" + sd;
    }
    if (!sd.empty() && sd.back() != '/') sd += '/';
    return sd;
}
static std::string rr_resolve(const char *fname)
{
    if (!fname || !*fname) return rr_save_base();
    if (fname[0] == '/') return std::string(fname);
    return rr_save_base() + fname;
}
static void rr_trim(std::string &x)
{
    size_t a = x.find_first_not_of(" \t\r\n");
    size_t b = x.find_last_not_of(" \t\r\n");
    if (a == std::string::npos) x.clear(); else x = x.substr(a, b - a + 1);
}
static void rr_ini_load(const std::string &path, RRIniData &d)
{
    d.clear();
    FILE *f = fopen(path.c_str(), "rb");
    if (!f) return;
    char line[2048]; std::string cur;
    while (fgets(line, sizeof(line), f)) {
        std::string s(line); rr_trim(s);
        if (s.empty() || s[0] == ';' || s[0] == '#') continue;
        if (s.front() == '[' && s.back() == ']') { cur = s.substr(1, s.size() - 2); rr_trim(cur); }
        else {
            size_t eq = s.find('=');
            if (eq != std::string::npos) {
                std::string k = s.substr(0, eq), v = s.substr(eq + 1);
                rr_trim(k); rr_trim(v);
                if (v.size() >= 2 && v.front() == '"' && v.back() == '"') v = v.substr(1, v.size() - 2);
                d[cur][k] = v;
            }
        }
    }
    fclose(f);
}
static void rr_ini_store(const std::string &path, RRIniData &d)
{
    FILE *f = fopen(path.c_str(), "wb");
    if (!f) { warning("RR: failed to write ini %s\n", path.c_str()); return; }
    for (auto &sp : d) {
        fprintf(f, "[%s]\n", sp.first.c_str());
        for (auto &kv : sp.second) fprintf(f, "%s=\"%s\"\n", kv.first.c_str(), kv.second.c_str());
        fprintf(f, "\n");
    }
    fclose(f);
}

struct RRIniHandle { std::string path; RRIniData data; bool open = false, dirty = false; };
static RRIniHandle g_dini, g_fsini;

static const char *rr_astr(int argc, RValue *args, int i) { return (i < argc) ? (YYGetCStrHelper(args, i) ? YYGetCStrHelper(args, i) : "") : ""; }
static double      rr_areal(int argc, RValue *args, int i) { return (i < argc) ? args[i].rvalue.val : 0; }
static void        rr_rreal(RValue *ret, double v) { ret->kind = VALUE_REAL; ret->rvalue.val = v; }

static void rr_h_open(RRIniHandle &h, const char *fname) {
    if (h.open && h.dirty) rr_ini_store(h.path, h.data);
    h.path = rr_resolve(fname); rr_ini_load(h.path, h.data); h.open = true; h.dirty = false;
}
static void rr_h_close(RRIniHandle &h) {
    if (h.open && h.dirty) rr_ini_store(h.path, h.data);
    h.open = false; h.dirty = false;
}
static bool rr_h_get(RRIniHandle &h, const char *s, const char *k, std::string &out) {
    auto si = h.data.find(s); if (si == h.data.end()) return false;
    auto ki = si->second.find(k); if (ki == si->second.end()) return false;
    out = ki->second; return true;
}
static void rr_h_set(RRIniHandle &h, const char *s, const char *k, const std::string &v) { h.data[s][k] = v; h.dirty = true; }

/* ---- d_ini_* (Prefs.ini etc.) ---- */
ABI_ATTR static void RR_dini_open(RValue *r, void*, void*, int argc, RValue *a){ rr_h_open(g_dini, rr_astr(argc,a,0)); rr_rreal(r,0); }
ABI_ATTR static void RR_dini_close(RValue *r, void*, void*, int argc, RValue *a){ rr_h_close(g_dini); rr_rreal(r,0); }
ABI_ATTR static void RR_dini_read_real(RValue *r, void*, void*, int argc, RValue *a){ std::string v; rr_rreal(r, rr_h_get(g_dini,rr_astr(argc,a,0),rr_astr(argc,a,1),v)?atof(v.c_str()):rr_areal(argc,a,2)); }
ABI_ATTR static void RR_dini_read_string(RValue *r, void*, void*, int argc, RValue *a){ std::string v; if(rr_h_get(g_dini,rr_astr(argc,a,0),rr_astr(argc,a,1),v)) YYCreateString(r,v.c_str()); else YYCreateString(r,rr_astr(argc,a,2)); }
ABI_ATTR static void RR_dini_write_real(RValue *r, void*, void*, int argc, RValue *a){ char b[64]; snprintf(b,sizeof(b),"%f",rr_areal(argc,a,2)); rr_h_set(g_dini,rr_astr(argc,a,0),rr_astr(argc,a,1),b); rr_rreal(r,0); }
ABI_ATTR static void RR_dini_write_string(RValue *r, void*, void*, int argc, RValue *a){ rr_h_set(g_dini,rr_astr(argc,a,0),rr_astr(argc,a,1),rr_astr(argc,a,2)); rr_rreal(r,0); }

/* ---- FS_ini_* stateful (Save.ini etc.) ---- */
ABI_ATTR static void RR_fsini_open(RValue *r, void*, void*, int argc, RValue *a){ rr_h_open(g_fsini, rr_astr(argc,a,0)); rr_rreal(r,0); }
ABI_ATTR static void RR_fsini_close(RValue *r, void*, void*, int argc, RValue *a){ rr_h_close(g_fsini); rr_rreal(r,0); }
ABI_ATTR static void RR_fsini_read_real(RValue *r, void*, void*, int argc, RValue *a){ std::string v; rr_rreal(r, rr_h_get(g_fsini,rr_astr(argc,a,0),rr_astr(argc,a,1),v)?atof(v.c_str()):rr_areal(argc,a,2)); }
ABI_ATTR static void RR_fsini_read_string(RValue *r, void*, void*, int argc, RValue *a){ std::string v; if(rr_h_get(g_fsini,rr_astr(argc,a,0),rr_astr(argc,a,1),v)) YYCreateString(r,v.c_str()); else YYCreateString(r,rr_astr(argc,a,2)); }
ABI_ATTR static void RR_fsini_write_real(RValue *r, void*, void*, int argc, RValue *a){ char b[64]; snprintf(b,sizeof(b),"%f",rr_areal(argc,a,2)); rr_h_set(g_fsini,rr_astr(argc,a,0),rr_astr(argc,a,1),b); rr_rreal(r,0); }
ABI_ATTR static void RR_fsini_write_string(RValue *r, void*, void*, int argc, RValue *a){ rr_h_set(g_fsini,rr_astr(argc,a,0),rr_astr(argc,a,1),rr_astr(argc,a,2)); rr_rreal(r,0); }
ABI_ATTR static void RR_fsini_key_exists(RValue *r, void*, void*, int argc, RValue *a){ std::string v; rr_rreal(r, rr_h_get(g_fsini,rr_astr(argc,a,0),rr_astr(argc,a,1),v)?1:0); }
ABI_ATTR static void RR_fsini_section_exists(RValue *r, void*, void*, int argc, RValue *a){ rr_rreal(r, g_fsini.data.count(rr_astr(argc,a,0))?1:0); }
ABI_ATTR static void RR_fsini_key_delete(RValue *r, void*, void*, int argc, RValue *a){ auto si=g_fsini.data.find(rr_astr(argc,a,0)); if(si!=g_fsini.data.end()){ si->second.erase(rr_astr(argc,a,1)); g_fsini.dirty=true;} rr_rreal(r,0); }
ABI_ATTR static void RR_fsini_section_delete(RValue *r, void*, void*, int argc, RValue *a){ if(g_fsini.data.erase(rr_astr(argc,a,0))) g_fsini.dirty=true; rr_rreal(r,0); }

/* ---- FS_ini_*_ext one-shot (file given per call as arg0) ---- */
ABI_ATTR static void RR_fsini_open_ext(RValue *r, void*, void*, int argc, RValue *a){ rr_h_open(g_fsini, rr_astr(argc,a,0)); rr_rreal(r,0); }
ABI_ATTR static void RR_fsini_close_ext(RValue *r, void*, void*, int argc, RValue *a){ rr_h_close(g_fsini); rr_rreal(r,0); }
ABI_ATTR static void RR_fsini_read_real_ext(RValue *r, void*, void*, int argc, RValue *a){ RRIniData d; rr_ini_load(rr_resolve(rr_astr(argc,a,0)),d); auto si=d.find(rr_astr(argc,a,1)); if(si!=d.end()){auto ki=si->second.find(rr_astr(argc,a,2)); if(ki!=si->second.end()){rr_rreal(r,atof(ki->second.c_str())); return;}} rr_rreal(r, rr_areal(argc,a,3)); }
ABI_ATTR static void RR_fsini_read_string_ext(RValue *r, void*, void*, int argc, RValue *a){ RRIniData d; rr_ini_load(rr_resolve(rr_astr(argc,a,0)),d); auto si=d.find(rr_astr(argc,a,1)); if(si!=d.end()){auto ki=si->second.find(rr_astr(argc,a,2)); if(ki!=si->second.end()){YYCreateString(r,ki->second.c_str()); return;}} YYCreateString(r, rr_astr(argc,a,3)); }
ABI_ATTR static void RR_fsini_write_real_ext(RValue *r, void*, void*, int argc, RValue *a){ std::string p=rr_resolve(rr_astr(argc,a,0)); RRIniData d; rr_ini_load(p,d); char b[64]; snprintf(b,sizeof(b),"%f",rr_areal(argc,a,3)); d[rr_astr(argc,a,1)][rr_astr(argc,a,2)]=b; rr_ini_store(p,d); rr_rreal(r,0); }
ABI_ATTR static void RR_fsini_write_string_ext(RValue *r, void*, void*, int argc, RValue *a){ std::string p=rr_resolve(rr_astr(argc,a,0)); RRIniData d; rr_ini_load(p,d); d[rr_astr(argc,a,1)][rr_astr(argc,a,2)]=rr_astr(argc,a,3); rr_ini_store(p,d); rr_rreal(r,0); }
ABI_ATTR static void RR_fsini_key_exists_ext(RValue *r, void*, void*, int argc, RValue *a){ RRIniData d; rr_ini_load(rr_resolve(rr_astr(argc,a,0)),d); auto si=d.find(rr_astr(argc,a,1)); rr_rreal(r, (si!=d.end() && si->second.count(rr_astr(argc,a,2)))?1:0); }
ABI_ATTR static void RR_fsini_section_exists_ext(RValue *r, void*, void*, int argc, RValue *a){ RRIniData d; rr_ini_load(rr_resolve(rr_astr(argc,a,0)),d); rr_rreal(r, d.count(rr_astr(argc,a,1))?1:0); }
ABI_ATTR static void RR_fsini_key_delete_ext(RValue *r, void*, void*, int argc, RValue *a){ std::string p=rr_resolve(rr_astr(argc,a,0)); RRIniData d; rr_ini_load(p,d); auto si=d.find(rr_astr(argc,a,1)); if(si!=d.end()){ si->second.erase(rr_astr(argc,a,2)); rr_ini_store(p,d);} rr_rreal(r,0); }
ABI_ATTR static void RR_fsini_section_delete_ext(RValue *r, void*, void*, int argc, RValue *a){ std::string p=rr_resolve(rr_astr(argc,a,0)); RRIniData d; rr_ini_load(p,d); if(d.erase(rr_astr(argc,a,1))) rr_ini_store(p,d); rr_rreal(r,0); }

/* ===== Risk of Rain Returns: RoRRWinExt display/window helpers ===== */
ABI_ATTR static void RRR_disp_w(RValue *r, void*, void*, int argc, RValue *a){ rr_rreal(r, Graphics_DisplayWidth?Graphics_DisplayWidth():1024); }
ABI_ATTR static void RRR_disp_h(RValue *r, void*, void*, int argc, RValue *a){ rr_rreal(r, Graphics_DisplayHeight?Graphics_DisplayHeight():768); }
ABI_ATTR static void RRR_one(RValue *r, void*, void*, int argc, RValue *a){ rr_rreal(r, 1); }
ABI_ATTR static void RRR_kbd(RValue *r, void*, void*, int argc, RValue *a){ YYCreateString(r, "en-US"); }
ABI_ATTR static void RRR_pass(RValue *r, void*, void*, int argc, RValue *a){ rr_rreal(r, argc>=1?a[0].rvalue.val:0); }


ABI_ATTR static void game_end_reimpl(RValue *ret, void *self, void *other, int argc, RValue *args)
{
    *New_Room = 0xffffff9c;
}

ABI_ATTR static double force_platform_type()
{
    return FORCE_PLATFORM;
}

ABI_ATTR void force_platform_type_gms2(void *self, int n, RValue *args)
{
    args[0].kind = VALUE_REAL;
    args[0].rvalue.val = FORCE_PLATFORM;
}

ABI_ATTR static void window_handle(RValue *ret, void *self, void *other, int argc, RValue *args)
{
    warning("-- Called window_handle stub ! --\n");
    ret->kind = VALUE_INT64;
    ret->rvalue.v64 = 0;
}

void send_async_social(const char* type)
{
        if (CreateAsynEventWithDSMap != NULL && CreateDsMap != NULL && dsMapAddString != NULL) {
        int ds_map = CreateDsMap(0);
        dsMapAddString(ds_map, "type", type);
        CreateAsynEventWithDSMap(ds_map, 70);
        }
}

#ifndef VIDEO_SUPPORT
ABI_ATTR static void video_open_reimpl(RValue *ret, void *self, void *other, int argc, RValue *args)
{
    ret->kind = VALUE_BOOL;
    ret->rvalue.val = 1;

    if (CreateAsynEventWithDSMap != NULL && CreateDsMap != NULL && dsMapAddString != NULL) {
        int ds_map = CreateDsMap(0);
        dsMapAddString(ds_map, "type", "video_end");
        if (argc > 0 && args[0].kind == VALUE_STRING) {
            dsMapAddString(ds_map, "path", YYGetCStrHelper(args, 0));
        }
        CreateAsynEventWithDSMap(ds_map, 70);
    }
}
#endif

// Implementation of game_change which is not available for android normally
//  Takes two arguments: work_dir and launch_params (example: "/chapter1_windows" "-game data.win")
//  https://manual.gamemaker.io/beta/en/GameMaker_Language/GML_Reference/General_Game_Control/game_change.htm
ABI_ATTR void game_change_reimpl(RValue *ret, void *self, void *other, int argc, RValue *args) {
    std::string buffer = "game_change(): ";
    for (int i = 0; i < argc; i++) {
        const char *arg = (args[i].kind == VALUE_STRING) ? YYGetCStrHelper(args, i) : "INVALID";
        if (i > 0)
            buffer += ", ";
        buffer += arg;
    }

    warning("%s\n", buffer.c_str());

    if (ret) {
        ret->kind = VALUE_BOOL;
        ret->rvalue.val = 0;
    }

    if (argc < 2) {
        warning("game_change(): Requires at least two arguments (workdir and params)\n");
        return;
    }

    for (int i = 0; i < argc; i++) {
        if (args[i].kind != VALUE_STRING || !args[i].rvalue.str || !args[i].rvalue.str->m_thing) {
            warning("game_change(): Argument %d is not a valid string\n", i);
            return;
        }
    }

    free((void *)gc_workdir);
    gc_workdir = strdup(YYGetCStrHelper(args, 0));
    if (!gc_workdir) {
        warning("game_change(): Failed to duplicate workdir\n");
        return;
    }

    std::string sub_path(gc_workdir);
    if (!sub_path.empty() && sub_path.back() != '/') {
        sub_path += '/';
    }

    std::string full_path = gmloader_config.save_dir + sub_path;
    warning("game_change(): Resolved game path: '%s'\n", full_path.c_str());

    relaunch_flag = 1;

    if (ret) {
        ret->kind = VALUE_BOOL;
        ret->rvalue.val = 1;
    }
}

extern int setup_ended;
ABI_ATTR long PrepareGame_hook()
{
    setup_ended = 1;
    rehook_unhook(&REHPrepareGame);
    long ret = PrepareGame();
    warning("- PrepareGame done.\n");
    return ret;
}

void patch_libyoyo(so_module *mod)
{
    // Load all of the native symbols referenced
    // ENSURE_SYMBOL(mod, Audio_GetTrackPos, "_Z17Audio_GetTrackPosi");
    ENSURE_SYMBOL(mod, Audio_PrepareGroup, "_Z18Audio_PrepareGroupi");
    // ENSURE_SYMBOL(mod, Audio_WAVs, "_Z10Audio_WAVsPhjS_i");
    ENSURE_SYMBOL(mod, CreateAsynEventWithDSMap, "_Z24CreateAsynEventWithDSMapii");
    ENSURE_SYMBOL(mod, CreateDsMap, "_Z11CreateDsMapiz", "dsMapCreate");
    ENSURE_SYMBOL(mod, dsMapAddString, "dsMapAddString");
    // ENSURE_SYMBOL(mod, dsMapAddDouble, "dsMapAddDouble");
    ENSURE_SYMBOL(mod, dsMapAddInt, "dsMapAddInt");
    ENSURE_SYMBOL(mod, Current_Room, "Current_Room");
    ENSURE_SYMBOL(mod, Function_Add, "_Z12Function_AddPKcPFvR6RValueP9CInstanceS4_iPS1_Eib", "_Z12Function_AddPcPFvR6RValueP9CInstanceS3_iPS0_Eib");
    ENSURE_SYMBOL(mod, FreePNGFile, "_Z11FreePNGFilev");
    ENSURE_SYMBOL(mod, GamepadUpdate, "_Z14GamepadUpdateMv");
    ENSURE_SYMBOL(mod, Graphics_DisplayHeight, "_Z22Graphics_DisplayHeightv");
    ENSURE_SYMBOL(mod, Graphics_DisplayWidth, "_Z21Graphics_DisplayWidthv");
    ENSURE_SYMBOL(mod, g_fNoAudio, "g_fNoAudio");
    ENSURE_SYMBOL(mod, g_fYYC, "g_fYYC");
    FIND_SYMBOL(mod, g_fVMTrace, "g_fVMTrace");
    ENSURE_SYMBOL(mod, g_GameFileLength, "g_GameFileLength");
    ENSURE_SYMBOL(mod, g_GML_DeltaTime, "g_GML_DeltaTime");
    ENSURE_SYMBOL(mod, g_IOFrameCount, "g_IOFrameCount");
    ENSURE_SYMBOL(mod, g_MousePosX, "g_MousePosX");
    ENSURE_SYMBOL(mod, g_MousePosY, "g_MousePosY");
    ENSURE_SYMBOL(mod, g_nYYCode, "g_nYYCode");
    ENSURE_SYMBOL(mod, g_pGameFileBuffer, "g_pGameFileBuffer");
    ENSURE_SYMBOL(mod, g_pLLVMVars, "g_pLLVMVars");
    ENSURE_SYMBOL(mod, g_pWorkingDirectory, "g_pWorkingDirectory");
    ENSURE_SYMBOL(mod, g_TextureScale, "g_TextureScale");
    ENSURE_SYMBOL(mod, InitLLVM, "_Z8InitLLVMP9SLLVMVars");
    ENSURE_SYMBOL(mod, InvalidateTextureState, "_Z23_InvalidateTextureStatev");
    ENSURE_SYMBOL(mod, Mutex__ctor, "_ZN5MutexC1EPKc");
    ENSURE_SYMBOL(mod, Mutex__dtor, "_ZN5MutexD2Ev", "_ZN5MutexD1Ev");
    ENSURE_SYMBOL(mod, New_Room, "New_Room");
    ENSURE_SYMBOL(mod, ReadPNGFile, "_Z11ReadPNGFilePviPiS0_b");
    ENSURE_SYMBOL(mod, the_functions, "the_functions");
    ENSURE_SYMBOL(mod, the_numb, "the_numb");
    ENSURE_SYMBOL(mod, g_pGlobal, "g_pGlobal");
    ENSURE_SYMBOL(mod, g_pFirstCode, "g_pFirstCode");
    ENSURE_SYMBOL(mod, g_TotalCodeBlocks, "g_TotalCodeBlocks");
    ENSURE_SYMBOL(mod, g_ArgumentCount, "g_ArgumentCount");
    ENSURE_SYMBOL(mod, Argument, "Argument");
    ENSURE_SYMBOL(mod, YYGetInt32, "_Z10YYGetInt32PK6RValuei");
    ENSURE_SYMBOL(mod, YYGetInt64, "_Z10YYGetInt64PK6RValuei");
    ENSURE_SYMBOL(mod, YYGetReal, "_Z9YYGetRealPK6RValuei");
    ENSURE_SYMBOL(mod, _IO_ButtonDown, "_IO_ButtonDown");
    ENSURE_SYMBOL(mod, _IO_ButtonPressed, "_IO_ButtonPressed");
    ENSURE_SYMBOL(mod, _IO_ButtonReleased, "_IO_ButtonReleased");
    ENSURE_SYMBOL(mod, _IO_CurrentKey, "_IO_CurrentKey", "l_IO_CurrentKey");
    ENSURE_SYMBOL(mod, _IO_KeyDown, "_IO_KeyDown", "l_IO_KeyDown");
    ENSURE_SYMBOL(mod, _IO_KeyPressed, "_IO_KeyPressed", "l_IO_KeyPressed");
    ENSURE_SYMBOL(mod, _IO_KeyReleased, "_IO_KeyReleased", "l_IO_KeyReleased");
    ENSURE_SYMBOL(mod, _IO_LastKey, "_IO_LastKey", "l_IO_LastKey");
    ENSURE_SYMBOL(mod, Variable_GetValue_Direct, "_Z24Variable_GetValue_DirectP12YYObjectBaseiiP6RValue", "_Z24Variable_GetValue_DirectP12YYObjectBaseiiP6RValuebb", "_Z24Variable_GetValue_DirectP9CInstanceiiP6RValue");
    ENSURE_SYMBOL(mod, Variable_SetValue_Direct, "_Z24Variable_SetValue_DirectP12YYObjectBaseiiP6RValue", "_Z24Variable_SetValue_DirectP9CInstanceiiP6RValue");
    ENSURE_SYMBOL(mod, Code_Function_GET_the_function, "_Z30Code_Function_GET_the_functioniPPKcPPvPi", "_Z30Code_Function_GET_the_functioniPPKcPPvPiS4_", "_Z30Code_Function_GET_the_functioniPPcPPvPiS3_");
    ENSURE_SYMBOL(mod, Variable_BuiltIn_Find, "_Z21Variable_BuiltIn_FindPKc", "_Z21Variable_BuiltIn_FindPc");
    // Versioned symbols
    FIND_SYMBOL(mod, ExecuteIt, "_Z9ExecuteItP9CInstanceS0_P5CCodeP6RValue");
    FIND_SYMBOL(mod, ExecuteIt_flags, "_Z9ExecuteItP9CInstanceS0_P5CCodeP6RValuei");
    FIND_SYMBOL(mod, MemoryManager__Free, "_ZN13MemoryManager4FreeEPv", "_ZN13MemoryManager4FreeEPKv");
    FIND_SYMBOL(mod, MemoryManager__Free_2, "_ZN13MemoryManager4FreeEPKvb");

    // Unavailable in newer versions
    FIND_SYMBOL(mod, g_nInstanceVariables, "g_nInstanceVariables"); /* Can be used to sus out for the older OUYA runners... */
    FIND_SYMBOL(mod, Variable_FindName, "_Z17Variable_FindNamePc");

    // Unavailable in older versions
    FIND_SYMBOL(mod, Variable_GetBuiltIn_Direct, "_Z26Variable_GetBuiltIn_DirectP12YYObjectBaseiiP6RValue");
    FIND_SYMBOL(mod, Variable_SetBuiltIn_Direct, "_Z26Variable_SetBuiltIn_DirectP12YYObjectBaseiiP6RValue");
    FIND_SYMBOL(mod, Code_Variable_Find_Slot_From_Name, "_Z33Code_Variable_Find_Slot_From_NameP12YYObjectBasePKc");
    FIND_SYMBOL(mod, YYCreateString, "_Z14YYCreateStringP6RValuePKc");
    FIND_SYMBOL(mod, _RefThing__dec, "_ZN9_RefThingIPKcE3decEv");

    // Depth disable
    FIND_SYMBOL(mod, surface_depth_disable, "_Z21F_SurfaceDepthDisableR6RValueP9CInstanceS2_iPS_");

    // Hook the start of the game so we know setup is done
    ENSURE_SYMBOL(mod, PrepareGame, "_Z11PrepareGamev");
    rehook_new(mod, &REHPrepareGame, (uintptr_t)PrepareGame, (uintptr_t)&PrepareGame_hook);
    rehook_hook(&REHPrepareGame);

    // Disable extension support
    FIND_SYMBOL(mod, Extension_Main_number, "Extension_Main_number");
    if (gmloader_config.disable_extensions == 1) {
        // hook_symbol(mod, "_Z20Extension_Initializev", (uintptr_t)&dont_init_extensions, 1);
        // hook_symbol(mod, "_Z20Extension_PrePreparev", (uintptr_t)&dont_init_extensions, 1);
        hook_symbol(mod, "_Z14Extension_LoadPhjS_", (uintptr_t)&dont_init_extensions, 1);
    }

    // Hook messages for debug
    hook_symbol(mod, "_Z11ShowMessagePKc", (uintptr_t)&show_message, 1);
    hook_symbol(mod, "_ZN12DummyConsole6OutputEPKcz", (uintptr_t)&_dbg_csol_print, 1);
    hook_symbol(mod, "_ZN8TConsole6OutputEPKcz", (uintptr_t)&_dbg_csol_print, 1);
    hook_symbol(mod, "_ZN11TRelConsole6OutputEPKcz", (uintptr_t)&_dbg_csol_print, 1);
    hook_symbol(mod, "_ZN17TErrStreamConsole6OutputEPKcz", (uintptr_t)&_dbg_csol_print, 1);

    // Avoid OpenAL crashes due to the static overrides 
    hook_symbol(mod, "alBufferMarkNeedsFreed", (uintptr_t)&alNoop, 1);
    hook_symbol(mod, "_Z22alBufferMarkNeedsFreedj", (uintptr_t)&alNoop, 1);
    hook_symbol(mod, "_Z17alBufferDebugNamejPKc", (uintptr_t)&alNoop, 1);

    // Lie about the runner's platform to the game.
    hook_symbol(mod, "_Z23YoYo_GetPlatform_DoWorkv", (uintptr_t)&force_platform_type, 1);
    hook_symbol(mod, "_Z20GET_YoYo_GetPlatformP9CInstanceiP6RValue", (uintptr_t)&force_platform_type_gms2, 1);

    // Enable tracing of the vm
    const char *trace_vm = getenv("GMLOADER_TRACE_VM");
    if (trace_vm && *trace_vm == '1') {
        if (g_fVMTrace) {
            *g_fVMTrace = 1;
        } else {
            fatal_error("Requested VM Tracing, but could not find g_fVMTrace!\n");
        }
    }

    // Wrap Function_Add
    if (Function_Add && !Original_Function_Add)
    {
        Original_Function_Add = Function_Add;
        rehook_new(mod, &REHFunctionAdd, (uintptr_t)Function_Add, (uintptr_t)&Function_Add_Hook);
        rehook_hook(&REHFunctionAdd);
    }
    
#if defined(__arm__)
    // Add a "game_end" function (useless on 1.4.1788+ until 1.4.1804ish?)
    // From 'https://store.yoyogames.com/downloads/gm-studio/release-notes-studio.html':
    //   "Changed game_end() to be ignored on Android and iOS, following crash 
    //   logs that indicated this was causing memory corruption and crashes in
    //   subsequent runs of your app"
    
    uint32_t *_Z15Command_EndGamev = (uint32_t *)so_symbol(mod, "_Z15Command_EndGamev");
    // _Z15Command_EndGamev+0x00: bx lr (noop function)
    if (_Z15Command_EndGamev != NULL && *_Z15Command_EndGamev == 0xE12FFF1E) {
        Function_Add("game_end", game_end_reimpl, 1, 1);
    }
#endif

    // Define some stubbed gamemaker functions for compatibility with odd games
    for (int i = 0; i < ARRAY_SIZE(fake_functs); i++) {
        Function_Add(fake_functs[i], stub_gml, 1, 1);
    }

    // Risk of Rain (2013): typed stubs for file/misc extension funcs
    for (size_t i = 0; i < ARRAY_SIZE(rr_stub_real0); i++) Function_Add(rr_stub_real0[i], stub_gml, -1, 1);
    for (size_t i = 0; i < ARRAY_SIZE(rr_stub_str); i++) Function_Add(rr_stub_str[i], rr_stub_string, -1, 1);
    // Risk of Rain (2013): REAL INI read/write (settings + saves)
    Function_Add("d_ini_open", RR_dini_open, -1, 1);
    Function_Add("d_ini_close", RR_dini_close, -1, 1);
    Function_Add("d_ini_read_real", RR_dini_read_real, -1, 1);
    Function_Add("d_ini_read_string", RR_dini_read_string, -1, 1);
    Function_Add("d_ini_write_real", RR_dini_write_real, -1, 1);
    Function_Add("d_ini_write_string", RR_dini_write_string, -1, 1);
    Function_Add("FS_ini_open", RR_fsini_open, -1, 1);
    Function_Add("FS_ini_close", RR_fsini_close, -1, 1);
    Function_Add("FS_ini_read_real", RR_fsini_read_real, -1, 1);
    Function_Add("FS_ini_read_string", RR_fsini_read_string, -1, 1);
    Function_Add("FS_ini_write_real", RR_fsini_write_real, -1, 1);
    Function_Add("FS_ini_write_string", RR_fsini_write_string, -1, 1);
    Function_Add("FS_ini_key_exists", RR_fsini_key_exists, -1, 1);
    Function_Add("FS_ini_section_exists", RR_fsini_section_exists, -1, 1);
    Function_Add("FS_ini_key_delete", RR_fsini_key_delete, -1, 1);
    Function_Add("FS_ini_section_delete", RR_fsini_section_delete, -1, 1);
    Function_Add("FS_ini_open_ext", RR_fsini_open_ext, -1, 1);
    Function_Add("FS_ini_close_ext", RR_fsini_close_ext, -1, 1);
    Function_Add("FS_ini_read_real_ext", RR_fsini_read_real_ext, -1, 1);
    Function_Add("FS_ini_read_string_ext", RR_fsini_read_string_ext, -1, 1);
    Function_Add("FS_ini_write_real_ext", RR_fsini_write_real_ext, -1, 1);
    Function_Add("FS_ini_write_string_ext", RR_fsini_write_string_ext, -1, 1);
    Function_Add("FS_ini_key_exists_ext", RR_fsini_key_exists_ext, -1, 1);
    Function_Add("FS_ini_section_exists_ext", RR_fsini_section_exists_ext, -1, 1);
    Function_Add("FS_ini_key_delete_ext", RR_fsini_key_delete_ext, -1, 1);
    Function_Add("FS_ini_section_delete_ext", RR_fsini_section_delete_ext, -1, 1);

    // Risk of Rain Returns: steam funcs missing from gmloader (mirror steam_stub -> 0)
    for (size_t i = 0; i < ARRAY_SIZE(rrr_steam_real0); i++) Function_Add(rrr_steam_real0[i], stub_gml, -1, 0);
    for (size_t i = 0; i < ARRAY_SIZE(rrr_steam_str); i++) Function_Add(rrr_steam_str[i], rr_stub_string, -1, 0);
    // Risk of Rain Returns: RoRRWinExt
    Function_Add("win_display_count", RRR_one, -1, 1);
    Function_Add("win_display_update_cache", stub_gml, -1, 1);
    Function_Add("win_display_get_x", stub_gml, -1, 1);
    Function_Add("win_display_get_y", stub_gml, -1, 1);
    Function_Add("win_display_get_width", RRR_disp_w, -1, 1);
    Function_Add("win_display_get_height", RRR_disp_h, -1, 1);
    Function_Add("win_display_get_default_monitor_index", stub_gml, -1, 1);
    Function_Add("win_window_set_popup", stub_gml, -1, 1);
    Function_Add("win_get_keyboard_layout_name", RRR_kbd, -1, 1);
    Function_Add("win_vsc_to_vk", RRR_pass, -1, 1);
    Function_Add("win_vk_to_vsc", RRR_pass, -1, 1);
    Function_Add("display_mouse_unlock", stub_gml, -1, 1);
    Function_Add("display_mouse_bounds_raw", stub_gml, -1, 1);


    Function_Add("window_handle", window_handle, 0, 1);
    #ifndef VIDEO_SUPPORT //Add the stub if real video support isn't included
    Function_Add("video_open", video_open_reimpl, 1, 1);
    #endif
    Function_Add("game_change", game_change_reimpl, 2, 0);

    so_symbol_fix_ldmia(mod, "_Z11Shader_LoadPhjS_");
}

void disable_depth()
{      
    if (surface_depth_disable != NULL) {
        RValue ret;
        RValue args[1] = {
            {
                { .val = 1 },  // Union initialization
                0,             // Default value for flags
                VALUE_BOOL     // RValueType kind
            }
        };

        surface_depth_disable(&ret, NULL, NULL, 1, &args[0]);
        warning("HACK:: Disabled surface depth.\n");
    } else {
        warning("Depth disable hack requested but surface_depth_disable unusable.\n");
    }

}

int UsesRefStrings()
{
    return g_nInstanceVariables != NULL;
}

void YYCreateStringHelper(RValue *rval, const char *str)
{
    if (UsesRefStrings())
    {
        if (YYCreateString != NULL)
        {
            YYCreateString(rval, str);
        }
        else // need to create it manually...
        {
            YYThingDerefHelper(rval);
            Ref *ref = (Ref*)malloc(sizeof(*ref));
            *ref = (Ref){
                .m_thing = strdup(str),
                .m_refCount = 1,
                .m_size = strlen(str)
            };

            rval->kind = VALUE_STRING;
            rval->rvalue.str = ref;
        }
    }
    else // very old runners, strings werent refs
    {
        YYThingDerefHelper(rval);
        rval->kind = VALUE_STRING;
        rval->rvalue.str = (Ref*)strdup(str);
    }
}

void YYThingDerefHelper(RValue *rval)
{
    if (rval->kind != VALUE_STRING && rval->kind != VALUE_REF)
        return;

    if (UsesRefStrings())
    {
        if (_RefThing__dec)
        {
            _RefThing__dec(rval);
        }
        else if (rval->rvalue.str)
        {
            free(rval->rvalue.str->m_thing);
            rval->rvalue.str->m_thing = NULL;
            free(rval->rvalue.str);
            rval->rvalue.str = NULL;
            rval->kind = VALUE_UNSET;
        }
    }
    else if (rval->rvalue.str)
    {
        free(rval->rvalue.str);
        rval->rvalue.str = NULL;
        rval->kind = VALUE_UNSET;
    }
}

const char *YYGetCStrHelper(struct RValue *rval, int idx)
{
    if (UsesRefStrings())
        return (const char *)rval[idx].rvalue.str->m_thing;
    else
        return (const char *)rval[idx].rvalue.str;
}