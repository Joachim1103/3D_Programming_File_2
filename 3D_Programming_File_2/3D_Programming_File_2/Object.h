#include <glm.hpp>

class Object {
public:
    Object(glm::vec3 position, glm::vec3 scale, glm::vec3 color)
        : position(position), scale(scale), color(color) {}

    void setPosition(glm::vec3 newPosition) {
        position = newPosition;
    }

    void setScale(glm::vec3 newScale) {
        scale = newScale;
    }

    void setColor(glm::vec3 newColor) {
        color = newColor;
    }

    glm::vec3 getPosition() const {
        return position;
    }

    glm::vec3 getScale() const {
        return scale;
    }

    glm::vec3 getColor() const {
        return color;
    }

private:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 color;
};


