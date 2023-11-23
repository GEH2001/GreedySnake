#pragma once
#include "point.h"
#include <vector>
#include <string>


class Map {
private:
    std::vector<Point> walls;   // 墙
    std::vector<Point> blocks; // 障碍物
    int valid[4];      // 四面墙的虚实(上下左右), 1为实, 0为虚
    std::string mapfile;    // 地图文件相对路径 maps/default.map
    int LoadMap(const std::string _mapfile);
public:
    static int width;
    static int height;

    Map();
    Map(const std::string _mapfile);
    bool IsValid(int direction) const;
    bool InBlocks(const Point &p) const;
    void Print() const;
};
