CXX = g++
CXXFLAGS = -Wall -g

mytest: rqueue.o mytest.cpp
	$(CXX) $(CXXFLAGS) rqueue.o mytest.cpp -o mytest

rqueue.o: rqueue.cpp rqueue.h
	$(CXX) $(CXXFLAGS) -c rqueue.cpp
run:
	./mytest

val:
	valgrind --leak-check=full ./mytest

vally:
	valgrind --track-origins=yes ./mytest

clean:
	rm *~
	rm *.o
