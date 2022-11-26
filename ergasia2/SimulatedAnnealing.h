#if !defined(SIMANNEALING)
#define SIMANNEALING

#include "util.h"

class SimulatedAnnealing
{
private:
    Polygon starting_polygon;
    Polygon new_polygon;
    AnnealingType type;

public:
    SimulatedAnnealing(Polygon polygon, AnnealingType an_type);
    ~SimulatedAnnealing();
    const void MinimizeArea();
    const void MaximizeArea();
};

#endif // SIMANNEALING