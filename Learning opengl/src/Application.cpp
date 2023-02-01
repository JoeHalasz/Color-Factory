#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include <stdio.h>
#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "World.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

int main(void)
{
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
    

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        printf("Glew Init did not work!\n");
        return 1;
    }

    { // this fixes some error when trying to close the window



        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        // set up world
        World world(window);

        // renderer.AddQuad(1, 50, 200, 100);
        double lastTime = glfwGetTime();
        int nbFrames = 0;
        int counter = 1;

        bool printStuff = true;
        while (!glfwWindowShouldClose(window))
        {
            bool beenOneSecond = false;
            // Measure fps
            if (printStuff) {
                double currentTime = glfwGetTime();
                nbFrames++;
                if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
                    std::cout << "\x1B[2J\x1B[H";
                    // printf and reset timer
                    beenOneSecond = true;
                    printf("%f ms/frame %f fps\n", 1000.0 / double(nbFrames), double(nbFrames));
                    nbFrames = 0;
                    lastTime += 1.0;
                    counter++;
                }
            }
            
            // inputs and update world
            world.OnUpdate();


            // draw background
            int size = 75;

            float zoomedWidth  = -1 * (-1 * (float)(WIDTH / 2) - ((WIDTH / 2) * world.GetZoomAmount()));
            float zoomedHeight = -1 * (-1 * (float)(HEIGHT / 2) - ((HEIGHT / 2) * world.GetZoomAmount()));

            int startDrawX = (-1 * ((world.GetPosition().x / size) + (zoomedWidth / size))) - 1;
            int startDrawY = (-1 * ((world.GetPosition().y / size) + (zoomedHeight / size))) - 1;
            int amountToDrawX = (zoomedWidth / size)*2;
            int amountToDrawY = (zoomedHeight / size)*2;
            
            int extraQuads = 3;
            for (int x = startDrawX; x < startDrawX + amountToDrawX + extraQuads; x++) {
                for (int y = startDrawY; y < startDrawY + amountToDrawY + extraQuads; y++) {
                    renderer.AddQuad(1, size, x * size, y * size);
                }
            }
            
            renderer.AddQuad(0, size, 0, 0);

            ImGui_ImplGlfwGL3_NewFrame();
            if (beenOneSecond && printStuff)
            {
                std::cout << "Drawing " << renderer.GetAmountOfCurrentQuads() << "/" << renderer.GetMaxAmountOfQuads() 
                    << " Quads: " << ((float)renderer.GetAmountOfCurrentQuads() / renderer.GetMaxAmountOfQuads()) * 100 << "% " << std::endl;
            }

            renderer.OnRender(WIDTH, HEIGHT, world.GetPosition(), world.GetZoomAmount(), world.GetRotation());

            renderer.DeleteQuads();

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}