//
// Created by moyu on 2022/11/21.
//
#include "global.h"
//num_nodes num_edges
//0 1 10 to 0 1
//0 2 5     0 2
//          5
//          10

int main(int argc, char **argv) {
    string input_file(argv[1]);
    string output_file(argv[2]);

    ifstream ifs;
    ifs.open(input_file);
    ofstream ofs;
    ofs.open(output_file);

    srand((int)time(0));
    string line;
    getline(ifs, line);

    int pos = line.find(' ');
    uint num_nodes = stoi(line.substr(0, pos));
    uint num_edges = stoi(line.substr(pos + 1));
    printf("%d %d\n", num_nodes, num_edges);

    for (int i = 0; i < num_edges; i++) {
        getline(ifs, line);
        pos = line.find(' ');
        line = line.substr(0,pos);
        ofs << line << endl;
    }
    ifs.seekg(0);
    getline(ifs, line);
    for (int i = 0; i < num_edges; i++) {
        getline(ifs, line);
        pos = line.find(' ');
        line = line.substr(pos + 1);
        ofs << line << endl;
    }
    ifs.close();
    ofs.close();


}