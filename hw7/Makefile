Gregg7: main.o LexicalAnalyzer.o Parser.o SymTable.o TAC.o TempMap.o
	g++ -std=c++11 -o Gregg7 main.o LexicalAnalyzer.o Parser.o SymTable.o TAC.o TempMap.o

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

LexicalAnalyzer.o: LexicalAnalyzer.cpp LexicalAnalyzer.h
	g++ -std=c++11 -c LexicalAnalyzer.cpp

Parser.o: Parser.cpp Parser.h
	g++ -std=c++11 -c Parser.cpp

SymTable.o: SymTable.cpp SymTable.h
	g++ -std=c++11 -c SymTable.cpp

TAC.o: TAC.cpp TAC.h
	g++ -std=c++11 -c TAC.cpp

TempMap.o: TempMap.cpp TempMap.h
	g++ -std=c++11 -c TempMap.cpp

clean:
	rm Gregg7 main.o LexicalAnalyzer.o Parser.o SymTable.o TAC.o TempMap.o
