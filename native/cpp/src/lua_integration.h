#ifndef LUA_INTEGRATION_H
#define LUA_INTEGRATION_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>

// Minimal Lua integration without external library
namespace godot {

class LuaScriptEngine : public RefCounted {
    GDCLASS(LuaScriptEngine, RefCounted)

private:
    Dictionary variables;
    Dictionary functions;
    
protected:
    static void _bind_methods();

public:
    LuaScriptEngine();
    ~LuaScriptEngine();
    
    // Execute Lua-like script
    String execute(const String& script);
    
    // Set/Get variables
    void set_variable(const String& name, Variant value);
    Variant get_variable(const String& name);
    
    // Register GDScript callable
    void register_function(const String& name, Callable function);
    
    // Call registered function
    Variant call_function(const String& name, Array args);
    
    // Parse simple expressions
    float evaluate_expression(const String& expression);
    
    // C API for Lua
    static const char* execute_lua(const char* script);
    static void lua_set_global(const char* name, float value);
    static float lua_get_global(const char* name);
};

// C interface
extern "C" {
    const char* lua_execute_script(const char* script);
    float lua_eval(const char* expression);
    void lua_set_var(const char* name, float value);
    float lua_get_var(const char* name);
}

}

#endif // LUA_INTEGRATION_H
