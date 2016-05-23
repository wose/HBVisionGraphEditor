// Copyright (c) 2016 The HellBender Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "Node.h"

int Node::nextNodeID_ = 1;

Node::Node(ImVec2 position, int objectInputs, int valueInputs, int objectOutputs, int valueOutputs) :
    position_{position},
    objectInputs_{objectInputs},
    valueInputs_{valueInputs},
    objectOutputs_{objectOutputs},
    valueOutputs_{valueOutputs},
    size_{200, 10},
    ID_{++nextNodeID_}
{
}

void Node::draw(const ImVec2& offset, int& selectedNode, int& hoveredNode)
{
    bool isNodeHovered = false;
    ImGui::BeginGroup();
    ImGui::PushID(ID_);

    ImVec2 node_rect_min = offset + position_;

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->ChannelsSplit(2);
    draw_list->ChannelsSetCurrent(1);

    bool old_any_active = ImGui::IsAnyItemActive();
    ImGui::SetCursorScreenPos(node_rect_min + ImVec2(4,4));
    ImGui::BeginGroup(); // Lock horizontal position
    drawNodeContent();
    ImGui::EndGroup();

    size_ = ImGui::GetItemRectSize() + ImVec2(8, 8);
    ImVec2 node_rect_max = node_rect_min + size_;

    draw_list->ChannelsSetCurrent(0); // Background
    ImGui::SetCursorScreenPos(node_rect_min);
    ImGui::InvisibleButton("node", size_);

    if (ImGui::IsItemHovered()) {
        isNodeHovered = true;
        hoveredNode = ID_;
    }

    bool isMovingActive = ImGui::IsItemActive();

    ImColor backgroundColor = isNodeHovered ? ImColor(70, 70, 70) : ImColor(60, 60, 60);
    draw_list->AddRectFilled(node_rect_min, node_rect_max, backgroundColor, 4.0f);
    draw_list->AddRect(node_rect_min, node_rect_max, ImColor(100,100,100), 4.0f);

    for(auto slot = 0; slot < objectInputs_; ++slot) {
        ImVec2 slotPosition = objectInputSlotPosition(slot);
        draw_list->AddCircleFilled(offset + slotPosition, 4.0f, ImColor(150, 150, 150));
    }

    for(auto slot = 0; slot < objectOutputs_; ++slot) {
        ImVec2 slotPosition = objectOutputSlotPosition(slot);
        draw_list->AddCircleFilled(offset + slotPosition, 4.0f, ImColor(150, 150, 150));
    }

    if(isMovingActive) {
        selectedNode = ID_;
    }

    if(isMovingActive && ImGui::IsMouseDragging(0)) {
        position_ = position_ + ImGui::GetIO().MouseDelta;
    }

    ImGui::PopID();
    ImGui::EndGroup();

    draw_list->ChannelsMerge();
}

ImVec2 Node::objectInputSlotPosition(int slot) const
{
    return ImVec2(position_.x, position_.y + size_.y * ((float) slot + 1) / ((float) objectInputs_ + 1));
}

ImVec2 Node::objectOutputSlotPosition(int slot) const
{
    return ImVec2(position_.x + size_.x, position_.y + size_.y * ((float)slot + 1) / ((float) objectOutputs_ + 1));
}

ImVec2 Node::getSize() const
{
    return size_;
}
