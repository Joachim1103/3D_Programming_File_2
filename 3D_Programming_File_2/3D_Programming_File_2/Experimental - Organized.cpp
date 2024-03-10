//#include <iostream>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>
//
//// Window dimensions
//const GLint WIDTH = 1920, HEIGHT = 1080;
//
//// Camera struct
//struct Camera {
//    glm::mat4 view;
//    glm::mat4 projection;
//};
//
//// Camera settings
//Camera camera;
//
//// Vertex Shader source code
//const char* vertexShaderSource = R"(
//    #version 330 core
//    layout (location = 0) in vec3 aPos;
//    uniform mat4 model;
//    uniform mat4 view;
//    uniform mat4 projection;
//    void main() {
//        gl_Position = projection * view * model * vec4(aPos, 1.0);
//    }
//)";
//
//// Fragment Shader source code
//const char* fragmentShaderSource = R"(
//    #version 330 core
//    out vec4 FragColor;
//    uniform vec4 objectColor;
//    void main() {
//        FragColor = objectColor; // Use the color passed as a uniform
//    }
//)";
//
//// Initialize GLFW and create window
//void initWindow() {
//
//    glfwInit();
//
//    // Set OpenGL version to 3.3
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Simple 3D Game", nullptr, nullptr);
//    if (window == nullptr) {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        exit(EXIT_FAILURE);
//    }
//
//    glfwMakeContextCurrent(window);
//    gladLoadGL();
//}
//
//// Render loop
//void renderLoop(GLFWwindow* window) {
//
//    // Set clear color (background)
//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//
//    // Enable depth testing
//    glEnable(GL_DEPTH_TEST);
//
//    // Set up camera
//    camera.projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
//    camera.view = glm::lookAt(glm::vec3(1.0f, 0.0f, 3.5f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//
//    // Compile vertex shader
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
//    glCompileShader(vertexShader);
//
//    // Compile fragment shader
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//    glCompileShader(fragmentShader);
//
//    // Create shader program
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    // Check for shader program linking errors
//    int success;
//    char infoLog[512];
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    // Vertex data for ground plane
//    GLfloat planeVertices[] = {
//        // Positions
//        -1.0f, -0.5f, 0.0f,
//        2.0f, -0.5f, 0.0f,
//        2.0f, -0.5f, 2.0f,
//        -1.0f, -0.5f, 2.0f
//    };
//    // Indices for ground plane
//    GLuint planeIndices[] = {
//        0, 1, 2,
//        2, 3, 0
//    };
//
//    // Vertex data for house, including the door
//    GLfloat houseVertices[] = {
//        // Positions
//        // Front face
//        -0.5f, -0.5f, 0.0f, // 0
//        0.5f, -0.5f, 0.0f,  // 1
//        0.5f, 0.5f, 0.0f,   // 2
//        -0.5f, 0.5f, 0.0f,  // 3
//        // Back face
//        -0.5f, -0.5f, 1.0f, // 4
//        0.5f, -0.5f, 1.0f,  // 5
//        0.5f, 0.5f, 1.0f,   // 6
//        -0.5f, 0.5f, 1.0f,  // 7
//        // Roof
//        0.0f, 1.0f, 0.5f,   // 8
//        // Door
//        -0.1f, -0.495f, 0.0f, // 9
//        0.1f, -0.51f, 0.0f,  // 10
//        0.1f, 0.0f, 0.0f,   // 11
//        -0.1f, 0.0f, 0.0f   // 12
//    };
//
//    // Indices for house, including the door
//    GLuint houseIndices[] = {
//        // Front face
//        0, 1, 2,
//        2, 3, 0,
//        // Back face
//        4, 5, 6,
//        6, 7, 4,
//        // Left face
//        0, 4, 7,
//        7, 3, 0,
//        // Right face
//        1, 5, 6,
//        6, 2, 1,
//        // Bottom face
//        0, 1, 5,
//        5, 4, 0,
//        // Top face
//        3, 2, 6,
//        6, 7, 3,
//        // Door
//        9, 10, 11,
//        11, 12, 9
//    };
//
//    // Create VAO, VBO, and EBO for ground plane
//    GLuint planeVAO, planeVBO, planeEBO;
//    glGenVertexArrays(1, &planeVAO);
//    glGenBuffers(1, &planeVBO);
//    glGenBuffers(1, &planeEBO);
//    glBindVertexArray(planeVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glBindVertexArray(0);
//
//    // Create VAO, VBO, and EBO for house
//    GLuint houseVAO, houseVBO, houseEBO;
//    glGenVertexArrays(1, &houseVAO);
//    glGenBuffers(1, &houseVBO);
//    glGenBuffers(1, &houseEBO);
//    glBindVertexArray(houseVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, houseVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, houseEBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(houseIndices), houseIndices, GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glBindVertexArray(0);
//
//    // Main render loop
//    while (!glfwWindowShouldClose(window)) {
//
//        // Clear the color and depth buffers
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        // Use shader program
//        glUseProgram(shaderProgram);
//
//        // View and Projection uniforms
//        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
//        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
//        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
//        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));
//
//        // For each object set model matrix and color
//        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
//        GLint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
//
//        // Draw ground plane
//        glBindVertexArray(planeVAO);
//        glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f); // RGBA green
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
//
//        // Draw house
//        glBindVertexArray(houseVAO);
//        glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); // RGBA red
//        model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.5f));
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0);
//
//
//        // Draw Door
//        glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); // RGBA blue
//        model = glm::mat4(1.0f);
//        float DoorWidth = 0.3f;
//        float DoorHeight = 1.0f;
//        model = glm::translate(model, glm::vec3(1.3f, -0.45f + DoorHeight / 2, 0.01f));
//        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//        glBindVertexArray(houseVAO);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(36 * sizeof(GLuint)));
//
//        // Swap the front and back buffers
//        glfwSwapBuffers(window);
//        // Poll for and process events
//        glfwPollEvents();
//    }
//
//    // Delete allocated resources
//    glDeleteVertexArrays(1, &planeVAO);
//    glDeleteBuffers(1, &planeVBO);
//    glDeleteBuffers(1, &planeEBO);
//    glDeleteVertexArrays(1, &houseVAO);
//    glDeleteBuffers(1, &houseVBO);
//    glDeleteBuffers(1, &houseEBO);
//    glDeleteProgram(shaderProgram);
//}
//
//int main() {
//    // Initialize GLFW and create window
//    initWindow();
//    // Render loop
//    renderLoop(glfwGetCurrentContext());
//    // Terminate GLFW
//    glfwTerminate();
//    return 0;
//}
