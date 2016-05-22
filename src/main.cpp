#include "OpenGLTextureSink.h"
#include "Node.h"
#include "NodeGraph.h"

#include <VisionSystem/OCVCamera/OCVCamera.h>

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <opencv2/opencv.hpp>


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow* window = glfwCreateWindow(1280, 720, "HBVisionGraphEditor", NULL, NULL);
    glfwMakeContextCurrent(window);
    gl3wInit();

    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    //    hellbender::vs::OCVCamera ocvcam(0);
    //    OCVCameraNode node(std::make_shared<hellbender::vs::OCVCamera>(0));
    NodeGraph nodeGraph;

    ImVec4 clear_color = ImColor(39, 40, 34);

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_Text]                  = ImVec4(0.83f, 0.95f, 0.95f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.39f, 0.80f, 0.80f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.09f, 0.27f, 0.27f, 0.87f);
    style.Colors[ImGuiCol_ChildWindowBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
    style.Colors[ImGuiCol_Border]                = ImVec4(0.25f, 0.75f, 0.75f, 1.00f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.18f, 0.55f, 0.55f, 0.67f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.25f, 0.75f, 0.75f, 0.67f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.16f, 0.49f, 0.49f, 0.45f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.16f, 0.49f, 0.49f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.16f, 0.49f, 0.49f, 0.60f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.18f, 0.56f, 0.56f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.16f, 0.47f, 0.47f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.11f, 0.33f, 0.33f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.14f, 0.42f, 0.42f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.33f, 0.78f, 0.78f, 0.67f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.33f, 0.78f, 0.78f, 1.00f);
    style.Colors[ImGuiCol_ComboBg]               = ImVec4(0.18f, 0.55f, 0.55f, 0.99f);
    style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.44f, 0.81f, 0.81f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.33f, 0.78f, 0.78f, 0.60f);
    style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.33f, 0.78f, 0.78f, 1.00f);
    style.Colors[ImGuiCol_Button]                = ImVec4(0.18f, 0.55f, 0.55f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(0.33f, 0.78f, 0.78f, 0.67f);
    style.Colors[ImGuiCol_ButtonActive]          = ImVec4(0.33f, 0.78f, 0.78f, 1.00f);
    style.Colors[ImGuiCol_Header]                = ImVec4(0.15f, 0.44f, 0.44f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(0.33f, 0.78f, 0.78f, 0.67f);
    style.Colors[ImGuiCol_HeaderActive]          = ImVec4(0.33f, 0.78f, 0.78f, 1.00f);
    style.Colors[ImGuiCol_Column]                = ImVec4(0.25f, 0.74f, 0.74f, 0.40f);
    style.Colors[ImGuiCol_ColumnHovered]         = ImVec4(0.25f, 0.74f, 0.74f, 0.60f);
    style.Colors[ImGuiCol_ColumnActive]          = ImVec4(0.25f, 0.74f, 0.74f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.20f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.53f, 0.84f, 0.84f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.53f, 0.84f, 0.84f, 1.00f);
    style.Colors[ImGuiCol_CloseButton]           = ImVec4(0.22f, 0.67f, 0.67f, 1.00f);
    style.Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(0.53f, 0.84f, 0.84f, 0.40f);
    style.Colors[ImGuiCol_CloseButtonActive]     = ImVec4(0.53f, 0.84f, 0.84f, 1.00f);
    style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.53f, 0.84f, 0.84f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.00f, 0.84f, 0.84f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.53f, 0.84f, 0.84f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.00f, 0.84f, 0.84f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.13f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.09f, 0.27f, 0.27f, 0.67f);

    cv::Mat r_hist, b_hist, g_hist;
    cv::Mat lastImage;
    while (!glfwWindowShouldClose(window))
    {
        if(!glfwGetWindowAttrib(window, GLFW_ICONIFIED) && glfwGetWindowAttrib(window, GLFW_VISIBLE))
        {
            glfwPollEvents();
            ImGui_ImplGlfwGL3_NewFrame();

            static bool showDebug = false;
            if(!ImGui::GetIO().WantCaptureKeyboard) {
                if(ImGui::IsKeyPressed('`')) showDebug= !showDebug;
                // somehow letters are not working...
                if(ImGui::IsKeyPressed('0')) glfwSetWindowShouldClose(window, GL_TRUE);
            }

            {
                if(showDebug) {
                    ImGui::SetNextWindowPos(ImVec2(0,0));
                    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x,ImGui::GetIO().DisplaySize.y / 2 ));

                    ImGui::Begin("Debug Console", &showDebug, ImVec2(0,0), 0.5f,
                                 ImGuiWindowFlags_NoTitleBar
                                 |ImGuiWindowFlags_NoResize
                                 |ImGuiWindowFlags_NoMove
                                 |ImGuiWindowFlags_NoSavedSettings);

                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                    ImGui::Separator();

                    ImGui::Text("Window %.1f x %.1f", ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
                    int histSize = 256;
                    float range[] = { 0, 256 } ;
                    const float* histRange = { range };

                    /*  lastImage = oglTextureSink.getLastImage();
                        std::vector<cv::Mat> bgr_planes;
                        cv::split(lastImage, bgr_planes);

                        cv::calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, true, false);
                        cv::calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, true, false);
                        cv::calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, true, false);

                        cv::normalize(b_hist, b_hist, 0, 1.0, cv::NORM_MINMAX, -1, cv::Mat());
                        cv::normalize(g_hist, g_hist, 0, 1.0, cv::NORM_MINMAX, -1, cv::Mat());
                        cv::normalize(r_hist, r_hist, 0, 1.0, cv::NORM_MINMAX, -1, cv::Mat());

                        float* blueData = (float*)b_hist.data;
                        float* greenData = (float*)g_hist.data;
                        float* redData = (float*)r_hist.data;
                        ImGui::PlotHistogram("Blue Channel", blueData, b_hist.rows, 0, NULL, 0.0f, 1.0f, ImVec2(0,80));
                        ImGui::PlotHistogram("Green Channel", greenData, g_hist.rows, 0, NULL, 0.0f, 1.0f, ImVec2(0,80));
                        ImGui::PlotHistogram("Red Channel", redData, r_hist.rows, 0, NULL, 0.0f, 1.0f, ImVec2(0,80));
                    */
                    ImGui::End();
                }
            }

            nodeGraph.draw();

            /*
            ImGui::Begin("Node Editor");
            ImGui::BeginGroup();
            static ImVec2 scrolling(0.0, 0.0);
            ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1,1));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
            ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(40,40,40,200));

            ImGui::BeginChild("scrolling_region", ImVec2(0,0), true, ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoMove);
            ImGui::PushItemWidth(192.0f);

            // Scrolling
            if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
            {
                scrolling.x -= ImGui::GetIO().MouseDelta.x;
                scrolling.y -= ImGui::GetIO().MouseDelta.y;
            }
            ImVec2 offset = ImGui::GetCursorScreenPos() - scrolling;

            node.draw(offset);


            ImGui::PopItemWidth();
            ImGui::EndChild();
            ImGui::PopStyleColor();
            ImGui::PopStyleVar(2);
            ImGui::EndGroup();
            ImGui::End();

            */

            // Rendering
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui::Render();
            glfwSwapBuffers(window);
        } else {
            glfwWaitEvents();
        }
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}
