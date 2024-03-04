#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Player.h"
#include "NPC.h"
#include "Object.h"

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Function prototypes
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void processInput(GLFWwindow* window, Player& player);

// Camera
Camera camera(WIDTH, HEIGHT);

// Player
Player player(glm::vec3(0.0f, 0.0f, 0.0f), 2.5f);

// NPC
std::vector<glm::vec2> npcPath = { glm::vec2(-3.0f, -3.0f), glm::vec2(3.0f, 3.0f) };
NPC npc(npcPath, 1.0f);

// Objects
Object substrate(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(10.0f, 0.1f, 10.0f), glm::vec3(0.5f, 0.5f, 0.5f));

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // GLFW window setup
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Game", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, keyCallback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        static float lastFrame = 0.0f;
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Handle input
        processInput(window, player);

        // Update NPC
        npc.update(deltaTime);

        // Clear the color and depth buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update camera view matrix
        glm::mat4 viewMatrix = camera.getViewMatrix();

        // Render substrate
        // For simplicity, we're not rendering other objects in this example
        // In a real game, you'd render all objects in the scene here
        // You'd also use shaders and textures for rendering
        // For now, we're just rendering a simple colored square representing the substrate
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixf(glm::value_ptr(viewMatrix));
        glTranslatef(substrate.getPosition().x, substrate.getPosition().y, substrate.getPosition().z);
        glScalef(substrate.getScale().x, substrate.getScale().y, substrate.getScale().z);
        glColor3f(substrate.getColor().x, substrate.getColor().y, substrate.getColor().z);
        glBegin(GL_QUADS);
        glVertex3f(-0.5f, 0.0f, -0.5f);
        glVertex3f(-0.5f, 0.0f, 0.5f);
        glVertex3f(0.5f, 0.0f, 0.5f);
        glVertex3f(0.5f, 0.0f, -0.5f);
        glEnd();
        glPopMatrix();

        // Swap the screen buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glfwTerminate();
    return 0;
}

// Key callback function
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // Toggle camera position inside/outside the house with 'O' key
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        camera.switchPosition();
    }
}

// Process input function
void processInput(GLFWwindow* window, Player& player) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.processInput(Player::FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.processInput(Player::BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.processInput(Player::LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.processInput(Player::RIGHT);
}
