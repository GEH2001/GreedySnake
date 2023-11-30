#pragma once

enum EventType { F, W, A, S, D };

class Event{
private:
    int time;
    int x, y; // 食物坐标、蛇头坐标
    int value; // 食物经验
    EventType type;
public:
    Event() {}
    Event(int _time, EventType _type, int _x, int _y) : time(_time), type(_type), x(_x), y(_y), value(0) {}
    Event(int _time, EventType _type, int _x, int _y, int _value) : time(_time), type(_type), x(_x), y(_y), value(_value) {}

    int GetTime() const;
    void SetTime(int _time);
    EventType GetType() const;
    void SetType(EventType _type);
    int GetX() const;
    int GetY() const;
    void SetX(int _x);
    void SetY(int _y);
    int GetValue() const;

    void Print() const;

    // bool operator==(const Point &p) const;
};