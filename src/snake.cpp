#include "snake.h"

#include <conio.h>
#include <ctime>

Snake::Snake() {
    int x_center = Map::width / 2 + 1;
    int y_center = Map::height / 2 + 1;

    body.emplace_back(x_center, y_center, HEAD);
    for(int i = 1; i < 4; i++) {
        body.emplace_back(x_center - i, y_center, BODY);
    }
    direction = RIGHT;
}

void Snake::Print() const {
    for(auto &point : body) {
        point.Print();
    }
}

// up, down, left, right
const int dx[] = {0, 0, -1, 1};
const int dy[] = {-1, 1, 0, 0};

/* 增长: 蛇头向前延申, 蛇尾不动 */
void Snake::Grow() {   
    // 增加一个新的蛇头, 原来的蛇头变成蛇身
    Point& head = body.front();
    head.SetType(BODY);
    // body.emplace_front(head.GetX() + dx[direction], head.GetY() + dy[direction], HEAD);
    // 穿墙效果
    // int x = head.GetX() + dx[direction];
    // int y = head.GetY() + dy[direction];
    // // 正常活动范围内, 坐标x取值为[1, width]
    // if(x == Map::width + 1) { // width+1是右边墙的位置, 如果新蛇头与右墙重合
    //     x = 1;
    // } else if(x == 0) { // 0是左墙位置
    //     x = Map::width;
    // }

    // if(y == Map::height + 1) { // height+1是下边墙位置
    //     y = 1;
    // } else if(y == 0) {
    //     y = Map::height;
    // }

    // 上面的太冗余了
    int x = (head.GetX() + dx[direction] - 1 + Map::width) % Map::width + 1; // [1, width]
    int y = (head.GetY() + dy[direction] - 1 + Map::height) % Map::height + 1; // [1, height]

    body.emplace_front(x, y, HEAD);
    head.Print();
    body.front().Print();
}

/* 移动 */
void Snake::Move() {
    // 蛇头加1, 蛇尾减1
    Grow();
    Point& tail = body.back();
    tail.SetType(BLANK);
    tail.Print();
    body.pop_back();
}


/* 方向控制W A S D; 空格暂停 */
int Snake::WaitKey(int interval) {
    char ch;
    Direction newDirection = direction;

    int start = clock();
    int end = start;

    while(end - start < interval) {
        if(kbhit()) {
            ch = getch();
            if(ch == 32) {  // 空格键暂停
                return -1;
            }
            if((ch == 'w' || ch == 'W') && direction != DOWN) {
                newDirection = UP;
            } else if((ch == 's' || ch == 'S') && direction != UP) {
                newDirection = DOWN;
            } else if((ch == 'a' || ch == 'A') && direction != RIGHT) {
                newDirection = LEFT;
            } else if((ch == 'd' || ch == 'D') && direction != LEFT) {
                newDirection = RIGHT;
            }
        }
        end = clock();
    }

    direction = newDirection; // 更新方向
    return 0;
}


/* 判断点p是否与蛇身重合, 用于生成食物的时候判断 */
bool Snake::InBody(Point &p) const {
    for(auto &point : body) {
        if(point == p) {
            return true;
        }
    }
    return false;
}

Point Snake::GetHead() const {
    return body.front();
}

/* 判断下一步是否即将 撞到墙壁或者障碍物 */
bool Snake::IsHitMap(const Map &map) const {
    Point head = body.front();
    int x = head.GetX() + dx[direction];
    int y = head.GetY() + dy[direction];
    
    // if(x == 0 && map.IsValid(LEFT)) { // 到达左边界, 左墙为实
    //     return true;
    // }

    // 上面的写法要重复4次，太冗余了，因为变量direction刚好存了当前的方向, 而下面的4个条件和direction刚好一一匹配
    // 未穿墙前判断是否碰壁
    if(x == 0 || x == Map::width + 1 || y == 0 || y == Map::height + 1) {
        if(map.IsValid(this->direction)) {
            return true;
        }
    }

    // 穿墙之后判断是否碰到障碍物
    x = (x - 1 + Map::width) % Map::width + 1;
    y = (y - 1 + Map::height) % Map::height + 1;
    if(map.InBlocks(Point(x, y, HEAD))) {
        return true;
    }

    return false;
}

/* 下一步: 咬到自己 */
bool Snake::IsHitSelf() const {
    Point head = body.front();
    int x = head.GetX() + dx[direction];
    int y = head.GetY() + dy[direction];
    Point next(x, y, HEAD);
    for(auto it = body.begin() + 1; it != body.end(); it++) {
        if(next == *it) {
            return true;
        }
    }
    return false;
}

void Snake::Die() {
    Point& head = body.front();
    head.SetType(DEADHEAD);
    head.Print();
    for(auto it = body.begin() + 1; it != body.end(); it++) {
        it->SetType(DEADBODY);
        it->Print();
    }
}