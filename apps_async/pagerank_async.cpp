//
// Created by moyu on 2022/9/14.
//
#include "../Graph.h"
#include "../Graph.cpp"
#if defined(OPENMP)
#include "../kernels_async_parallel.h"
#else
#include "../kernels_async.h"
//#include "../kernels_async_dev.h"
#endif
#include "../timer.h"

int main(int argc, char ** argv){
    string filename(argv[1]);
    Graph<OutEdge> graph(filename, false);
    int sourceNode = 0;

    for(unsigned int i = 0; i < graph.num_nodes; i ++){
        graph.pr_value[i] = 0;
        graph.delta1[i] = 1 - damping_factor;
    }

    uint itr = 0;   //迭代次数
    Timer timer;
    timer.Start();

    pagerank_async_priority(graph.num_nodes,
                   &itr,
                   graph.offset,
                   graph.edgeList,
                   graph.outDegree,
                   graph.pr_value,
                   graph.delta1);

    cout << "processing times: " << itr << endl;
    float runtime = timer.Finish();
    cout << "Processing finished in " << runtime/1000 << " (s).\n";

    for(int i = 0; i < 20; i ++){
        cout << graph.pr_value[i] << " ";
    }
    cout <<endl;
    return 0;
}