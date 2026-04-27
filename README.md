# Zombie Shooter 3D

A 3D zombie shooter game built with Godot 4.3 featuring native C/C++ modules, Kotlin Android integration, and Lua scripting.

## Features

- **3D Graphics**: Real-time 3D rendering with shadows, lighting, and post-processing
- **Native Performance**: Critical game logic in optimized C/C++
- **Android Integration**: Kotlin-based Android plugins for native features
- **Scriptable**: Lua scripts for game configuration and AI behavior
- **Physics**: Custom physics calculations in C for high performance

## Project Structure

```
ZombieShooterGodot/
├── android/              # Android-specific code
│   └── plugin/          # Kotlin Android plugin
├── native/              # Native modules
│   ├── cpp/            # C/C++ GDExtension
│   │   ├── src/       # Source files
│   │   ├── bin/       # Compiled libraries
│   │   └── SConstruct # Build configuration
│   └── lua/           # Lua scripts
│       └── scripts/   # Game scripts
├── scenes/             # Godot scene files
├── scripts/            # GDScript files
├── materials/          # Materials and shaders
└── build/             # Build outputs
```

## Native Modules

### C++ GDExtension (`native/cpp/`)

- **NativeEnemyController**: High-performance enemy AI
- **NativePhysics**: Optimized physics calculations
- **LuaScriptEngine**: Embedded Lua scripting support

Build with:
```bash
cd native/cpp
chmod +x build.sh
./build.sh
```

### Kotlin Android Plugin (`android/plugin/`)

Features:
- Device information
- Toast messages
- Score sharing
- Vibration control
- High score persistence

### Lua Scripts (`native/lua/scripts/`)

- **enemy_ai.lua**: Enemy AI behavior
- **game_config.lua**: Game balance and settings

## Building

### Prerequisites
- Godot 4.3+
- Android SDK (for mobile builds)
- SCons (for native module builds)
- Kotlin (for Android plugin)

### Export for Android
```bash
godot --headless --export-release "Android" "build/ZombieShooter.apk"
```

## Controls

- **WASD**: Move
- **Mouse**: Look
- **Click**: Shoot
- **Space**: Jump

## License

MIT License
