#include "elevator.h"

Elevator::Elevator(int elevatorCapacity) : elevatorCapacity(elevatorCapacity) {}

void Elevator::set_current_floor(const int floor){
    current_floor = floor;
}

int Elevator::get_current_floor() const {
    return current_floor;
}

void Elevator::set_current_weight(const int weight){
    current_weight = weight;
}

int Elevator::get_current_weight() const {
    return current_weight;
}

void Elevator::set_destination_floor(int floor){
    destination_floor = floor;
}

int Elevator::get_destination_floor() const {
    return destination_floor;
}

void Elevator::set_passengers(Passenger& passenger){
    passengers.push_back(passenger);
}

void Elevator::remove_passengers(Passenger& passenger){
    for (auto it = passengers.begin(); it != passengers.end(); ++it) {
        if (it->get_id() == passenger.get_id()) {
            passengers.erase(it);
            break;
        }
    }
}

std::vector<Passenger> Elevator::get_passengers() const {
    return passengers;
}

void Elevator::set_state(std::string state){
    this->state = state;
}

std::string Elevator::get_state() const {
    return state;
}

int Elevator::get_elevator_capacity() const {
    return elevatorCapacity;
}
