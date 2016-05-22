// Copyright (c) 2016 The HellBender Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OpenGLTextureSink.h"
#include <imgui.h>

OpenGLTextureSink::OpenGLTextureSink() :
    aspect_{1.0},
    imageID_{0}
{
}

void OpenGLTextureSink::generateTexture()
{
    glGenTextures(1, &textureID_);
}

void OpenGLTextureSink::onNewImage(cv::Mat& image)
{
    std::lock_guard<std::mutex> lock(imageMutex_);
    image_ = image.clone();
    aspect_ = (float) image_.cols / (float) image_.rows;
    ++imageID_;
}

GLuint OpenGLTextureSink::textureID() const
{
    return textureID_;
}

cv::Mat OpenGLTextureSink::getLastImage()
{
    if(lastImageID_ > 0)
    {
        std::lock_guard<std::mutex> lock(imageMutex_);
        return image_.clone();
    } else {
        return cv::Mat(200, 200, CV_8UC3);
    }
}

long OpenGLTextureSink::getCurrentImageID() const
{
    return lastImageID_;
}

void OpenGLTextureSink::draw()
{
    updateTexture();
    float width = ImGui::CalcItemWidth();
    ImGui::Image(reinterpret_cast<void*>(textureID_), ImVec2(width, width / aspect_));
}

void OpenGLTextureSink::updateTexture()
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
