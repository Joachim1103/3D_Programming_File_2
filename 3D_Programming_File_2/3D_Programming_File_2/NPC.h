#include <glm.hpp>
#include <vector>

class NPC {
public:
    NPC(const std::vector<glm::vec2>& pathPoints, float speed)
        : path(pathPoints), movementSpeed(speed), currentIndex(0) {}

    void update(float deltaTime) {
        if (path.empty())
            return;

        // Calculate direction towards the current target point
        glm::vec2 currentTarget = path[currentIndex];
        glm::vec2 npcPosition(position.x, position.z);
        glm::vec2 direction = glm::normalize(currentTarget - npcPosition);

        // Move the NPC towards the current target point
        position.x += movementSpeed * deltaTime * direction.x;
        position.z += movementSpeed * deltaTime * direction.y;

        // Check if the NPC has reached the current target point
        if (glm::distance(npcPosition, currentTarget) < 0.1f) {
            // Move to the next target point in the path
            currentIndex = (currentIndex + 1) % path.size();
        }
    }

    glm::vec3 getPosition() const {
        return position;
    }

private:
    std::vector<glm::vec2> path;
    glm::vec3 position;
    float movementSpeed;
    size_t currentIndex;
};

