#pragma once
#include <string>

struct Config {
    int level;  // 难度等级 1~10
    int seed;   // 随机种子, -1表示以当前时间为种子
    int foodNum;    // 食物数量 1~5
    float foodProb[3];   // 食物生成概率
};

typedef struct Config UConfig;

class Controller {
private:
    std::string configname;  // 游戏配置名
    std::string mapname;    // 地图配置名
    static std::string setting; // 持久化存储用户设置
    UConfig uconfig;
    int ReadSetting();
    int ReadConfig();
    int WriteSetting();
    int DrawSidebar();
    int UpdateScore(int score);
    int DrawMapTable(int rows, int cols, int offset, int blocks[20][20], int valid[4]);
public:
    Controller();
    void Run();
    int MainInterface(); // 主界面
    int GamePlayInterface(); // 游戏界面
    int GamePause();        // 游戏暂停
    int ConfigChooseInterface(); // 配置选择界面
    int ConfigCreateInterface(); // 配置创建界面
    int MapChooseInterface(); // 地图选择界面
    int MapCreateInterface(); // 地图创建界面
};