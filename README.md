# Compiling
## Linux
Enter `make -jN` in the root of the project directory.

You will need to make sure the following libraries are available on your system.

- OpenGL
- GLFW
- Assimp

For [Assimp](https://github.com/assimp/assimp) you can clone the repo, build it following the instruction in their README and simply copy the shared library files into this projects **lib/** directory. There will only be one library file when you build Assimp and two files symbolically linked like so:

`libassimp (symbolic link) => libassimp.so.X (symbolic link) => libassimp.so.X.X.X (acutal lib)`

Make sure to maintain this linking structure. Check out this projects `Makefile` to see how I did that.

You might be able to install Assimp from your distros repository if you don't want to build it yourself, but I haven't tried this so I can't say if it will work or not.

GLFW requires a bunch of libraries as dependancies. Checkout the this [StackOverflow post](https://stackoverflow.com/questions/17768008/how-to-build-install-glfw-3-and-use-it-in-a-linux-project) to see which ones.

## Windows
I am not sure how to get this to run on Windows but I can give some suggestions based on how the project is structured. Read about the required libraries in the *Compiling and Running on Linux* section above.

- **src/** contains all the `.cpp` and personal `.h` files.
- **inc/** contains all the vendor `.h` files.
- **lib/** should contain the dynamic library files which are not already include in your `PATH`.
- **rsc/** contains the assets (models, shaders and textures) which will be symbollically linked in the **bin/** folder when compiling. This step must happen, otherwise the files will need to be copied into the **bin/** directory because the executable accesses these assuming they are symbolically linked relative to the executable.
- **obj/** an intermediary build folder for `.o` files. Not required but keeps **bin/** clean.
- **bin/** and **lib/** are for the outputs of compiling. You will find the executable in **bin/**.

# Running
Head into the **bin/** directory and enter `./myapp <model dir> <texture dir>`, where the arguments will be `models/ images/` if the files/folders in **rsc/** are properly symbolically linked,

# Controls
- Rotations *W, A, S, D, E, Q*.
- Zoom in *Z*.
- Zoom out *X*.
- Camera Movement *SHIFT + W, A, S, D, E, Q*.
- Camera Direction *MOVE CURSOR*.
- Select Model *1-6*.
- Toggle Depth/Orientation based mapping *F*
- Increase / Decrease Orientation r *G / SHIFT+G*
- Increase / Decrease Depth r *R / SHIFT+R*
- Increase / Decrease Depth zmin *T / SHIFT+T*

# Depth Based Attribute Mapping
The closest I could get to make the terrain look like the examples provided was by setting *r* to around 3 and *zmin* to around 0.1. You will have to move the camera from its default position to be closer to the terrain to see comparable results.
