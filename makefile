candy : canvas.o windows.o canvas.o hint.o  score.o candy.o objective.o rectangle.o circle.o Animation.o GameManager.o
	g++ canvas.o windows.o score.o candy.o hint.o objective.o rectangle.o circle.o Animation.o GameManager.o -lfltk -o candy

windows.o: windows.cpp
	g++ -c windows.cpp

canvas.o: canvas.cpp canvas.h
	g++ -c canvas.cpp

hint.o: hint.cpp hint.h
	g++ -c hint.cpp

score.o: score.cpp score.h
	g++ -c score.cpp

candy.o: candy.cpp candy.h
	g++ -c candy.cpp

objective.o: objective.cpp objective.h
	g++ -c objective.cpp

rectangle.o: rectangle.cpp rectangle.h
	g++ -c rectangle.cpp

circle.o: circle.cpp circle.h
	g++ -c circle.cpp

GameManager.o: GameManager.cpp GameManager.h
	g++ -c GameManager.cpp

Animation.o: Animation.cpp Animation.h
	g++ -c Animation.cpp

clean:
	rm *.o candy