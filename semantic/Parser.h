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
void DeclarativePart(TableRecord * p_to_proc);

// ConstPart -> constt ConstTail | e
void ConstPart();

// ConstTail -> idt = Value ; ConstTail | e
void ConstTail(std::vector<std::pair<Token, Token>> & constants);

// VarPart -> vart VarTail | e
int VarPart();

// VarTail -> IdentifierList : TypeMark ; VarTail | e
int VarTail();

void VarTail(std::vector<std::pair<Token, Token_T>> & vars);

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
std::pair<bool, TableRecord *> ProcHeading();

// ProcBody -> DeclarativePart StatementPart endt
void ProcBody(TableRecord * p_to_proc);

// Args -> ( ArgList ) | e
std::vector<ParameterInfo> Args();

// ArgList -> Mode IdentifierList : TypeMark MoreArgs
std::vector<ParameterInfo> ArgList();

// MoreArgs -> ; ArgList | e
void MoreArgs(std::vector<ParameterInfo> & params);

// Mode -> vart | e
Param_Mode Mode();

// StatementPart -> begint SeqOfStatements | e
void StatementPart();

// SeqOfStatements -> e
void SeqOfStatements();

int typeToSize(Var_T type, Token t);
Var_T tokenTypeToVarType(Token_T);
