#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "glew.h"
#include "glfw3.h"
#include "Application.h"
#include "imgui.h"
#include "Platform_Utils.h"
#include "Image.h"


namespace MyApp
{
    void DockSpace(bool *p_open)
    {
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport *viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", p_open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO &io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
                }
                if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
                }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
                }
                if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                }
                if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen))
                {
                    dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                }
                ImGui::Separator();

                if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                    *p_open = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

    void RenderUI(GLFWwindow* window)
    {
        static bool checkboxval = false;
        static int int_value = 3;
        static int text_input_size = 10;

        /**
         * Docking Space 
         */

        DockSpace(nullptr);

        /* ***************** */

        /* First Window */
        ImGui::Begin("Hello, ImGui!", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        // ImGui::SetWindowSize(ImVec2(500,500));
        ImGui::Text("This is a simple ImGui example.");
        /* Set position of widget */
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX()+100 ,ImGui::GetCursorPosY()));
        if (ImGui::Button("Quit"))
        {
            glfwSetWindowShouldClose(window, true);
        }
        ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX()+100 ,ImGui::GetCursorPosY()));
        ImGui::Checkbox("Checkbox", &checkboxval);
        ImGui::SliderInt("Int Slider", &int_value, 1, 10);
        if (ImGui::Button("Int value"))
        {
            std::cout << int_value << std::endl;
        }
        /* Text Input */
        static char text_input[1024 * 16] = "Test\n";
        static ImGuiInputTextFlags text_flags = ImGuiInputTextFlags_AllowTabInput;
        ImGui::CheckboxFlags("ReadOnly", &text_flags, ImGuiInputTextFlags_ReadOnly);
        /* Place widget on the same line specify spacing */
        ImGui::SameLine(0, 20);
        ImGui::CheckboxFlags("AllowTabInput", &text_flags, ImGuiInputTextFlags_AllowTabInput);
        ImGui::SameLine(0, 20);
        ImGui::CheckboxFlags("CtrlEnterForNewLine", &text_flags, ImGuiInputTextFlags_CtrlEnterForNewLine);
        ImGui::InputTextMultiline("##source", text_input, IM_ARRAYSIZE(text_input), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * text_input_size), text_flags);
        /* Text Input size */
        ImGui::SliderInt("Input size", &text_input_size, 1, 10);


        /* Save Text from Text input */
        static std::vector<std::string> list;
        if (ImGui::Button("Send Text"))
        {
            std::string str = text_input;
            list.push_back(str);
        }
        if (ImGui::Button("Remove"))
        {
            if(!list.empty()){
                list.pop_back();
            }
        }

        ImGui::End();

        /* *********************************************************************** */

        if (checkboxval)
        {
            ImGui::Begin("New Window");
            ImGui::Text("Check box is pressed");
            ImGui::End();
        }

        /****************** Table ************************************************/
        static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingStretchSame |
                                             ImGuiTableFlags_Resizable         |
                                             ImGuiTableFlags_BordersOuter      |
                                             ImGuiTableFlags_BordersInner      |
                                             ImGuiTableFlags_BordersV          |
                                             ImGuiTableFlags_BordersH          |
                                             ImGuiTableFlags_ContextMenuInBody;
        static char buf[1024 * 16] = "Table";
        ImGui::Begin("Table", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::BeginTable("table1", int_value, table_flags))
        {
            for (int i = 0; i < int_value; i++)
            {
                ImGui::TableSetupColumn("Header");
            }
            ImGui::TableHeadersRow();
            for(std::string s : list)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < int_value; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    strcpy(buf,s.c_str());
                    ImGui::TextUnformatted(buf);
                }
            }
            ImGui::EndTable();
        }
        ImGui::End();
        /* *********************************************************************** */


        /**
         * Loading an Image 
         */
        static std::string path = "assets/pic1.jpg";   // Initial Path
        static Image img(path.c_str());                // Initial Image
        static bool loaded = true;

        ImGui::Begin("OpenGL Texture");
        ImGui::Text("pointer = %p", img.image_texture);
        ImGui::Text("size = %d x %d", img.width, img.height);
        ImGui::SliderInt("Width", &img.width, 1, 1000);
        ImGui::SliderInt("Height", &img.height, 1, 1000);
        ImGui::Image((void*)(intptr_t)img.image_texture, ImVec2(img.width, img.height));

        if (ImGui::Button("Open"))
        {
            path = Windows_file_dialog::OpenFile("All Files (*.*)\0*.*\0", window);
            std::replace(path.begin(), path.end(), '\\', '/');   // Fix path
            std::cout << path.c_str() << std::endl;              // Debugging
            loaded = img.GetImageFromPath(path.c_str());        
        }
        if (!loaded)
        {
            /* Colored Text */
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Couldn't load Image.");
        }
        ImGui::End();
        /* ***************************** */
    }
}