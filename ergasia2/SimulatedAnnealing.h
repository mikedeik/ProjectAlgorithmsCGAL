#if !defined(SIMANNEALING)
#define SIMANNEALING

#include "util.h"
#include <CGAL/Kd_tree.h>
#include <CGAL/Fuzzy_iso_box.h>
#include <CGAL/Search_traits_2.h>

typedef CGAL::Search_traits_2<Kernel> Traits;
typedef CGAL::Kd_tree<Traits> Tree;
typedef CGAL::Fuzzy_iso_box<Traits> Fuzzy_iso_box;

class SimulatedAnnealing
{
private:
    Polygon starting_polygon;
    Polygon new_polygon;
    vector<Point> points;

    // vector<Point> *subdivision_vectors;
    // int m = 0;
    AnnealingType type;
    Target target;
    Tree KD_tree;
    double starting_area;
    int L;
    double T = 1.0;

    /* Calculates the energy of given Polygon */
    const FT calculate_energy(Polygon p);

    /*Epistrefei mia lista me ta simeia mesa sto orthogwnio pou dhmiourgeitai apo ta p,q,r,s */
    const std::list<Point> find_points_in_rectangle(Point p, Point q, Point r, Point s);

    /* Epistrefei true an einai valid epilogi alliws false - Local Step*/
    bool check_validity(Point p, Point q, Point r, Point s);

    /* Epistrefei true an einai valid epilogi alliws false - Global Step*/
    bool check_validity(Point p, Point q, Point r, Segment st);

public:
    SimulatedAnnealing(vector<Point> inc_points, AnnealingType an_type, Target Area_target, int L);
    ~SimulatedAnnealing();

    const void MinimizeArea();
    const void Local_Optimization();
    const void Global_Optimization();
    const void Global_Optimization(Polygon *polygon, int position_of_left_most_point, int position_of_rightmost_point, vector<Point> edge_points);
    const void Global_Optimization(Polygon *polygon, Point left_most, Point right_most);
    const void Subdivision_Optimization();
};

#endif // SIMANNEALING