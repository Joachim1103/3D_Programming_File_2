#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera {
public:
    enum class Position { OUTSIDE, INSIDE };

    Camera(float screenWidth, float screenHeight)
        : screenWidth(screenWidth), screenHeight(screenHeight), currentPosition(Position::OUTSIDE) {
        // Define initial camera positions
        outsidePosition = glm::vec3(0.0f, 0.0f, 3.0f);
        insidePosition = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::mat4 getViewMatrix() const {
        if (currentPosition == Position::OUTSIDE) {
            return glm::lookAt(outsidePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        else {
            return glm::lookAt(insidePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }
    }

    void switchPosition() {
        currentPosition = (currentPosition == Position::OUTSIDE) ? Position::INSIDE : Position::OUTSIDE;
    }

private:
    float screenWidth;
    float screenHeight;
    Position currentPosition;
    glm::vec3 outsidePosition;
    glm::vec3 insidePosition;
};

