#pragma once
#include "elevator.hpp"

class Elevator;
enum class ElevatorState;

enum class HumanState {
  UpWaiting,   // エレベーターを待っている
  DownWaiting, // エレベーターを待っている
  OnBoarding,  // エレベーターに乗っている
  Other        // エレベーター降りた後や待っていない状態
};

class Human {
 public:
  Human(int id, int weight);
  void determine_call_elevator(int calling_id, int floor_org, int floor_dest);
  void determine_board_elevator(int current_floor, ElevatorState current_elevator_state);
  void determine_get_off_elevator(int current_floor, ElevatorState current_elevator_state);
  int get_floor_original() const;
  int get_floor_destination() const;
  HumanState get_human_state() const;
  int get_id() const;

 private:
  int id;
  int weight;
  int floor_original;
  int floor_destination;
  HumanState human_state;
};