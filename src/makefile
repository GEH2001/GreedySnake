main: main.o controller.o food.o snake.o map.o point.o utils.o
	g++ main.o controller.o food.o snake.o map.o point.o utils.o -o main -std=c++11
	
main.o: main.cpp
	g++ -c main.cpp -std=c++11

controller.o: controller.cpp
	g++ -c controller.cpp -std=c++11

food.o: food.cpp
	g++ -c food.cpp -std=c++11

snake.o: snake.cpp
	g++ -c snake.cpp -std=c++11

map.o: map.cpp
	g++ -c map.cpp -std=c++11

point.o: point.cpp
	g++ -c point.cpp -std=c++11

utils.o: utils.cpp
	g++ -c utils.cpp -std=c++11

clean:
	del *.o