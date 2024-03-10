#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>

// Window dimensions
const GLint WIDTH = 1920, HEIGHT = 1080;

// Camera struct
struct Camera {
    glm::mat4 view;
    glm::mat4 projection;
};

// Camera settings
Camera camera;

// Vertex Shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

// Fragment Shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    uniform vec4 objectColor;
    void main() {
        FragColor = objectColor; // Use the color passed as a uniform
    }
)";

// Vertex data for player cube
GLfloat playerVertices[] = {
    // Positions         // Description
    -0.5f, -0.5f, 0.5f,   // Front bottom left
     0.5f, -0.5f, 0.5f,   // Front bottom right
     0.5f,  0.5f, 0.5f,   // Front top right
    -0.5f,  0.5f, 0.5f,   // Front top left
    -0.5f, -0.5f, -0.5f,  // Back bottom left
     0.5f, -0.5f, -0.5f,  // Back bottom right
     0.5f,  0.5f, -0.5f,  // Back top right
    -0.5f,  0.5f, -0.5f   // Back top left
};

// Indices for player
GLuint playerIndices[] = {
    // Front face
    0, 1, 2, 2, 3, 0,
    // Back face
    4, 5, 6, 6, 7, 4,
    // Left face
    4, 0, 3, 3, 7, 4,
    // Right face
    1, 5, 6, 6, 2, 1,
    // Top face
    3, 2, 6, 6, 7, 3,
    // Bottom face
    0, 1, 5, 5, 4, 0
};

// Define vertices and indices for a sphere
const int SPHERE_SECTORS = 36;
const int SPHERE_STACKS = 18;

std::vector<GLfloat> sphereVertices;
std::vector<GLuint> sphereIndices;

void createSphere(float radius, int sectors, int stacks) {
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = glm::pi<float>() * static_cast<float>(i) / stacks;
        float stackRadius = radius * sin(stackAngle);
        float stackHeight = radius * cos(stackAngle);
        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = 2 * glm::pi<float>() * static_cast<float>(j) / sectors;
            float x = stackRadius * sin(sectorAngle);
            float z = stackRadius * cos(sectorAngle);
            float y = stackHeight;
            sphereVertices.push_back(x);
            sphereVertices.push_back(y);
            sphereVertices.push_back(z);
        }
    }
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < sectors; ++j) {
            int k0 = i * (sectors + 1) + j;
            int k1 = k0 + 1;
            int k2 = (i + 1) * (sectors + 1) + j + 1;
            int k3 = (i + 1) * (sectors + 1) + j;
            sphereIndices.push_back(k0);
            sphereIndices.push_back(k1);
            sphereIndices.push_back(k2);
            sphereIndices.push_back(k0);
            sphereIndices.push_back(k2);
            sphereIndices.push_back(k3);
        }
    }
}

// Define sphere positions
std::vector<glm::vec3> spherePositions;

// Function to initialize sphere positions
void initSpherePositions() {
    // Add the six positions within the camera view
    spherePositions.push_back(glm::vec3(-1.5f, -0.4f, 0.0f));
    spherePositions.push_back(glm::vec3(0.0f, -0.4f, 0.5f));
    spherePositions.push_back(glm::vec3(2.5f, -0.4f, 0.2f));
    spherePositions.push_back(glm::vec3(1.5f, -0.4f, 2.0f));
    spherePositions.push_back(glm::vec3(0.0f, -0.4f, 2.0f));
    spherePositions.push_back(glm::vec3(0.0f, -0.4f, -1.0f));
}

// Player position and movement speed
glm::vec3 playerPosition = glm::vec3(1.0f, -0.4, 2.0f); // x = 2.0, y adjusted for ground, z = -3.0
float playerSpeed = 0.001f;

// Check proximity between two points in 3D space
bool isNear(glm::vec3 point1, glm::vec3 point2, float distanceThreshold) {
    float distanceSquared = glm::length(point1 - point2);
    return distanceSquared <= (distanceThreshold * distanceThreshold);
}

// Handle keyboard input for player movement
void processInput(GLFWwindow* window) {
    float sprintFactor = 1.0f; // Default sprint factor

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        // Sprint key is pressed
        sprintFactor = 2.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        playerPosition.z -= playerSpeed * sprintFactor;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        playerPosition.z += playerSpeed * sprintFactor;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        playerPosition.x -= playerSpeed * sprintFactor;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        playerPosition.x += playerSpeed * sprintFactor;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        // Check proximity to each sphere
        for (auto it = spherePositions.begin(); it != spherePositions.end(); ) {
            if (isNear(playerPosition, *it, 0.3f)) { // Adjust threshold as needed
                it = spherePositions.erase(it); // Remove sphere from vector
            }
            else {
                ++it;
            }
        }
    }
}

// Initialize GLFW and create window
void initWindow() {

    glfwInit();

    // Set OpenGL version to 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Simple 3D Game", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
}

// Render loop
void renderLoop(GLFWwindow* window) {

    // Set clear color (background)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set up camera
    camera.projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    camera.view = glm::lookAt(glm::vec3(1.0f, 0.0f, 3.5f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for shader program linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex data for ground plane
    GLfloat planeVertices[] = {
        // Positions
        -2.0f, -0.5f, -2.0f,
        2.0f, -0.5f, -2.0f,
        2.0f, -0.5f, 3.0f,
        -2.0f, -0.5f, 3.0f
    };
    // Indices for ground plane
    GLuint planeIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Vertex data for house, including the door
    GLfloat houseVertices[] = {
        // Positions
        // Front face
        -0.5f, -0.5f, 0.0f, // 0
        0.5f, -0.5f, 0.0f,  // 1
        0.5f, 0.5f, 0.0f,   // 2
        -0.5f, 0.5f, 0.0f,  // 3
        // Back face
        -0.5f, -0.5f, 1.0f, // 4
        0.5f, -0.5f, 1.0f,  // 5
        0.5f, 0.5f, 1.0f,   // 6
        -0.5f, 0.5f, 1.0f,  // 7
        // Roof
        0.0f, 1.0f, 0.5f,   // 8
        // Door
        -0.1f, -0.495f, 0.0f, // 9
        0.1f, -0.51f, 0.0f,  // 10
        0.1f, 0.0f, 0.0f,   // 11
        -0.1f, 0.0f, 0.0f   // 12
    };

    // Indices for house, including the door
    GLuint houseIndices[] = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Back face
        4, 5, 6,
        6, 7, 4,
        // Left face
        0, 4, 7,
        7, 3, 0,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Bottom face
        0, 1, 5,
        5, 4, 0,
        // Top face
        3, 2, 6,
        6, 7, 3,
        // Door
        9, 10, 11,
        11, 12, 9
    };

    // Create VAO, VBO, and EBO for ground plane
    GLuint planeVAO, planeVBO, planeEBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glGenBuffers(1, &planeEBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndices), planeIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Create VAO, VBO, and EBO for house
    GLuint houseVAO, houseVBO, houseEBO;
    glGenVertexArrays(1, &houseVAO);
    glGenBuffers(1, &houseVBO);
    glGenBuffers(1, &houseEBO);
    glBindVertexArray(houseVAO);
    glBindBuffer(GL_ARRAY_BUFFER, houseVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(houseVertices), houseVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, houseEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(houseIndices), houseIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Create VAO, VBO, and EBO for player cube
    GLuint playerVAO, playerVBO, playerEBO;
    glGenVertexArrays(1, &playerVAO);
    glGenBuffers(1, &playerVBO);
    glGenBuffers(1, &playerEBO);
    glBindVertexArray(playerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, playerVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(playerVertices), playerVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, playerEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(playerIndices), playerIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Create VAO, VBO, and EBO for sphere
    GLuint sphereVAO, sphereVBO, sphereEBO;
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(GLfloat), sphereVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(GLuint), sphereIndices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Initialize sphere positions
    initSpherePositions();

    // Main render loop
    while (!glfwWindowShouldClose(window)) {

        // Process input
        processInput(window);

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program
        glUseProgram(shaderProgram);

        // View and Projection uniforms
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));

        // For each object set model matrix and color
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");

        // Draw ground plane
        glBindVertexArray(planeVAO);
        glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f); // RGBA green
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Draw house
        glBindVertexArray(houseVAO);
        glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); // RGBA red
        model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Draw Door
        glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); // RGBA blue
        model = glm::mat4(1.0f);
        float DoorWidth = 0.3f;
        float DoorHeight = 1.0f;
        model = glm::translate(model, glm::vec3(1.3f, -0.45f + DoorHeight / 2, 0.01f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(houseVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(36 * sizeof(GLuint)));

        // Render Player Cube
        glUniform4f(colorLoc, 0.5f, 0.0f, 0.5f, 1.0f); // RGBA purple
        glBindVertexArray(playerVAO);
        glm::mat4 playerModel = glm::mat4(1.0f);
        playerModel = glm::translate(playerModel, playerPosition);
        playerModel = glm::scale(playerModel, glm::vec3(0.1f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(playerModel));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Render spheres
        glBindVertexArray(sphereVAO);
        for (const auto& position : spherePositions) {
            glm::mat4 sphereModel = glm::mat4(1.0f);
            sphereModel = glm::translate(sphereModel, position); // Translate the sphere to its position
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sphereModel));
            glUniform4f(colorLoc, 1.0f, 0.843f, 0.0f, 1.0f); // RGBA gold
            glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
        }

        // Swap the front and back buffers
        glfwSwapBuffers(window);
        // Poll for and process events
        glfwPollEvents();
    }

    // Delete allocated resources
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &planeEBO);
    glDeleteVertexArrays(1, &houseVAO);
    glDeleteBuffers(1, &houseVBO);
    glDeleteBuffers(1, &houseEBO);
    glDeleteVertexArrays(1, &playerVAO);
    glDeleteBuffers(1, &playerVBO);
    glDeleteBuffers(1, &playerEBO);
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteBuffers(1, &sphereVBO);
    glDeleteBuffers(1, &sphereEBO);
    glDeleteProgram(shaderProgram);
}

int main() {
    // Initialize GLFW and create window
    initWindow();
    // Create Sphere
    createSphere(0.05f, SPHERE_SECTORS, SPHERE_STACKS);
    // Render loop
    renderLoop(glfwGetCurrentContext());
    // Terminate GLFW
    glfwTerminate();
    return 0;
}
