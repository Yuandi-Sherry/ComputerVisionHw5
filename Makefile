compile: main.cpp Morphing.cpp Matrix.cpp Delaunay.cpp Point.cpp Triangle.cpp Edge.cpp
	g++ -o a.exe main.cpp Morphing.cpp Matrix.cpp Delaunay.cpp Point.cpp Triangle.cpp Edge.cpp -lgdi32 -O3

run: 
	./a.exe

gdb:
	gdb a.exe