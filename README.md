# Greedy Snake

清华大学2023程序设计基础课程大作业

## 小组人员信息

郭恩惠 2020010548 软件12

肖锦松 2020010563 电02

## 源码格式
UTF-8

## 项目运行环境
- Windows 11 22H2
- MinGW-W64-builds-4.3.5
- C++11

## 编译运行
```bash
$ cd src
$ make
```
打开main.exe, 注意不要通过命令行方式`./main.exe`打开

## 功能实现逻辑

### 代码框架

```
Greedy Snake:.
│   .gitignore
│   README.md
|
│
├───doc
│       作业需求文档.pdf
│
└───src
    │   controller.cpp
    │   controller.h
    │   event.cpp
    │   event.h
    │   food.cpp
    │   food.h
    │   main.cpp
    │   main.exe
    │   makefile
    │   map.cpp
    │   map.h
    │   point.cpp
    │   point.h
    │   setting.txt
    │   snake.cpp
    │   snake.h
    │   utils.cpp
    │   utils.h
    │
    ├───config
    │       default.config
    │       medium.config
    │
    ├───maps
    │       default.map
    │       example.map
    │
    └───record
            record_1.rec
```



### 基础游戏环境

#### 主界面

`Controller::MainInterface()`用于维护主界面，主要功能为给出游戏提示以及处理用户输入，包括非法输入。得到用户输入后返回选项给`Controller::Run()`进行下一步操作

主界面所有跳转逻辑如下

- g：开始游戏，进入游戏界面。
- q：退出游戏
- i：新建自定义配置。可用：输入新配置文件名称，如果是q 退出配置返回主界面，输入配置内容
- u：进行配置载入。可用：输入新配置文件名称，q 退出配置载入过程
- n：进行地图新建。可用：输入新地图文件名称，如果是q 退出配置返回主界面，输入配置内容
- m：进行地图载入。可用：输入新地图文件名称，q 退出地图载入过程
- r：进行记录重放。可用：输入记录文件名称，重放过程 q 退出重放回到主界面
- 非法输入：不处理

游戏界面

- WASD：移动蛇

- space：进入暂停界面。可用：q 退出游戏返回主界面，space 继续游戏
- 游戏结束后
  - b：保存本局游戏记录。可用：输入记录文件名称
  - 其他输入：回到主界面



#### 游戏界面绘制

游戏界面绘制依赖于`class Point`，其维护了点坐标和类型，类型包括：空白、虚实墙体、障碍物、蛇头、蛇身、食物等等，后续维护每个类型为`vector<Point>`，蛇为`deque<Point>`，无论是添加还是打印都非常方便

```C++
class Point {
private:
    int x, y;
    PointType type;
public:
    ...
}
```

- 游戏地图：用`class Map`实现，`Loadmap`用于读取当前地图配置，类中维护了包括地图大小、四面墙的虚实、障碍物的数量和坐标等信息。其中`walls`和`blocks`均为`vector<Point>`，容易添加和打印

- 蛇：用`class Snake`实现，蛇的身体部分利用双端队列`deque<Point>`维护。蛇**增长**时，只需要在列表前端增加身体并且将`head`重新定位到列表第一个元素就行；蛇**移动**时，则是先增长，然后再蛇尾丢弃，类型改成空白地图

- 生成食物：用`class FoodManager`实现，读取自定义配置文件后可以获得生成的食物数量和三种概率，用随机数的方法可以很快生成，打印食物还需要判断其与食物、蛇身、墙体是否重合，`int FoodManager::BeEaten(const Point &head)`判断蛇头是否吃到食物，并且返回应该增加的得分。同样地，食物用`vector<Point>`维护。

- 显示分数配置：`Controller::DrawSidebar()`用于在游戏界面右侧展示出当前的分数、游戏配置名称和地图名称，更新分数或是配置显示可以采用输出覆盖（具体是利用`utils/SetCursor`将光标移动到相同位置后输出）



#### 其他周边功能

`class Controller`中的`ReadSetting()`和`WriteSetting()`用于读写配置文件，`setting`用于储存总配置文件名，其中包含`configname`和`mapname`.

##### 自定义配置功能

自定义配置对应`struct Config`

`class Controller`中维护了成员`UConfig uconfig`用于存储自定义配置，`ReadConfig`用于读取自定义配置，存储配置包括：难度等级、随机种子、食物数量、食物生成概率

##### 地图配置功能

见游戏界面绘制

##### 游戏记录回放功能

新增事件类，用于记录两类事件，食物生成和蛇活动

```C++
class Event{
private:
    int time;
    int x, y; // 食物坐标、蛇头坐标
    int value; // 食物经验
    EventType type;
public:
    ...
}
```

游戏过程用`vector<Event>`记录所有事件，等待用户选择是否记录。回放时读取记录文件，进行强制操作即可



## 小组分工情况

郭恩惠：基本的代码框架，2.1-3.2代码实现

肖锦松：修改已实现的部分代码，主要是增加接口，重载部分函数，3.3代码实现，报告，视频











