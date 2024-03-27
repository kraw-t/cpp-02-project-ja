#include "elevator.h"
#include <cassert>
#include <numeric>

bool Elevator::tryAddPassengerQueue(const std::shared_ptr<Person>& person){
    assert(person);

    if (person->weight_ + current_weight_ + expected_weight_increase_> max_weight_)
        return false;

    passengers_to_pick_up_.push(person);
    expected_weight_increase_ += person->weight_;
    return true;
};

void Elevator::process(){
    maybePickUpOrDropOff();
    move();
};

void Elevator::maybePickUpOrDropOff(){
    // drop off
    for (auto itr = std::begin(passengers_); itr != std::end(passengers_);){
        assert(*itr);
        if ((*itr)->drop_off_floor_ == current_floor_){
            (*itr)->exitElevator();
            current_weight_ -= (*itr)->weight_;
            itr = passengers_.erase(itr);
            continue;
        }
        itr++;
    }
    updateState();

    // pick up
    if(passengers_to_pick_up_.size() == 0)
        return;
    auto &person = passengers_to_pick_up_.front();
    if(person->pick_up_floor_ != current_floor_)
        return;
    switch(current_state_){
        case State::GoingUp: //fallthrough
        case State::GoingDown:
            if(person->isGoingUp() != isGoingUp()){
                break;
            }
            //fallthrough
        case State::Idle:
            passengers_.push_back(person);
            current_weight_ += person->weight_;
            expected_weight_increase_ -= person->weight_;
            passengers_to_pick_up_.pop();
            break;
        }
    updateState();
}

void Elevator::updateState(){
    switch(current_state_){
        case State::GoingUp: //fallthrough 
        case State::GoingDown:
            if(passengers_.size() == 0)
                current_state_ = State::Idle;
            break;
        case State::Idle:
            if(passengers_.size()>0){
                current_state_ = passengers_.front()->isGoingUp() ? State::GoingUp : State::GoingDown;
            }
            break;
        }
}

void Elevator::move(){
   if(current_floor_== 0)
       assert(current_state_ != State::GoingDown);
   if(current_floor_== num_of_floors_-1)
       assert(current_state_ != State::GoingUp);
    
    switch(current_state_){
        case State::GoingUp:
            current_floor_ += speed_;
            break;
        case State::GoingDown:
            current_floor_ -= speed_;
            break;
        case State::Idle:
            break;
    }
}