#include "solution.h"
#include <vector>
#include <tuple>
#include <climits>

// TODO: implement Kruskal using union-find; sort edges, greedily pick cheapest non-cycle edge
int kruskal_mst(int n, std::vector<std::tuple<int,int,int>> edges) {
    // TODO: sort by weight, use union-find
    return 0;
}

// TODO: implement Prim starting from node 0 using a min-priority queue
int prim_mst(int n, const std::vector<std::vector<std::pair<int,int>>>& adj) {
    // TODO: greedily grow the MST using a priority queue
    return 0;
}

// TODO: apply Kruskal or Prim to the connections list; return -1 if not fully connected
int min_cost_connect(int n, const std::vector<std::vector<int>>& connections) {
    // TODO: build edge list, run MST algorithm
    return 0;
}
