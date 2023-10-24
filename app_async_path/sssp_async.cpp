//
// Created by moyu on 2022/9/27.
//

#include "../Graph.h"
#include "../Graph.cpp"
#include "../kernels_async_path.h"
#include "../timer.h"


int main(int argc, char ** argv){
    string filename(argv[1]);
    Graph<OutEdgeWeighted> graph(filename, true);
    uint sourceNode = 0;

    uint *RRG = (uint *)malloc(graph.num_nodes * sizeof(uint));
    bool *visited = (bool *)malloc(graph.num_nodes * sizeof(bool));

    for(unsigned int i = 0; i < graph.num_nodes; i ++){
        RRG[i] = 0;
        visited[i] = false;
        graph.value[i] = DIST_INFINITY;
        graph.label1[i] = false;
        graph.label2[i] = false;
    }
    graph.value[sourceNode] = 0;
    graph.label1[sourceNode] = true;
    visited[sourceNode] = true;

    Timer timer;
    timer.Start();

    sssp_async(graph.num_nodes,
               graph.offset,
               graph.edgeList,
               graph.outDegree,
               graph._offset,
               graph._edgeList,
               graph.inDegree,
                RRG,
               graph.value,
               graph.label1,
               graph.label2,
                visited);

    float runtime = timer.Finish();
    cout << "Processing finished in " << runtime/1000 << " (s).\n";

    for(int i = 0; i < graph.num_nodes; i ++){
        cout << RRG[i] << " ";
//        cout << graph.value[i] << " ";
    }
    cout <<endl;
    return 0;
}