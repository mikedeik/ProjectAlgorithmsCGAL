#if !defined(LOCALSEARCH)
#define LOCALSEARCH

#include "util.h"
#include "incrementing.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <boost/optional/optional_io.hpp>
#include <chrono>
#include <CGAL/ch_selected_extreme_points_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef Polygon_2::Vertex_iterator VertexIterator;
using std::vector;

class LocalSearch
{
private:
    Polygon current_polygon;
    float threshold;
    // kratao ena history apo ola ta versions tou polygon
    //  meta apo kath itteration
    vector<Polygon> polygon_history;

public:
    LocalSearch(vector<Point>,char* operation,float threshold);//'max' for maximazation 'min' for minimization
    ~LocalSearch();
    const void MinimizeArea();
    const void MaximizeArea();
};
Polygon RemovePath(Polygon, vector<Point>);
const void Print_Polygon(Polygon);
#endif // LOCALSEARCH
