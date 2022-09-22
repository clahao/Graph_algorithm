//
// Created by moyu on 2022/9/16.
//

#ifndef GRAPH_ALGORITHM_KERNELS_ASYNC_PRIORITY_SCHEDULE_H
#define GRAPH_ALGORITHM_KERNELS_ASYNC_PRIORITY_SCHEDULE_H

#include <queue>
#include "global.h"

//int update = 0;
//uint priority = 0;
//uint min_prior;
//
//template<class E>
//void rebfs(int id, int depth, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, bool *finished, queue<uint> *queue) {
//    if (depth == MAX_BDFS_DEPTH)
//        return;
//    active[id] = false;
//    uint nbegin = offset[id];
//    uint nend = nbegin + outDegree[id];
//    uint finalDist = value[id] + 1;
//
//    for(uint j = nbegin; j < nend; j ++){
//        uint dest = edgeList[j].end;
//        if(finalDist < value[dest]){
//            //update++;
//            value[dest] = finalDist;
//            active[dest] = true;
//
//            if(depth == MAX_BDFS_DEPTH - 1){
//                *finished = false;
//                uint prior = finalDist >> package_interval;
//                prior = prior < package_num ? prior : package_num - 1;
//                min_prior = min_prior < prior ? min_prior : prior;//min(min_prior,prior);
//                queue[prior].push(dest);
//            }
//
//        }
//        if (active[dest])
//            rebfs(dest, depth + 1, offset, edgeList, outDegree, value, active, finished, queue);
//    }
//}
//
//template<class E>
//void __attribute__((linx_kernel, noinline))
//bfs_async(uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, queue<uint> *queue){
//    bool finished = false;
//
//
//    while (!finished || priority < package_num){
//        finished = true;
//        uint id = queue[priority].front();
//        queue[priority].pop();
//
//        if(active[id]){
//            min_prior = priority;
//            //value[id] = Dist;
//            rebfs(id, 0, offset, edgeList, outDegree, value, active, &finished, queue);
//
//            priority = min_prior;
//        }
//
//        while(priority < package_num && queue[priority].empty())
//            priority++;
//    }
//    //cout<<"update times: "<<update<<endl;
//}

//template<class E>
//void __attribute__((linx_kernel, noinline))
//bfs_async(uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, queue<uint> *queue){
//    bool finished = false;
//    uint priority = 0;
//    uint min_prior;
//    //uint update_times = 0;
//
//    while (!finished || priority < package_num){
//        finished = true;
//        uint id = queue[priority].front();
//        queue[priority].pop();
//
//        if(active[id]){
//            active[id] = false;
//            uint Dist = value[id];
//            min_prior = priority;
//            //value[id] = Dist;
//            uint nbegin = offset[id];
//            uint nend = nbegin + outDegree[id];
//            uint finalDist = Dist + 1;
//
//            for(uint j = nbegin; j < nend; j ++){
//                uint dest = edgeList[j].end;
//                if(finalDist < value[dest]){
//                    //update_times ++;
//                    active[dest] = true;
//                    finished = false;
//                    value[dest] = finalDist;
//                    uint prior = finalDist >> package_interval;
//                    prior = prior < package_num ? prior : package_num - 1;
//                    min_prior = min_prior < prior ? min_prior : prior;//min(min_prior,prior);
//                    queue[prior].push(dest);
//                }
//            }
//            priority = min_prior;
//        }
//
//        while(priority < package_num && queue[priority].empty())
//            priority++;
//    }
//    //cout<<"update times: "<<update_times<<endl;
//}


template<class E>
void __attribute__((linx_kernel, noinline))
bfs_async(uint num_nodes, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, uint **queue){
    bool finished = false;
    uint priority = 0;
    uint min_prior;
    uint queue_start[package_num];
    uint queue_end[package_num];
    for(int i = 0; i < package_num; i ++){
        queue_start[i] = 0;
        queue_end[i] = 0;
    }
    queue_end[0] ++;
    //uint update_times = 0;

    while (!finished || priority < package_num){
        finished = true;
        uint id = queue[priority][queue_start[priority] ++];
        queue_start[priority] = queue_start[priority] % num_nodes;

        if(active[id]){
            active[id] = false;
            uint Dist = value[id];
            min_prior = priority;
            //value[id] = Dist;
            uint nbegin = offset[id];
            uint nend = nbegin + outDegree[id];
            uint finalDist = Dist + 1;

            for(uint j = nbegin; j < nend; j ++){
                uint dest = edgeList[j].end;
                if(finalDist < value[dest]){
                    //update_times ++;
                    active[dest] = true;
                    finished = false;
                    value[dest] = finalDist;
                    uint prior = finalDist >> package_interval;
                    prior = prior < package_num ? prior : package_num - 1;
                    min_prior = min_prior < prior ? min_prior : prior;//min(min_prior,prior);
                    queue[prior][queue_end[prior] ++] = dest;
                    queue_end[priority] = queue_end[priority] % num_nodes;
                }
            }
            priority = min_prior;
        }

        while(priority < package_num && (queue_start[priority] == queue_end[priority]))
            priority++;
    }
    //cout<<"update times: "<<update_times<<endl;
}


//template<class E>
//void __attribute__((linx_kernel, noinline))
//sssp_async(uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, queue<uint> *queue){
//    bool finished = false;
//    uint priority = 0;
//    uint min_prior;
//    //uint update_times = 0;
//
//    while (!finished || priority < package_num){
//        finished = true;
//        uint id = queue[priority].front();
//        queue[priority].pop();
//
//        if(active[id]){
//            active[id] = false;
//            uint Dist = value[id];
//            min_prior = priority;
//            //value[id] = Dist;
//            uint nbegin = offset[id];
//            uint nend = nbegin + outDegree[id];
//
//            for(uint j = nbegin; j < nend; j ++){
//                uint dest = edgeList[j].end;
//                uint finalDist = Dist + edgeList[j].w8;
//                if(finalDist < value[dest]){
//                    //update_times ++;
//                    active[dest] = true;
//                    finished = false;
//                    value[dest] = finalDist;
//                    uint prior = finalDist >> package_interval;
//                    prior = prior < package_num ? prior : package_num - 1;
//                    min_prior = min_prior < prior ? min_prior : prior;//min(min_prior,prior);
//                    queue[prior].push(dest);
//                }
//            }
//            priority = min_prior;
//        }
//
//        while(priority < package_num && queue[priority].empty())
//            priority++;
//    }
//    //cout<<"update times: "<<update_times<<endl;
//}


template<class E>
void __attribute__((linx_kernel, noinline))
sssp_async(uint num_nodes, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, uint **queue){
    bool finished = false;
    uint priority = 0;
    uint min_prior;
    uint queue_start[package_num];
    uint queue_end[package_num];
    for(int i = 0; i < package_num; i ++){
        queue_start[i] = 0;
        queue_end[i] = 0;
    }
    queue_end[0] ++;
    //uint update_times = 0;

    while (!finished || priority < package_num){
        finished = true;
        uint id = queue[priority][queue_start[priority] ++];
        queue_start[priority] = queue_start[priority] % num_nodes;

        if(active[id]){
            active[id] = false;
            uint Dist = value[id];
            min_prior = priority;
            //value[id] = Dist;
            uint nbegin = offset[id];
            uint nend = nbegin + outDegree[id];

            for(uint j = nbegin; j < nend; j ++){
                uint dest = edgeList[j].end;
                uint finalDist = Dist + edgeList[j].w8;
                if(finalDist < value[dest]){
                    //update_times ++;
                    active[dest] = true;
                    finished = false;
                    value[dest] = finalDist;
                    uint prior = finalDist >> package_interval;
                    prior = prior < package_num ? prior : package_num - 1;
                    min_prior = min_prior < prior ? min_prior : prior;//min(min_prior,prior);
                    queue[prior][queue_end[prior] ++] = dest;
                    queue_end[priority] = queue_end[priority] % num_nodes;
                }
            }
            priority = min_prior;
        }

        while(priority < package_num && (queue_start[priority] == queue_end[priority]))
            priority++;
    }
    //cout<<"update times: "<<update_times<<endl;
}


//template<class E>
//void __attribute__((linx_kernel, noinline))
//cc_async(uint num_nodes, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, queue<uint> queue){
//
//    //uint update_times = 0;
//    for(uint i = 0; i < num_nodes; i ++){
//
//        if(active[i]){
//            active[i] = false;
//            queue.push(i);
//
//            while(!queue.empty()){
//                uint id = queue.front();
//                queue.pop();
//                uint nbegin = offset[id];
//                uint nend = nbegin + outDegree[id];
//
//                for(uint j = nbegin; j < nend; j ++){
//                    uint dest = edgeList[j].end;
//                    if(value[id] < value[dest]){
//                        //update_times ++;
//                        active[dest] = false;
//                        value[dest] = value[id];
//                        queue.push(dest);
//                    }
//                }
//            }
//        }
//    }
//    //cout<<"update times: "<<update_times<<endl;
//}


template<class E>
void __attribute__((linx_kernel, noinline))
cc_async(uint num_nodes, uint *offset, E *edgeList, uint *outDegree, uint *value, bool *active, uint *queue){

    //uint update_times = 0;
    uint queue_start = 0;
    uint queue_end = 0;
    for(uint i = 0; i < num_nodes; i ++){

        if(active[i]){
            active[i] = false;
            queue[queue_end ++] = i;
            queue_end = queue_end % num_nodes;

            while(queue_end != queue_start){
                uint id = queue[queue_start ++];
                queue_start = queue_start % num_nodes;
                uint nbegin = offset[id];
                uint nend = nbegin + outDegree[id];

                for(uint j = nbegin; j < nend; j ++){
                    uint dest = edgeList[j].end;
                    if(value[id] < value[dest]){
                        //update_times ++;
                        active[dest] = false;
                        value[dest] = value[id];
                        queue[queue_end ++] = dest;
                        queue_end = queue_end % num_nodes;
                    }
                }
            }
        }
    }
    //cout<<"update times: "<<update_times<<endl;
}

#endif //GRAPH_ALGORITHM_KERNELS_ASYNC_PRIORITY_SCHEDULE_H
