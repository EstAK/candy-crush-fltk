candy : canvas.o windows.o canvas.o hint.o  score.o candy.o objective.o rectangle.o circle.o Animation.o GameManager.o
	g++ canvas.o windows.o score.o candy.o hint.o objective.o rectangle.o circle.o Animation.o GameManager.o -lfltk -o candy

windows.o: windows.cpp
	g++ -c -g windows.cpp

canvas.o: canvas.cpp canvas.h
	g++ -c -g canvas.cpp

hint.o: hint.cpp hint.h
	g++ -c -g hint.cpp

score.o: score.cpp score.h
	g++ -c -g score.cpp

candy.o: candy.cpp candy.h
	g++ -c -g candy.cpp

objective.o: objective.cpp objective.h
	g++ -c -g objective.cpp

rectangle.o: rectangle.cpp rectangle.h
	g++ -c -g rectangle.cpp

circle.o: circle.cpp circle.h
	g++ -c -g circle.cpp

GameManager.o: GameManager.cpp GameManager.h
	g++ -c -g GameManager.cpp

Animation.o: Animation.cpp Animation.h
	g++ -c -g Animation.cpp

clean:
	rm *.o candy