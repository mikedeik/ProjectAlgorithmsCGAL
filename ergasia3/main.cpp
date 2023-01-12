#include <iostream>
#include "util.h"
#include "incrementing.h"
#include "visibleedge.h"
#include "SimulatedAnnealing.h"
#include "LocalSearch.h"
#include <map>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

int main(int argc, char **argv)
{

    int L_SA = 5000;
    int L_LS = 5;
    double threshold = 5.0;
    Target target = MIN;

    string input_dir = "";
    string output_file = "out.txt";

    for (int i = 1; i < argc; i = i + 2)
    {

        if (argv[i] && argv[i + 1])
        {
            if (!strcmp(argv[i], "-i"))
            {
                input_dir = argv[i + 1];
            }
            if (!strcmp(argv[i], "-o"))
            {
                output_file = argv[i + 1];
            }
            if (!strcmp(argv[i], "-L_SA"))
            {
                L_SA = stoi(argv[i + 1]);
            }
            if (!strcmp(argv[i], "-L_LS"))
            {
                L_LS = stoi(argv[i + 1]);
            }
            if (!strcmp(argv[i], "-threshold"))
            {
                threshold = stof(argv[i + 1]);
            }
        }
    }

    // map<int, vector<double>> Score_Map;

    vector<Point> points;

    for (const auto &entry : fs::directory_iterator(input_dir))
    {
        std::cout << entry << std::endl;
        std::filesystem::path outfilename = entry.path();
        string filename = outfilename.string();

        points.clear();

        get_points_from_file(filename, &points);

        if (points.empty())
        {
            cout << "error reading points \n";
            exit(1);
        }

        SimulatedAnnealing SA(points, LOCAL, target, L_SA, output_file);
        SA.OptimizeArea();

        LocalSearch LS(points, target, threshold, output_file, L_LS);

        getchar();
    }
}
