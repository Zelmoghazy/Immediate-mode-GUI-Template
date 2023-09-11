#include <string>
#include <windows.h>
#include "commdlg.h"
#include "glfw3.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "glfw3native.h"

namespace Windows_file_dialog
{
    std::string OpenFile(const char* filter,GLFWwindow *window)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[260] ={0};

        ZeroMemory(&ofn,sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)window);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetOpenFileName(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }
        return std::string();
    }

    std::string SaveFile(const char* filter,GLFWwindow *window)
    {
        OPENFILENAMEA ofn;
        CHAR szFile[260] ={0};

        ZeroMemory(&ofn,sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)window);
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetSaveFileName(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }
        return std::string();
    }
}