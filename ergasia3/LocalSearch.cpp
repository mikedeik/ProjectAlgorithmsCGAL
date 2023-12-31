#include "LocalSearch.h"
using namespace std::chrono;

LocalSearch::LocalSearch(vector<Point> points, Target inc_target, float inc_threshold, string inc_output_file, int inc_L) : target(inc_target),
                                                                                                                            output_file(inc_output_file),
                                                                                                                            L(inc_L),
                                                                                                                            threshold(inc_threshold)
{

    Incrementing algo(points, X_ASCENDING);
    algo.Create_Polygon_Line();
    Polygon polygon = algo.Get_Simple_Polygon();
    current_polygon = polygon;
    polygon_history.push_back(polygon);

    threshold = (CGAL::to_double(polygon.area())) / 10000.0;

    switch (target)
    {
    case MIN:
        MinimizeArea();
        break;
    case MAX:
        MaximizeArea();
        break;
    default:
        break;
    }
}

LocalSearch::~LocalSearch()
{

    current_polygon.clear();
}

const void LocalSearch::MaximizeArea()
{
    auto start = high_resolution_clock::now();

    double DA = threshold; // difference of area (curr polygon area - prev polygon area)

    vector<Point> path;
    while (DA >= threshold || DA == 0)
    {
        Polygon best_polygon = current_polygon;
        // for every edge e ε S do
        for (Segment edge : current_polygon.edges())
        {
            // cout << edge << "\n";
            // for every path V of length <=k do
            for (int i = 1; i < L; i++)
            {
                int j = 0;
                for (Point point : current_polygon.vertices())
                {
                    Polygon temp_polygon = current_polygon;
                    int path_length = 0;
                    // create path

                    // looparo ola ta points kai se kathe iteration thelo na ksekinaei to path apo neo point
                    // opote exo to bool passed initial point opou ksekinaei na prosthetei points sto path mono
                    // an einai stin sosti thesi

                    int x = 0;
                    for (Point path_point : current_polygon.vertices())
                    {
                        if (x >= j)
                        {

                            path.push_back(path_point);
                            path_length++;
                            if (path_length >= i)
                            {
                                break;
                            }
                        }
                        x++;
                    }

                    // edo exo to path

                    // remove path from polygon
                    temp_polygon = RemovePath(temp_polygon, path); // edo to temp polygon exei afairemeno to path
                    // insert path between edge
                    // epeidi ta vazo ola amesos meta to proto simeio tou edge mpainoun anapoda(opos theloume)
                    for (int i = 0; i < path.size(); i++)
                    {
                        PointIterator position_to_insert = find(temp_polygon.vertices().begin(), temp_polygon.vertices().end(), edge.vertex(0));
                        int index = position_to_insert - temp_polygon.vertices().begin();
                        temp_polygon.insert(temp_polygon.begin() + index, path[i]);
                    }

                    if (temp_polygon.is_simple())
                    {
                        // cout << "in here";
                        if (temp_polygon.area() > best_polygon.area())
                        {
                            cout << "temp area " << CGAL::to_double(temp_polygon.area()) << " best area" << CGAL::to_double(best_polygon.area()) << std::endl;
                            best_polygon = temp_polygon;
                            DA = abs(abs(CGAL::to_double(best_polygon.area())) - abs(CGAL::to_double(current_polygon.area())));

                            if (DA < threshold || DA == 0)
                            {
                                break;
                            }
                        }
                    }
                    path.clear();
                    j++;
                    if (DA < threshold || DA == 0)
                    {
                        break;
                    }
                }
                // if V moving to e increases area save to list
                if (DA < threshold || DA == 0)
                {
                    break;
                }
            }
        }
        DA = abs(abs(CGAL::to_double(best_polygon.area())) - abs(CGAL::to_double(current_polygon.area())));
        cout << " DA IS : " << DA << std::endl;
        current_polygon = best_polygon;
        polygon_history.push_back(current_polygon);
        // Aplly all changes
        // Keep best solution
    }

    vector<Point> CH_new_points;
    Polygon CH_new;

    CGAL::convex_hull_2(polygon_history[polygon_history.size() - 1].begin(), polygon_history[polygon_history.size() - 1].end(), std::back_inserter(CH_new_points));

    for (Point p : CH_new_points)
    {
        CH_new.push_back(p);
    }

    ratio = CGAL::to_double(polygon_history[polygon_history.size() - 1].area()) / CGAL::to_double(CH_new.area());
}

const void LocalSearch::MinimizeArea()
{
    auto start = high_resolution_clock::now();

    double DA = threshold; // difference of area (curr polygon area - prev polygon area)

    vector<Point> path;
    while (DA >= threshold || DA == 0)
    {
        Polygon best_polygon = current_polygon;
        // for every edge e ε S do
        for (Segment edge : current_polygon.edges())
        {
            // for every path V of length <=k do
            for (int i = 1; i < L; i++)
            {

                int j = 0;
                for (Point point : current_polygon.vertices())
                {
                    Polygon temp_polygon = current_polygon;
                    int path_length = 0;
                    // create path

                    // looparo ola ta points kai se kathe iteration thelo na ksekinaei to path apo neo point
                    // opote exo to bool passed initial point opou ksekinaei na prosthetei points sto path mono
                    // an einai stin sosti thesi

                    int x = 0;
                    for (Point path_point : current_polygon.vertices())
                    {
                        if (x >= j)
                        {

                            path.push_back(path_point);
                            path_length++;
                            if (path_length >= i)
                            {
                                break;
                            }
                        }
                        x++;
                    }

                    // edo exo to path

                    // remove path from polygon
                    temp_polygon = RemovePath(temp_polygon, path); // edo to temp polygon exei afairemeno to path
                    // insert path between edge
                    // epeidi ta vazo ola amesos meta to proto simeio tou edge mpainoun anapoda(opos theloume)
                    for (int ii = 0; ii < path.size(); ii++)
                    {
                        PointIterator position_to_insert = find(temp_polygon.vertices().begin(), temp_polygon.vertices().end(), edge.vertex(0));
                        int index = position_to_insert - temp_polygon.vertices().begin();
                        temp_polygon.insert(temp_polygon.begin() + index, path[ii]);
                    }

                    if (temp_polygon.is_simple())
                    {
                        cout << "in here";
                        if (temp_polygon.area() < best_polygon.area())
                        {
                            cout << "temp area " << CGAL::to_double(temp_polygon.area()) << " best area" << CGAL::to_double(best_polygon.area()) << std::endl;
                            best_polygon = temp_polygon;
                            DA = abs(abs(CGAL::to_double(best_polygon.area())) - abs(CGAL::to_double(current_polygon.area())));

                            if (DA < threshold || DA == 0)
                            {
                                break;
                            }
                        }
                    }
                    path.clear();
                    j++;

                    if (DA < threshold || DA == 0)
                    {
                        break;
                    }
                }
                // if V moving to e increases area save to list
                if (DA < threshold || DA == 0)
                {
                    break;
                }
            }
        }
        DA = abs(abs(CGAL::to_double(best_polygon.area())) - abs(CGAL::to_double(current_polygon.area())));
        // cout << " DA IS :" << DA << std::endl;
        current_polygon = best_polygon;
        polygon_history.push_back(current_polygon);
        // Aplly all changes
        // Keep best solution
    }

    vector<Point> CH_new_points;
    Polygon CH_new;

    CGAL::convex_hull_2(polygon_history[polygon_history.size() - 1].begin(), polygon_history[polygon_history.size() - 1].end(), std::back_inserter(CH_new_points));

    for (Point p : CH_new_points)
    {
        CH_new.push_back(p);
    }

    ratio = CGAL::to_double(polygon_history[polygon_history.size() - 1].area()) / CGAL::to_double(CH_new.area());

    // auto stop = high_resolution_clock::now();

    // auto duration = duration_cast<milliseconds>(stop - start);
    // int time = duration.count();
    // cout << "Time taken by function: "
    //      << duration.count() << " milliseconds"
    //      << "\n";
    // string algo = "Local Search";
    // print_to_file(polygon_history[0], polygon_history[polygon_history.size() - 1], output_file, time, algo, target);
}
Polygon RemovePath(Polygon polygon, vector<Point> path)
{

    Point start, end;
    for (Point point : polygon.vertices())
    {
        if (point.x() == path[0].x() && point.y() == path[0].y())
        {

            start = point;
        }

        if (point.x() == path[path.size() - 1].x() && point.y() == path[path.size() - 1].y())
        {

            end = point;
        }
    }
    PointIterator position = find(polygon.begin(), polygon.end(), start);
    int position_of_start = position - polygon.begin();
    PointIterator position2 = find(polygon.begin(), polygon.end(), end);
    int position_of_end = position2 - polygon.begin();

    if (position_of_start > position_of_end)
    {
        position_of_end = position_of_start; // failsafe
    }

    if (position_of_start == position_of_end)
    {

        polygon.erase(polygon.begin() + position_of_start);
    }
    else
    {
        polygon.erase(polygon.begin() + position_of_start, polygon.begin() + position_of_end + 1);
    }
    return polygon;
}

const void Print_Polygon(Polygon polygon)
{
    cout << "==== Polygon Points ====\n";
    for (Point point : polygon.vertices())
    {
        cout << point << "\n";
    }
}

const double LocalSearch::get_ratio()
{
    return ratio;
}