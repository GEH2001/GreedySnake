#pragma once

enum PointType { BLANK, SOLIDHORWALL, SOLIDVERWALL, NSOLIDHORWALL, NSOLIDVERWALL, BLOCK, HEAD, BODY, DEADHEAD, DEADBODY, FOOD1, FOOD2, FOOD3 };

class Point {
private:
    int x, y;
    PointType type;
public:
    Point() {}
    Point(int _x, int _y, PointType _type) : x(_x), y(_y), type(_type) {}
    void Print() const;
    int GetX() const;
    int GetY() const;
    void SetType(PointType _type);
    PointType GetType() const;
    bool operator==(const Point &p) const;
};