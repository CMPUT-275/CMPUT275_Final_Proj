FLAGS = -Wall -c
EXECUTABLES = server
FILES = digraph maze_generator server serial_port


server: server.o digraph.o maze_generator.o maze_solver.o unionfind.o serial_port.o
	@g++ server.o digraph.o maze_generator.o maze_solver.o unionfind.o serial_port.o -o server

server.o: server.cpp
	@g++ server.cpp -o server.o $(FLAGS)

digraph.o: digraph.cpp digraph.h wdigraph.h
	@g++ digraph.cpp -o digraph.o $(FLAGS)

maze_generator.o: maze_generator.cpp maze_generator.h heap.h
	@g++ maze_generator.cpp -o maze_generator.o $(FLAGS)

unionfind.o: unionfind.cpp unionfind.h
	@g++ unionfind.cpp -o unionfind.o $(FLAGS)

maze_solver.o: maze_solver.cpp maze_solver.h
	@g++ maze_solver.cpp -o maze_solver.o $(FLAGS)

serial_port.o: serialport.cpp serialport.h
	@g++ serialport.cpp -o serial_port.o $(FLAGS)

clean:
	@rm -f $(FILES) *.o