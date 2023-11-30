#pragma once
#include "point.h"
#include "map.h"
#include "food.h"
#include <deque>

enum Direction {UP, DOWN, LEFT, RIGHT};

class Snake {
private:
    std::deque<Point> body;
    Direction direction;
public:
    Snake();
    void Print() const;
    void Move();
    void Grow();
    int WaitKey(int interval);
    int Wait(int interval);
    int RecDirection(Direction _direction);
    bool InBody(Point &p) const;
    Point GetHead() const;
    bool IsHitMap(const Map &map) const;
    bool IsHitSelf() const;
    void Die();
};