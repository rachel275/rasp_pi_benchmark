#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #include <glad.h>

//#include "ComputeShader.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>

#define GLEW_STATIC

// const GLuint WIDTH = 800, HEIGHT = 600;
const GLuint WIDTH = 1024, HEIGHT = 1024;
GLuint N = 512; //(512x512)

GLuint CompileShader(const char* shaderCode, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, nullptr);
    // compile
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compilation Failed: " << infoLog << std::endl;
        return 0;
    }
    return shader;
}


GLuint CreateShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) {
    // read shader code
    std::ifstream vertexFile(vertexShaderPath);
    std::stringstream vertexStream;
    vertexStream << vertexFile.rdbuf();
    std::string vertexCode = vertexStream.str();

    std::ifstream fragmentFile(fragmentShaderPath);
    std::stringstream fragmentStream;
    fragmentStream << fragmentFile.rdbuf();
    std::string fragmentCode = fragmentStream.str();

    GLuint vertexShader = CompileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

    // link shaders to the program
    // create a program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // // Bind attribute locations
    // glBindAttribLocation(shaderProgram, 0, "aPosition");
    // glBindAttribLocation(shaderProgram, 1, "aTexCoord");

    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Program Linking Failed: " << infoLog << std::endl;
        return 0;
    }

    // already linked, can delete the shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void printMatrix(std::vector<float>& matrix, size_t rows, size_t cols) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            std::cout << matrix[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }
}

GLfloat quadVertices[] = {
    // Positions   // Texture Coords
    -1.0f,  1.0f,   0.0f, 1.0f, // Top-left
    -1.0f, -1.0f,   0.0f, 0.0f, // Bottom-left
     1.0f, -1.0f,   1.0f, 0.0f, // Bottom-right

    -1.0f,  1.0f,   0.0f, 1.0f, // Top-left
     1.0f, -1.0f,   1.0f, 0.0f, // Bottom-right
     1.0f,  1.0f,   1.0f, 1.0f  // Top-right
};

int main(int argc, char *argv[]) {

    if (argc > 1) {
        N = std::stoi(argv[1]);
    }

    glfwInit();

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Matrix Multiplication", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    GLuint shaderProgram = CreateShaderProgram("vertex_shader.glsl", "MatrixMultiplicationShader.glsl");

    //glViewport(0, 0, WIDTH, HEIGHT);

    // Define matrices A and B (10x10)
    std::vector<float> matrixA(N*N, 10.0f);  // 10x10 matrix filled with 1.0f
    std::vector<float> matrixB(N*N, 10.0f);  // 10x10 matrix filled with 2.0f
    std::vector<float> result(N*N, 0.0f);   // Output matrix C initialized to 0


    // printMatrix(matrixA, 10, 10);
    // printMatrix(matrixB, 10, 10);

    GLuint textureA, textureB, outputTexture;

    // Create texture for matrix A
    glGenTextures(1, &textureA);
    glBindTexture(GL_TEXTURE_2D, textureA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, N, N, 0, GL_RED, GL_FLOAT, matrixA.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Create texture for matrix B
    glGenTextures(1, &textureB);
    glBindTexture(GL_TEXTURE_2D, textureB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, N, N, 0, GL_RED, GL_FLOAT, matrixB.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Create output texture for matrix C
    glGenTextures(1, &outputTexture);
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, N, N, 0, GL_RED, GL_FLOAT, nullptr);

    
    // Create framebuffer and attach output texture
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind and buffer the vertex data
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind the VAO
    

    // Use the shader program
    auto start = std::chrono::high_resolution_clock::now();
    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureA);

    // set varianles in shader file
    glUniform1i(glGetUniformLocation(shaderProgram, "matrixA"), 0);

    glUniform1i(glGetUniformLocation(shaderProgram, "N"), N);
    glUniform1i(glGetUniformLocation(shaderProgram, "blockSize"), 16);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureB);
    glUniform1i(glGetUniformLocation(shaderProgram, "matrixB"), 1);

    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, outputTexture);
    // glUniform1i(glGetUniformLocation(shaderProgram, "outputMatrixC"), 2);
    
    glBindVertexArray(VAO);
    //auto start_exe = std::chrono::high_resolution_clock::now();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // auto end_exe = std::chrono::high_resolution_clock::now();
    // read result from the output texture
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glReadPixels(0, 0, N, N, GL_RED, GL_FLOAT, result.data());

    auto end = std::chrono::high_resolution_clock::now();
    
    // std::cout << "Result Matrix C:" << std::endl;
    // printMatrix(result, N, N);
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    // std::chrono::duration<double> duration2 = end_exe - start_exe;
    // std::chrono::duration<double> duration3 = end - end_exe;

    std::cout << "time = " << duration_us.count() << " us\n";
    // std::cout << "computation time: " << duration2.count() * 1000 << " ms\n";
    // std::cout << "write time: " << duration3.count() * 1000 << " ms\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteTextures(1, &textureA);
    glDeleteTextures(1, &textureB);
    glDeleteTextures(1, &outputTexture);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}


