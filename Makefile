CPP_FLAGS=-ggdb -Wall -Werror -Wextra

all: game/training.cpp neural_network.o SDLSnakeGameAi.o snake.o 
	g++ -o bin/mainGame game/training.cpp bin/*.o -lSDL2 $(CPP_FLAGS)

debug: game/game.cpp neural_network.o SDLSnakeGameAi.o snake.o 
	g++ -o bin/debug game/game.cpp bin/*.o -lSDL2 $(CPP_FLAGS)

neural_network.o: src/neural_network.cpp include/neural_network.hpp
	g++ -c src/neural_network.cpp -o bin/neural_network.o

SDLSnakeGameAi.o: src/SDLSnakeGameAi.cpp include/SDLSnakeGameAi.hpp
	g++ -c src/SDLSnakeGameAi.cpp -o bin/SDLSnakeGameAi.o

snake.o: src/snake.cpp include/snake.hpp
	g++ -c src/snake.cpp -o bin/snake.o

game: game/game.cpp src/neural_network.cpp src/SDLSnakeGameAi.cpp src/snake.cpp include/neural_network.hpp include/SDLSnakeGameAi.hpp include/snake.hpp
	mkdir -p bin/
	g++ -o bin/mainGame game/game.cpp src/*.cpp -lSDL2

game_t: game/game.cpp src/neural_network.cpp src/SDLSnakeGameAi.cpp src/snake.cpp include/neural_network.hpp include/SDLSnakeGameAi.hpp include/snake.hpp
	mkdir -p bin/
	g++ -o bin/mainGame_t game/game.cpp src/*.cpp -lSDL2


run_game: 
	./bin/mainGame
