candy : windows.o score.o candy.o objective.o rectangle.o circle.o Animation.o
	g++ windows.o score.o candy.o objective.o rectangle.o circle.o Animation.o -lfltk -o candy


windows.o : windows.cpp
	g++ -c windows.cpp

score.o : score.cpp
	g++ -c score.cpp

candy.o : candy.cpp
	g++ -c candy.cpp

objective.o : objective.cpp
	g++ -c objective.cpp

rectangle.o : rectangle.cpp
	g++ -c rectangle.cpp

circle.o : circle.cpp
	g++ -c circle.cpp

Animation.o : Animation.cpp
	g++ -c Animation.cpp

canvas.o: canvas.cpp
	g++ -c canvas.cpp
	
clean : 
	rm *.o candy