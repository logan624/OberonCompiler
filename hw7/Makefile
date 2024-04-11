Gregg7: main.o LexicalAnalyzer.o Parser.o SymTable.o
	g++ -std=c++11 -o Gregg7 main.o LexicalAnalyzer.o Parser.o SymTable.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

LexicalAnalyzer.o: LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -std=c++11 -c LexicalAnalyzer.cpp

Parser.o: Parser.cpp Parser.h
	g++ -std=c++11 -c Parser.cpp

SymTable.o: SymTable.cpp SymTable.h
	g++ -std=c++11 -c SymTable.cpp

clean:
	rm Gregg7 main.o LexicalAnalyzer.o Parser.o SymTable.o
