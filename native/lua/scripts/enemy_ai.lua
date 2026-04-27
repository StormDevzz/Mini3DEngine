-- Enemy AI Script for Zombie Shooter
-- This script controls basic enemy behavior

-- Enemy state
enemy = {
    health = 100,
    speed = 3.0,
    damage = 10,
    detection_range = 20.0,
    attack_range = 1.5,
    state = "idle"  -- idle, chase, attack, dead
}

-- Initialize enemy
function init_enemy(params)
    enemy.health = params.health or 100
    enemy.speed = params.speed or 3.0
    enemy.damage = params.damage or 10
    enemy.state = "idle"
    return "Enemy initialized with health: " .. enemy.health
end

-- Calculate distance between two points
function distance(x1, z1, x2, z2)
    local dx = x2 - x1
    local dz = z2 - z1
    return math.sqrt(dx * dx + dz * dz)
end

-- Normalize direction vector
function normalize_direction(from_x, from_z, to_x, to_z)
    local dx = to_x - from_x
    local dz = to_z - from_z
    local dist = math.sqrt(dx * dx + dz * dz)
    
    if dist > 0.001 then
        return dx / dist, dz / dist
    end
    return 0, 0
end

-- Update enemy AI
function update_ai(enemy_x, enemy_z, player_x, player_z, delta)
    local dist = distance(enemy_x, enemy_z, player_x, player_z)
    
    -- State machine
    if enemy.state == "idle" then
        if dist < enemy.detection_range then
            enemy.state = "chase"
            return "chase", 0, 0
        end
        return "idle", 0, 0
        
    elseif enemy.state == "chase" then
        if dist < enemy.attack_range then
            enemy.state = "attack"
            return "attack", 0, 0
        elseif dist > enemy.detection_range * 1.5 then
            enemy.state = "idle"
            return "idle", 0, 0
        end
        
        -- Move towards player
        local dx, dz = normalize_direction(enemy_x, enemy_z, player_x, player_z)
        local move_x = dx * enemy.speed * delta
        local move_z = dz * enemy.speed * delta
        
        return "chase", move_x, move_z
        
    elseif enemy.state == "attack" then
        if dist > enemy.attack_range * 1.2 then
            enemy.state = "chase"
            return "chase", 0, 0
        end
        -- Deal damage
        return "attack", 0, 0, enemy.damage
    end
    
    return enemy.state, 0, 0
end

-- Take damage
function take_damage(amount)
    enemy.health = enemy.health - amount
    if enemy.health <= 0 then
        enemy.health = 0
        enemy.state = "dead"
        return true, "Enemy killed!"
    end
    return false, "Enemy health: " .. enemy.health
end

-- Get current state
function get_state()
    return enemy.state
end

-- Get health
function get_health()
    return enemy.health
end

-- Spawn wave of enemies
function spawn_wave(wave_number)
    local count = 5 + (wave_number * 2)
    local health_mult = 1.0 + (wave_number * 0.1)
    local speed_mult = 1.0 + (wave_number * 0.05)
    
    return {
        count = count,
        health_multiplier = health_mult,
        speed_multiplier = speed_mult,
        message = "Wave " .. wave_number .. " spawning " .. count .. " enemies!"
    }
end

-- Calculate score
function calculate_score(kills, wave, time_bonus)
    local base_score = kills * 100
    local wave_bonus = wave * 50
    local total = base_score + wave_bonus + time_bonus
    return total
end
