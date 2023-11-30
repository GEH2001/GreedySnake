#include "event.h"
#include "utils.h"
#include <iostream>

int Event::GetTime() const {
    return time;
}

void Event::SetTime(int _time) {
    time = _time;
}

EventType Event::GetType() const {
    return type;
}

void Event::SetType(EventType _type) {
    type = _type;
}

int Event::GetX() const {
    return x;
}

int Event::GetY() const {
    return y;
}

void Event::SetX(int _x) {
    x = _x;
}

void Event::SetY(int _y) {
    y = _y;
}

int Event::GetValue() const {
    return value;
}

void Event::Print() const {
    std::cout << time << " " << type << " " 
    << x << " " << y << std::endl;
}