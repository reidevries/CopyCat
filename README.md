# CopyCat
This is a small game engine I'm working on in C++17, for a tactical RPG I'm designing. I'm nearly finished with a computer science degree but I'm still not an expert on C++ or game engine architecture so probably dont use this as a reference. I'm keeping the scope small, preferring it to be able to run on older PCs or consoles rather than implementing complex features, I'm trying to make a game not compete with Godot. There will be no physics or 3D models.

### Current features:
* Uses raylib (a plain C library) for rendering (and in the future, input and audio)
* Uses EnTT (a C++ library) for its efficient Entity-Component-System
* Sprite-based 3D rendering - Every in-game sprite is drawn on a freely-transformable quadrilateral, Sega Saturn inspired
* Efficient, small scale resource manager for asynchronous image/audio loading
* Level saving/loading to/from json using nlohmann::json library

### Some goals:
* Mouse-based level editing within the engine, using Raygui for the user interface
* Dynamic music engine, probably based on .xm filetype for the small filesize
* Put resource manager on a separate thread

### Building it:
Ive only tested it on linux machines, but theoretically it should be simple to build on other systems. Just run cmake configure->generate->build then do ./build/copycat to run the executable
