#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define N 1024  // Size of the matrix
#define NUM_THREADS 4 // Number of CPU threads to use

// Shader source code for matrix multiplication
const char* vertexShaderSource = """
#version 330 core
layout(location = 0) in vec4 position;
void main() {
    gl_Position = position;
}
""";

const char* computeShaderSource = """
#version 430 core
layout (local_size_x = 16, local_size_y = 16) in;
layout (std430, binding = 0) buffer MatrixA {
    float A[];
};
layout (std430, binding = 1) buffer MatrixB {
    float B[];
};
layout (std430, binding = 2) buffer MatrixC {
    float C[];
};

uniform int n;

void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    if (id.x < n && id.y < n) {
        float sum = 0.0;
        for (int i = 0; i < n; i++) {
            sum += A[id.y * n + i] * B[i * n + id.x];
        }
        C[id.y * n + id.x] = sum;
    }
}
""";

void initializeMatrix(float *matrix, int size) {
    for (int i = 0; i < size; i++) {
        matrix[i] = rand() % 100;  // Random values between 0 and 99
    }
}

void checkShaderCompilation(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader Compilation Error: %s\n", infoLog);
    }
}

void checkProgramLinking(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("Program Linking Error: %s\n", infoLog);
    }
}

typedef struct {
    float *A;
    float *B;
    float *C;
    int start_row;
    int end_row;
} ThreadData;

void* cpuMatrixMultiplication(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    float *A = data->A;
    float *B = data->B;
    float *C = data->C;
    int start_row = data->start_row;
    int end_row = data->end_row;
    
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < N; j++) {
            float sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
    return NULL;
}

int main() {
    float *h_A, *h_B, *h_C;
    size_t size = N * N * sizeof(float);

    h_A = (float *)malloc(size);
    h_B = (float *)malloc(size);
    h_C = (float *)malloc(size);

    initializeMatrix(h_A, N * N);
    initializeMatrix(h_B, N * N);

    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL and CPU Matrix Multiplication", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &computeShaderSource, NULL);
    glCompileShader(computeShader);
    checkShaderCompilation(computeShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, computeShader);
    glLinkProgram(shaderProgram);
    checkProgramLinking(shaderProgram);

    GLuint bufferA, bufferB, bufferC;
    glGenBuffers(1, &bufferA);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferA);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, h_A, GL_STATIC_DRAW);
    
    glGenBuffers(1, &bufferB);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferB);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, h_B, GL_STATIC_DRAW);
    
    glGenBuffers(1, &bufferC);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferC);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_STATIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bufferA);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, bufferB);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, bufferC);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "n"), N);

    glDispatchCompute((GLuint)N / 16, (GLuint)N / 16, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];

    int rows_per_thread = N / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].A = h_A;
        threadData[i].B = h_B;
        threadData[i].C = h_C;
        threadData[i].start_row = i * rows_per_thread;
        threadData[i].end_row = (i == NUM_THREADS - 1) ? N : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, cpuMatrixMultiplication, (void*)&threadData[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    glDeleteBuffers(1, &bufferA);
    glDeleteBuffers(1, &bufferB);
    glDeleteBuffers(1, &bufferC);
    glDeleteProgram(shaderProgram);
    glDeleteShader(computeShader);
    glfwDestroyWindow(window);
    glfwTerminate();

    free(h_A);
    free(h_B);
    free(h_C);

    return 0;
}
