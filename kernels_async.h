//
// Created by moyu on 2022/9/14.
//

#ifndef GRAPH_ALGORITHM_KERNELS_ASYNC_H
#define GRAPH_ALGORITHM_KERNELS_ASYNC_H

#include "global.h"

struct cmp_pr{
    bool operator()(pair<int,double> &a, pair<int,double> &b){
        if(a.second != b.second)
            return a.second < b.second;
        else
            return a.first < b.first;
    }
};

template<class E>
void __attribute__((linx_kernel, noinline))
pagerank_async_priority(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, Adouble *value, Adouble *delta){

    priority_queue<pair<int,double>,vector<pair<int,double>>,cmp_pr> priorityQueue;
    priority_queue<pair<int,double>,vector<pair<int,double>>,cmp_pr> priorityQueue1;
    for(int i = 0; i < num_nodes; i ++){
        priorityQueue.push(make_pair(i,delta[i]));
    }
    bool finished = false;

    while (!priorityQueue.empty() || !priorityQueue1.empty()){

//        if(priorityQueue.empty()){
//            priorityQueue = priorityQueue1;
//            priorityQueue1 = priority_queue<pair<int,double>,vector<pair<int,double>>,cmp_pr>();
//        }

        int id = priorityQueue.top().first;
        priorityQueue.pop();
        if(delta[id] > threshold){
            value[id] = value[id] + delta[id];

            if(outDegree[id] > 0){
                double finalDelta = delta[id] * damping_factor / outDegree[id];
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];

                for(uint j = nbegin; j < nend; j ++){
                    uint dest = edgeList[j].end;
                    delta[dest] = delta[dest] + finalDelta;
                    if(delta[dest] > threshold){
                        priorityQueue.push(make_pair(dest,delta[dest]));
                    }
                }
                finished = false;
            }
            delta[id] = 0;
        }
        (*iter) ++;
    }
}

template<class E>
void __attribute__((linx_kernel, noinline))
pagerank_async(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, Adouble *value, Adouble *delta){

    bool finished = false;

    while (!finished){
        finished = true;

        for(uint id = 0; id < num_nodes; id ++){
            if(delta[id] > threshold){
                value[id] = value[id] + delta[id];

                if(outDegree[id] > 0){
                    double finalDelta = delta[id] * damping_factor / outDegree[id];
                    uint nbegin = offset[id];
                    uint nend = nbegin + outDegree[id];

                    for(uint j = nbegin; j < nend; j ++){
                        uint dest = edgeList[j].end;
                        delta[dest] = delta[dest] + finalDelta;
                    }
                    finished = false;
                }
                delta[id] = 0;
            }
        }
        (*iter) ++;
    }
}

template<class E>
void __attribute__((linx_kernel, noinline))
bfs_async(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active){

    bool finished = false;
//    uint update_times = 0;

    while (!finished){
        finished = true;

        for(uint id = 0; id < num_nodes; id ++){
            if(active[id]){
                active[id] = false;
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];
                uint finalDist = value[id] + 1;

                for(uint j = nbegin; j < nend; j ++){
                    uint dest = edgeList[j].end;
                    if(finalDist < value[dest]){
//                        update_times ++;
                        value[dest] = finalDist;
                        finished = false;
                        active[dest] = true;
                    }
                }
            }
        }
        (*iter) ++;
    }
//    cout<<"update times: "<<update_times<<endl;
}

template<class E>
void __attribute__((linx_kernel, noinline))
sssp_async(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active){

    bool finished = false;
    uint update_times = 0;

    while (!finished){
        finished = true;

        for(uint id = 0; id < num_nodes; id ++){
            if(active[id]){
                active[id] = false;
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];

                for(uint j = nbegin; j < nend; j ++){
                    uint finalDist = value[id] + edgeList[j].w8;
                    if(finalDist < value[edgeList[j].end]){
                        update_times ++;
                        value[edgeList[j].end] = finalDist;
                        finished = false;
                        active[edgeList[j].end] = true;
                    }
                }
            }
        }
        (*iter) ++;
    }
    cout<<"update times: "<<update_times<<endl;
}

template<class E>
void __attribute__((linx_kernel, noinline))
cc_async(uint num_nodes, uint *iter, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active){

    bool finished = false;
    uint update_times = 0;

    while (!finished){
        finished = true;

        for(uint id = 0; id < num_nodes; id ++){
            if(active[id]){
                active[id] = false;
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];

                for(uint j = nbegin; j < nend; j ++){
                    uint dest = edgeList[j].end;
                    if(value[id] < value[dest]){
                        update_times ++;
                        value[dest] = value[id];
                        finished = false;
                        active[dest] = true;
                    }
                }
            }
        }
        (*iter) ++;
    }
    cout<<"update times: "<<update_times<<endl;
}

#endif //GRAPH_ALGORITHM_KERNELS_ASYNC_H
