// Copyright (c) 2016 The HellBender Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef HB_VISIONGRAPHEDITOR_OPENGLTEXTURESINK_H
#define HB_VISIONGRAPHEDITOR_OPENGLTEXTURESINK_H

#include <VisionSystem/VSCore/AbstractImageSink.h>

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

#include <atomic>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

class OpenGLTextureSink : public hellbender::vs::AbstractImageSink
{
public:
    OpenGLTextureSink()
    {
        aspect_ = 1.0;
        imageID_ = 0;
    }

    void generateTexture()
    {
        glGenTextures(1, &textureID_);
    }

    virtual void onNewImage(cv::Mat& image) override
    {
        std::lock_guard<std::mutex> lock(imageMutex_);
        image_ = image.clone();
        aspect_ = (float) image_.cols / (float) image_.rows;
        ++imageID_;
    }

    GLuint textureID() const
    {
        return textureID_;
    }

    cv::Mat getLastImage()
    {
        if(lastImageID_ > 0)
        {
            std::lock_guard<std::mutex> lock(imageMutex_);
            return image_.clone();
        } else {
            return cv::Mat(200, 200, CV_8UC3);
        }
    }

    long getCurrentImageID() const
    {
        return lastImageID_;
    }

    void draw()
    {
        updateTexture();
        ImGui::Image(reinterpret_cast<void*>(textureID_), ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvailWidth() / aspect_));
    }

private:
    void updateTexture()
    {
        if(imageID_ != lastImageID_) {
            glBindTexture(GL_TEXTURE_2D, textureID_);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // TODO use FBOs
            std::lock_guard<std::mutex> lock(imageMutex_);
            if(lastImageID_ > 0) {
                glTexSubImage2D(GL_TEXTURE_2D,
                             0,
                             0,
                             0,
                             image_.cols,
                             image_.rows,
                             GL_BGR,
                             GL_UNSIGNED_BYTE,
                             image_.ptr());
            } else {
                glTexImage2D(GL_TEXTURE_2D,
                             0,
                             GL_RGB,
                             image_.cols,
                             image_.rows,
                             0,
                             GL_BGR,
                             GL_UNSIGNED_BYTE,
                             image_.ptr());
            }
            lastImageID_ = imageID_;
        }
    }

    std::atomic<float> aspect_;
    cv::Mat image_;
    std::mutex imageMutex_;
    GLuint textureID_;
    std::atomic<long> imageID_;
    long lastImageID_ = 0;
};

#endif // HB_VISIONGRAPHEDITOR_OPENGLTEXTURESINK_H
