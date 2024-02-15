#pragma once

#include <string>

#include "LexicalAnalyzer.h"

extern bool prev_empty;

void checkNextToken(Token_T expected, bool empty_ok);

// Prog -> modulet idt DeclarativePart StatementPart endt idt
void Prog();

// DeclarativePart -> ConstPart VarPart ProcPart
void DeclarativePart();

// ConstPart -> constt ConstTail | e
void ConstPart();

// ConstTail -> idt = Value ; ConstTail | e
void ConstTail();

// VarPart -> vart VarTail | e
void VarPart();

// VarTail -> IdentifierList : TypeMark ; VarTail | e
void VarTail();

// IdentifierList -> idt | IdentifierList , idt
void IdentifierList();

// TypeMark -> integert | realt | chart
void TypeMark();

// Value -> NumericalLiteral
void Value();

// ProcPart -> ProcedureDecl ProcPart | e
void ProcPart();

// ProcedureDecl -> ProcHeading ; ProcBody idt ;
void ProcedureDecl();

// ProcHeading -> proct idt Args
void ProcHeading();

// ProcBody -> DeclarativePart StatementPart endt
void ProcBody();

// Args -> ( ArgList ) | e
void Args();

// ArgList -> Mode IdentifierList : TypeMark MoreArgs
void ArgList();

// MoreArgs -> ; ArgList | e
void MoreArgs();

// Mode -> vart | e
void Mode();

// StatementPart -> begint SeqOfStatements | e
void StatementPart();

// SeqOfStatements -> e
void SeqOfStatements();
