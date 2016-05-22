// Copyright (c) 2016 The HellBender Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef HB_VISIONGRAPHEDITOR_NODE_H
#define HB_VISIONGRAPHEDITOR_NODE_H

#include <imgui.h>

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)            { return ImVec2(lhs.x+rhs.x, lhs.y+rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)            { return ImVec2(lhs.x-rhs.x, lhs.y-rhs.y); }

class Node
{
public:
    Node ();
    virtual ~Node() {}

    virtual void drawNodeContent() = 0;
    void draw(const ImVec2& offset);

    ImVec2 objectInputSlotPosition(int slot) const;
    ImVec2 objectOutputSlotPosition(int slot) const;

    ImVec2 getSize() const;

private:
    static int nextNodeID_;

    ImVec2 position_;
    ImVec2 size_;
    int ID_;
    int objectInputSlots_ = 1;
    int valueInputSlots_ = 2;
    int objectOutputSlots_ = 1;
    int valueOutputSlots_ = 1;
};

#endif // HB_VISIONGRAPHEDITOR_NODE_H
