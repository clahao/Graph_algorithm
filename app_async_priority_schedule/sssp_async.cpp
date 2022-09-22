//
// Created by moyu on 2022/9/16.
//

#include "../Graph.h"
#include "../Graph.cpp"
#include "../kernels_async_priority_schedule.h"
#include "../timer.h"


int main(int argc, char ** argv){
    string filename(argv[1]);
    Graph<OutEdgeWeighted> graph(filename, true);
    uint sourceNode = 0;

    for(unsigned int i = 0; i < graph.num_nodes; i ++){
        graph.value[i] = DIST_INFINITY;
        graph.label1[i] = false;
    }
    graph.value[sourceNode] = 0;
    graph.label1[sourceNode] = true;
    graph.queue[0][sourceNode] = 0;
//    queue<uint> queue[package_num];
//    queue[0].push(sourceNode);
    Timer timer;
    timer.Start();

    sssp_async(graph.num_nodes,
               graph.offset,
               graph.edgeList,
               graph.outDegree,
               graph.value,
               graph.label1,
               graph.queue);

    float runtime = timer.Finish();
    cout << "Processing finished in " << runtime/1000 << " (s).\n";

    for(int i = 0; i < 20; i ++){
        cout << graph.value[i] << " ";
    }
    cout <<endl;
    return 0;
}