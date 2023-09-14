#include <iostream>
#include <string>
#include "glew.h"
#include "glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Application.h"
#include "Style.h"
#include "Image.h"


int main(void) 
{
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui Example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the GLFW window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW (OpenGL Extension Wrangler)
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return -1;
    }

        /* Load Images */
    Image img("assets/pic1.jpg");

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // Setup ImGui style
    ImGui::StyleColorsDark();

    // Initialize ImGui GLFW and OpenGL bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    /* Change Style */
    Style::Dracula();
    /* ******************* */

    /* Change Font */
    ImFont *pFont = io.Fonts->AddFontFromFileTTF("fonts/Jetbrains.ttf",20);
    /* *********************************************************************/


    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::PushFont(pFont);
        

        /* Code Starts here */

        MyApp::RenderUI(window);
        // ImGui::ShowDemoWindow();


        /**
         * Loading an Image 
         */
        ImGui::Begin("OpenGL Texture");
        ImGui::Text("pointer = %p", img.image_texture);
        ImGui::Text("size = %d x %d", img.width, img.height);
        ImGui::SliderInt("Width", &img.width, 1, 1000);
        ImGui::SliderInt("Height", &img.height, 1, 1000);
        ImGui::Image((void*)(intptr_t)img.image_texture, ImVec2(img.width, img.height));
        ImGui::End();
        /* *************************************************************************************** */


        /* Code ends  here */
        ImGui::PopFont();

        // Rendering
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
