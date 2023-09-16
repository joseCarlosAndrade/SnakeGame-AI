game: game/game.cpp src/neural_network.cpp src/SDLSnakeGameAi.cpp src/snake.cpp include/neural_network.hpp include/SDLSnakeGameAi.hpp include/snake.hpp
	mkdir -p bin/
	g++ -o bin/mainGame game/game.cpp src/*.cpp -lSDL2

run_game: 
	./bin/mainGame