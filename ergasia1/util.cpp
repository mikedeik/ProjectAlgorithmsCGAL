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
bool Sort_Desc_Y(Point p1, Point p2)
{
    if (!(CGAL::compare_y(p1, p2)))
    {
        return CGAL::compare_x(p1, p2) < 0;
    }
    return CGAL::compare_y(p1, p2) < 0;
}

// sorting vasi y auksousa
bool Sort_Asc_Y(Point p1, Point p2)
{
    if (!(CGAL::compare_y(p1, p2)))
    {
        return CGAL::compare_x(p1, p2) > 0;
    }
    return CGAL::compare_y(p1, p2) > 0;
}

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
}

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

bool check_intersection(Triangle t, Segment e)
{
    const auto result = CGAL::intersection(t, e);

    if (result)
    {
        // an to apotelesma einai euthigramo tmima tote i akmi den einai kokkini
        if (const Segment *s = boost::get<Segment>(&*result))
        {
            return 1;
        }
    }
    return 0;
}

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

void traverse_cw(const Polygon ch_polygon, int position_to_start, Point p, vector<Segment> *red_edges, Sorter sorter)
{
    for (auto edge_it = ch_polygon.edges_begin() + position_to_start; edge_it != ch_polygon.edges_end(); ++edge_it)
    {
        cout << "checking edge: " << *edge_it;
        getchar();
        bool intersects = 0;

        // ftiaxnw trigwno me tin kainourgia akmi
        Triangle t((*edge_it).source(), p, (*edge_it).target());

        // gia kathe epomeni tou akmi koitaw an tin kovei (pou simainei pws den einai orati)

        for (auto intersect_it = ch_polygon.edges_begin() + position_to_start + 1; intersect_it != ch_polygon.edges_end(); ++intersect_it)
        {
            if (intersect_it == edge_it)
            {
                cout << "same\n";
                continue;
            }
            // tha doume an to result einai apla ena simeio (diladi to )
            const Segment to_test = *intersect_it;

            if (check_intersection(t, to_test))
            {
                cout << "intersects\n";
                intersects = 1;
                break;
            }
        }
        // opote paw stin epomeni
        if (intersects)
        {
            break;
        }

        // paw twra pros tin antitheti kateuthinsi kai kanw to idio
        for (auto intersect_it = ch_polygon.edges_end() - position_to_start - 1; intersect_it != ch_polygon.edges_begin(); --intersect_it)
        {
            if (intersect_it == edge_it)
            {
                cout << "same\n";
                continue;
            }
            const Segment to_test = *intersect_it;
            if (check_intersection(t, to_test))
            {
                cout << "intersects\n";
                intersects = 1;
                break;
            }
        }

        if (intersects)
        {
            break;
        }
        // an den kovei kamia akmi tote einai kokkini
        red_edges->push_back(*edge_it);
    }

    cout << "---- Red Edges ----\n";
    for (Segment edge : *red_edges)
    {
        cout << edge << "\n";
    }
}

void traverse_ccw(const Polygon ch_polygon, int position_to_start, Point p, vector<Segment> *red_edges, Sorter sorter)
{
    for (auto edge_it = ch_polygon.edges_end() - position_to_start - 1; edge_it != ch_polygon.edges_begin(); --edge_it)
    {
        cout << "checking edge: " << *edge_it;
        getchar();
        bool intersects = 0;

        Triangle t((*edge_it).source(), p, (*edge_it).target());

        // gia kathe epomeni akmi tsekarw an kanoun intersect

        // const Polygon::Edges& ch_edges = ch_polygon.edges();
        // check_intersection_BFS(ch_edges, )

        for (auto intersect_it = ch_polygon.edges_end() - position_to_start; intersect_it != ch_polygon.edges_end(); ++intersect_it)
        {
            if (intersect_it == edge_it)
            {
                cout << "same\n";
                continue;
            }
            // tha doume an to result einai apla ena simeio (diladi to )
            const Segment to_test = *intersect_it;
            if (check_intersection(t, to_test))
            {
                cout << "intersects 1\n";
                intersects = 1;
                break;
            }
        }
        if (intersects)
        {
            break;
        }
        // to idio kai gia tis proigoumenes
        for (auto intersect_it = ch_polygon.edges_end() - position_to_start - 1; intersect_it != ch_polygon.edges_begin(); --intersect_it)
        {
            if (intersect_it == edge_it)
            {
                cout << "same\n";
                continue;
            }

            const Segment to_test = *intersect_it;
            if (check_intersection(t, to_test))
            {
                cout << "intersects 2\n";
                intersects = 1;
                break;
            }
        }

        if (!intersects)
        {
            red_edges->insert(red_edges->begin(), *edge_it);
        }
    }

    cout << "---- Red Edges ----\n";
    for (Segment edge : *red_edges)
    {
        cout << edge << "\n";
    }
}