all: main.cpp
	mkdir -p build
	g++ -Iinclude -std=c++20 -o ./build/main include/cmdlib/common.cpp include/cmdlib/logging.cpp Token.cpp Lexer.cpp main.cpp
run: all
	./build/main
