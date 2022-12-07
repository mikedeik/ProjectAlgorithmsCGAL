#include "util.h"

// sorting vasi x fthinousa
// an einai suneutheiaka sort kata y
bool Sort_Desc_X(Point p1, Point p2)
{

    if (!(CGAL::compare_x(p1, p2)))
    {
        return CGAL::compare_y(p1, p2) < 0;
    }

    return CGAL::compare_x(p1, p2) < 0;
}

// sorting vasi x auksousa
// an einai suneutheiaka sort kata y
bool Sort_Asc_X(Point p1, Point p2)
{
    if (!(CGAL::compare_x(p1, p2)))
    {
        return CGAL::compare_y(p1, p2) > 0;
    }
    return CGAL::compare_x(p1, p2) > 0;
}

// sorting vasi y fthinousa
// an einai suneutheiaka sort kata x
bool Sort_Desc_Y(Point p1, Point p2)
{
    if (!(CGAL::compare_y(p1, p2)))
    {
        return CGAL::compare_x(p1, p2) < 0;
    }
    return CGAL::compare_y(p1, p2) < 0;
}

// sorting vasi y auksousa
// an einai suneutheiaka sort kata x
bool Sort_Asc_Y(Point p1, Point p2)
{
    if (!(CGAL::compare_y(p1, p2)))
    {
        return CGAL::compare_x(p1, p2) > 0;
    }
    return CGAL::compare_y(p1, p2) > 0;
}

/*  string filename : to path enos arxeiou me ta points
    vector<Point>* points : deiktis se vector pou krataei points
    diavazei ta points apo arxeio kai ta thetei mesa se dosmeno vector*/

void get_points_from_file(string filename, vector<Point> *points)
{
    string line;
    std::ifstream myfile(filename);
    Point p;
    while (getline(myfile, line))
    {
        if (line[0] == '#')
        {
            continue;
        }
        std::istringstream input(line);
        int to_drop;
        input >> to_drop;
        input >> p;
        points->push_back(p);
    }

    myfile.close();
}

/*  vector<Point>* points : deiktis se vector pou krataei points
    Sorter: sorter : kathorizei pws theloume na sortaristei to array
    analogws ton Sorter sortarei to array*/
void sort_points(vector<Point> *points, Sorter sorter)
{
    switch (sorter)
    {
    case X_ASCENDING:
        sort(points->begin(), points->end(), Sort_Asc_X);
        break;
    case X_DESCENDING:
        sort(points->begin(), points->end(), Sort_Desc_X);
        break;
    case Y_ASCENDING:
        sort(points->begin(), points->end(), Sort_Asc_Y);
        break;
    case Y_DESCENDING:
        sort(points->begin(), points->end(), Sort_Desc_Y);
        break;
    default:
        break;
    }
}
/*  Epistrefei an to trigwno t kai to segment e temnontai*/
bool check_intersection(Triangle t, Segment e)
{
    const auto result = CGAL::intersection(e, t);

    if (result)
    {
        // an to apotelesma einai euthigramo tmima tote i akmi kovetai apo to trigwno
        if (const Segment *s = boost::get<Segment>(&*result))
        {
            return 1;
        }
        else
        {
            const Point *p = boost::get<Point>(&*result);
            if (*p != e.target() && *p != e.source())
            {
                return 1;
            }
        }
    }
    return 0;
}

bool check_intersection(Segment t1, Segment t2)
{
    const auto result = CGAL::intersection(t1, t2);

    if (result)
    {
        // if (const Point *p = boost::get<Point>(&*result))
        // {
        //     if ((*p == t1.source() && *p == t2.target()) || (*p == t2.source() && *p == t1.target()))
        //     {
        //         return 0;
        //     }
        // }

        return 1;
    }

    return 0;
}

/*Unsued*/
bool check_intersection_BFS(vector<Segment> edges, int position, Triangle t)
{
    queue<Segment> bag;
    bool visited[edges.size()];
    Segment edge;
    visited[position] = true;

    bag.push(edges[position]);

    int is_first_edge = 1;
    while (!bag.empty())
    {
        edge = bag.front();
        bag.pop();

        if (!is_first_edge)
        {
            const auto result = CGAL::intersection(t, edge);
            if (result)
            {
                if (const Segment *s = boost::get<Segment>(&*result))
                {
                    cout << "intersects\n";
                    return 1;
                }
            }
        }

        if (position == edges.size() - 1 || !position)
        {
            continue;
        }

        if (!visited[position + 1])
        {
            visited[position + 1] = true;
            bag.push(edges[position + 1]);
        }

        if (!visited[position - 1])
        {
            visited[position + 1] = true;
            bag.push(edges[position + 1]);
        }

        is_first_edge = 0;
    }

    return 0;
}

// pairnei ws orisma ena polygwno(tou kyrtou perivlimatos), mia thesi (int), ena simeio, kai ena pointer se array apo Segments
// dianyei to polygwno me fora CCW ksekinwntas apo do to position_to_start edge kai prosthei tis
// kokkines akmes apo to shmeio p pros to kyrto perivlima
void traverse_ccw(const Polygon ch_polygon, int position_to_start, Point p, vector<Segment> *red_edges)
{

    for (auto edge_it = ch_polygon.edges_begin() + position_to_start; edge_it != ch_polygon.edges_end(); ++edge_it)
    {

        bool intersects = 0;

        // ftiaxnw trigwno me tin kainourgia akmi
        Triangle t((*edge_it).source(), p, (*edge_it).target());

        // an to p einai syneutheiako me thn akmi tha parw mia alli
        if (t.is_degenerate())
        {
            continue;
        }

        // gia kathe epomeni tou akmi koitaw an tin kovei (pou simainei pws den einai orati)

        for (auto intersect_it = ch_polygon.edges_begin(); intersect_it != ch_polygon.edges_end(); ++intersect_it)
        {

            if (*intersect_it == *edge_it)
            {
                continue;
            }
            // an kovei kapoia alli akmi den einai kokkini
            const Segment to_test = *intersect_it;

            if (check_intersection(t, to_test))
            {
                intersects = 1;
                break;
            }
        }
        // an den einai kokkini tote den einai kai kamia epomeni
        if (intersects)
        {
            break;
        }

        // an den kovei kamia akmi tote einai kokkini

        red_edges->push_back(*edge_it);
        // opote paw stin epomeni
    }
}

// pairnei ws orisma ena polygwno(tou kyrtou perivlimatos), mia thesi (int), ena simeio, kai ena pointer se array apo Segments
// dianyei to polygwno me fora CW ksekinwntas apo do to position_to_start edge - 1 kai prosthei tis
// kokkines akmes apo to shmeio p pros to kyrto perivlima prin apo tis proigoumenes gia na einai swsta sorted
void traverse_cw(const Polygon ch_polygon, int position_to_start, Point p, vector<Segment> *red_edges)
{

    if (!position_to_start)
    {
        position_to_start = ch_polygon.edges().size();
    }
    // ksekiname apo tin akmi position_to_start - 1 kai pame pros ta pisw mexri tin prwti
    for (auto edge_it = ch_polygon.edges_begin() + position_to_start - 1; edge_it >= ch_polygon.edges_begin(); --edge_it)
    {

        bool intersects = 0;

        Triangle t((*edge_it).source(), p, (*edge_it).target());

        if (t.is_degenerate())
        {
            continue;
        }

        // gia kathe epomeni akmi tsekarw an kanoun intersect

        for (auto intersect_it = ch_polygon.edges_begin(); intersect_it != ch_polygon.edges_end(); ++intersect_it)
        {

            if (*intersect_it == *edge_it)
            {

                continue;
            }
            // tha doume an to result einai apla ena simeio (diladi to )
            const Segment to_test = *intersect_it;
            if (check_intersection(t, to_test))
            {

                intersects = 1;
                break;
            }
        }
        // an den einai orari tote den einai kai oi apomenes
        if (intersects)
        {
            break;
        }
        // edw tis prosthetw stin arxi tou array
        red_edges->insert(red_edges->begin(), *edge_it);
    }
}

void print_to_file(const Polygon ch_polygon, string filename, int time)
{
    std::ofstream MyFile(filename);
    for (Point p : ch_polygon)
    {
        MyFile << p << "\n";
    }
    for (Segment edge : ch_polygon.edges())
    {
        MyFile << edge << "\n";
    }
    MyFile << "area" << ch_polygon.area() << "\n";
    MyFile << "construction time " << time << "\n";
    MyFile << "Polygon is Simple: " << ch_polygon.is_simple() << "\n";
    MyFile.close();
}

bool Compute_Metropolis(double DE, double T, double R)
{
    return exp((-1) * DE / T) >= R;
}

void Print_Errors(int position)
{
    switch (position)
    {
    case 1:
        cout << "Error on input file\n";
        break;
    case 2:
        cout << "Error on output file\n";
        break;
    case 3:
        cout << "Error on algorithm\n";
        break;
    case 4:
        cout << "Error on L parameter\n";
        break;
    case 5:
        cout << "Error on Target\n";
        break;
    case 6:
        cout << "Error on Initialization\n";
        break;

    default:
        cout << "Error on given arguments count. 11 arguments expexted\n";
        break;
    }

    cout << "** Arguments should be: **\n"
         << "1. -i <point set input file>\n"
         << "2. -o <output file>\n"
         << "3. -algorithm <local_search or simulated_annealing>\n"
         << "4. -L <L parameter according to algorithm>\n"
         << "5. -max or -min [depending on Polygon Area Target]\n"
         << "6. -threshold <double> OR -annealing <local or global or subdivision> [depending on algorithm]\n";
}

typedef CGAL::Search_traits_2<Kernel> Traits;
typedef CGAL::Kd_tree<Traits> Tree;
typedef CGAL::Fuzzy_iso_box<Traits> Fuzzy_iso_box;

void testing_KD_tree(vector<Point> inc_points)
{

    std::list<Point> points;
    points.push_back(Point(0, 0));
    Tree tree;
    for (Point p : inc_points)
    {
        tree.insert(p);
    }

    std::list<Point> result;
    // define range query
    Point p = inc_points[3];
    Point q = inc_points[7];
    // Searching an exact range
    // using default value 0.0 for epsilon fuzziness paramater
    Fuzzy_iso_box exact_range(p, q);
    tree.search(std::back_inserter(result), exact_range);
    std::cout << "The points in the box [" << p.x() << "," << q.y() << "]^2 are: " << std::endl;
    std::copy(result.begin(), result.end(), std::ostream_iterator<Point>(std::cout, "\n"));
    std::cout << std::endl;
    result.clear();
}

double generate_random_01()
{
    return rand() / (RAND_MAX + 1.);
}