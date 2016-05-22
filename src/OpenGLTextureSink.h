// Copyright (c) 2016 The HellBender Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef HB_VISIONGRAPHEDITOR_OPENGLTEXTURESINK_H
#define HB_VISIONGRAPHEDITOR_OPENGLTEXTURESINK_H

#include <VisionSystem/VSCore/ImageSinkBase.h>

#include <atomic>
#include <mutex>
#include <opencv2/opencv.hpp>

#include <GL/gl3w.h>

class OpenGLTextureSink : public hellbender::vs::ImageSinkBase
{
public:
    OpenGLTextureSink();

    void generateTexture();

    virtual void onNewImage(cv::Mat& image) override;

    GLuint textureID() const;
    cv::Mat getLastImage();
    long getCurrentImageID() const;
    void draw();

private:
    void updateTexture();

    std::atomic<float> aspect_;
    cv::Mat image_;
    std::mutex imageMutex_;
    GLuint textureID_;
    std::atomic<long> imageID_;
    long lastImageID_ = 0;
};

#endif // HB_VISIONGRAPHEDITOR_OPENGLTEXTURESINK_H
