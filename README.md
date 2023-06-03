# Holona
FPS starter for Godot using C++ GDExtension at the moment.
Based upon Whinfoome's [First Person Starter](https://github.com/Whimfoome/godot-FirstPersonStarter/) repository.

GDExtension file is written for the current master branch of Godot Engine (minimum version number is marked 4.1).

## Compilation
1. Clone [godot-cpp](https://github.com/godotengine/godot-cpp) to the repository root **USING** the name of _Include_!
   Alternatively you can have godot-cpp somewhere else in your system and make symbolic link named _Include_ in the folder.
2. run scons. Optionally you can give `custom_api_file=<path>` as a argument to give godot-cpp's API generator.
   For example compile with API JSON file named _godot_api.json_ in parent directory:
   ```shell
   scons custom_api_file="../godot_api.json"
   ```
3. Open project file in Godot.
