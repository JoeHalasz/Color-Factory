#include "Input.h"
#include <iostream>



Input* i;
static void cursor_position_callback(GLFWwindow* window, double mousePosX, double mousePosY)
{
    glfwGetCursorPos(window, &mousePosX, &mousePosY);
    i->SetMouseXandY(mousePosX, mousePosY);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    i->SetScrollOffset(yoffset);
}

Input::Input(GLFWwindow* window, bool IS3D)
    :m_Window(window), IS3D(IS3D)
{
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    i = this;
}

bool Input::KeyHeld(int key)
{
    int action = glfwGetKey(m_Window, key);
    if (action == GLFW_PRESS || action == GLFW_REPEAT) return true;
    return false;
}

bool Input::KeyPressed(int key)
{
    int action = glfwGetKey(m_Window, key);
    if (action == GLFW_RELEASE) return true;
    return false;
}



void Input::CheckInputPresses()
{
    float MOVE_SPEED = ORIGINAL_MOVE_SPEED;
    int action;

    if (KeyHeld(GLFW_KEY_LEFT_SHIFT)) MOVE_SPEED *= 2;

    if (KeyHeld(GLFW_KEY_W)) m_Speed += glm::vec3(0.0f, -1 * MOVE_SPEED, 0.0f);
    if (KeyHeld(GLFW_KEY_S)) m_Speed += glm::vec3(0.0f, MOVE_SPEED, 0.0f);
    if (KeyHeld(GLFW_KEY_A)) m_Speed += glm::vec3(MOVE_SPEED, 0.0f, 0.0f);
    if (KeyHeld(GLFW_KEY_D)) m_Speed += glm::vec3(-1 * MOVE_SPEED, 0.0f, 0.0f);

    if (KeyPressed(GLFW_KEY_SPACE)) changePos = true; m_NewPos = glm::vec3(0, 0, 0);

    if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        m_MouseDown = true;
    }
    else {
        if (m_MouseDown) m_MouseUp = true;
        else m_MouseUp = false;
        m_MouseDown = false;
    }

    if (IS3D) {
        if (KeyHeld(GLFW_KEY_E)) m_Speed += glm::vec3(0.0f, 0.0f, -1 * MOVE_SPEED);
        if (KeyHeld(GLFW_KEY_Q)) m_Speed += glm::vec3(0.0f, 0.0f, MOVE_SPEED);
        if (KeyHeld(GLFW_KEY_G)) m_RotationChange += ROTATION_SPEED;
        if (KeyHeld(GLFW_KEY_H)) m_RotationChange -= ROTATION_SPEED;
    }
}

void Input::Reset()
{
    m_RotationChange = 0;
    m_Speed = glm::vec3(0.0f);
    m_ScrollOffset = 0;
}

void Input::SetMouseXandY(double x, double y)
{
    m_MousePosX = x;
    m_MousePosY = y;
}

void Input::SetScrollOffset(double offset)
{
    m_ScrollOffset += offset;
}
