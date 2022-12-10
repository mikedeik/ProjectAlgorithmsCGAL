#include <iostream>
#include "util.h"
#include "incrementing.h"
#include "visibleedge.h"
#include "SimulatedAnnealing.h"
#include "LocalSearch.h"
#include <map>

using namespace std;

int main(int argc, char **argv)
{
    int L;
    double threshold;
    Target target;
    string input_file_path = "", output_file_path = "", algorithm = "", initialization = "";

    if (argc != 12)
    {
        Print_Errors();
        return 1;
    }

    for (int i = 1; i <= 6; i++)
    {
        switch (i)
        {
        case 1:
            if (!strcmp(argv[1], "-i"))
            {
                input_file_path = argv[2];
                break;
            }
            Print_Errors(i);
            return i;
        case 2:
            if (!strcmp(argv[3], "-o"))
            {
                output_file_path = argv[4];
                break;
            }
            Print_Errors(i);
            return i;
        case 3:
            if (!strcmp(argv[5], "-algorithm"))
            {
                algorithm = argv[6];
                break;
            }
            Print_Errors(i);
            return i;
        case 4:
            if (!strcmp(argv[7], "-L"))
            {
                L = atoi(argv[8]);
                break;
            }
            Print_Errors(i);
            return i;
        case 5:
            if (!strcmp(argv[9], "-max"))
            {
                target = MAX;
                break;
            }
            if (!strcmp(argv[9], "-min"))
            {
                target = MIN;
                break;
            }
            Print_Errors(i);
            return i;
        case 6:
            if (!strcmp(argv[10], "-threshold"))
            {
                threshold = atof(argv[11]);
                break;
            }
            if (!strcmp(argv[10], "-annealing"))
            {
                initialization = argv[11];
                break;
            }
            Print_Errors(i);
            return i;

        default:
            break;
        }
    }

    map<string, AnnealingType> an_type;

    an_type.insert(pair<string, AnnealingType>("local", LOCAL));
    an_type.insert(pair<string, AnnealingType>("global", GLOBAL));
    an_type.insert(pair<string, AnnealingType>("subdivision", SUBDIVISION));

    vector<Point> points;
    cout << input_file_path;
    get_points_from_file(input_file_path, &points);

    Incrementing algo(points, X_DESCENDING, output_file_path);
    algo.Create_Polygon_Line();
    algo.Print_Polygon();
    Polygon p = algo.Get_Simple_Polygon();

    SimulatedAnnealing SA(p, an_type[initialization], target, L);
    SA.MinimizeArea();
    // testing_KD_tree(points);

    return 0;
}