#pragma once

#ifndef SCHMGR_CORE_BACK_NAV_NAV_
#define SCHMGR_CORE_BACK_NAV_NAV_

#include "schmgr/core/mtl/mtl.h"

#include <iostream>

#include <limits>
#include <fstream>
#include <algorithm>

namespace core::nav
{

class Graph {
private:
    using pii = std::pair<int, int>;
    mtl::vector<mtl::vector<pii>> adjacencyList;  // 图的邻接表表示, 第一层为顶点, 第二层为邻居顶点和边的权重

public:

    Graph() = default;
    Graph(int numVertices) {
        adjacencyList.resize(numVertices);
    }

    bool readGraphFromFile(const mtl::string& filename);                         // 从文件中读取图
    void addEdge(int vertex1, int vertex2, int distance);                   // 添加无向边
    int getDistance(int vertex1, int vertex2);                              // 计算两点之间的距离
    int getShortestDistance(int start, int end);                            // 计算两点之间的最短距离
    mtl::vector<int> getShortestPath(int start, int end);                        // 计算两点之间的最短路径
    mtl::vector<int> getShortestTrip(int start, mtl::vector<int> destinations);       // 计算起点途径多个终点的最优路线
    // void printGraph();                                                      // 打印图, 用于调试
};


} // namespace core::nav

#endif // SCHMGR_CORE_BACK_NAV_NAV_