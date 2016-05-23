// Copyright (c) 2016 The HellBender Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef HB_VISIONGRAPHEDITOR_OCVCAMERANODE_H
#define HB_VISIONGRAPHEDITOR_OCVCAMERANODE_H

#include "Node.h"
#include "OpenGLTextureSink.h"
#include "imgui.h"

#include "VisionSystem/OCVCamera/OCVCamera.h"

#include <opencv2/opencv.hpp>

class OCVCameraNode : public Node
{
public:
    OCVCameraNode(ImVec2 position);
    OCVCameraNode(ImVec2 position, std::shared_ptr<hellbender::vs::OCVCamera> cam);

    void setCamera(std::shared_ptr<hellbender::vs::OCVCamera> cam);
    virtual void drawNodeContent() override;

private:
    char videoFile_[256];
    char imageDirectory_[256];
    int sourceType_ = 0;
    int cameraID_ = 0;
    cv::Mat lastImage_;
    std::shared_ptr<hellbender::vs::OCVCamera> cam_;
    OpenGLTextureSink oglTextureSink;
};

#endif // HB_VISIONGRAPHEDITOR_OCVCAMERANODE_H
