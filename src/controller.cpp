#include "controller.h"
#include "map.h"
#include "snake.h"
#include "food.h"
#include "utils.h"
#include "point.h"
#include <conio.h>
#include <ctime>
#include <iostream>
#include <fstream>
#include <regex>

std::string Controller::setting = "setting.txt";

Controller::Controller() {
    play_time = 0;
    ReadSetting();
    ReadConfig();
}

int Controller::GetTime() const {
    return play_time;
}
void Controller::AddTime() {
    play_time ++;
}

// 读取setting.txt, 载入用户上次的设置
int Controller::ReadSetting() {
    // TODO: 判断config和map是否存在, 如果不存在, 则设置为默认值
    std::ifstream fin(setting);
    if(!fin.is_open()) {
        std::cout << "setting.txt not found!" << std::endl;
        system("pause");
        exit(0);
    }
    fin >> configname >> mapname;
    fin.close();
    return 0;
}

/* 持久化存储setting.txt */
int Controller::WriteSetting() {
    std::ofstream fout(setting);
    if(!fout.is_open()) {
        std::cout << "Open " << setting << " error" << std::endl;
        return -1;
    }
    fout << configname << std::endl;
    fout << mapname << std::endl;
    fout.close();

    return 0;
}

// 读取config文件
int Controller::ReadConfig() {
    std::string configPath = "config/" + configname + ".config";
    std::ifstream fin(configPath);
    if(!fin.is_open()) {
        std::cout << configPath << " not found!" << std::endl;
        system("pause");
        exit(0);
    }

    fin >> uconfig.level >> uconfig.seed >> uconfig.foodNum;
    for(int i = 0; i < 3; i++) {
        fin >> uconfig.foodProb[i];
    }

    fin.close();

    return 0;
}

void Controller::Run() {
    while(true) {
        // 主界面，用户输入选择功能
        int choice = MainInterface();
        switch (choice)
        {
        case 0:
            exit(0);
            break;
        case 1:
            GamePlayInterface();
            break;
        case 2:
            ConfigChooseInterface();
            break;
        case 3:
            ConfigCreateInterface();
            break;
        case 4:
            MapChooseInterface();
            break;
        case 5:
            MapCreateInterface();
            break;
        case 6:
            RecordPlayInterface();
            break;
        default:
            break;
        }
    }
}

/* 主界面：一级界面*/
int Controller::MainInterface() {
    system("cls");
    std::cin.sync(); // 清空输入缓冲区
    int beginX = 30, beginY = 5;
    SetCursor(beginX, beginY++);
    std::cout << "Welcome to Snake Game!";
    SetCursor(beginX, beginY + 1);
    std::cout << "Exit(q)";
    SetCursor(beginX, beginY + 2);
    std::cout << "Play Game(g)";
    SetCursor(beginX, beginY + 3);
    std::cout << "Choose Config(u)";
    SetCursor(beginX, beginY + 4);
    std::cout << "Create Config(i)";
    SetCursor(beginX, beginY + 5);
    std::cout << "Choose Map(m)";
    SetCursor(beginX, beginY + 6);
    std::cout << "Create Map(n)";
    SetCursor(beginX, beginY + 7);
    std::cout << "Play Record(r)";
    char ch;
    int choice;
    while(ch = getch()) {
        switch (ch)
        {
        case 'q':
        case 'Q':
            choice = 0;
            break;
        case 'g':
        case 'G':
            choice = 1;
            break;
        case 'u':
        case 'U':
            choice = 2;
            break;
        case 'i':
        case 'I':
            choice = 3;
            break;
        case 'm':
        case 'M':
            choice = 4;
            break;
        case 'n':
        case 'N':
            choice = 5;
            break;
        case 'r':
        case 'R':
            choice = 6;
            break;
        default:
            choice = -1;
            break;
        }
        if(choice != -1) { // 无效输入不会退出while循环
            break;
        }
    }
    return choice;
}

/* 游戏右边显示current score, config, map */
// 缺省值是类里维护的,可以自己传入,因为有回放
int Controller::DrawSidebar() {
    int beginX = Map::width + 10, beginY = 1;
    SetCursor(beginX, beginY);
    std::cout << "current score:  0";
    SetCursor(beginX, beginY + 1);
    std::cout << "config:         " << this->configname;
    SetCursor(beginX, beginY + 2);
    std::cout << "map:            " << this->mapname;

    SetCursor(beginX, beginY + 5);
    std::cout << "Move(W A S D)";
    SetCursor(beginX, beginY + 6);
    std::cout << "Pause(Space)";

    return 0;
}

int Controller::DrawSidebar(std::string _configname, std::string _mapname) {
    int beginX = Map::width + 10, beginY = 1;
    SetCursor(beginX, beginY);
    std::cout << "current score:  0";
    SetCursor(beginX, beginY + 1);
    std::cout << "config:         " << _configname;
    SetCursor(beginX, beginY + 2);
    std::cout << "map:            " << _mapname;

    SetCursor(beginX, beginY + 5);
    std::cout << "Move(W A S D)";
    SetCursor(beginX, beginY + 6);
    std::cout << "Pause(Space)";

    return 0;
}

/* 更新得分 */
int Controller::UpdateScore(int score) {
    int beginX = Map::width + 10, beginY = 1;
    SetCursor(beginX + 16, beginY); // 覆盖掉
    std::cout << score;

    return 0;
}

/* 游戏界面：二级界面, 操控蛇的移动 */
int Controller::GamePlayInterface() {
    system("cls");
    std::cin.sync(); // 清空输入缓冲区

    // 时间种子
    if(this->uconfig.seed == -1) {
        srand((unsigned int)time(NULL));
    } else {
        srand((unsigned int)uconfig.seed);
    }

    int score = 0;  // 游戏得分

    // Map map;
    Map map(this->mapname);
    map.Print();

    DrawSidebar();  // 侧边栏

    Snake snake;
    snake.Print();
    FoodManager foodManager(uconfig.foodNum, uconfig.foodProb);
    // 这里要修改代码,生成食物要有所记录,直接在该函数里面记录了
    foodManager.Generate(snake, map, this->play_time, this->events); 

    while(true) {
        // if(snake.WaitKey(int(1000 / uconfig.level)) == -1) { // 暂停
        //     int status = GamePause();
        //     if(status == 0) { // 退出游戏
        //         return 0;
        //     }
        // }

        int user_op = snake.WaitKey(int(1000 / uconfig.level));

        if (user_op == -1) { // 暂停
            int status = GamePause();
            if(status == 0) { // 退出游戏
                return 0;
            }
        }

        // begin record
        // 用户操作记录格式
        // 时间 + 操作方向 + 蛇头X + 蛇头Y + 目前得分
        EventType user_event_type;
        switch (user_op)
        {
        case UP:
            user_event_type = W;
            break;
        case DOWN:
            user_event_type = S;
            break;
        case LEFT:
            user_event_type = A;
            break;
        case RIGHT:
            user_event_type = D;
            break;
        default:
            break;
        }
        Event user_event = Event(this->play_time, user_event_type, snake.GetHead().GetX(), \
                                    snake.GetHead().GetY(), score); 
        events.emplace_back(user_event);

        // end record

        if(snake.IsHitMap(map) || snake.IsHitSelf()) {
            snake.Die();
            break; // 移动之前判断是否 即将死亡
        }

        int foodscore = foodManager.BeEaten(snake.GetHead());

        if(foodscore) {
            score += foodscore;
            UpdateScore(score);
            snake.Grow();
            foodManager.Generate(snake, map, this->play_time, this->events);
            continue;
        }
        snake.Move();
        this->AddTime();
    }
    // 蛇死亡, 退出循环

    SetCursor(0, Map::height + 2);
    std::cout << "Game Over!" << std::endl;
    std::cout << "Press b to record it!" << std::endl;
    std::cout << "Others to back!" << std::endl;

    char ch = getch();
    if (ch == 'b') {
        // record
        RecordCreateInterface();
    }
    return 0;
}

/* 暂停界面 */
int Controller::GamePause() {
    char ch;
    int ret;

    // 暂停提示
    int beginX = Map::width + 10, beginY = 10;
    SetCursor(beginX, beginY);
    SetColor(0x03);
    std::cout << "Game Pause!";
    SetColor(0x07);
    SetCursor(beginX, beginY + 1);
    std::cout << "Continue(Space)";
    SetCursor(beginX, beginY + 2);
    std::cout << "Exit(q)";

    while(ch = getch()) {
        switch (ch)
        {
        case 32:    // 空格键 继续游戏
            ret = 1;
            break;
        case 'q':
        case 'Q':
            ret = 0;  // 退出游戏
            break;
        default:
            ret = -1;
            break;
        }
        if(ret != -1) { // 输入有效则退出循环
            break;
        }
    }

    // 清除暂停提示
    SetCursor(beginX, beginY);
    std::cout << "            ";
    SetCursor(beginX, beginY + 1);
    std::cout << "                ";
    SetCursor(beginX, beginY + 2);
    std::cout << "                ";

    return ret;
}

/* 配置选择界面：二级界面 */
int Controller::ConfigChooseInterface() {
    system("cls");
    std::cin.sync(); // 清空输入缓冲区
    std::cout << "Choose Config" << std::endl << std::endl;
    std::cout << "Current config: " << this->configname << std::endl << std::endl;
    
    SetColor(0x03);
    std::cout << "Input <q/Q> to exit..." << std::endl << std::endl;
    SetColor(0x07);

    std::string name;
    std::string filepath;
    while(true) {
        std::cout << "New config: ";
        SetColor(0x02);
        std::getline(std::cin, name);
        SetColor(0x07);

        if(name == "q" || name == "Q") { // 退出
            return -1;
        }

        filepath = "config/" + name + ".config";
        std::ifstream f(filepath);
        if(!f) {
            std::cout << name << " not exists. Input again." << std::endl; 
            continue;
        }
        break;
    }

    this->configname = name;
    ReadConfig();
    WriteSetting();

    std::cout << "Load new config successfully." << std::endl; 
    system("pause");
    return 0;
}

/* 配置创建界面：二级界面 */
int Controller::ConfigCreateInterface() {
    system("cls");

    std::cin.sync(); //清空输入缓冲区

    std::cout << "New Config" << std::endl << std::endl;

    std::cout << "Note! New name must be: ";
    SetColor(0x06);
    std::cout << "[0-9a-zA-Z_]{1, 10}" << std::endl;
    SetColor(0x07);

    SetColor(0x03);
    std::cout << "Input <q/Q> to exit..." << std::endl;
    SetColor(0x07);

    std::string name;
    std::string filepath;
    std::regex pattern("[a-zA-Z0-9_]{1,10}");
    // 配置文件名
    while (true) {        
        std::cout << "New config name: ";
        
        SetColor(0x02);
        // std::cin >> name; // BUG: 用户键入a cd然后回车，那么只能读取到a, 缓冲区内还有" cd"
        std::getline(std::cin, name);
        SetColor(0x07);

        if(name == "q" || name == "Q") {   // 输入单独的"q"直接退出
            return 0;
        }

        if(!std::regex_match(name, pattern)) {
            std::cout << "Invalid! Input again." << std::endl;
            continue;
        }
        // 判断文件是否已经存在
        filepath = "config/" + name + ".config";
        std::ifstream file(filepath);
        if(file) {
            file.close();
            std::cout << "Name already exist. Input again." << std::endl;
            continue;
        }
        
        break;
    }
    
    std::cout << std::endl;

    int level, seed, foodnum;
    float prob[3];
    // 游戏难度
    while(true) {
        std::cout << "Level(1~10): ";
        SetColor(0x02);
        std::cin >> level;
        SetColor(0x07);
        if(level < 1 || level > 10) {
            std::cout << "Invalid!" << std::endl;
            continue;
        } else {
            break;
        }
    }
    std::cout << std::endl;
    // 随机种子
    while(true) {
        std::cout << "Seed(>= -1): ";
        SetColor(0x02);
        std::cin >> seed;
        SetColor(0x07);
        if(seed < -1) {
            std::cout << "Invalid!" << std::endl;
            continue;
        } else {
            break;
        }
    }
    std::cout << std::endl;
    // 食物数量
    while(true) {
        std::cout << "Food Num(1~5): ";
        SetColor(0x02);
        std::cin >> foodnum;
        SetColor(0x07);
        if(foodnum < 1 || foodnum > 5) {
            std::cout << "Invalid!" << std::endl;
            continue;
        } else {
            break;
        }
    }
    std::cout << std::endl;
    // 生成概率
    while(true) {
        std::cout << "Probability(3 floating numbers, sum is 1): ";
        SetColor(0x02);
        std::cin >> prob[0] >> prob[1] >> prob[2];
        SetColor(0x07);
        
        if(prob[0] < 0 || prob[1] < 0 || prob[2] < 0) { // 必须为正
            std::cout << "Invalid!" << std::endl;
            continue;
        }
        if(abs(1000 * prob[0] + 1000 * prob[1] + 1000 * prob[2] - 1000) > 1) { // abs返回值为int，可以考虑用fabs
            std::cout << "Invalid!" << std::endl;
            continue;
        } else {
            break;
        }
    }

    // 写入配置文件
    std::ofstream fout(filepath);
    if(!fout.is_open()) {
        std::cout << "Error creating file: " << filepath << std::endl;
        std::cout << "About to exit..." << std::endl;
        system("pause");
        return -1;
    }

    fout << level << std::endl;
    fout << seed << std::endl;
    fout << foodnum << std::endl;
    fout << prob[0] << " " <<  prob[1] << " " << prob[2] << std::endl;
    fout.close();

    std::cout << std::endl;
    std::cout << "New config successfully." << std::endl;
    system("pause");

    return 0;
}

/* 地图选择界面：二级界面 */
int Controller::MapChooseInterface() {
    system("cls");
    std::cin.sync(); // 清空输入缓冲区
    std::cout << "Choose map" << std::endl << std::endl;
    std::cout << "Current map: " << this->mapname << std::endl << std::endl;
    
    SetColor(0x03);
    std::cout << "Input <q/Q> to exit..." << std::endl << std::endl;
    SetColor(0x07);

    std::string name;
    std::string filepath;
    while(true) {
        std::cout << "New map: ";
        SetColor(0x02);
        std::getline(std::cin, name);
        SetColor(0x07);

        if(name == "q" || name == "Q") { // 退出
            return -1;
        }

        filepath = "maps/" + name + ".map";
        std::ifstream f(filepath);
        if(!f) {
            std::cout << name << " not exists. Input again." << std::endl; 
            continue;
        }
        break;
    }

    this->mapname = name;
    WriteSetting();

    std::cout << "Load new map successfully." << std::endl; 
    system("pause");
    return 0; 
}

/* CreateMap需要: 绘制地图表格, offset是左边距, blocks是障碍物, valid是墙体虚实 */
int Controller::DrawMapTable(int rows, int cols, int offset, int blocks[20][20], int valid[4]) {
    std::string red = "\033[1;31m";
    std::string green = "\033[1;32m";
    std::string blue = "\033[1;34m";
    std::string reset = "\033[0m";
    
    std::string color[4];
    for(int i = 0; i < 4; i++) {
        color[i] = valid[i] ? red : green;
    }

    // 顶部边界
    SetCursor(offset, 0);
    for(int i = 0; i < cols; i++) {
        std::cout << color[0] << "+---";
    }
    std::cout << "+" << reset << std::endl;

    // 表格内容, 内容占3格
    for(int y = 0; y < rows; y++) {
        // 行内容
        SetCursor(offset, 2 * y + 1);
        if(blocks[0][y]) {  // 左
                std::cout << color[2] + "| " + reset << blue + "O" + reset << " ";
        } else {
                std::cout << color[2] +  "|   " + reset;
        }
        for(int x = 1; x < cols; x++) { // 从1开始，因为上面单独处理了第0个
            if(blocks[x][y]) {
                std::cout << "| " << blue + "O" + reset << " ";
            } else {
                std::cout << "|   ";
            }
        }
        std::cout << color[3] + "|" + reset << std::endl; // 右
        // 行之间的边界
        SetCursor(offset, 2 * y + 2);
        
        if(y == rows - 1) { // 底部边界单独处理
            for(int x = 0; x < cols; x++) {
                std::cout << color[1] + "+---";
            }
            std::cout << "+" << reset << std::endl;
            break;
        }
        
        std::cout << color[2] + "+" + reset << "---";  // 左
        for(int x = 1; x < cols; x++) { // 从1开始，因为上面单独处理了第0个
            std::cout << "+---";
        }
        std::cout << color[3] +  "+" + reset << std::endl; // 右
    }

    return 0;
}

/* 地图创建界面：二级界面 */
int Controller::MapCreateInterface() {

    system("cls");
    std::cin.sync(); // 清空输入缓冲区
    std::cout << "New map" << std::endl << std::endl;

    std::string name;
    std::string filepath;
    std::regex pattern("[a-zA-Z0-9_]{1,10}");

    // 地图文件名
    while (true) {   
        std::cout << "Note! New name must be: ";
        SetColor(0x06);
        std::cout << "[0-9a-zA-Z_]{1, 10}" << std::endl;
        SetColor(0x07);

        SetColor(0x03);
        std::cout << "Input <q/Q> to exit..." << std::endl;
        SetColor(0x07);     

        std::cout << "New map name: ";
        
        SetColor(0x02);
        std::getline(std::cin, name);
        SetColor(0x07);

        if(name == "q" || name == "Q") {   // 输入单独的"q"直接退出
            return 0;
        }

        if(!std::regex_match(name, pattern)) {
            std::cout << "\nInvalid! Input again.\n" << std::endl;
            continue;
        }
        // 判断文件是否已经存在
        filepath = "maps/" + name + ".map";
        std::ifstream file(filepath);
        if(file) {
            file.close();
            std::cout << "\nName already exist. Input again.\n" << std::endl;
            continue;
        }
        
        break;
    }

    std::cout << std::endl;

    // 地图大小
    int width, height;
    while(true) {
        std::cout << "width and height(8~20): ";
        SetColor(0x02);
        std::cin >> width >> height;
        SetColor(0x07);

        if(width < 8 || width > 20 || height < 8 || height > 20) {
            std::cout << "\nInvalid! Input again.\n" << std::endl;
            continue;
        }
        break;
    }

    // 绘制地图表格
    system("cls");
    int blocks[20][20] = {0};   // 障碍物
    int valid[4] = {1, 1, 1, 1}; // 墙体虚实(上下左右), 1为实, 0为虚

    DrawMapTable(height, width, 40, blocks, valid);
    
    SetCursor(0, 0); // 光标重定位至左上角
    int row = 0; // 当前输出行数, 因为绘制地图元素需要改变坐标, 需要记录当前打印的信息
    char cmd;
    while(true) {
        int x, y, v;
        std::cout << "> ";
        std::cin >> cmd;
        switch(cmd) { // switch 开始
            case 'q':
            case 'Q':
                break;
            case 'f':
            case 'F':
                break;
            case 'o':   // 新建障碍物
            case 'O':
                std::cin >> x >> y;
                if(x < 0 || x > width - 1 || y < 0 || y > height - 1) {
                    std::cout << "X or Y out of map size." << std::endl;
                    row++;
                    break;
                }
                blocks[x][y] = 1;
                DrawMapTable(height, width, 40, blocks, valid);
                break;
            case 'p':
            case 'P':
                std::cin >> x >> y;
                if(x < 0 || x > width - 1 || y < 0 || y > height - 1) {
                    std::cout << "X or Y out of map size." << std::endl;
                    row++;
                    break;
                }
                if(blocks[x][y] != 1) {
                    std::cout << "No blocks to delete." << std::endl;
                    row++;
                    break;
                }
                blocks[x][y] = 0;
                DrawMapTable(height, width, 40, blocks, valid);
                break;
            case 'w':
            case 'W':
                std::cin >> v;
                valid[0] = v;
                DrawMapTable(height, width, 40, blocks, valid);
                break;
            case 's':
            case 'S':
                std::cin >> v;
                valid[1] = v;
                DrawMapTable(height, width, 40, blocks, valid);
                break;
            case 'a':
            case 'A':
                std::cin >> v;
                valid[2] = v;
                DrawMapTable(height, width, 40, blocks, valid);
                break;
            case 'd':
            case 'D':
                std::cin >> v;
                valid[3] = v;
                DrawMapTable(height, width, 40, blocks, valid);
                break;
        } // switch 结尾
        
        SetCursor(0, ++row); // 将光标重定位到最新行
        SetColor(0x07);
        if(cmd == 'q' || cmd == 'Q' || cmd == 'f' || cmd == 'F') {  // 退出循环
            break;
        }
    }

    if(cmd == 'q' || cmd == 'Q') { // 放弃地图创建
        std::cout << "About to leave without creating map." << std::endl;
        system("pause");
        return -1;
    }

    // 写入地图文件
    std::ofstream fout(filepath);
    if(!fout.is_open()) {
        std::cout << "Open " << filepath << " error." << std::endl;
        system("pause");
        return -1;
    }

    fout << width << " " << height << std::endl;
    for(int i = 0; i < 4; i++) {
        fout << valid[i] << " ";
    }
    fout << std::endl;

    int numBlocks = 0;
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            numBlocks += blocks[i][j];
        }
    }
    fout << numBlocks << std::endl;
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            if(blocks[i][j]) {
                fout << i << " " << j << std::endl;
            }
        }
    }

    fout.close();

    std::cout << "Create new map successfully." << std::endl;

    std::cout << std::endl;
    system("pause");
    return 0;
}

/* 回放写入文件 */
int Controller::RecordCreateInterface() {
    system("cls");
    std::cin.sync(); // 清空输入缓冲区
    std::cout << "New record" << std::endl << std::endl;

    std::string name;
    std::string filepath;
    std::regex pattern("[a-zA-Z0-9_]{1,10}");

    // 回放文件名
    while (true) {   
        std::cout << "Note! New name must be: ";
        SetColor(0x06);
        std::cout << "[0-9a-zA-Z_]{1, 10}" << std::endl;
        SetColor(0x07);

        SetColor(0x03);
        std::cout << "Input <q/Q> to exit..." << std::endl;
        SetColor(0x07);     

        std::cout << "New record name: ";
        
        SetColor(0x02);
        std::getline(std::cin, name);
        SetColor(0x07);

        if(name == "q" || name == "Q") {   // 输入单独的"q"直接退出
            return 0;
        }

        if(!std::regex_match(name, pattern)) {
            std::cout << "\nInvalid! Input again.\n" << std::endl;
            continue;
        }
        // 判断文件是否已经存在
        filepath = "record/" + name + ".rec";
        std::ifstream file(filepath);
        if(file) {
            file.close();
            std::cout << "\nName already exist. Input again.\n" << std::endl;
            continue;
        }
        
        break;
    }
    std::cout << std::endl;

    // 配置记录,地图记录
    // 写入配置文件
    std::ofstream fout(filepath);
    if(!fout.is_open()) {
        std::cout << "Error creating file: " << filepath << std::endl;
        std::cout << "About to exit..." << std::endl;
        system("pause");
        return -1;
    }
    // 判断配置和地图还在不在
    filepath = "config/" + this->configname + ".config";
    std::ifstream f1(filepath);
    if(!f1) {
        std::cout << name << " Config does not exists..." << std::endl; 
    }
    else f1.close();
    ReadConfig();

    filepath = "maps/" + this->mapname + ".config";
    std::ifstream f2(filepath);
    if(!f2) {
        std::cout << name << " Map does not exists..." << std::endl; 
    }
    else f2.close();

    fout << this->configname << std::endl;
    fout << this->mapname << std::endl;

    // 操作记录
    for (auto e: events) {
        fout << e.GetTime() << " " << e.GetType() << " " << e.GetX() << " " 
        << e.GetY() << " " << e.GetValue() << std::endl;
    }

    fout.close();

    std::cout << std::endl;
    std::cout << "New record successfully." << std::endl;
    system("pause");
    return 0;
}

/* 回放重播 */
int Controller::RecordPlayInterface() {
    // 选择并加载回放
    system("cls");
    std::cin.sync(); // 清空输入缓冲区
    std::cout << "Choose record" << std::endl << std::endl;
    SetColor(0x03);
    std::cout << "Input <q/Q> to exit..." << std::endl << std::endl;
    SetColor(0x07);

    std::string name;
    std::string filepath;
    while(true) {
        std::cout << "New record: ";
        SetColor(0x02);
        std::getline(std::cin, name);
        SetColor(0x07);

        if(name == "q" || name == "Q") { // 退出
            return -1;
        }

        filepath = "record/" + name + ".rec";
        std::ifstream f(filepath);
        if(!f) {
            std::cout << name << " not exists. Input again." << std::endl; 
            continue;
        }
        break;
    }

    this->recname = name;
    std::cout << "Load new record successfully." << std::endl; 

    std::string rec_configname;
    std::string rec_mapname;

    filepath = "record/" + this->recname + ".rec";
    std::ifstream f(filepath);
    if(f.is_open()) {
        std::getline(f, rec_configname);
        std::getline(f, rec_mapname);
    }

    // 正式开始回放 ****************** rec *********************

    system("cls");
    std::cin.sync(); // 清空输入缓冲区

    int score = 0;  // 游戏得分

    // Map map;
    Map map(rec_mapname);
    map.Print();
    DrawSidebar(rec_configname, rec_mapname);  // 侧边栏

    Snake snake;
    snake.Print();

    // 这里食物生成不需要记录了,而是传入坐标自动打印
    FoodManager foodManager(uconfig.foodNum, uconfig.foodProb); 
    if(!f.is_open()) {
        std::cout << "file not exists" << std::endl;
    }
    else{
        std::string line;
        int rec_time = 0;
        // 逐行读取文件内容
        while (std::getline(f, line)) {
            std::istringstream iss(line);
            // 每行有5个整数
            int time, type, x, y, value;
            iss >> time >> type >> x >> y >> value;
            
            // 时间不同步,才让蛇爬,时间推移
            // 同一时间可能出现多个操作
            while (time > rec_time) {
                // 不需要判断死亡
                int foodscore = foodManager.BeEaten(snake.GetHead());

                if(foodscore) {
                    score += foodscore;
                    UpdateScore(score);
                    snake.Grow();
                    rec_time++;
                    continue;
                }
                // Grow 也算时间 就 不 Move 了
                snake.Move();
                rec_time++;
            }

            // 时间同步才改变操作
            switch (type)
            {
            case F:
                foodManager.Generate(snake, map, x, y, value); 
                break;
            case W:
                snake.Wait(int(1000 / uconfig.level));
                snake.RecDirection(UP);
                break;
            case A:
                snake.Wait(int(1000 / uconfig.level));
                snake.RecDirection(LEFT);
                break;
            case S:
                snake.Wait(int(1000 / uconfig.level));
                snake.RecDirection(DOWN);
                break;
            case D:
                snake.Wait(int(1000 / uconfig.level));
                snake.RecDirection(RIGHT);
                break;

            default:
                break;
            }
        }
        // 蛇死了
    }
    system("pause");
    return 0;
}