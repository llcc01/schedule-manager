#include "schmgr/core/back/nav/nav.h"
#include "schmgr/core/back/api_back.h"

namespace core::nav
{

bool Graph::readGraphFromFile(const mtl::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filename << std::endl;
        return false;
    }

    adjacencyList.clear();
    int vertex1, vertex2, distance;
    while (file >> vertex1 >> vertex2 >> distance) {
        // back::sysLogger.Log("NAV", "load vertex: "+std::to_string(vertex1) + " " + std::to_string(vertex2) + " " + std::to_string(distance));
        addEdge(vertex1, vertex2, distance);
    }

    file.close();

    return true;
}

// 添加无向边
void Graph::addEdge(int vertex1, int vertex2, int distance) {
    // 无向图, 边是双向的, 分别添加了方向为vertex1->vertex2和vertex2->vertex1的边
    if (adjacencyList.size() < std::max(vertex1, vertex2) + 1) {
        adjacencyList.resize(std::max(vertex1, vertex2) + 1);
    }
    adjacencyList[vertex1].push_back(std::make_pair(vertex2, distance));
    adjacencyList[vertex2].push_back(std::make_pair(vertex1, distance));
}

int Graph::getDistance(int vertex1, int vertex2) {
    for (auto neighbor : adjacencyList[vertex1]) {
        if (neighbor.first == vertex2)
            return neighbor.second;
    }
    return std::numeric_limits<int>::max();
}

int Graph::getShortestDistance(int start, int end) {
    mtl::vector<int> path = getShortestPath(start, end);
    int distance = 0;
    for (int i = 0; i < path.size() - 1; ++i) {
        distance += getDistance(path[i], path[i + 1]);
    }

    return distance;
}

mtl::vector<int> Graph::getShortestPath(int start, int end) {
    size_t numVertices = adjacencyList.size(); // 图的顶点数

    // 初始化距离数组和前驱数组
    mtl::vector<int> dist(numVertices, std::numeric_limits<int>::max());
    mtl::vector<int> prev(numVertices, -1);

    // 初始化访问数组
    mtl::vector<bool> visited(numVertices, false);

    // 使用小顶堆实现优先队列, 排序的依据为距离, 距离越小排在越前面
    mtl::priority_queue<pii, mtl::vector<pii>, std::greater<pii>> pq;

    // 将起始顶点加入优先队列
    pq.push(std::make_pair(0, start));

    // 初始化起始顶点的距离为0
    dist[start] = 0;

    while (!pq.empty()) {
        int currVertex = pq.top().second;   // 当前顶点, 为优先队列中距离最小的顶点
        pq.pop();                           // 从优先队列中删除当前顶点
        visited[currVertex] = true;         // 标记当前顶点已访问

        // 检查是否达到目标顶点
        if (currVertex == end)
            break;

        // 遍历当前顶点的所有邻居
        for (auto neighbor : adjacencyList[currVertex]) {
            int nextVertex = neighbor.first;            // 邻居顶点
            int weight = neighbor.second;               // 边的权重
            int newDist = dist[currVertex] + weight;    // 新的距离, 为当前顶点的距离加上边的权重

            // 如果新的路径更短，则更新距离和前驱
            if (!visited[nextVertex] && newDist < dist[nextVertex]) {   // 如果邻居顶点未访问且新的距离更短
                dist[nextVertex] = newDist;                             // 更新距离
                prev[nextVertex] = currVertex;                          // 更新前驱
                pq.push(std::make_pair(newDist, nextVertex));                // 将邻居顶点加入优先队列
            }
        }
    }

    // 返回最短路径
    mtl::vector<int> path;
    int currVertex = end;       // 从目标顶点开始
    while (currVertex != -1) {  // 直到到达起始顶点
        // back::sysLogger.Log("INFO", std::to_string(currVertex));
        path.insert(path.begin(), 1, currVertex);   // 将当前顶点加入路径
        currVertex = prev[currVertex];              // 更新当前顶点为前驱顶点
    }

    return path;
}

mtl::vector<int> Graph::getShortestTrip(int start, mtl::vector<int> destinations) {
    // 贪心算法实现, 先通过shortestPath计算出它到各个目的地的最短路径, 然后去往距离最短的目的地
    // 然后再从该目的地出发, 重复上述过程, 直到所有目的地都到达

    // back::sysLogger.Log("NAV", "getShortestTrip "+std::to_string(start));
    // for (auto destination : destinations) {
    //     back::sysLogger.Log("NAV", "to " + std::to_string(destination));
    // }
    // back::sysLogger.Log("NAV", "adjacencyList: " + std::to_string(adjacencyList.size()));

    mtl::vector<int> path;       // 最优路线
    int currVertex = start; // 当前顶点, 初始为起始顶点

    // 从起始顶点开始, 依次到达所有目的地
    while (!destinations.empty()) {
        // 计算当前顶点到所有目的地的最短路径
        mtl::vector<int> distances;
        for (auto destination : destinations) {
            distances.push_back(getShortestDistance(currVertex, destination));
        }

        // 找到距离最短的目的地
        int minDistance = std::numeric_limits<int>::max();
        int minIndex = -1;
        for (int i = 0; i < distances.size(); i++) {
            if (distances[i] < minDistance) {
                minDistance = distances[i];
                minIndex = i;
            }
        }

        // 从当前顶点到达剩下的最近的目的地
        int destination = destinations[minIndex];
        mtl::vector<int> shortestPath = getShortestPath(currVertex, destination);
        for (mtl::vector<int>::iterator it = shortestPath.begin(); it != shortestPath.end(); it++) {
            path.push_back(*it);
        }

        // 更新当前顶点为目的地
        currVertex = destination;

        // 从目的地列表中删除已到达的目的地
        destinations.erase(destinations.begin() + minIndex);
    }

    return path;

}

// void Graph::printGraph() {
//     int numVertices = adjacencyList.size();
//     for (int i = 0; i < numVertices; i++) {
//         cout << i << ": ";
//         for (auto neighbor : adjacencyList[i]) {
//             cout << neighbor.first << "(" << neighbor.second << ") ";
//         }
//         cout << endl;
//     }
// }

// int main() {
//     Graph map(92);
//     map.readGraphFromFile("Graph.txt");

//     int start = 55;
//     vector<int> destinations = { 21, 7, 87, 65 };
//     vector<int> path = map.getShortestTrip(start, destinations);

//     cout << "Shortest round trip: ";
//     for (auto vertex : path) {
//         cout << vertex << " ";
//     }
//     cout << endl;

//     return 0;
// }

} // namespace core::nav