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
        // set up inputs
        Input input(window, world.IS3D);

        // renderer.AddQuad(1, 50, 200, 100);
        double lastTime = glfwGetTime();
        int nbFrames = 0;
        int counter = 1;

        bool printStuff = true;
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
            world.OnUpdate(&input);
            

            // draw background
            int size = world.GetBlockSize();

            float zoomedWidth = (WIDTH / 2) + (world.GetZoomAmount() * (WIDTH/20));
            float zoomedHeight = (HEIGHT / 2) + (world.GetZoomAmount() * (HEIGHT/20));
            
            // std::cout << world.GetZoomAmount() << std::endl;
            int startDrawX = (-1 * ((world.GetPosition().x / size) + (zoomedWidth / size))) - 1;
            int startDrawY = (-1 * ((world.GetPosition().y / size) + (zoomedHeight / size))) - 1;

            int amountToDrawX = (zoomedWidth / size)*2;
            int amountToDrawY = (zoomedHeight / size)*2;
            
            int extraQuads = 3;
            for (int x = startDrawX; x < startDrawX + amountToDrawX + extraQuads; x++) {
                for (int y = startDrawY; y < startDrawY + amountToDrawY + extraQuads; y++) {
                    renderer.AddQuad(0, size, x * size, y * size);
                }
            }

            double mousePosX = ((((input.GetMousePosX() / (WIDTH / 2)) * zoomedWidth) - zoomedWidth) - world.GetPosition().x )/size;
            double mousePosY = -1*((((input.GetMousePosY() / (HEIGHT / 2)) * zoomedHeight) - zoomedHeight) + world.GetPosition().y) / size;

            if (mousePosX < 0) mousePosX = floor(mousePosX);
            else mousePosX = floor(mousePosX);
            if (mousePosY < 0) mousePosY = floor(mousePosY);
            else mousePosY = floor(mousePosY);

            if (input.GetMouseDown())
            {
                // place block here
                //std::cout << world.GetMousePosX() << " " << world.GetMousePosY() << std::endl;
                renderer.AddQuad(3, size, mousePosX*size, mousePosY * size);
                renderer.AddQuad(4, size, mousePosX*size, mousePosY * size);
            }

            ImGui_ImplGlfwGL3_NewFrame();
            if (beenOneSecond && printStuff)
            {
                std::cout << "Drawing " << renderer.GetAmountOfCurrentQuads() << "/" << renderer.GetMaxAmountOfQuads() 
                    << " Quads: " << ((float)renderer.GetAmountOfCurrentQuads() / renderer.GetMaxAmountOfQuads()) * 100 << "% " << std::endl;
            }

            renderer.OnRender(WIDTH, HEIGHT, world);

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