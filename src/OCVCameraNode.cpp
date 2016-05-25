// Copyright (c) 2016 The HellBender Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "OCVCameraNode.h"

#include <VisionSystem/VSCore/ImageSinkBase.h>
#include <VisionSystem/OCVCamera/OCVCamera.h>
#include "imgui.h"

OCVCameraNode::OCVCameraNode(ImVec2 position) :
    Node(position, 0, 0, 1, 0)
{
    oglTextureSink.generateTexture();
}

OCVCameraNode::OCVCameraNode(ImVec2 position, std::shared_ptr<hellbender::vs::OCVCamera> cam) :
    Node(position, 0, 0, 1, 0),
    cam_(cam)
{
    oglTextureSink.connectTo(cam_);
    oglTextureSink.generateTexture();
}

void OCVCameraNode::setCamera(std::shared_ptr<hellbender::vs::OCVCamera> cam)
{
    cam_ = cam;

    oglTextureSink.connectTo(cam_);
    oglTextureSink.generateTexture();
}

void OCVCameraNode::drawNodeContent()
{
    ImGui::Text("OpenCV Camera");

    ImGui::Combo("Source Type", &sourceType_, "Camera\0Video File\0Image Folder\0");
    switch (sourceType_) {
        case 0:
            ImGui::SliderInt("Camera", &cameraID_, 0, 4);
            break;
        case 1:
            ImGui::InputText("File", videoFile_, 256);
            break;
        case 2:
            ImGui::InputText("Directory", imageDirectory_, 256);
            break;
    }

    if(cam_) {
        if(ImGui::Button("Close Device")) {
            cam_.reset();
        }
    } else {
        if(ImGui::Button("Open Device")) {
            if(cam_) {
                oglTextureSink.disconnect();
            }

            cam_.reset();
            cam_ = std::make_shared<hellbender::vs::OCVCamera>(cameraID_);
            oglTextureSink.connectTo(cam_);
        }
    }

    if(oglTextureSink.isConnected()) {
        oglTextureSink.draw();
    }
    if(cam_) {
        if(ImGui::Button("Pause")) {
            if(cam_->isGrabbing()) {
                cam_->stop();
            } else {
                cam_->start();
            }
        }
    } else {
        ImGui::TextColored(ImVec4(0.8, 0.0, 0.0, 1.0), "Invalid Camera pointer");
    }
}
