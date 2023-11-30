#include "map.h"

#include <windows.h>
#include <fstream>
#include <iostream>

int Map::width = 15;
int Map::height = 15;

/* 初始构造 */ 
Map::Map() {

    int w = Map::width + 2;
    int h = Map::height + 2;    // 加上边界

    for (int i = 0; i < w; i++) {
        walls.emplace_back(i, 0, SOLIDHORWALL);
        walls.emplace_back(i, h - 1, SOLIDHORWALL);
    }
    for (int i = 1; i < h - 1; i++) {
        walls.emplace_back(0, i, SOLIDVERWALL);
        walls.emplace_back(w - 1, i, SOLIDVERWALL);
    }
}

/* 读取地图配置构造 */
Map::Map(const std::string _mapfile) {
    this->mapfile = "maps/" + _mapfile + ".map";
    LoadMap(this->mapfile);
}

/* 读取地图文件 */
int Map::LoadMap(const std::string _mapfile) {

    std::ifstream fin(_mapfile);
    if(!fin.is_open()) {
        system("cls");
        std::cout << _mapfile << " not found!" << std::endl;
        system("pause");
        exit(0);
    }

    fin >> Map::width >> Map::height; // 地图大小
    fin >> valid[0] >> valid[1] >> valid[2] >> valid[3]; // 四面墙的虚实
    int n, x, y;
    fin >> n; // 障碍物数量
    while(n--) { // 障碍物坐标
        fin >> x >> y;
        blocks.emplace_back(x + 1, y + 1, BLOCK);
    }

    fin.close();

    // 墙 注意 + 2
    int w = Map::width + 2;
    int h = Map::height + 2;    // 加上边界

    PointType walltype[4];
    for(int i = 0; i < 2; i++) { // 上下墙
        if(this->valid[i]) {
            walltype[i] = SOLIDHORWALL;
        } else {
            walltype[i] = NSOLIDHORWALL;
        }
    }
    for(int i = 2; i < 4; i++) { // 左右墙
        if(this->valid[i]) {
            walltype[i] = SOLIDVERWALL;
        } else {
            walltype[i] = NSOLIDVERWALL;
        }
    } 

    // 绘制出虚实墙体
    for (int i = 0; i < w; i++) {
        walls.emplace_back(i, 0, walltype[0]); // 上
        walls.emplace_back(i, h - 1, walltype[1]); // 下
    }
    for (int i = 1; i < h - 1; i++) {
        walls.emplace_back(0, i, walltype[2]); // 左
        walls.emplace_back(w - 1, i, walltype[3]);// 右
    }

    return 0;
}

// 0  1  2  3
// 上 下 左 右
// 判断四面墙的虚实(上下左右), direction(0~3)
bool Map::IsValid(int direction) const {
    if(direction > 3 || direction < 0) return true;
    return valid[direction];
}

/* 判断p是否与障碍物重合, 用于判断蛇头是否撞到障碍物, 生成的食物是否与障碍物重合 */
bool Map::InBlocks(const Point& p) const {
    for(auto &point : blocks) {
        if(point == p) {
            return true;
        }
    }
    return false;
}


void Map::Print() const {
    for (auto &wall : walls) {
        wall.Print();
        Sleep(10);  // 动画效果
    }
    for (auto &block : blocks) {
        block.Print();
    }
}