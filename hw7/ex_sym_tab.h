#ifndef SYM_H
#define SYM_H
#include "Lex.h"

const int TableSize = 211; //use a prime number

enum VarType {charType, intType, floatType };

enum EntryType {constEntry, varEntry, functionEntry};

struct ParamNode;

typedef ParamNode * ParamPtr;

struct ParamNode {
  VarType typeOfParameter;
  ParamPtr Next;
};

struct TableEntry;

typedef TableEntry * EntryPtr; //pointer to actual table entry

struct TableEntry {
  TokenType Token;
  LexemeType Lexeme;
  int depth;
  EntryType TypeOfEntry; // tag field for the union
  union {
    struct {
      VarType TypeOfVariable;
      int Offset;
      int size;
    } var;
    struct {
      VarType TypeOfConstant; //int or float constant
      int Offset;
      union {
        int Value;
        float ValueR;
      };
    } constant;
    struct { //this is the entry for a function
      int SizeOfLocal;
      int NumberOfParameters;
      VarType ReturnType;
      ParamPtr ParamList; //linked list of paramter types
    } function;
  }; //end of union
 EntryPtr next; //points to next list item
};

//function prototypes for symbol table routines

extern EntryPtr LookUp(LexemeType);
extern void Insert(LexemeType, TokenType, int);
extern void WriteTable(int);
extern void DeleteDepth(int);
extern void InitTable();

#endif
