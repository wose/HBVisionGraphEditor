// Copyright (c) 2016 The HellBender Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "NodeGraph.h"
#include "OCVCameraNode.h"

NodeGraph::NodeGraph() :
    scrollPosition_{0.0, 0.0}
{
}

void NodeGraph::draw()
{
    ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
    ImGui::Begin("Node Graph", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_MenuBar);

    if(ImGui::BeginMenuBar()) {
        if(ImGui::BeginMenu("Nodes")) {
            ImGui::MenuItem("Image Source");
            ImGui::Separator();
            ImGui::MenuItem("Histogram");
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::BeginGroup();
    ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrollPosition_.x, scrollPosition_.y);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1,1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(40,40,40,200));

    ImGui::BeginChild("scrolling_region", ImVec2(0,0), true, ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoMove);

    ImGui::PushItemWidth(192.0f);
    ImVec2 offset = ImGui::GetCursorScreenPos() - scrollPosition_;

    for(auto& node : nodes_) {
        node->draw(offset);
    }

    // Scrolling
    if(ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
    {
        scrollPosition_.x -= ImGui::GetIO().MouseDelta.x;
        scrollPosition_.y -= ImGui::GetIO().MouseDelta.y;
    }

    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);

    if(ImGui::BeginPopupContextItem("Context Node Menu")) {
        if(ImGui::MenuItem("OpenCV Camera")) {
            addNode("OCVCamera");
        }
        ImGui::Separator();
        ImGui::MenuItem("Dummy");
        ImGui::EndPopup();
    }

    ImGui::EndGroup();
    ImGui::End();
    ImGui::PopStyleVar();
}

void NodeGraph::addNode(const std::string& nodeType)
{
    if(nodeType == "OCVCamera") {
        std::unique_ptr<Node> node(new OCVCameraNode);
        nodes_.emplace_back(std::move(node));
    } else {
    }
}
