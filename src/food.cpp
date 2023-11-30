#include "food.h"
#include "snake.h"
#include "map.h"
#include "event.h"

FoodManager::FoodManager(int _num, float _prob[]) {
    num = _num;
    for(int i = 0; i < 3; i++) {
        prob[i] = _prob[i];
    }
}

/* 判断新生成的food是否和已有的重合 */
bool FoodManager::InFoods(const Point &food) const {
    for (auto &f : foods) {
        if (f == food) {
            return true;
        }
    }
    return false;
}

/* 系统随机生成食物 */
void FoodManager::Generate(Snake &snake, Map &map, int _time, std::vector<Event>& _events) {
    int len = foods.size();
    while(len < num) {
        int _value;
        float r = float(rand()) / RAND_MAX;
        PointType type;
        if(r < prob[0]) {
            type = FOOD1;
            _value = 1;
        } else if(r < prob[0] + prob[1]) {
            type = FOOD2;
            _value = 2;
        } else {
            type = FOOD3;
            _value = 3;
        }
        int x = rand() % (Map::width) + 1;
        int y = rand() % (Map::height) + 1;
        Point food(x, y, type);
        if(!snake.InBody(food) && !InFoods(food) && !map.InBlocks(food)) {
            food.Print();
            foods.push_back(food);
            // 增加food event
            Event food_event = Event(_time, F, x, y, _value);
            _events.push_back(food_event);
            len++;
        }
    }
}

/* 根据rec 生成食物*/
void FoodManager::Generate(Snake &snake, Map &map, int x, int y, int value) {
    PointType type;
    if(value == 1) {
        type = FOOD1;
    } else if(value == 2) {
        type = FOOD2;
    } else {
        type = FOOD3;
    }
    Point food(x, y, type);
    if(!snake.InBody(food) && !InFoods(food) && !map.InBlocks(food)) {
        food.Print();
        foods.push_back(food);
    }
}


/* 判断蛇头是否吃到食物 */
int FoodManager::BeEaten(const Point &head) {
    for (auto it = foods.begin(); it != foods.end(); it++) {
        if (*it == head) {
            int score = 0;
            switch (it->GetType()) {
                case FOOD1:
                    score = 1;
                    break;
                case FOOD2:
                    score = 2;
                    break;
                case FOOD3:
                    score = 3;
                    break;
                default:
                    break;
            }
            it->SetType(BLANK);
            it->Print();
            foods.erase(it);
            return score;
        }
    }
    return 0;
}