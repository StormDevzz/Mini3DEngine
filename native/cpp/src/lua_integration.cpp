#include "lua_integration.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/utility/utility_functions.hpp>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace godot;

static Dictionary lua_globals;
static String last_result = "";

LuaScriptEngine::LuaScriptEngine() {
}

LuaScriptEngine::~LuaScriptEngine() {
}

void LuaScriptEngine::_bind_methods() {
    ClassDB::bind_method(D_METHOD("execute", "script"), &LuaScriptEngine::execute);
    ClassDB::bind_method(D_METHOD("set_variable", "name", "value"), &LuaScriptEngine::set_variable);
    ClassDB::bind_method(D_METHOD("get_variable", "name"), &LuaScriptEngine::get_variable);
    ClassDB::bind_method(D_METHOD("register_function", "name", "function"), &LuaScriptEngine::register_function);
    ClassDB::bind_method(D_METHOD("call_function", "name", "args"), &LuaScriptEngine::call_function);
    ClassDB::bind_method(D_METHOD("evaluate_expression", "expression"), &LuaScriptEngine::evaluate_expression);
}

// Simple expression parser for basic math
float LuaScriptEngine::evaluate_expression(const String& expression) {
    String expr = expression;
    expr = expr.replace(" ", "");
    
    // Handle variables
    for (const Variant& key : lua_globals.keys()) {
        String var_name = key;
        float value = lua_globals[key];
        expr = expr.replace(var_name, String::num(value));
    }
    
    // Very basic parser - just handle simple operations
    // This is a simplified version - real Lua would need a full parser
    float result = 0.0f;
    
    // Try to evaluate simple numeric expressions
    if (expr.contains("+")) {
        PackedStringArray parts = expr.split("+");
        for (int i = 0; i < parts.size(); i++) {
            result += parts[i].to_float();
        }
    } else if (expr.contains("-")) {
        PackedStringArray parts = expr.split("-");
        if (parts.size() > 0) result = parts[0].to_float();
        for (int i = 1; i < parts.size(); i++) {
            result -= parts[i].to_float();
        }
    } else if (expr.contains("*")) {
        PackedStringArray parts = expr.split("*");
        if (parts.size() > 0) result = parts[0].to_float();
        for (int i = 1; i < parts.size(); i++) {
            result *= parts[i].to_float();
        }
    } else if (expr.contains("/")) {
        PackedStringArray parts = expr.split("/");
        if (parts.size() > 0) result = parts[0].to_float();
        for (int i = 1; i < parts.size(); i++) {
            float divisor = parts[i].to_float();
            if (divisor != 0) result /= divisor;
        }
    } else {
        result = expr.to_float();
    }
    
    return result;
}

String LuaScriptEngine::execute(const String& script) {
    // Simple script execution
    // Handle variable assignment: x = 10
    if (script.contains("=") && !script.contains("==")) {
        int eq_pos = script.find("=");
        String var_name = script.substr(0, eq_pos).replace(" ", "");
        String expr = script.substr(eq_pos + 1);
        float value = evaluate_expression(expr);
        set_variable(var_name, value);
        last_result = String::num(value);
        return last_result;
    }
    // Handle return
    else if (script.contains("return")) {
        String expr = script.replace("return", "").replace(" ", "");
        float value = evaluate_expression(expr);
        last_result = String::num(value);
        return last_result;
    }
    // Simple expression
    else {
        float value = evaluate_expression(script);
        last_result = String::num(value);
        return last_result;
    }
}

void LuaScriptEngine::set_variable(const String& name, Variant value) {
    variables[name] = value;
    lua_globals[name] = value;
}

Variant LuaScriptEngine::get_variable(const String& name) {
    if (variables.has(name)) {
        return variables[name];
    }
    return Variant();
}

void LuaScriptEngine::register_function(const String& name, Callable function) {
    functions[name] = function;
}

Variant LuaScriptEngine::call_function(const String& name, Array args) {
    if (functions.has(name)) {
        Callable func = functions[name];
        return func.callv(args);
    }
    return Variant();
}

// C API implementation
extern "C" {

const char* lua_execute_script(const char* script) {
    String s(script);
    LuaScriptEngine engine;
    String result = engine.execute(s);
    
    // Store in static buffer (not thread-safe but simple)
    static char buffer[256];
    strncpy(buffer, result.utf8().get_data(), 255);
    buffer[255] = '\0';
    return buffer;
}

float lua_eval(const char* expression) {
    String expr(expression);
    LuaScriptEngine engine;
    return engine.evaluate_expression(expr);
}

void lua_set_var(const char* name, float value) {
    String var_name(name);
    lua_globals[var_name] = value;
}

float lua_get_var(const char* name) {
    String var_name(name);
    if (lua_globals.has(var_name)) {
        return lua_globals[var_name];
    }
    return 0.0f;
}

}
