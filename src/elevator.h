#include "passenger.h"

#include <vector>
#include <string>

class Elevator
{
public:
    Elevator(int elevatorCapacity);
    void set_current_floor(const int floor);
    int get_current_floor() const;
    void set_current_weight(const int weight);
    int get_current_weight() const;
    void set_destination_floor(const int floor);
    int get_destination_floor() const;
    void set_passengers(Passenger& passenger);
    void remove_passengers(Passenger& passenger);
    std::vector<Passenger> get_passengers() const;
    void set_state(const std::string state);
    std::string get_state() const;
    int get_elevator_capacity() const;

private:
    int elevatorCapacity;
    int current_floor = 0;
    int current_weight = 0;
    int destination_floor = 0;
    std::string state = "waiting";
    std::vector<Passenger> passengers;
};