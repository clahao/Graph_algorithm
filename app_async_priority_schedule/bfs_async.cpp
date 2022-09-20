//
// Created by moyu on 2022/9/16.
//

#include "../Graph.h"
#include "../Graph.cpp"
#include "../kernels_async_priority_schedule.h"
#include "../timer.h"


int main(int argc, char ** argv){
    string filename(argv[1]);
    Graph<OutEdge> graph(filename, false);
    int sourceNode = 0;

    for(unsigned int i = 0; i < graph.num_nodes; i ++){
        graph.value[i] = DIST_INFINITY;
    }
    graph.value[sourceNode] = 0;
//    IndexMinPriorityQueue<uint> queue(graph.num_nodes);
//    queue.insert(sourceNode,0);
//    queue<int> queue;
//    queue.push(sourceNode);
//    graph.value[sourceNode] = 0;
//    graph.label1[sourceNode] = true;
    queue<pair<uint,uint>> queue[package_num];
    queue[0].push(make_pair(sourceNode,0));
    Timer timer;
    timer.Start();

    bfs_async(graph.offset,
              graph.edgeList,
              graph.outDegree,
              graph.value,
              queue);

    float runtime = timer.Finish();
    cout << "Processing finished in " << runtime/1000 << " (s).\n";

    for(int i = 0; i < 20; i ++){
        cout << graph.value[i] << " ";
    }
    cout <<endl;
    return 0;
}