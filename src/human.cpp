#include "elevator.hpp"
#include "human.hpp"

Human::Human(int id, int weight) : id(id), weight(weight) {
  this->human_state = HumanState::Other;
}

// Sim入力を受けて、このHumanがエレベーターを呼ぶかどうかを判断
void Human::determine_call_elevator(int calling_id, int floor_org, int floor_dest) {
  if ((this->id == calling_id)
   && (this->human_state == HumanState::Other)
   && (floor_dest != floor_org)) {
    this->floor_original = floor_org;
    this->floor_destination = floor_dest;
    if (floor_dest > floor_org) {
      this->human_state = HumanState::UpWaiting;
    } else {
      this->human_state = HumanState::DownWaiting;
    }
  } else{
    // 現在階と行先が同じ場合は「乗らない」とする
  }
  return;
}

// 現在フロアを受けて、待っているHumanが乗るかどうかを判断
void Human::determine_board_elevator(int current_floor, ElevatorState current_elevator_state) {
  if ((current_floor == this->floor_original)
   && (((this->human_state == HumanState::UpWaiting)
     && (current_elevator_state == ElevatorState::Rising))
    || ((this->human_state == HumanState::DownWaiting)
     && (current_elevator_state == ElevatorState::Descending)))) {
    this->human_state = HumanState::OnBoarding;
  }
}

// 現在フロアを受けて、乗っているHumanが降りるかどうかを判断
void Human::determine_get_off_elevator(int current_floor, ElevatorState current_elevator_state) {
  if ((this->human_state == HumanState::OnBoarding)
   && (current_floor == this->floor_destination)) {
    this->human_state = HumanState::Other;
  }
}

int Human::get_floor_original() const {
  return this->floor_original;
}

int Human::get_floor_destination() const {
  return this->floor_destination;
}

HumanState Human::get_human_state() const {
  return this->human_state;
}

int Human::get_id() const {
  return this->id;
}
