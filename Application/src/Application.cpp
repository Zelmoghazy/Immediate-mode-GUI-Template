#include "Application.h"
#include "imgui.h"
#include <iostream>
#include "Platform_Utils.h"
#include "glfw3.h"

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
        static int test = 3;
        static bool send_text = false;

        /**
         * Docking Space 
         */

        DockSpace(nullptr);

        /* ***************** */

        /* First Window */

        ImGui::Begin("Hello, ImGui!", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        // ImGui::SetWindowSize(ImVec2(500,500));
        ImGui::Text("This is a simple ImGui example.");
        if (ImGui::Button("Quit"))
        {
            glfwSetWindowShouldClose(window, true);
        }
        ImGui::Checkbox("Checkbox", &checkboxval);
        ImGui::SliderInt("Int Slider", &test, 1, 10);
        if (ImGui::Button("Int value"))
        {
            std::cout << test << std::endl;
        }
        static char text_input[1024 * 16] =
            "Test\n";
        static ImGuiInputTextFlags text_flags = ImGuiInputTextFlags_AllowTabInput;
        ImGui::CheckboxFlags("ReadOnly", &text_flags, ImGuiInputTextFlags_ReadOnly);
        ImGui::CheckboxFlags("AllowTabInput", &text_flags, ImGuiInputTextFlags_AllowTabInput);
        ImGui::CheckboxFlags("CtrlEnterForNewLine", &text_flags, ImGuiInputTextFlags_CtrlEnterForNewLine);
        ImGui::InputTextMultiline("##source", text_input, IM_ARRAYSIZE(text_input), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), text_flags);
        if (ImGui::Button("Send Text"))
        {
            send_text = true;
        }
        ImGui::End();

        /* *********************************************************************** */

        if (checkboxval)
        {
            ImGui::Begin("New Window");
            ImGui::Text("Check box is pressed");
            ImGui::End();
        }

        /* Table */
        static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingStretchSame |
                                       ImGuiTableFlags_Resizable |
                                       ImGuiTableFlags_BordersOuter |
                                       ImGuiTableFlags_BordersInner |
                                       ImGuiTableFlags_BordersV |
                                       ImGuiTableFlags_BordersH |
                                       ImGuiTableFlags_ContextMenuInBody;
        static char buf[1024 * 16] = "Table";
        ImGui::Begin("Table", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::BeginTable("table1", test, table_flags))
        {
            for (int i = 0; i < test; i++)
            {
                ImGui::TableSetupColumn("Header");
            }
            ImGui::TableHeadersRow();
            for (int row = 0; row < 5; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < test; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if(send_text)
                    {
                        strcpy(buf,text_input);
                        send_text = false;
                    }
                    ImGui::TextUnformatted(buf);
                }
            }
            ImGui::EndTable();
        }
        ImGui::End();
        /* *********************************************************************** */


         /* Windows Open and save dialogs */
        ImGui::Begin("Windows Dialogs");
        if(ImGui::Button("Open")){
            std::string filepath = Windows_file_dialog::OpenFile("All Files (*.*)\0*.*\0",window);
            std::cout << filepath << std::endl;
        }
        if(ImGui::Button("Save")){
        }
        ImGui::End();
        /* ***************************** */
    }
}