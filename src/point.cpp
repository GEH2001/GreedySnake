#include "point.h"
#include "utils.h"
#include <iostream>

void Point::Print() const {
    SetCursor(x, y);
    switch(type) {
        case BLANK:
            std::cout << " ";
            break;
        case SOLIDHORWALL:
            SetColor(0x0C);
            std::cout << "-";
            break;
        case SOLIDVERWALL:
            SetColor(0x0C);
            std::cout << "|";
            break;
        case NSOLIDHORWALL:
            SetColor(0x02);
            std::cout << "-";
            break;
        case NSOLIDVERWALL:
            SetColor(0x02);
            std::cout << "|";
            break;
        case BLOCK:
            SetColor(0x0C);
            std::cout << "O";
            break;
        case HEAD:
            SetColor(0x27);
            std::cout << "#";
            break;
        case BODY:
            SetColor(0x27);
            std::cout << "*";
            break;
        case DEADHEAD:
            SetColor(0x47);
            std::cout << "#";
            break;
        case DEADBODY:
            SetColor(0x47);
            std::cout << "*";
            break;
        case FOOD1:
            SetColor(0x17);
            std::cout << "@";
            break;
        case FOOD2:
            SetColor(0x57);
            std::cout << "@";
            break;
        case FOOD3:
            SetColor(0x67);
            std::cout << "@";
            break;
        default:
            break;
    }
    SetColor(0x07); // 恢复默认颜色
}


int Point::GetX() const {
    return x;
}

int Point::GetY() const {
    return y;
}

void Point::SetType(PointType _type) {
    type = _type;
}

PointType Point::GetType() const {
    return type;
}

bool Point::operator==(const Point &p) const {
    return x == p.x && y == p.y;
}