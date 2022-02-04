all: main.cpp
	g++ -std=c++20 -o main include/cmdlib/common.cpp include/cmdlib/logging.cpp Token.cpp Lexer.cpp main.cpp
run: all
	./main
