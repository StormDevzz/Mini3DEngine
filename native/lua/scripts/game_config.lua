-- Game Configuration Script
-- Modify these values to balance the game

config = {
    -- Player settings
    player = {
        health = 100,
        speed = 8.0,
        sprint_multiplier = 1.5,
        jump_force = 4.5,
        mouse_sensitivity = 0.002,
        
        -- Weapon settings
        weapon = {
            damage = 25,
            fire_rate = 0.15,  -- seconds between shots
            range = 100,
            ammo_capacity = 30,
            reload_time = 1.5,
            accuracy = 0.95  -- 0-1, higher is more accurate
        }
    },
    
    -- Enemy settings
    enemy = {
        -- Basic zombie
        zombie_basic = {
            health = 50,
            speed = 3.0,
            damage = 10,
            detection_range = 25,
            attack_range = 1.5,
            attack_cooldown = 1.0,
            xp_reward = 10
        },
        
        -- Fast zombie
        zombie_fast = {
            health = 30,
            speed = 5.0,
            damage = 8,
            detection_range = 30,
            attack_range = 1.2,
            attack_cooldown = 0.7,
            xp_reward = 15
        },
        
        -- Tank zombie
        zombie_tank = {
            health = 150,
            speed = 1.5,
            damage = 20,
            detection_range = 20,
            attack_range = 2.0,
            attack_cooldown = 2.0,
            xp_reward = 30
        },
        
        -- Boss zombie
        zombie_boss = {
            health = 500,
            speed = 2.0,
            damage = 35,
            detection_range = 40,
            attack_range = 3.0,
            attack_cooldown = 1.5,
            xp_reward = 100,
            special_abilities = {"ground_slam", "summon"}
        }
    },
    
    -- Wave settings
    waves = {
        spawn_interval = 2.0,  -- seconds between spawns
        time_between_waves = 10.0,
        max_enemies_at_once = 20,
        
        -- Difficulty scaling
        health_multiplier_per_wave = 0.1,
        speed_multiplier_per_wave = 0.05,
        damage_multiplier_per_wave = 0.1,
        spawn_count_base = 5,
        spawn_count_per_wave = 2
    },
    
    -- Game settings
    game = {
        day_length = 600,  -- seconds (10 minutes)
        night_length = 300,  -- seconds (5 minutes)
        
        -- Weather chances (0-1)
        weather = {
            clear = 0.6,
            cloudy = 0.25,
            foggy = 0.1,
            stormy = 0.05
        },
        
        -- Loot drops
        loot = {
            health_pack_chance = 0.15,
            ammo_pack_chance = 0.25,
            weapon_upgrade_chance = 0.05
        }
    },
    
    -- Graphics settings
    graphics = {
        shadow_quality = "high",  -- low, medium, high, ultra
        draw_distance = 300,
        fog_density = 0.02,
        bloom_enabled = true,
        ssao_enabled = true
    },
    
    -- Audio settings
    audio = {
        master_volume = 1.0,
        music_volume = 0.7,
        sfx_volume = 1.0,
        ambient_volume = 0.8,
        
        -- 3D audio
        enable_spatial_audio = true,
        doppler_effect = true
    },
    
    -- Control settings
    controls = {
        invert_mouse_y = false,
        controller_support = true,
        vibration_enabled = true,
        aim_assist = 0.3,  -- 0-1, console only
        
        -- Key bindings (Godot action names)
        bindings = {
            move_forward = "ui_up",
            move_back = "ui_down",
            move_left = "ui_left",
            move_right = "ui_right",
            jump = "ui_select",
            shoot = "shoot",
            reload = "reload",
            interact = "interact",
            pause = "ui_cancel"
        }
    }
}

-- Get config value by path (e.g., "player.health")
function get_config(path)
    local keys = {}
    for key in string.gmatch(path, "[^.]+") do
        table.insert(keys, key)
    end
    
    local current = config
    for _, key in ipairs(keys) do
        if type(current) == "table" and current[key] ~= nil then
            current = current[key]
        else
            return nil
        end
    end
    
    return current
end

-- Set config value
function set_config(path, value)
    local keys = {}
    for key in string.gmatch(path, "[^.]+") do
        table.insert(keys, key)
    end
    
    local current = config
    for i = 1, #keys - 1 do
        if type(current[keys[i]]) ~= "table" then
            current[keys[i]] = {}
        end
        current = current[keys[i]]
    end
    
    current[keys[#keys]] = value
    return true
end

-- Export config to JSON-like format
function export_config()
    return config
end

-- Print config for debugging
function print_config()
    print("=== Game Configuration ===")
    print("Player Health: " .. config.player.health)
    print("Player Speed: " .. config.player.speed)
    print("Weapon Damage: " .. config.player.weapon.damage)
    print("Max Enemies: " .. config.waves.max_enemies_at_once)
    print("==========================")
end
