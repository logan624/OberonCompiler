Gregg1: main.o LexicalAnalyzer.o Parser.o
	g++ -std=c++11 -o Gregg1 main.o LexicalAnalyzer.o Parser.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

LexicalAnalyzer.o: LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -std=c++11 -c LexicalAnalyzer.cpp

Parser.o: Parser.cpp Parser.h
	g++ -std=c++11 -c Parser.cpp

clean:
	rm Gregg1 main.o LexicalAnalyzer.o Parser.o
