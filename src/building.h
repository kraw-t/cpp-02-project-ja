#include "elevator.h"

class Building
{
public:
    Building(int numberOfFloors, int numberOfElevators, Elevator elevator);
    Elevator get_elevator() const;

private:
    Elevator elevator;
    int numberOfFloors;
    int numberOfElevators;
};