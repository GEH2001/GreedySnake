#include "food.h"
#include "snake.h"
#include "map.h"

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

/* 生成食物 */
void FoodManager::Generate(Snake &snake, Map &map) {
    int len = foods.size();
    while(len < num) {
        float r = float(rand()) / RAND_MAX;
        PointType type;
        if(r < prob[0]) {
            type = FOOD1;
        } else if(r < prob[0] + prob[1]) {
            type = FOOD2;
        } else {
            type = FOOD3;
        }
        int x = rand() % (Map::width) + 1;
        int y = rand() % (Map::height) + 1;
        Point food(x, y, type);
        if(!snake.InBody(food) && !InFoods(food) && !map.InBlocks(food)) {
            food.Print();
            foods.push_back(food);
            len++;
        }
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