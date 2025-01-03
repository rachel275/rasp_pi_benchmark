## OpenGL 

### libs
```bash
sudo apt install libglew-dev
sudo apt install libgl1-mesa-dev
sudo apt install libglfw3-dev
```

### compile
```bash
g++ opengl.cpp -o opengl -lGL -lGLEW -lglfw
```
**Note:** Shader files `vertex_shader.glsl` and `MatrixMultiplicationShader.glsl` are in the same directory as your program.

### run
```bash
./opengl N
```
- **N**: matrix size
- Currently, **Maximum N** is 1024. For larger matrix sizes, we will need to adjust the window width/height



