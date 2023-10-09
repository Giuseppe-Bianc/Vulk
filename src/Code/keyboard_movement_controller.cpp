#include "keyboard_movement_controller.hpp"

namespace lve {

    void KeyboardMovementController::moveInPlaneXZ(GLFWwindow *window, long double dt, LveGameObject &gameObject) const {
        glm::dvec3 rotate{0};
        if(glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.0;
        if(glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.0;
        if(glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.0;
        if(glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.0;

        if(glm::dot(rotate, rotate) > std::numeric_limits<double>::epsilon()) {
            gameObject.transform.rotation += C_D(lookSpeed * dt) * glm::normalize(rotate);
        }

        // limit pitch values between about +/- 85ish degrees
        gameObject.transform.rotation.x = C_F(glm::clamp(C_LD(gameObject.transform.rotation.x), -1.5L, 1.5L));
        gameObject.transform.rotation.y = C_F(glm::mod(C_LD(gameObject.transform.rotation.y), glm::two_pi<long double>()));

        const double yaw = gameObject.transform.rotation.y;
        const glm::dvec3 forwardDir{std::sin(yaw), 0.0, std::cos(yaw)};
        const glm::dvec3 rightDir{forwardDir.z, 0.0, -forwardDir.x};
        const glm::dvec3 upDir{0.0, -1.0, 0.0};

        glm::dvec3 moveDir{0.0};
        if(glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
        if(glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
        if(glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
        if(glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
        if(glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
        if(glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

        if(glm::dot(moveDir, moveDir) > std::numeric_limits<double>::epsilon()) {
            gameObject.transform.translation += C_D(moveSpeed * dt) * glm::normalize(moveDir);
        }
    }
}  // namespace lve
