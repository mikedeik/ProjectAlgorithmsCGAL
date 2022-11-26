#include <iostream>
#include "util.h"
#include "incrementing.h"
#include "visibleedge.h"

#include <map>

using namespace std;

int main(int argc, char **argv)
{

    string input_file_path = "", output_file_path = "", algorithm = "", initialization = "";
    int edge_selection = -1;
    for (int i = 1; i < argc; i = i + 2)
    {

        if (argv[i] && argv[i + 1])
        {
            if (!strcmp(argv[i], "-i"))
            {
                input_file_path = argv[i + 1];
            }
            if (!strcmp(argv[i], "-o"))
            {
                output_file_path = argv[i + 1];
            }
            if (!strcmp(argv[i], "-algorithm"))
            {
                algorithm = argv[i + 1];
            }
            if (!strcmp(argv[i], "-edge_selection"))
            {
                edge_selection = stoi(argv[i + 1]);
            }
            if (!strcmp(argv[i], "-initialization"))
            {
                initialization = argv[i + 1];
            }
        }
    }

    cout << algorithm;
    getchar();

    map<string, Sorter> sorter;

    string inp = initialization;

    sorter.insert(pair<string, Sorter>("1a", X_ASCENDING));
    sorter.insert(pair<string, Sorter>("1b", X_DESCENDING));
    sorter.insert(pair<string, Sorter>("2a", Y_ASCENDING));
    sorter.insert(pair<string, Sorter>("2b", Y_DESCENDING));
    vector<Point> points;
    get_points_from_file(input_file_path, &points);

    if (algorithm == "incremental")
    {

        // get_points_from_file("test.txt", &points);
        Incrementing algo(points, sorter[inp], output_file_path);
        algo.Create_Polygon_Line();
        algo.Print_Polygon();
    }
    else
    {
        VisibleEdge algo2(points, output_file_path);
    }

    cout << "input_file_path :" << input_file_path << endl;
    cout << "output_file_path :" << output_file_path << endl;
    cout << "Algorithm :" << algorithm << endl;
    cout << "ES :" << edge_selection << endl;
    cout << "Initialization :" << initialization << endl;

    return 0;
}