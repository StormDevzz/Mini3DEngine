#ifndef NATIVE_MODULE_H
#define NATIVE_MODULE_H

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

// C++ Native Enemy Controller
class NativeEnemyController : public Node {
    GDCLASS(NativeEnemyController, Node)

private:
    float speed = 3.0f;
    float health = 100.0f;
    Vector3 target_position;
    
protected:
    static void _bind_methods();

public:
    NativeEnemyController();
    ~NativeEnemyController();
    
    void _process(double delta) override;
    void _ready() override;
    
    void set_speed(float p_speed);
    float get_speed() const;
    
    void set_health(float p_health);
    float get_health() const;
    
    void set_target_position(Vector3 p_position);
    Vector3 get_target_position() const;
    
    void take_damage(float damage);
    bool is_alive() const;
    
    // C-style function callback
    typedef void (*CallbackFunction)(const char* message);
    void set_callback(CallbackFunction callback);
};

// C++ Native Physics
class NativePhysics : public RefCounted {
    GDCLASS(NativePhysics, RefCounted)

protected:
    static void _bind_methods();

public:
    NativePhysics();
    
    static Vector3 calculate_velocity(Vector3 current, Vector3 target, float speed, double delta);
    static float calculate_distance(Vector3 a, Vector3 b);
    static Vector3 normalize_direction(Vector3 from, Vector3 to);
};

// C interface for external calls
extern "C" {
    typedef struct {
        float x, y, z;
    } Vec3;
    
    // C API functions
    void* native_enemy_create();
    void native_enemy_destroy(void* enemy);
    void native_enemy_set_position(void* enemy, Vec3 pos);
    Vec3 native_enemy_get_position(void* enemy);
    void native_enemy_update(void* enemy, float delta);
    
    // Lua integration
    const char* lua_execute(const char* script);
    void lua_register_function(const char* name, void (*func)());
}

}

#endif // NATIVE_MODULE_H
