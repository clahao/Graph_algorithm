//
// Created by moyu on 2022/9/27.
//

#ifndef GRAPH_ALGORITHM_KERNELS_ASYNC_PATH_H
#define GRAPH_ALGORITHM_KERNELS_ASYNC_PATH_H

#include "global.h"
#include<set>

template<class E>
void __attribute__((linx_kernel, noinline))
sssp_async(uint num_nodes, uint *offset, E *edgeList, uint *outDegree, uint *_offset, E *_edgeList, uint *inDegree, uint *RRG, uint *value, bool *label1, bool *label2, bool *visited){

    bool *active;
    bool *nextActive;
    bool finished = false;
    uint iter = 1;
    uint max_iter = 1;



    while (!finished){
        finished = true;
        max_iter = max(max_iter,iter);
        active = (iter % 2 == 1) ? label1 : label2;
        nextActive = (iter % 2 == 0) ? label1 : label2;

        for(uint id = 0; id < num_nodes; id ++){
            if(active[id]){
                active[id] = false;
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];

                for(uint j = nbegin; j < nend; j ++){
                    uint node = edgeList[j].end;
                    if(RRG[node] < iter){
//                        cout<<"iter: "<<iter<<"  node: "<<node<<"  RRG: "<<RRG[node]<<endl;
                        RRG[node] = iter;
                    }
                    if(!visited[node]){
                        finished = false;
                        visited[node] = true;
                        nextActive[node] = true;
                    }
                }
            }
        }
        iter ++;
    }
    uint update_times = 0;
    uint min_iter;
    uint id;
    std::set<uint> active_node;
    active_node.insert(0);
//    while(!active_node.empty()){
//        min_iter = DIST_INFINITY;
//        for(auto it = active_node.begin(); it != active_node.end(); it ++){
//            if(RRG[*it] < min_iter){
//                min_iter = RRG[*it];
//                id = *it;
//            }
//        }
//        active_node.erase(id);
//        uint nbegin = offset[id];
//        uint nend = nbegin + outDegree[id];
//        for(uint j = nbegin; j < nend; j ++){
//            uint finalDist = value[id] + edgeList[j].w8;
//            if(finalDist < value[edgeList[j].end]){
//                update_times ++;
//                value[edgeList[j].end] = finalDist;
//                active_node.insert(edgeList[j].end);
//            }
//        }
////        cout<<"update times: "<<update_times<<endl;
//    }
    cout<<"update times: "<<update_times<<endl;
//    for(uint iter = 1; iter <= max_iter; iter ++){
//        for(uint id = 0; id < num_nodes; id ++){
//            if(RRG[id] == iter){
//                uint nbegin = _offset[id];
//                uint nend = nbegin + inDegree[id];
//
//                for(uint j = nbegin; j < nend; j ++){
//                    if(value[_edgeList[j].end] != DIST_INFINITY){
//                        uint finalDist = value[_edgeList[j].end] + _edgeList[j].w8;
//                        if(finalDist < value[id]){
//                            value[id] = finalDist;
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }

}

#endif //GRAPH_ALGORITHM_KERNELS_ASYNC_PATH_H
