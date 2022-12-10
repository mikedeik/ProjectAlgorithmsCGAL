#include "SimulatedAnnealing.h"
#include "CGAL/convex_hull_2.h"
#include "incrementing.h"

SimulatedAnnealing::SimulatedAnnealing(vector<Point> inc_points, AnnealingType an_type, Target Area_target, int L) : points(inc_points),
                                                                                                                     type(an_type),
                                                                                                                     target(Area_target),
                                                                                                                     L(L)
{

    Incrementing *algo = new Incrementing(points, X_ASCENDING, "out.txt");
    algo->Create_Polygon_Line();
    starting_polygon = algo->Get_Simple_Polygon();

    calculate_energy(starting_polygon);
    starting_area = CGAL::to_double(starting_polygon.area());

    // initialize the KD tree
    for (Point point : starting_polygon)
    {
        KD_tree.insert(point);
    }
    new_polygon = starting_polygon;
}

SimulatedAnnealing::~SimulatedAnnealing()
{

    starting_polygon.clear();
    new_polygon.clear();
}

const void SimulatedAnnealing::MinimizeArea()
{

    srand(time(NULL));
    switch (type)
    {
    case LOCAL:
        Local_Optimization();
        break;
    case GLOBAL:
        Global_Optimization();
        break;
    case SUBDIVISION:
        Subdivision_Optimization();
    default:
        break;
    }

    cout << "starting area :" << starting_area << "\n";
    cout << "new area :" << new_polygon.area() << "\n";
    cout << "is it simple? -> " << new_polygon.is_simple() << "\n";
    // calculate_energy(new_polygon);
}

const void SimulatedAnnealing::Local_Optimization()
{
    cout << "Running Local\n";
    while (T > 0)
    {

        int random = rand() % (starting_polygon.size() - 4) + 2;

        /*********************************************************************/
        /* Edw tha prepei na mpoun oi epiloges analoga to Type(Local, Global)*/
        /*********************************************************************/

        // arxiko simeio
        Point q = *(starting_polygon.begin() + random);
        // to proigoumeno tou
        Point p = *(starting_polygon.begin() + random - 1);
        // rs einai i epomeni akmi
        Point r = *(starting_polygon.begin() + random + 1);
        Point s = *(starting_polygon.begin() + random + 2);

        if (!check_validity(p, q, r, s))
        {
            // cout << "not valid\n";
            continue;
        }
        // cout << "valid\n";

        new_polygon.erase(new_polygon.begin() + random);
        new_polygon.insert(new_polygon.begin() + random + 1, q);
        double DE = CGAL::to_double(calculate_energy(new_polygon) - calculate_energy(starting_polygon));

        if (DE < 0.0 || Compute_Metropolis(DE, T, generate_random_01()))
        {
            starting_polygon = new_polygon;
        }
        else
        {
            new_polygon = starting_polygon;
        }

        T = T - (1.0 / double(L));
    }
}

const void SimulatedAnnealing::Global_Optimization()
{
    cout << "Running Global\n";
    while (T > 0)
    {
        vector<bool> visited;
        int random_point = rand() % (starting_polygon.size() - 4) + 2;

        int random_edge = rand() % (new_polygon.edges().size() - 4) + 2;

        // den thelw na spasw tin epomeni i proigoumeni akmi tou simeiou
        while (random_edge == random_point || random_edge == (random_point - 1))
        {
            random_edge = rand() % (new_polygon.edges().size() - 4) + 2;
        }

        Point q = *(starting_polygon.begin() + random_point);
        Point p = *(starting_polygon.begin() + random_point - 1);
        // rs einai i epomeni akmi
        Point r = *(starting_polygon.begin() + random_point + 1);

        Segment st = *(starting_polygon.edges_begin() + random_edge);

        if (!check_validity(p, q, r, st))
        {
            continue;
        }

        new_polygon.erase(new_polygon.begin() + random_point);

        // find where t is in the new polygon after the erase of q
        PointIterator to_insert = find(new_polygon.begin(), new_polygon.end(), st.target());
        int position_to_insert = to_insert - new_polygon.begin();

        // insert q before t
        new_polygon.insert(new_polygon.begin() + position_to_insert, q);

        // as with local check if it's a good change
        double DE = CGAL::to_double(calculate_energy(new_polygon) - calculate_energy(starting_polygon));

        if (DE < 0.0 || Compute_Metropolis(DE, T, generate_random_01()))
        {
            starting_polygon = new_polygon;
        }
        else
        {
            new_polygon = starting_polygon;
        }

        T = T - (1.0 / double(L));
    }
}

const void SimulatedAnnealing::Global_Optimization(Polygon *polygon, int position_of_left_most_point, int position_of_rightmost_point, vector<Point> edge_points)
{
    cout << "Running Global for subdivision\n";

    Polygon output_polygon = *polygon;
    T = 1.0;

    // cout << "are both polygons simple? " << polygon->is_simple() << " output polygon " << output_polygon.is_simple() << "\n";
    // getchar();
    while (T > 0)
    {
        vector<bool> visited;
        int random_point = rand() % (polygon->size() - 4) + 2;

        int random_edge = rand() % (polygon->edges().size() - 4) + 2;

        // den thelw na spasw tin epomeni i proigoumeni akmi tou simeiou
        while (random_edge == random_point || random_edge == (random_point - 1))
        {
            random_edge = rand() % (output_polygon.edges().size() - 4) + 2;
        }

        Point q = *(polygon->begin() + random_point);

        if (position_of_left_most_point >= 0)
        {

            if (q == edge_points[position_of_left_most_point])
            {
                continue;
            }
        }

        if (position_of_rightmost_point >= 0)
        {
            if (q == edge_points[position_of_rightmost_point])
            {
                continue;
            }
        }

        Point p = *(polygon->begin() + random_point - 1);
        // rs einai i epomeni akmi
        Point r = *(polygon->begin() + random_point + 1);

        Segment st = *(polygon->edges_begin() + random_edge);

        if (!check_validity(p, q, r, st))
        {
            continue;
        }

        output_polygon.erase(output_polygon.begin() + random_point);

        // find where t is in the new polygon after the erase of q
        PointIterator to_insert = find(output_polygon.begin(), output_polygon.end(), st.target());
        int position_to_insert = to_insert - output_polygon.begin();

        // insert q before t
        output_polygon.insert(output_polygon.begin() + position_to_insert, q);

        // as with local check if it's a good change
        double DE = CGAL::to_double(calculate_energy(output_polygon) - calculate_energy(*polygon));

        if (DE < 0.0 || Compute_Metropolis(DE, T, generate_random_01()))
        {
            *polygon = output_polygon;
        }
        else
        {
            output_polygon = *polygon;
        }

        T = T - (1.0 / double(L));
        cout << "T is " << T << "\n";
    }
}

const void SimulatedAnnealing::Global_Optimization(Polygon *polygon, Point left_most, Point right_most)
{
    cout << "Running Global for subdivision\n";
    cout << "stating polygon is simple " << polygon->is_simple() << "\n";

    Polygon output_polygon = *polygon;
    T = 1.0;
    while (T > 0)
    {

        int random_point = rand() % (polygon->size() - 4) + 2;

        int random_edge = rand() % (polygon->edges().size() - 4) + 2;

        // den thelw na spasw tin epomeni i proigoumeni akmi tou simeiou
        if (random_edge == random_point || random_edge == (random_point - 1))
        {
            continue;
        }

        Point q = *(polygon->begin() + random_point);

        if (q == left_most || q == right_most)
        {
            continue;
        }

        Point p = *(polygon->begin() + random_point - 1);
        // rs einai i epomeni akmi
        Point r = *(polygon->begin() + random_point + 1);

        Segment st = *(polygon->edges_begin() + random_edge);

        if (st.target() == right_most)
        {
            continue;
        }

        if (!check_validity(p, q, r, st))
        {
            continue;
        }

        output_polygon.erase(output_polygon.begin() + random_point);

        // find where t is in the new polygon after the erase of q
        PointIterator to_insert = find(output_polygon.begin(), output_polygon.end(), st.target());
        int position_to_insert = to_insert - output_polygon.begin();

        // insert q before t
        output_polygon.insert(output_polygon.begin() + position_to_insert, q);

        // as with local check if it's a good change
        double DE = CGAL::to_double(calculate_energy(output_polygon) - calculate_energy(*polygon));

        if (DE < 0.0 || Compute_Metropolis(DE, T, generate_random_01()))
        {
            *polygon = output_polygon;
        }
        else
        {
            output_polygon = *polygon;
        }

        if (!polygon->is_simple())
        {
            cout << "not simple anymore\n";
            getchar();
        }

        T = T - (1.0 / double(L));
        cout << "T is " << T << "\n";
    }
}

const void SimulatedAnnealing::Subdivision_Optimization()
{
    sort_points(&points, X_ASCENDING);
    srand(time(NULL));
    // int m = (rand() % 91) + 10;
    int m = 10;
    int ceil;

    vector<vector<Point>> subdivision_vectors;

    if ((points.size() - 1) % (m - 1))
    {
        ceil = (points.size() - 1) / (m - 1) + 1;
    }
    else
    {
        ceil = (points.size() - 1) / (m - 1);
    }
    int position = 0;
    int count = 0;
    vector<Point> temp;
    subdivision_vectors.push_back(temp);

    vector<Point> edge_points;
    for (Point p : points)
    {
        subdivision_vectors[position].push_back(p);
        count++;
        if (!(count % ceil))
        {
            vector<Point> t;
            subdivision_vectors.push_back(t);
            subdivision_vectors[position + 1].push_back(p);
            edge_points.push_back(p);
            position++;
        }
    }

    vector<Polygon> polygon_array;

    int position_of_points_vector = 0;
    for (vector<Point> vec : subdivision_vectors)
    {
        // Incrementing init_polygons(vec, X_ASCENDING);
        // init_polygons.Create_Polygon_Line();
        // polygon_array.push_back(init_polygons.Get_Simple_Polygon());

        // cout << "old area is: " << polygon_array[position_of_points_vector].area() << "\n";

        // if (!position_of_points_vector)
        // {
        //     Global_Optimization(&polygon_array[position_of_points_vector], -1, position_of_points_vector, edge_points);
        // }
        // else if (position_of_points_vector == subdivision_vectors.size() - 1)
        // {
        //     Global_Optimization(&polygon_array[position_of_points_vector], position_of_points_vector, -1, edge_points);
        // }
        // else
        // {
        //     Global_Optimization(&polygon_array[position_of_points_vector], position_of_points_vector - 1, position_of_points_vector, edge_points);
        // }

        // cout << "new area is: " << polygon_array[position_of_points_vector].area() << "\n";
        // getchar();
        // position_of_points_vector++;

        Incrementing init_poly(vec, X_ASCENDING);
        init_poly.Create_Polygon_Line();
        Polygon temp_polygon = init_poly.Get_Simple_Polygon();

        if (!position_of_points_vector)
        {
            Global_Optimization(&temp_polygon, Point(-1, -1), vec[vec.size() - 1]);

            new_polygon = Polygon(temp_polygon);
        }
        else if (position_of_points_vector == subdivision_vectors.size() - 1)
        {
            Global_Optimization(&temp_polygon, vec[0], Point(-1, -1));

            PointIterator position_to_add = find(new_polygon.begin(), new_polygon.end(), vec[0]);
            int position = position_to_add - new_polygon.begin();

            // vazw ta points sto polugwno me tin antitheti seira
            for (auto point_to_add = new_polygon.end() - 1; point_to_add != new_polygon.begin(); point_to_add--)
            {
                new_polygon.insert(new_polygon.begin() + position, *point_to_add);
            }
        }
        else
        {
            Global_Optimization(&temp_polygon, vec[0], vec[vec.size() - 1]);

            PointIterator position_to_add = find(new_polygon.begin(), new_polygon.end(), vec[0]);
            int position = position_to_add - new_polygon.begin();

            // vazw ta points sto polugwno me tin antitheti seira
            for (auto point_to_add = new_polygon.end() - 1; point_to_add != new_polygon.begin(); point_to_add--)
            {
                new_polygon.insert(new_polygon.begin() + position, *point_to_add);
            }
        }
        position_of_points_vector++;
        temp_polygon.clear();

        cout << "AFTER " << position_of_points_vector << "iteration new_polygon is simple -> " << new_polygon.is_simple() << "\n";
        getchar();
    }
}

const FT SimulatedAnnealing::calculate_energy(Polygon p)
{

    // pairnw ton arithmo twn simeiwn tou polygwnou
    int N = p.vertices().size();

    // ypologizw to Convex Hull
    vector<Point> convex_hull;
    CGAL::convex_hull_2(p.begin(), p.end(), std::back_inserter(convex_hull));

    Polygon CH(convex_hull.begin(), convex_hull.end());

    FT polygon_area = p.area();
    FT CH_area = CH.area();

    FT E;
    switch (target)
    {
    case MIN:
        E = N * (polygon_area / CH_area);
        break;
    case MAX:
        E = N * (1 - (polygon_area / CH_area));
        break;

    default:
        E = -1;
        break;
    }

    // cout << "Energy: " << E << "\n";

    return E;
}

const std::list<Point> SimulatedAnnealing::find_points_in_rectangle(Point p, Point q, Point r, Point s)
{
    std::list<Point> result;
    vector<Segment> edges_to_check;
    // sort by x and by y
    vector<Point> sorted_by_x;
    vector<Point> sorted_by_y;
    sorted_by_x.push_back(p);
    sorted_by_x.push_back(q);
    sorted_by_x.push_back(r);
    sorted_by_x.push_back(s);
    sort_points(&sorted_by_x, X_ASCENDING);

    sorted_by_y.push_back(p);
    sorted_by_y.push_back(q);
    sorted_by_y.push_back(r);
    sorted_by_y.push_back(s);
    sort_points(&sorted_by_y, Y_ASCENDING);

    // pairnw to mikrotero kai megalytero simeio lexikografika gia na ftiaksw to rectangle
    Point min = Point(sorted_by_x[0].x(), sorted_by_y[0].y());
    Point max = Point(sorted_by_x[3].x(), sorted_by_y[3].y());

    Fuzzy_iso_box exact_range(min, max);

    sorted_by_x.clear();
    sorted_by_y.clear();

    KD_tree.search(std::back_inserter(result), exact_range);

    // contains also the points we are checking
    return result;
}

bool SimulatedAnnealing::check_validity(Point p, Point q, Point r, Segment st)
{
    Segment pr(p, r);
    Segment sq(st.source(), q);
    Segment qt(q, st.target());

    if (check_intersection(sq, pr) || check_intersection(pr, qt))
    {
        return 0;
    }

    for (Segment edge : starting_polygon.edges())
    {
        if (edge == pr || edge == sq || edge == qt || edge == st || edge == Segment(p, q) || edge == Segment(q, r))
        {
            continue;
        }

        if (check_intersection(edge, pr) || check_intersection(edge, sq) || check_intersection(edge, qt))
        {
            return 0;
        }
    }

    return 1;
}

bool SimulatedAnnealing::check_validity(Point p, Point q, Point r, Point s)
{

    Segment pr(p, r);
    Segment qs(q, s);

    // arxika an kanoun intersect autes i akmes den einai valid epilogi
    if (check_intersection(pr, qs))
    {
        return 0;
    }

    std::list<Point> points_in_rectangle = find_points_in_rectangle(p, q, r, s);

    for (Point point : points_in_rectangle)
    {

        if (point == q || point == r)
        {
            continue;
        }

        // vriskw ti thesi tou simeiou sto polygwno
        PointIterator position_to_check = find(starting_polygon.begin(), starting_polygon.end(), point);
        int index = position_to_check - starting_polygon.begin();

        // prepei na tsekarw tis akmes stis theseis index kai index - 1
        Segment front_edge_to_check = *(starting_polygon.edges_begin() + index);

        Segment back_edge_to_check;

        // an to position einai 0 prepei na parw tin akmi sti thesi size - 1
        if (!index)
        {
            back_edge_to_check = *(starting_polygon.edges_begin() + starting_polygon.size() - 1);
        }
        else
        {
            back_edge_to_check = *(starting_polygon.edges_begin() + index - 1);
        }

        if (point == p)
        {

            if (check_intersection(back_edge_to_check, qs))
            {
                return 0;
            }
            continue;
        }

        if (point == s)
        {
            // cout << "for S checking edge " << front_edge_to_check << " whith " << pr << "\n";
            if (check_intersection(front_edge_to_check, pr))
            {

                return 0;
            }
            continue;
        }

        // tsekarw gia to front edge an kanoun intersect epistrefw 0
        if (check_intersection(front_edge_to_check, pr) || check_intersection(front_edge_to_check, qs))
        {
            return 0;
        }

        // tsekarw gia to back edge an kanoun intersect epistrefw 0
        if (check_intersection(back_edge_to_check, pr) || check_intersection(back_edge_to_check, qs))
        {
            return 0;
        }
    }

    // an gia oles autes tis akmes den exoume tomi tote i allagi einai valid
    return 1;
}