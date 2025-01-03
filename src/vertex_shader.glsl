#version 300 es
in vec4 aPosition;
in vec2 aTexCoord;
out vec2 TexCoords;

void main() {
    gl_Position = aPosition;
    TexCoords = aTexCoord;
}


