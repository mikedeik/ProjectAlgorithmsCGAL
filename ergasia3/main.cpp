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
    double threshold = 1000000.0;
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

    map<int, double> Score_Map_LS_MIN;
    map<int, double> Score_Map_LS_MAX;
    map<int, double> Score_Map_SA_MIN;
    map<int, double> Score_Map_SA_MAX;

    for (int points_size = 10; points_size < 101; points_size += 10)
    {
        Score_Map_LS_MIN.insert(pair<int, double>(points_size, 1.0));
        Score_Map_SA_MIN.insert(pair<int, double>(points_size, 1.0));
        Score_Map_LS_MAX.insert(pair<int, double>(points_size, 0.0));
        Score_Map_SA_MAX.insert(pair<int, double>(points_size, 0.0));
    }

    double new_ratio;
    vector<Point> points;
    // todo na valoume orisma se kathe sinartisi ena max time kai na kanei abort otan to ksepernaei
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

        // Running Local Search MIN
        LocalSearch LS_min(points, MIN, threshold, output_file, L_LS);
        new_ratio = LS_min.get_ratio();

        if (new_ratio < Score_Map_LS_MIN[points.size()])
        {
            Score_Map_LS_MIN[points.size()] = new_ratio;
        }

        // Running Local Search MAX
        LocalSearch LS_max(points, MAX, threshold, output_file, L_LS);
        new_ratio = LS_max.get_ratio();

        if (new_ratio > Score_Map_LS_MAX[points.size()])
        {
            Score_Map_LS_MAX[points.size()] = new_ratio;
        }
    }

    for (auto const &size : Score_Map_LS_MIN)
    {
        cout << "For " << size.first << " points best is " << size.second << "\n";
    }
}
