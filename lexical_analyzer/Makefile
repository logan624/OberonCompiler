Gregg1: main.o LexicalAnalyzer.o
	g++ -std=c++11 -o Gregg1 main.o LexicalAnalyzer.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

LexicalAnalyzer.o: LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -std=c++11 -c LexicalAnalyzer.cpp

clean:
	rm Gregg1 main.o LexicalAnalyzer.o 
