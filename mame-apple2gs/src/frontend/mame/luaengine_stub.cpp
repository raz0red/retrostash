// Stub implementations of lua_engine for emscripten builds without Lua support
#include "emu.h"
#include "luaengine.h"

lua_engine::lua_engine() { }
lua_engine::~lua_engine() { }
bool lua_engine::frame_hook() { return false; }
void lua_engine::initialize() { }
void lua_engine::set_machine(running_machine *machine) { }
void lua_engine::attach_notifiers() { }
void lua_engine::on_periodic() { }
void lua_engine::on_machine_before_load_settings() { }
sol::environment lua_engine::make_environment() { return sol::environment(); }
bool lua_engine::on_missing_mandatory_image(const std::string &instance_name) { return false; }
void lua_engine::on_sound_update(const std::map<std::string, std::vector<std::pair<const sound_stream::sample_t *, int>>> &sound) { }
sol::load_result lua_engine::load_script(std::string const &filename) { return sol::load_result(); }
sol::load_result lua_engine::load_string(std::string const &value) { return sol::load_result(); }
sol::object lua_engine::call_plugin(const std::string &name, sol::object in) { return sol::object(); }
std::optional<long> lua_engine::menu_populate(const std::string &menu, std::vector<std::tuple<std::string, std::string, std::string>> &menu_list, std::string &flags) { return std::nullopt; }
std::pair<bool, std::optional<long>> lua_engine::menu_callback(const std::string &menu, int index, const std::string &event) { return {false, std::nullopt}; }
