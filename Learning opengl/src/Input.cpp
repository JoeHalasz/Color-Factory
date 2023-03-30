#include "Input.h"
#include <iostream>



Input* i;
int x = 0;
static void cursor_position_callback(GLFWwindow* window, double mousePosX, double mousePosY)
{
    glfwGetCursorPos(window, &mousePosX, &mousePosY);
    i->SetMouseXandY(mousePosX, mousePosY);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    i->SetScrollOffset(yoffset);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    bool numberPressedThisFrame = false;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) { i->ChangeDirection(); }
    for (int x = 0; x < 9; x++)
    {
        if (glfwGetKey(window, GLFW_KEY_0+x)) {
            numberPressedThisFrame = true; 
            if (!i->m_WaitingForSecondNumber)
            {
                i->m_WaitingForSecondNumber = true;
                i->m_LastNumPressed = x;
                i->m_SecondNumPressed = 1;
            }
            else {
                i->m_WaitingForSecondNumber = false;
                i->m_SecondNumPressed = x;
            }
        }
    }

    if (numberPressedThisFrame)
        i->m_NumberPressed = true;
    else 
        i->m_NumberPressed = false;
    
}

Input::Input(GLFWwindow* window, bool IS3D)
    :m_Window(window), IS3D(IS3D)
{
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
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
    if (action == GLFW_PRESS) return true;
    return false;
}

bool Input::KeyUp(int key)
{
    int action = glfwGetKey(m_Window, key);
    if (action == GLFW_REPEAT) return true;
    return false;
}

void Input::CheckInputPresses()
{
    float MOVE_SPEED = ORIGINAL_MOVE_SPEED;

    if (KeyHeld(GLFW_KEY_LEFT_SHIFT)) MOVE_SPEED *= 2;

    if (KeyHeld(GLFW_KEY_W)) { m_Speed += glm::vec3(0.0f, -1 * MOVE_SPEED, 0.0f); m_WaitingForSecondNumber = false; }
    if (KeyHeld(GLFW_KEY_S)) { m_Speed += glm::vec3(0.0f, MOVE_SPEED, 0.0f); m_WaitingForSecondNumber = false; }
    if (KeyHeld(GLFW_KEY_A)) { m_Speed += glm::vec3(MOVE_SPEED, 0.0f, 0.0f); m_WaitingForSecondNumber = false; }
    if (KeyHeld(GLFW_KEY_D)) { m_Speed += glm::vec3(-1 * MOVE_SPEED, 0.0f, 0.0f); m_WaitingForSecondNumber = false; }

    if (KeyPressed(GLFW_KEY_SPACE)) { changePos = true; m_NewPos = glm::vec3(0, 0, 0); m_WaitingForSecondNumber = false;}
     
    if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        m_LeftMouseDown = true;
        m_WaitingForSecondNumber = false;
    }
    else {
        if (m_LeftMouseDown) m_LeftMouseUp = true;
        else m_LeftMouseUp = false;
        m_LeftMouseDown = false;
    }
    if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        m_RightMouseDown = true;
        m_WaitingForSecondNumber = false;
    }
    else {
        if (m_RightMouseDown) m_RightMouseUp = true;
        else m_RightMouseUp = false;
        m_RightMouseDown = false;
    }

    if (IS3D) {
        if (KeyHeld(GLFW_KEY_E)) { m_Speed += glm::vec3(0.0f, 0.0f, -1 * MOVE_SPEED); m_WaitingForSecondNumber = false; }
        if (KeyHeld(GLFW_KEY_Q)) { m_Speed += glm::vec3(0.0f, 0.0f, MOVE_SPEED); m_WaitingForSecondNumber = false; }
        if (KeyHeld(GLFW_KEY_G)) { m_RotationChange += ROTATION_SPEED; m_WaitingForSecondNumber = false; }
        if (KeyHeld(GLFW_KEY_H)) { m_RotationChange -= ROTATION_SPEED; m_WaitingForSecondNumber = false;}
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
