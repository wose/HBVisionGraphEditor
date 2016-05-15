#include "OpenGLTextureSink.h"

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
    hellbender::vs::OCVCamera ocvcam(0);
    OpenGLTextureSink oglTextureSink;
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
    oglTextureSink.generateTexture();
    oglTextureSink.connectTo(&ocvcam);

    ImVec4 clear_color = ImColor(30, 30, 30);


    cv::Mat r_hist, b_hist, g_hist;
    cv::Mat lastImage;
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        {
            //            oglTextureSink.updateTexture();

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::Separator();

            int histSize = 256;
            float range[] = { 0, 256 } ;
            const float* histRange = { range };

            lastImage = oglTextureSink.getLastImage();
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
        }

        ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Image Source");
        ImGui::Text("%dpx x %dpx", lastImage.cols, lastImage.rows);
        ImGui::SameLine();
        ImGui::Text("ID: %ld", oglTextureSink.getCurrentImageID());
        ImGui::Separator();
        oglTextureSink.draw();
        ImGui::Separator();
        if(ImGui::Button("Pause")) {
            if(ocvcam.isGrabbing()) {
                ocvcam.stop();
            } else {
                ocvcam.start();
            }
        }
        ImGui::End();

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    glfwTerminate();

    return 0;
}
