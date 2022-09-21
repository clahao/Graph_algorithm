//
// Created by moyu on 2022/9/21.
//

#include "../Graph.h"
#include "../Graph.cpp"
#include "../kernels_async_priority_schedule.h"
#include "../timer.h"


int main(int argc, char ** argv){
    string filename(argv[1]);
    Graph<OutEdge> graph(filename, false);
    uint sourceNode = 0;

    for(unsigned int i = 0; i < graph.num_nodes; i ++){
        graph.value[i] = i;
        graph.label1[i] = true;
    }

    queue<uint> queue;
    //queue[0].push(sourceNode);
    Timer timer;
    timer.Start();

    cc_async(graph.num_nodes,
             graph.offset,
             graph.edgeList,
             graph.outDegree,
             graph.value,
             graph.label1,
              queue);

    float runtime = timer.Finish();
    cout << "Processing finished in " << runtime/1000 << " (s).\n";

    for(int i = 0; i < 100; i ++){
        cout << graph.value[i] << " ";
    }
    cout <<endl;
    return 0;
}