#if !defined(VISIBLEEDGE)
#define VISIBLEEDGE

#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include "util.h"
#include <CGAL/convex_hull_2.h>
#include <boost/optional/optional_io.hpp>
#include <chrono>
typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point;
typedef CGAL::Polygon_2<Kernel> Polygon;
typedef Kernel::Segment_2 Segment;
typedef std::vector<Point> Points;
typedef Kernel::FT FT;
typedef std::vector<Point>::const_iterator PointIterator;

class VisibleEdge
{
private:
    vector<Point> points, convex_hull_points, points_not_in_chain, nearest_poits; // result has the convex hull
    Polygon polygon;

    vector<Segment> edges;
    void Create_Convex_Hull();
    bool Is_Point_Included_In_Polygonal_Chain(Point);
    Point Find_Nearest_Point_To_Segment(Segment);
    int Find_Index_Of_Point_In_Vector(Point, vector<Point>);
    int Find_Index_In_Polygon(Point);
    int time;
public:
    VisibleEdge(std::vector<Point> points,string output_file);
    ~VisibleEdge();
    void Add_Point(Point);
    void Print_Edges();
    // Create_Polygon requires initialization with create_convex_hull
    // and adds the internal points to the polygonal chain
    void Create_Polygon();
    FT Get_Polygon_Area();
};

#endif