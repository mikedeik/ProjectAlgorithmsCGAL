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
    double threshold = 100000.0;
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
    map<int, double> Score_Map_SA_LOCAL_MIN;
    map<int, double> Score_Map_SA_LOCAL_MAX;
    map<int, double> Score_Map_SA_GLOBAL_MIN;
    map<int, double> Score_Map_SA_GLOBAL_MAX;
    map<int, double> Bound_Map_LS_MIN;
    map<int, double> Bound_Map_LS_MAX;
    map<int, double> Bound_Map_SA_LOCAL_MIN;
    map<int, double> Bound_Map_SA_LOCAL_MAX;
    map<int, double> Bound_Map_SA_GLOBAL_MIN;
    map<int, double> Bound_Map_SA_GLOBAL_MAX;

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

        // ========== SA with LOCAL step and MIN =================

        SimulatedAnnealing SA_LOCAL_MIN(points, LOCAL, MIN, L_SA, output_file);
        SA_LOCAL_MIN.OptimizeArea();

        // an den exoume score gia ton arithmo twn points ftiakse neo mapping alliws prosthese to neo score
        new_ratio = SA_LOCAL_MIN.get_ratio();

        if (Score_Map_SA_LOCAL_MIN.find(points.size()) == Score_Map_SA_LOCAL_MIN.end())
        {
            Score_Map_SA_LOCAL_MIN.insert(pair<int, double>(points.size(), new_ratio));
        }
        else
        {
            Score_Map_SA_LOCAL_MIN[points.size()] += new_ratio;
        }

        // omoiws an to kainourgio ratio einai megalutero apo to palio bound antikathistatai
        if (Bound_Map_SA_LOCAL_MIN.find(points.size()) == Bound_Map_SA_LOCAL_MIN.end())
        {
            Bound_Map_SA_LOCAL_MIN.insert(pair<int, double>(points.size(), new_ratio));
        }
        else
        {

            if (new_ratio > Bound_Map_SA_LOCAL_MIN[points.size()])
            {
                Bound_Map_SA_LOCAL_MIN[points.size()] = new_ratio;
            }
        }

        // ========== SA with LOCAL step and MAX =================

        SimulatedAnnealing SA_LOCAL_MAX(points, LOCAL, MAX, L_SA, output_file);
        SA_LOCAL_MAX.OptimizeArea();

        new_ratio = SA_LOCAL_MAX.get_ratio();

        if (Score_Map_SA_LOCAL_MAX.find(points.size()) == Score_Map_SA_LOCAL_MAX.end())
        {
            Score_Map_SA_LOCAL_MAX.insert(pair<int, double>(points.size(), new_ratio));
        }
        else
        {
            Score_Map_SA_LOCAL_MAX[points.size()] += new_ratio;
        }

        // omoiws an to kainourgio ratio einai mikrotero apo to palio bound antikathistatai
        if (Bound_Map_SA_LOCAL_MAX.find(points.size()) == Bound_Map_SA_LOCAL_MAX.end())
        {
            Bound_Map_SA_LOCAL_MAX.insert(pair<int, double>(points.size(), new_ratio));
        }
        else
        {

            if (new_ratio < Bound_Map_SA_LOCAL_MAX[points.size()])
            {
                Bound_Map_SA_LOCAL_MAX[points.size()] = new_ratio;
            }
        }

        // ========== SA with GLOBAL step and MIN =================

        SimulatedAnnealing SA_GLOBAL_MIN(points, LOCAL, MIN, L_SA, output_file);
        SA_GLOBAL_MIN.OptimizeArea();

        new_ratio = SA_GLOBAL_MIN.get_ratio();

        if (Score_Map_SA_GLOBAL_MIN.find(points.size()) == Score_Map_SA_GLOBAL_MIN.end())
        {
            Score_Map_SA_GLOBAL_MIN.insert(pair<int, double>(points.size(), new_ratio));
        }
        else
        {
            Score_Map_SA_GLOBAL_MIN[points.size()] += new_ratio;
        }

        // omoiws an to kainourgio ratio einai megalutero apo to palio bound antikathistatai
        if (Bound_Map_SA_GLOBAL_MIN.find(points.size()) == Bound_Map_SA_GLOBAL_MIN.end())
        {
            Bound_Map_SA_GLOBAL_MIN.insert(pair<int, double>(points.size(), new_ratio));
        }
        else
        {

            if (new_ratio > Bound_Map_SA_GLOBAL_MIN[points.size()])
            {
                Bound_Map_SA_GLOBAL_MIN[points.size()] = new_ratio;
            }
        }

        // =========== SA with GLOBAL step with MAX =================================

        SimulatedAnnealing SA_GLOBAL_MAX(points, LOCAL, MAX, L_SA, output_file);
        SA_GLOBAL_MAX.OptimizeArea();

        new_ratio = SA_GLOBAL_MAX.get_ratio();

        if (Score_Map_SA_GLOBAL_MAX.find(points.size()) == Score_Map_SA_GLOBAL_MAX.end())
        {
            Score_Map_SA_GLOBAL_MAX.insert(pair<int, double>(points.size(), new_ratio));
        }
        else
        {
            Score_Map_SA_GLOBAL_MAX[points.size()] += new_ratio;
        }

        // omoiws an to kainourgio ratio einai mikrotero apo to palio bound antikathistatai
        if (Bound_Map_SA_GLOBAL_MAX.find(points.size()) == Bound_Map_SA_GLOBAL_MAX.end())
        {
            Bound_Map_SA_GLOBAL_MAX.insert(pair<int, double>(points.size(), new_ratio));
        }
        else
        {

            if (new_ratio < Bound_Map_SA_GLOBAL_MAX[points.size()])
            {
                Bound_Map_SA_GLOBAL_MAX[points.size()] = new_ratio;
            }
        }

        // Running Local Search MIN
        LocalSearch LS_min(points, MIN, threshold, output_file, L_LS);
        new_ratio = LS_min.get_ratio();

        if (Score_Map_LS_MIN.find(points.size()) == Score_Map_LS_MIN.end())
        {
            Score_Map_LS_MIN.insert(pair<int, double>(points.size(), new_ratio));
        }
        else
        {
            Score_Map_LS_MIN[points.size()] += new_ratio;
        }

        // omoiws an to kainourgio ratio einai megalutero apo to palio bound antikathistatai
        if (Bound_Map_LS_MIN.find(points.size()) == Bound_Map_LS_MIN.end())
        {
            Bound_Map_LS_MIN.insert(pair<int, double>(points.size(), new_ratio));
        }
        else
        {

            if (new_ratio > Bound_Map_LS_MIN[points.size()])
            {
                Bound_Map_LS_MIN[points.size()] = new_ratio;
            }
        }

        // Running Local Search MAX
        LocalSearch LS_max(points, MAX, threshold, output_file, L_LS);
        new_ratio = LS_max.get_ratio();

        if (Score_Map_LS_MAX.find(points.size()) == Score_Map_LS_MAX.end())
        {
            Score_Map_LS_MAX.insert(pair<int, double>(points.size(), new_ratio));
        }
        else
        {
            Score_Map_LS_MAX[points.size()] += new_ratio;
        }

        // omoiws an to kainourgio ratio einai mikrotero apo to palio bound antikathistatai
        if (Bound_Map_LS_MAX.find(points.size()) == Bound_Map_LS_MAX.end())
        {
            Bound_Map_LS_MAX.insert(pair<int, double>(points.size(), new_ratio));
        }
        else
        {

            if (new_ratio < Bound_Map_LS_MAX[points.size()])
            {
                Bound_Map_LS_MAX[points.size()] = new_ratio;
            }
        }
    }

    for (auto const &size : Score_Map_LS_MIN)
    {
        cout << "For " << size.first << " points best is " << size.second << "\n";
    }
    for (auto const &size : Score_Map_LS_MAX)
    {
        cout << "For " << size.first << " points best is " << size.second << "\n";
    }
    for (auto const &size : Score_Map_SA_LOCAL_MIN)
    {
        cout << "For " << size.first << " points best is " << size.second << "\n";
    }
    for (auto const &size : Score_Map_SA_LOCAL_MAX)
    {
        cout << "For " << size.first << " points best is " << size.second << "\n";
    }
    for (auto const &size : Score_Map_SA_GLOBAL_MIN)
    {
        cout << "For " << size.first << " points best is " << size.second << "\n";
    }
    for (auto const &size : Bound_Map_SA_GLOBAL_MAX)
    {
        cout << "For " << size.first << " points best is " << size.second << "\n";
    }

    // cout << "Size "
    //      << "min_score "
    //      << "max_core"
    //      << "min _ bound"
    //      << "max_bound \n";
    // cout << size.first << Score_Map_LS_MIN[size.first] << Score_Map_LS_MAX[size.first] << Bound_Map_LS_MIN[size.first] << Bound_Map_LS_MAX[size.first]
}
