#include "native_module.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/utility/utility_functions.hpp>
#include <cmath>

using namespace godot;

// ==================== NativeEnemyController ====================

NativeEnemyController::NativeEnemyController() {
    speed = 3.0f;
    health = 100.0f;
    target_position = Vector3(0, 0, 0);
}

NativeEnemyController::~NativeEnemyController() {
}

void NativeEnemyController::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &NativeEnemyController::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &NativeEnemyController::get_speed);
    ClassDB::bind_method(D_METHOD("set_health", "health"), &NativeEnemyController::set_health);
    ClassDB::bind_method(D_METHOD("get_health"), &NativeEnemyController::get_health);
    ClassDB::bind_method(D_METHOD("set_target_position", "position"), &NativeEnemyController::set_target_position);
    ClassDB::bind_method(D_METHOD("get_target_position"), &NativeEnemyController::get_target_position);
    ClassDB::bind_method(D_METHOD("take_damage", "damage"), &NativeEnemyController::take_damage);
    ClassDB::bind_method(D_METHOD("is_alive"), &NativeEnemyController::is_alive);
    
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "health"), "set_health", "get_health");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "target_position"), "set_target_position", "get_target_position");
}

void NativeEnemyController::_ready() {
    // Initialize from C++
    utility_functions::print("NativeEnemyController initialized from C++");
}

void NativeEnemyController::_process(double delta) {
    // Native pathfinding logic
    if (health > 0) {
        Vector3 current_pos = get_position();
        Vector3 direction = NativePhysics::normalize_direction(current_pos, target_position);
        
        // Simple movement towards target
        float move_distance = speed * delta;
        float total_distance = NativePhysics::calculate_distance(current_pos, target_position);
        
        if (total_distance > 0.5f) {
            Vector3 new_pos = current_pos + direction * move_distance;
            set_position(new_pos);
        }
    }
}

void NativeEnemyController::set_speed(float p_speed) {
    speed = p_speed;
}

float NativeEnemyController::get_speed() const {
    return speed;
}

void NativeEnemyController::set_health(float p_health) {
    health = p_health;
}

float NativeEnemyController::get_health() const {
    return health;
}

void NativeEnemyController::set_target_position(Vector3 p_position) {
    target_position = p_position;
}

Vector3 NativeEnemyController::get_target_position() const {
    return target_position;
}

void NativeEnemyController::take_damage(float damage) {
    health -= damage;
    if (health < 0) health = 0;
}

bool NativeEnemyController::is_alive() const {
    return health > 0;
}

// ==================== NativePhysics ====================

NativePhysics::NativePhysics() {
}

void NativePhysics::_bind_methods() {
    ClassDB::bind_static_method("NativePhysics", D_METHOD("calculate_velocity", "current", "target", "speed", "delta"), &NativePhysics::calculate_velocity);
    ClassDB::bind_static_method("NativePhysics", D_METHOD("calculate_distance", "a", "b"), &NativePhysics::calculate_distance);
    ClassDB::bind_static_method("NativePhysics", D_METHOD("normalize_direction", "from", "to"), &NativePhysics::normalize_direction);
}

Vector3 NativePhysics::calculate_velocity(Vector3 current, Vector3 target, float speed, double delta) {
    Vector3 direction = normalize_direction(current, target);
    return direction * speed * delta;
}

float NativePhysics::calculate_distance(Vector3 a, Vector3 b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

Vector3 NativePhysics::normalize_direction(Vector3 from, Vector3 to) {
    Vector3 diff = to - from;
    float length = std::sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);
    if (length > 0.0001f) {
        return Vector3(diff.x/length, diff.y/length, diff.z/length);
    }
    return Vector3(0, 0, 0);
}

// ==================== C API ====================

extern "C" {

void* native_enemy_create() {
    return new NativeEnemyController();
}

void native_enemy_destroy(void* enemy) {
    if (enemy) {
        delete static_cast<NativeEnemyController*>(enemy);
    }
}

void native_enemy_set_position(void* enemy, Vec3 pos) {
    if (enemy) {
        static_cast<NativeEnemyController*>(enemy)->set_position(Vector3(pos.x, pos.y, pos.z));
    }
}

Vec3 native_enemy_get_position(void* enemy) {
    Vec3 result = {0, 0, 0};
    if (enemy) {
        Vector3 pos = static_cast<NativeEnemyController*>(enemy)->get_position();
        result.x = pos.x;
        result.y = pos.y;
        result.z = pos.z;
    }
    return result;
}

void native_enemy_update(void* enemy, float delta) {
    if (enemy) {
        static_cast<NativeEnemyController*>(enemy)->_process(delta);
    }
}

}
