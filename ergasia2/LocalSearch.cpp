#include "LocalSearch.h"
using namespace std::chrono;

LocalSearch::LocalSearch(vector<Point> points, char* operation)
{
    Incrementing algo(points, X_ASCENDING);
    algo.Create_Polygon_Line();
    Polygon polygon = algo.Get_Simple_Polygon();
    current_polygon = polygon;
    cout << polygon_history.size() << std::endl;
    polygon_history.push_back(polygon);
    cout << polygon_history.size() << std::endl;
    if (strcmp(operation, "max")==0)
    {

        MaximizeArea();
    }
    else
    {

        MinimizeArea();
    }
}

LocalSearch::~LocalSearch()
{

    current_polygon.clear();
}

const void LocalSearch::MaximizeArea()
{
    int max_path_length = 3;
    float threshold = 0.01;
    float DA = threshold; // difference of area (curr polygon area - prev polygon area)

    vector<Point> path;
    while (DA >= threshold)
    {
        Polygon best_polygon = current_polygon;
        // for every edge e ε S do
        for (Segment edge : current_polygon.edges())
        {
            // cout << edge << "\n";
            // for every path V of length <=k do
            for (int i = 0; i < max_path_length; i++)
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
                        if (temp_polygon.area() > best_polygon.area())
                        {
                            cout << "temp area" << CGAL::to_double(temp_polygon.area()) << "best area" << CGAL::to_double(best_polygon.area()) << std::endl;
                            best_polygon = temp_polygon;
                        }
                    }
                    path.clear();
                    j++;
                }
                // if V moving to e increases area save to list
            }
        }
        DA = abs(CGAL::to_double(best_polygon.area())) - abs(CGAL::to_double(current_polygon.area()));
        cout << " DA IS :" << DA << std::endl;
        current_polygon = best_polygon;
        polygon_history.push_back(current_polygon);
        // Aplly all changes
        // Keep best solution
    }
}

const void LocalSearch::MinimizeArea()
{
    int max_path_length = 3;
    float threshold = 0.01;
    float DA = threshold; // difference of area (curr polygon area - prev polygon area)

    vector<Point> path;
    while (DA >= threshold)
    {
        Polygon best_polygon = current_polygon;
        // for every edge e ε S do
        for (Segment edge : current_polygon.edges())
        {
            // for every path V of length <=k do
            for (int i = 0; i < max_path_length; i++)
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
                        if (temp_polygon.area() < best_polygon.area())
                        {
                            cout << "temp area" << CGAL::to_double(temp_polygon.area()) << "best area" << CGAL::to_double(best_polygon.area()) << std::endl;
                            best_polygon = temp_polygon;
                        }
                    }
                    path.clear();
                    j++;
                }
                // if V moving to e increases area save to list
            }
        }
        DA = abs(CGAL::to_double(best_polygon.area())) - abs(CGAL::to_double(current_polygon.area()));
        cout << " DA IS :" << DA << std::endl;
        current_polygon = best_polygon;
        polygon_history.push_back(current_polygon);
        // Aplly all changes
        // Keep best solution
    }
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