#include <iostream>
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include "src/Shader.h"
#define DEBUGMODE 


void log(std::string message) {
    #ifdef DEBUGMODE
        std::cout << message << std::endl;
    #endif // !DEBUGMODE
}

void listenForExit(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main()
{
    log("starting...");
    const unsigned int globalWidth = 800;
    const unsigned int globalHeight = 600;
    // init GLFW, set version and core profile
    log("initializing GLFW");
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    log("done. \nCreating GLFW window");
    // create a window with GLFW
    GLFWwindow* window = glfwCreateWindow(globalWidth, globalHeight, "OpenGLZeroOne", NULL, NULL);

    if (window == NULL) {
        std::cout << "couldn't create window." << std::endl;
        glfwTerminate();
        return -1;
    }


    // connect GL context to GLFW's window
    glfwMakeContextCurrent(window);

    log("initializing GLAD");
    // init GLAD, get function pointers to GL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){

        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, globalWidth, globalHeight);

    // if the user resizes the window, framebufferSizeCallback is called 
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    log("starting main render loop");

    // create a shader object -- reads and compiles shaders
    Shader shader;


    // main triangle coords [3D]
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
    };
    unsigned int indices[] = {  
        1, 0, 2,
        2, 3, 4
    };

    unsigned int vao;
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set paths
    shader.setFragmentShaderPath("D:/Dev/OpenGLStart/src/res/shaders/fragment.shader");
    shader.setVertexShaderPath("D:/Dev/OpenGLStart/src/res/shaders/vertex.shader");
    
    shader.load();

    // read files and compile and attach
    shader.makeVertexShader();
    shader.makeFragmentShader();

    // link shader program
    shader.link();
    shader.use();

    float offset = 0.5f;
    shader.setFloat("xOffset", offset);
    //std::cout << glGetUniformfv(shader.shaderProgramID, 0) << std::endl;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //main render loop
    while (!glfwWindowShouldClose(window)){
        // listens for the ESC key 
        listenForExit(window);

        // clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(vao);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    shader.~Shader();

    glfwTerminate();
    std::cout << "GL terminated" << std::endl;
    return 0;
}

