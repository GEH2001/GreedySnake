#pragma once
#include "point.h"
#include "event.h"
#include <vector>

class Snake;
class Map;

class FoodManager {
private:
    std::vector<Point> foods;
    int num;    // 食物總数量
    float prob[3]; // 食物生成概率
    bool InFoods(const Point &food) const;
public:
    FoodManager(int _num);
    FoodManager(int _num, float _prob[]);
    void Generate(Snake &snake, Map &map, int _time, std::vector<Event>& _events);
    void Generate(Snake &snake, Map &map, int x, int y, int value);
    int BeEaten(const Point &head);
};