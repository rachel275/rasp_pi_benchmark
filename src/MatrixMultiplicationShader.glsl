#version 300 es
precision highp float;
precision mediump int;

in vec2 TexCoords; // Texture coordinates for the current fragment
out vec4 FragColor;

uniform sampler2D matrixA; // matrix as texture (10x10)
uniform sampler2D matrixB; 
uniform int N;

void main() {
    int row = int(TexCoords.x * float(N));
    int col = int(TexCoords.y * float(N));

    float result = 0.0;

    for (int k = 0; k < N; ++k) {
        float a = texture(matrixA, vec2(float(row) / float(N), float(k) / float(N))).r;
        float b = texture(matrixB, vec2(float(k) / float(N), float(col) / float(N))).r;
        result += a * b;
    }
    
    // Red channel write the result
    FragColor = vec4(result, 0.0, 0.0, 0.0);
}
