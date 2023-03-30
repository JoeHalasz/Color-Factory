#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <stdio.h>
#include <iostream>
#include <cmath>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "World.h"
#include "Texture.h"
#include "WorldTile.h"
#include "PaintBlob.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"


int main(void)
{
    double WORLDTICKSPERSECOND = 120.0;
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    auto monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    int WIDTH, HEIGHT;

    if (0) // fullscreen
    {
        WIDTH = mode->width;
        HEIGHT = mode->height;
        window = glfwCreateWindow(WIDTH, HEIGHT, "Color Factory", NULL, NULL);
        
        glfwSetWindowMonitor(window, monitor, 0, 0, WIDTH, HEIGHT, mode->refreshRate);
    }
    else
    {
        WIDTH = 960;
        HEIGHT = 540;
        window = glfwCreateWindow(WIDTH, HEIGHT, "Color Factory", NULL, NULL);
    }
    std::cout << "Resolution: " << WIDTH << "/" << HEIGHT << std::endl;
    std::cout << "Refresh rate: " << mode->refreshRate << std::endl;

    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) {
        printf("Glew Init did not work!\n");
        return 1;
    }

    { // this fixes some error when trying to close the window

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        // set up world
        World world(window);
        Renderer renderer(world.GetBlockSize());

        double lastTimeRendered = glfwGetTime();
        double lastTimeUpdatedWorld = glfwGetTime();
        int nbFrames = 0;

        bool printStuff = true;
        double currentTime = glfwGetTime();
        double lastTime = currentTime;
        while (!glfwWindowShouldClose(window))
        {
            int lastWidth = WIDTH;
            int lastHeight = HEIGHT;
            glfwGetWindowSize(window, &WIDTH, &HEIGHT);
            if (lastWidth != WIDTH || lastHeight != HEIGHT) {
                // window was resized. HELP
                std::cout << "window was resized. HELP" << std::endl;
            }
            bool beenOneSecond = false;
            // Measure fps
            currentTime = glfwGetTime();
            if (currentTime - lastTime < 1.0f / 1000.0f)
                continue;
            lastTime = currentTime;
            if (printStuff) {
                nbFrames++;
                if (currentTime - lastTimeRendered >= 1.0) { // If last prinf() was more than 1 sec ago
                    std::cout << "\x1B[2J\x1B[H";
                    // printf and reset timer
                    beenOneSecond = true;
                    printf("%f ms/frame %f fps\n", 1000.0 / double(nbFrames), double(nbFrames));
                    nbFrames = 0;
                    lastTimeRendered += 1.0;
                }
            }
            int numTimesUpdated = 0;
            while (currentTime - lastTimeUpdatedWorld >= (1.0 / WORLDTICKSPERSECOND)) // update the world until we have caught up to 60 ticks per second
            {
                currentTime = glfwGetTime();
                // inputs and update world
                world.OnUpdate();
                lastTimeUpdatedWorld += (1.0/ WORLDTICKSPERSECOND);
                numTimesUpdated++;
                if (numTimesUpdated > 10)
                {
                    std::cout << "WORLD UPDATED TOO MANY TIMES" << std::endl;
                    break;
                }
            }
            ImGui_ImplGlfwGL3_NewFrame();
            renderer.OnRender(WIDTH, HEIGHT, world, beenOneSecond, printStuff);

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}