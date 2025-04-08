.PHONY: run

run: engine
	LD_LIBRARY_PATH=raylib/lib/ ./engine 

engine: main.cpp
	g++ main.cpp -Iraylib/include -Lraylib/lib -lraylib -lm -ldl -lpthread -lGL -o engine

