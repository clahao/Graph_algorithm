//
// Created by moyu on 2023/4/12.
//
#include "global.h"

int main(int argc, char **argv) {
    string input_file1(argv[1]);
    string input_file2(argv[2]);
    string output_file(argv[3]);

    ifstream ifs;
    ifs.open(input_file1);
    ifstream ifs2;
    ifs2.open(input_file2);
    ofstream ofs;
    ofs.open(output_file);

    string line;
    for(int i = 0; i < 3; i ++){
        getline(ifs, line);
        ofs << line << endl;
    }


    line = line.substr(9);
    int pos = line.find(' ');
    int num_nodes = stoi(line.substr(0, pos));
    pos = line.find_last_of(' ');
    uint num_edges = stoi(line.substr(pos + 1));

    getline(ifs, line);
    ofs << line << endl;

    for (int i = 0; i < num_edges; i++) {
        getline(ifs2, line);
        ofs << line << endl;
    }

    printf("%d %d\n", num_nodes, num_edges);
    ifs.close();
    ifs2.close();
    ofs.close();


}