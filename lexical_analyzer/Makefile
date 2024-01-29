hw1: main.o LexicalAnalyzer.o
	g++ -o hw1 main.o LexicalAnalyzer.o

main.o: main.cpp
	g++ -c main.cpp

LexicalAnalyzer.o: LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -c LexicalAnalyzer.cpp

clean:
	rm hw1 main.o LexicalAnalyzer.o 
