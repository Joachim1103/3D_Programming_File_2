#include <glm.hpp>
#include <GLFW/glfw3.h>

class Player {
public:
    Player(glm::vec3 startPosition, float speed)
        : position(startPosition), movementSpeed(speed) {}

    void processInput(GLFWwindow* window, float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            position += movementSpeed * deltaTime * glm::vec3(0.0f, 0.0f, -1.0f);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            position += movementSpeed * deltaTime * glm::vec3(0.0f, 0.0f, 1.0f);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            position += movementSpeed * deltaTime * glm::vec3(-1.0f, 0.0f, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            position += movementSpeed * deltaTime * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    glm::vec3 getPosition() const {
        return position;
    }

private:
    glm::vec3 position;
    float movementSpeed;
};


