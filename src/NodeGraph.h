// Copyright (c) 2016 The HellBender Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef HB_VISIONGRAPHEDITOR_NODEGRAPH_H
#define HB_VISIONGRAPHEDITOR_NODEGRAPH_H

#include "Node.h"

#include <imgui.h>
#include <memory>
#include <vector>

class NodeGraph
{
 public:
    NodeGraph();

    void draw();

private:
    void addNode(const std::string& nodeType);

    ImVec2 scrollPosition_;
    std::vector<std::unique_ptr<Node>> nodes_;
};

#endif // HB_VISIONGRAPHEDITOR_NODEGRAPH_H
