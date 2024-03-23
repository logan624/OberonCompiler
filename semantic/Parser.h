#pragma once

#include <string>

#include "LexicalAnalyzer.h"
#include "SymTable.h"

extern bool prev_empty;
extern SymbolTable st;

void checkNextToken(Token_T expected, bool empty_ok);

// Prog -> modulet idt DeclarativePart StatementPart endt idt
void Prog();

// DeclarativePart -> ConstPart VarPart ProcPart
void DeclarativePart();

// ConstPart -> constt ConstTail | e
void ConstPart();

// ConstTail -> idt = Value ; ConstTail | e
void ConstTail(std::vector<std::pair<Token, Token>> & constants);

// VarPart -> vart VarTail | e
void VarPart();

// VarTail -> IdentifierList : TypeMark ; VarTail | e
void VarTail();

// IdentifierList -> idt | IdentifierList , idt
std::vector<Token> IdentifierList();

// TypeMark -> integert | realt | chart
Token_T TypeMark();

// Value -> NumericalLiteral
void Value();

// ProcPart -> ProcedureDecl ProcPart | e
void ProcPart();

// ProcedureDecl -> ProcHeading ; ProcBody idt ;
bool ProcedureDecl();

// ProcHeading -> proct idt Args
bool ProcHeading();

// ProcBody -> DeclarativePart StatementPart endt
void ProcBody();

// Args -> ( ArgList ) | e
std::vector<Token> Args();

// ArgList -> Mode IdentifierList : TypeMark MoreArgs
std::vector<Token> ArgList();

// MoreArgs -> ; ArgList | e
std::vector<Token> MoreArgs();

// Mode -> vart | e
void Mode();

// StatementPart -> begint SeqOfStatements | e
void StatementPart();

// SeqOfStatements -> e
void SeqOfStatements();
