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
    Polygon starting_polygon; // to arxiko polygwno pou tha krataei to aplo polygwno gia backtrack
    Polygon new_polygon;      // to teliko polygwno meta to optimization
    Polygon initial_polygon;  // to  arxiko polygwno meta apo tin xrisi tou prwtou algorithmou

    vector<Point> points; // ena vector apo points pou tha lavei ws orisma o algorithmos

    AnnealingType type;   // o tropos pou tha ginei to optimization
    Target target;        // to target tou optimization (MIN or MAX)
    Tree KD_tree;         // to KD_tree pou tha ftiaksoume apo ta points
    double starting_area; // metavliti pou tha krataei to starting area
    string output_file;   // to arxeio eksodou
    int L;                // to L (arithmos epanallipsewn)
    double T = 1.0;       // i thermokrasia T
    int run_time;         // o xronos ektelesis tou Optimization

    /* Calculates the energy of given Polygon */
    const FT calculate_energy(Polygon p);

    /*Epistrefei mia lista me ta simeia mesa sto orthogwnio pou dhmiourgeitai apo ta p,q,r,s */
    const std::list<Point> find_points_in_rectangle(Point p, Point q, Point r, Point s);

    /* Epistrefei true an einai valid epilogi alliws false - Local Step*/
    bool check_validity(Point p, Point q, Point r, Point s);

    /* Epistrefei true an einai valid epilogi alliws false - Global Step*/
    bool check_validity(Point p, Point q, Point r, Segment st);

    /* Epistrefei true an einai valid epilogi se dwto polygwno alliws false - Subdivision Step*/
    bool check_validity(Point p, Point q, Point r, Segment st, Polygon temp_polygon);

    /* Kanei optimization tou Area tou arxikou polygwnou me local step (epilogi tyxaiou simeiou kai prospatheia allagis me to epomeno) */
    const void Local_Optimization();

    /* Kanei optimization tou Area tou arxikou polygwnou me global step (epilogi tyxaiou simeiou, afairesi apo tin polygwniki grammi kai prosthiki se tyxaio edge)*/
    const void Global_Optimization();

    /* Omoiws global optimization xwris na peirazei to pio aristera kai deksia simeio tou dwtou polygwnou (xrhsimopoieitai sto subdivision )*/
    const void Global_Optimization(Polygon *polygon, Point left_most, Point right_most);

    /* Diaxwrizei ta points se (n-1)/(m-1) ypoprovlimata */
    const void Subdivision_Optimization();

public:
    /*  inc_points : vector of Points
        an_type : should be LOCAL or GLOBAL or SUBDIVSION
        Area_target: should be MIN or MAX
        L : should be an int (on thousands for better results)
        inc_output_file: output file path */
    SimulatedAnnealing(vector<Point> inc_points, AnnealingType an_type, Target Area_target, int L, string inc_output_file);
    ~SimulatedAnnealing();

    /* Veltistopoiei to emvado tis polygwnikis grammis analoga me ta orismata tou algorithmou*/
    const void OptimizeArea();
    const double get_ratio();
};

#endif // SIMANNEALING