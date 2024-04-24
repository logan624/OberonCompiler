#ifndef PARSER_H

#include "Parser.h"
#include "LexicalAnalyzer.h"
#include "TempMap.h"
#include "TAC.h"
#include "LiteralTable.h"
#include "Asm.h"

#include <sstream>
#include <stack>
#include <vector>

bool prev_empty;
SymbolTable st;
int global_depth;
int curr_scope_offset;
std::string curr_procedure;
std::stack<Token> token_stack;
TempMap temp_map;
TacWriter tac_writer;
LiteralTable lt;
std::ostringstream tac_file;
std::string main_module_name;
std::string proc_name;
std::ostringstream asm_file;
Asm asm_writer;

using namespace std;

int typeToSize(Var_T type, Token t)
{
    switch(type)
    {
        case Var_T::CHAR:
            return 1;
        case Var_T::INTEGER:
            return 2;
        case Var_T::REAL:
            return 4;
        default:
            return 4;
    }
}

Var_T tokenTypeToVarType(Token_T t)
{
    switch(t)
    {
        case Token_T::REAL:
            return Var_T::REAL;
        case Token_T::CHAR:
            return Var_T::CHAR;
        case Token_T::INTEGER:
            return Var_T::INTEGER;
        default:
            return Var_T::INTEGER;
    }
}

void checkNextToken(Token_T expected, bool empty_ok)
{
    if (!prev_empty)
    {
        LexicalAnalyzer::GetNextToken();
    }
    else
    {
        prev_empty = false;
    }

    Token test = token;

    if (token.m_token == expected)
    {
        return;
    }
    else
    {
        if (empty_ok)
        {
            prev_empty = true;
            return;
        }
        else
        {
            std::cout << "ERROR - PARSING - LINE " << line_no << " - Got Token: " << tokenTypeToString(token.m_token) << ", Expected Token: " << tokenTypeToString(expected) << std::endl;
            exit(120);
        }
    }
    
    return;
}

void checkNextToken(std::vector<Token_T> expected_options, bool empty_ok)
{
    if (!prev_empty)
    {
        LexicalAnalyzer::GetNextToken();
    }
    else
    {
        prev_empty = false;
    }

    bool accepted = false;

    for (const Token_T poss_type: expected_options)
    {
        if (token.m_token == poss_type)
        {
            accepted = true;
            break;
        }
    }   

    if (accepted)
    {
        return;
    }
    else
    {
        if (empty_ok)
        {
            prev_empty = true;
            return;
        }
        else
        {
            std::cout << "ERROR - PARSING - LINE " << line_no << " - Got Token: " << tokenTypeToString(token.m_token) << ", Expected Token: ";
            
            for (long int i = 0; i < expected_options.size(); i++)
            {
                std::cout << tokenTypeToString(expected_options[i]);
                
                if (i != expected_options.size() - 1)
                {
                    std::cout << ", ";
                }
                else
                {
                    std::cout << std::endl;
                }
            }

            exit(120);
        }
    }

    Token_T test_t = token.m_token;
    
    return;
}

// Prog -> modulet idt ; DeclarativePart StatementPart endt idt
void Prog()
{
    tac_writer.setupOffsetMap();

    prev_empty = false;
    global_depth = 1;

    TableRecord s_record;
    Token module_name;

    checkNextToken(Token_T::MODULE, false);
    checkNextToken(Token_T::IDENTIFIER, false);

    // Insert the Procedure - FIX IF NEEDED
    TableRecord * p_to_proc;
    st.Insert(token.m_lexeme, token, global_depth);
    p_to_proc = st.LookupAtCurrentDepth(token.m_lexeme);
    p_to_proc->m_entry = Entry_Type::FUNCTION;

    // # TODO
    module_name = token;

    Token t = token;
    checkNextToken(Token_T::SEMICOLON, false);

    global_depth++;

    DeclarativePart(p_to_proc);

    tac_file << "proc\t" << module_name.m_lexeme << std::endl << std::endl;
    main_module_name = module_name.m_lexeme;

    curr_procedure = module_name.m_lexeme;
    asm_writer.setModuleName(module_name.m_lexeme);

    StatementPart();

    checkNextToken(Token_T::END, false);
    checkNextToken(Token_T::IDENTIFIER, false);

    if (token.m_lexeme != module_name.m_lexeme)
    {
        std::cout << "ERROR - PARSING - " << line_no << " : Ending identifier '" << token.m_lexeme << 
            "' does not match module name '" << module_name.m_lexeme << "'" << std::endl;
        exit(101);
    }

    checkNextToken(Token_T::PERIOD, false);

    std::vector<std::string> var_names;

    // Update the proc information for the ASM file
    asm_writer.addProc(module_name.m_lexeme);
    Procedure * asm_proc = asm_writer.getProc(module_name.m_lexeme);
    asm_proc->size_of_locals = p_to_proc->item.procedure.local_vars_size;

    while (global_depth >= 1)
    {
        if (global_depth == 2)
        {
            var_names = st.GetVariablesAtCurrentDepth();
        }

        lt.addLiterals(var_names);

        // st.WriteTable(global_depth);
        st.DeleteDepth(global_depth);
        // std::cout << "Deleted Depth " << global_depth << std::endl; 
        global_depth--;
    }

    Asm::writeHeader();
    Asm::writeData(var_names);

    checkNextToken(Token_T::EOF_T, false);

    tac_file << "start proc " << module_name.m_lexeme << std::endl << std::endl;
}

// DeclarativePart -> ConstPart VarPart ProcPart
void DeclarativePart(TableRecord * p_to_proc)
{
    ConstPart();
    std::string tname = curr_procedure;
    int local_vars_size = VarPart();

    if (curr_procedure != "")
    {
        Procedure * p = asm_writer.getProc(curr_procedure);
        p->size_of_locals = local_vars_size;
    }

    p_to_proc->item.procedure.local_vars_size = local_vars_size;

    ProcPart();
}

// ConstPart -> constt ConstTail | e
void ConstPart()
{
    checkNextToken(Token_T::CONST, true);

    if (prev_empty)
    {
        return;
    }

    std::vector<std::pair<Token, Token>> constants;

    ConstTail(constants);

    for (long int i = 0; i < constants.size(); i++)
    {
        TableRecord * p_rec;
        TableRecord rec;

        if (st.LookupAtCurrentDepth(constants[i].first.m_lexeme) == nullptr)
        {
            st.Insert(constants[i].first.m_lexeme, constants[i].first, global_depth);
        }
        else
        {
            std::cout << "ERROR - MULTIPLE DECLARATION: '" << constants[i].first.m_lexeme << "' already at depth " << global_depth << std::endl;
            exit(109);
        }

        p_rec = st.LookupAtCurrentDepth(constants[i].first.m_lexeme);

        p_rec->m_entry = Entry_Type::CONST;
        p_rec->m_lexeme = constants[i].first.m_lexeme;
        p_rec->m_token = constants[i].first;
        p_rec->m_depth = global_depth;
        p_rec->item.constant.m_is_int = constants[i].second.m_is_int;

        if (constants[i].second.m_is_int)
        {
            p_rec->item.constant.m_value.m_int_val = constants[i].second.m_value;
        }
        else
        {
            p_rec->item.constant.m_value.m_real_val = constants[i].second.m_valuer;
        }

        p_rec = &rec;
    }
}

// ConstTail -> idt = Value ; ConstTail | e
void ConstTail(std::vector<std::pair<Token, Token>> & constants)
{   
    std::pair<Token, Token> new_const; 

    checkNextToken(Token_T::IDENTIFIER, true);

    if (prev_empty)
    {
        return;
    }

    new_const.first = token;

    checkNextToken(Token_T::EQUAL_SIGN, false);
    
    Value();
    new_const.second = token;

    checkNextToken(Token_T::SEMICOLON, false);

    constants.push_back(new_const);

    ConstTail(constants);
}

// VarPart -> vart VarTail | e
int VarPart()
{
    checkNextToken(Token_T::VAR, true);
    
    if (prev_empty)
    {
        curr_scope_offset = -2;
        return 0;
    }

    int size_of_vars = VarTail();

    return size_of_vars;
}

// THIS NEEDS CHECKED -- NOT SURE IF THE CASE WHERE IdentifierList is EMPTY IS BEING HANDLED CORRECLTY
// VarTail -> IdentifierList : TypeMark ; VarTail | e
int VarTail()
{
    LexicalAnalyzer::GetNextToken();
    prev_empty = true;

    if (token.m_token != Token_T::IDENTIFIER)
    {
        return 0;
    }

    std::vector<std::pair<Token, Token_T>> vars_to_insert; 
    std::vector<Token> 
    vars = IdentifierList();

    checkNextToken(Token_T::COLON, false);
    Token_T type = TypeMark();
    checkNextToken(Token_T::SEMICOLON, false);

    for(Token var : vars)
    {
        vars_to_insert.push_back(std::pair<Token, Token_T>(var, type));
    }

    VarTail(vars_to_insert);

    int size_of_vars = 0;

    std::vector<std::string> names_for_offsets;

    curr_scope_offset = curr_scope_offset - 2;
    int test = curr_scope_offset;

    // Insert the variables as this type into the symbol table
    for (long int i = 0; i < vars_to_insert.size(); i++)
    {
        TableRecord * p_rec;
        TableRecord rec;

        names_for_offsets.push_back(vars_to_insert[i].first.m_lexeme);

        if (st.LookupAtCurrentDepth(vars_to_insert[i].first.m_lexeme) == nullptr)
        {
            st.Insert(vars_to_insert[i].first.m_lexeme, vars_to_insert[i].first, global_depth);
        }
        else
        {
            std::cout << "ERROR - MULTIPLE DECLARATION: '" << vars_to_insert[i].first.m_lexeme << "' already at depth " << global_depth << std::endl;
            exit(109);
        }

        p_rec = st.LookupAtCurrentDepth(vars_to_insert[i].first.m_lexeme);

        p_rec->m_entry = Entry_Type::VAR;
        p_rec->m_lexeme = vars_to_insert[i].first.m_lexeme;
        p_rec->m_token = vars_to_insert[i].first;
        p_rec->m_depth = global_depth;
        p_rec->item.variable.m_type = tokenTypeToVarType(vars_to_insert[i].second);
        p_rec->item.variable.m_offset = curr_scope_offset;
        p_rec->item.variable.m_size = typeToSize(p_rec->item.variable.m_type, vars_to_insert[i].first);
        
        curr_scope_offset -= p_rec->item.variable.m_size;
        size_of_vars += p_rec->item.variable.m_size;
    }

    test = curr_scope_offset;

    // # TODO
    tac_writer.addLocalVars(names_for_offsets, global_depth);

    return size_of_vars;
}

void VarTail(std::vector<std::pair<Token, Token_T>> & vars)
{
    LexicalAnalyzer::GetNextToken();
    prev_empty = true;

    if (token.m_token != Token_T::IDENTIFIER)
    {
        return;
    }

    std::vector<Token> more_vars; 
    more_vars = IdentifierList();

    checkNextToken(Token_T::COLON, false);
    Token_T type = TypeMark();

    for (Token var : more_vars)
    {
        vars.push_back(std::pair<Token, Token_T>(var, type));
    }

    checkNextToken(Token_T::SEMICOLON, false);

    VarTail(vars);

}

// NEEDS TESTED
// IdentifierList -> idt | IdentifierList , idt
std::vector<Token> IdentifierList()
{
    // Create a vector of tokens that will be used for insertions in to the symbol table
    std::vector<Token> vars;

    checkNextToken(Token_T::IDENTIFIER, false);

    vars.push_back(token);

    // TODO - Condition might be wrong
    while (token.m_token == Token_T::IDENTIFIER)
    {
        checkNextToken(Token_T::COMMA, true);

        if (prev_empty)
        {
            return vars;
        }

        checkNextToken(Token_T::IDENTIFIER, false);

        vars.push_back(token);
    }

    return vars;
}

// TypeMark -> integert | realt | chart
Token_T TypeMark()
{
    std::vector<Token_T> types_to_check = { Token_T::INTEGER, Token_T::REAL, Token_T::CHAR };
    checkNextToken(types_to_check, false);

    return token.m_token;
}

// Value -> NumericalLiteral
void Value()
{
    checkNextToken(Token_T::NUMBER, false);
}

// ProcPart -> ProcedureDecl ProcPart | e
void ProcPart()
{
    bool ret = false;
    ret = ProcedureDecl();
    if (ret == true)
    {
        return;
    }
    ProcPart();
}

// ProcedureDecl -> ProcHeading ; ProcBody idt ;
bool ProcedureDecl()
{
    std::pair<bool, TableRecord *> ret;
    ret.first = false;

    ret = ProcHeading();

    Token t = token;

    if (ret.first == true)
    {
        prev_empty = true;
        return true;
    }
    checkNextToken(Token_T::SEMICOLON, false);
    ProcBody(ret.second);
    checkNextToken(Token_T::IDENTIFIER, false);

    if (token.m_lexeme != curr_procedure)
    {
        std::cout << "ERROR - PARSING - LINE " << line_no << ": Closing procedure identifier '" << token.m_lexeme 
            << "' does not match procedure name '" << curr_procedure << "'" << std::endl;
        exit(101); 
    }

    checkNextToken(Token_T::SEMICOLON, false);
        
    // Do at the end of each procedure
    // st.WriteTable(global_depth);
    st.DeleteDepth(global_depth);
    // std::cout << "Deleted Depth " << global_depth << std::endl;
    global_depth--;

    return false;
}

// ProcHeading -> proct idt Args
std::pair<bool, TableRecord *> ProcHeading()
{
    std::pair<bool, TableRecord *> ret;
    ret.first = false;
    ret.second = nullptr;

    checkNextToken(Token_T::PROCEDURE, true);
    Token t = token;
    if (prev_empty == true)
    {
        prev_empty = false;
        ret.first = true;
        return ret;
    }
    checkNextToken(Token_T::IDENTIFIER, false);
    curr_procedure = token.m_lexeme;

    tac_file << "proc\t" << curr_procedure << std::endl << std::endl;

    // Insert the Procedure - FIX IF NEEDED
    TableRecord * p_to_proc;
    // st.Insert(token.m_lexeme, token, global_depth);
    if (st.LookupAtCurrentDepth(token.m_lexeme) == nullptr)
    {
        st.Insert(token.m_lexeme, token, global_depth);
    }
    else
    {
        std::cout << "ERROR - MULTIPLE DECLARATION: '" << token.m_lexeme << "' already at depth " << global_depth << std::endl;
        exit(109);
    }

    p_to_proc = st.LookupAtCurrentDepth(token.m_lexeme);
    
    // Add the name of this new  procedure to the asm_writer
    asm_writer.addProc(token.m_lexeme);


    std::vector<ParameterInfo> params_to_insert = Args();

    // Connect/create a linked list of the parameter info nodes so that the head one can be inserted into the procedure
    ParameterInfo * head = nullptr;
    ParameterInfo * prev;

    for(int i = 0; i < params_to_insert.size(); i++)
    {
        ParameterInfo * node = new ParameterInfo();

        node->m_mode = params_to_insert[i].m_mode;
        node->m_type = params_to_insert[i].m_type;
        node->m_token = params_to_insert[i].m_token;
        node->next_node = nullptr;
        
        if (i == 0)
        {
            head = node;
        }
        else
        {
            prev->next_node = node;
        }

        prev = node;
    }

    // * 2 because of the size of integers (no floats for ASM testing) 
    int size_of_params = params_to_insert.size() * 2;

    if (curr_procedure != "")
    {
        Procedure * p = asm_writer.getProc(curr_procedure);
        p->size_of_params = size_of_params;
    }

    // Increment the global depth, and insert the parameters
    global_depth++;
    curr_scope_offset = -2;
    
    for (int i = 0; i < params_to_insert.size(); i++)
    {
        Token p_token_info = params_to_insert[i].m_token;

        TableRecord * p_rec;
        TableRecord rec;

        if (st.LookupAtCurrentDepth(p_token_info.m_lexeme) == nullptr)
        {
            st.Insert(p_token_info.m_lexeme, p_token_info, global_depth);
        }
        else
        {
            std::cout << "ERROR - MULTIPLE DECLARATION: '" << p_token_info.m_lexeme << "' already at depth " << global_depth << std::endl;
            exit(109);
        }

        p_rec = st.LookupAtCurrentDepth(p_token_info.m_lexeme);

        p_rec->m_entry = Entry_Type::VAR; // this part will be conditional!
        p_rec->m_lexeme = p_token_info.m_lexeme;
        p_rec->m_token = p_token_info;
        p_rec->m_depth = global_depth;
        p_rec->item.variable.m_type = tokenTypeToVarType(p_token_info.m_token);
        p_rec->item.variable.m_offset = curr_scope_offset;
        p_rec->item.variable.m_size = typeToSize(p_rec->item.variable.m_type, p_token_info);

        curr_scope_offset += p_rec->item.variable.m_size;
    }

    // Set all the procedure struct fields using the params_to_insert function
    p_to_proc->m_entry = Entry_Type::FUNCTION;

    // Set the number of parameters
    p_to_proc->item.procedure.num_params = params_to_insert.size();

    p_to_proc->item.procedure.param_info = head;

    ret.second = p_to_proc;

    // # TODO
    tac_writer.addParams(p_to_proc, global_depth);

    return ret;
}

// ProcBody -> DeclarativePart StatementPart endt
void ProcBody(TableRecord * p_to_proc)
{
    DeclarativePart(p_to_proc);
    prev_empty = true;
    StatementPart();

    checkNextToken(Token_T::END, false);
}

// Args -> ( ArgList ) | e
std::vector<ParameterInfo> Args()
{
    std::vector<ParameterInfo> params;
    
    checkNextToken(Token_T::L_SYMBOL, true);
    
    if (prev_empty)
    {
        return params;
    }

    params = ArgList();

    checkNextToken(Token_T::R_SYMBOL, false);

    return params;
}

// ArgList -> Mode IdentifierList : TypeMark MoreArgs
std::vector<ParameterInfo> ArgList()
{
    std::vector<Token> param_names;
    std::vector<ParameterInfo> params;
    
    Param_Mode mode = Mode();
    param_names = IdentifierList();
    checkNextToken(Token_T::COLON, false);
    Token_T p_type = TypeMark();
    Var_T param_type = tokenTypeToVarType(p_type);

    // Put all of the needed info into a struct
    for (Token p : param_names)
    {
        ParameterInfo info;
        info.m_mode = mode;
        info.m_type = param_type;
        info.m_token = p;

        params.push_back(info);
    }

    MoreArgs(params);

    return params;
}

// MoreArgs -> ; ArgList | e
void MoreArgs(std::vector<ParameterInfo> & params)
{
    
    checkNextToken(Token_T::SEMICOLON, true);

    if (prev_empty == true)
    {
        return;
    }

    std::vector<ParameterInfo> extra_args = ArgList();

    for (long int i = 0; i < extra_args.size(); i++)
    {
        params.push_back(extra_args[i]);
    }
}

// Mode -> vart | e
Param_Mode Mode()
{
    checkNextToken(Token_T::VAR, true);

    if (token.m_token == Token_T::VAR)
    {
        return Param_Mode::REF;
    }
    else
    {
        return Param_Mode::VAL;
    }
}

// StatementPart -> begint SeqOfStatements | e
void StatementPart()
{
    Token test = token;

    checkNextToken(Token_T::BEGIN, true);

    if (prev_empty)
    {
        if (global_depth == 2 || global_depth == 1)
        {
            tac_file << "endp\t" << curr_procedure << std::endl;
        }
        else
        {
            tac_file << "endp\t" << curr_procedure << std::endl;
        }

        return;
    }

    SeqOfStatements();

    if (global_depth > 1)
    {
        if (global_depth == 2)
        {
            curr_procedure = main_module_name;
        }

        if (global_depth == 2 || global_depth == 1)
        {
            tac_file << "endp\t" << curr_procedure << std::endl;
        }
        else
        {
            tac_file << "endp\t" << curr_procedure << std::endl;
        }
        
    }
    else
    {

    }

    temp_map = TempMap();
    tac_file << std::endl;
}

// SeqOfStatements -> e
void SeqOfStatements()
{
    bool io = Statement();

    if (io == false)
    {
        checkNextToken(Token_T::SEMICOLON, true);
    }

    if (prev_empty)
    {
        return;
    }

    StatTail();
}

// Bool flag indicates that an IoStatement happened
// StatTail -> Statement ; StatSail | e
void StatTail()
{
    bool io = Statement();

    if (io == false)
    {
        checkNextToken(Token_T::SEMICOLON, true);
    }
    

    if (prev_empty)
    {
        return;
    }

    StatTail();
}

// Statement -> AssignStat | IOStat
bool Statement()
{
    bool ret = false;

    if (!prev_empty)
    {
        LexicalAnalyzer::GetNextToken();
        prev_empty = true;
    }

    Token t = token;
    
    if (token.m_token == Token_T::IDENTIFIER)
    {
        Token t = token;
        AssignStat();
    }
    else
    {
        IOStat();

        ret = true;

        bool write_or_read = false;

        // Do the TAC printing for the IOStat Statements
        Token prev_token;
        prev_token.m_token = Token_T::UNKNOWN;
        Token curr_token;
        curr_token.m_token = Token_T::UNKNOWN;

        // Reverse the token stack
        std::stack<Token> ts;
        while (!token_stack.empty())
        {
            ts.push(token_stack.top());
            token_stack.pop();
        }
        token_stack = ts;

        while (!token_stack.empty())
        {
            curr_token = token_stack.top();
            token_stack.pop();

            if (write_or_read)
            {
                write_or_read = false;
                
                if (curr_token.m_token == Token_T::LITERAL)
                {
                    tac_file << curr_token.m_lexeme << std::endl;
                }
                else
                {
                    tac_file << TacWriter::printVar(curr_token) << std::endl;
                }
            }
            else if (curr_token.m_token == Token_T::WRITE)
            {
                tac_file << "write ";
                write_or_read = true;
            }
            else if (curr_token.m_token == Token_T::READ)
            {
                tac_file << "read ";
                write_or_read = true;
            }
            else if (curr_token.m_token == Token_T::WRITELN)
            {
                tac_file << "wrln" << std::endl << std::endl;
            }

            prev_token = curr_token;
        }
    }

    return ret;
}

// AssignStat -> idt := Expr | ProcCall
void AssignStat()
{
    Token t = token;

    checkNextToken(Token_T::IDENTIFIER, true);

    if (st.Lookup(token.m_lexeme) == nullptr)
    {
        std::cout << "ERROR - SEMANTIC ANALYSIS - LINE " << line_no << ": Identifier '"
            << token.m_lexeme << "' not previously declared" << std::endl;
        exit(101);
    }

    // Do Proc Call
    if (prev_empty)
    {
        ProcCall();
        t = token;

        // Do the procedure call in assembly (push everything onto stack, call 'ProcName')
    }
    else
    {
        // Push the identifier being assigned to

        t = token;

        token_stack.push(token);
    }

    checkNextToken(Token_T::ASSOP, true);

    if (prev_empty)
    {
        // std::stack<Token> test_stack = token_stack;
        std::string proc_name = t.m_lexeme;
        ::proc_name = proc_name;
        ProcCall();
        tac_file << "call " << proc_name << std::endl << std::endl;

        while(!token_stack.empty())
        {
            token_stack.pop();
        }

        // Do the procedure call in assembly (push everything onto stack, call 'ProcName')
        return;
    }

    t = token;

    token_stack.push(token);

    Expr();

    // Feed assignment statement(s) to the TAC Generator:
    //     * Process all the tokens that get created
    //     * Use a stack data structure
    // -------------------------------------------------
    // while (token_stack.pop) { Process things }

    // Preprocess Token Stack
    //      Create temp variables for the unary operands, parentheses
    //      since those have the highest precedence
    TacWriter::preprocStatement();

    // Process Token Stack
    //      Break multioperation statements down into ones of two max

    while (token_stack.empty() != true)
    {
        Token t = token_stack.top();
        token_stack.pop();

        DisplayToken(t);
    }
}

// idt ( Params )
void ProcCall()
{
    checkNextToken(Token_T::L_SYMBOL, false);
    
    std::vector<std::string> params = Params();

    for (std::string param : params)
    {
        tac_file << "push " << param << std::endl;
    }

    checkNextToken(Token_T::R_SYMBOL, false);
}

// idt ParamsTail | num ParamsTail | e
std::vector<std::string> Params()
{
    std::vector<std::string> params;
    std::vector<Token_T> types_to_check = {Token_T::IDENTIFIER, Token_T::NUMBER};
    checkNextToken(types_to_check, true);

    Token test = token;

    if (prev_empty)
    {
        return params;
    }
    else
    {
        if (token.m_token == Token_T::NUMBER)
        {
            params.push_back(TacWriter::printVar(token));
        }
        else
        {
            TableRecord * tr = st.LookupAtCurrentDepth(token.m_lexeme);

            if (tr == nullptr)
            {
                std::cout << "ERROR - PARSING - LINE " << line_no << ": Param '" << token.m_lexeme << "' not previously declared" << std::endl;
                exit(101);
            }
            else
            {
                params.push_back(TacWriter::printVar(token));
            }
        }

        ParamsTail(params);
    }

    int param_num = 1;

    for (std::string & param : params)
    {
        TableRecord * tr2 = st.Lookup(proc_name);
        Param_Mode mode = Param_Mode::VAL;

        int num_params = 1;
        
        ParameterInfo * node;

        if (tr2 != nullptr)
        {
            node = tr2->item.procedure.param_info;
            int num = 1;

            while (node != nullptr)
            {
                if (param_num == num)
                {
                    mode = node->m_mode;
                }

                node = node->next_node;
                num++;
            }

            // num_params++;
            num_params = tr2->item.procedure.num_params;
        }

        // std::cout << "NUM_PARAMS" << num_params << std::endl;
        // std::cout << "VECTOR SIZE" << params.size() << std::endl;

        if (num_params != params.size())
        {
            std::cout << "ERROR - PARSING - LINE " << line_no << ": Wrong number of arguments to procedure '" << proc_name << "'" << std::endl;
            exit(105);
        }

        if (mode == Param_Mode::REF)
        {
            param = "@" + param;
        }

        param_num++;
    }

    return params;
}

// , idt ParamsTail | , num ParamsTail | e
void ParamsTail(std::vector<std::string>  & params)
{
    checkNextToken(Token_T::COMMA, true);

    if (prev_empty)
    {
        return;
    }

    std::vector<Token_T> types_to_check = {Token_T::IDENTIFIER, Token_T::NUMBER};
    checkNextToken(types_to_check, false);

    if (token.m_token == Token_T::NUMBER)
    {
        params.push_back(TacWriter::printVar(token));
    }
    else
    {
        TableRecord * tr = st.LookupAtCurrentDepth(token.m_lexeme);

        if (tr == nullptr)
        {
            std::cout << "ERROR - PARSING - LINE " << line_no << ": Param '" << token.m_lexeme << "' not previously declared" << std::endl;
            exit(101);
        }
        else
        {
            params.push_back(TacWriter::printVar(token));
        }
    }

    ParamsTail(params);
}

// IOStat -> InStat | OutStat
void IOStat()
{
    if (token.m_token == Token_T::READ)
    {
        InStat();
    }
    else if (token.m_token == Token_T::WRITE || token.m_token == Token_T::WRITELN)
    {
        OutStat();
    }


}

// read ( Id )
void InStat()
{
    checkNextToken(Token_T::READ, false);
    token_stack.push(token);
    checkNextToken(Token_T::L_SYMBOL, false);
    checkNextToken(Token_T::IDENTIFIER, false);
    token_stack.push(token);
    checkNextToken(Token_T::R_SYMBOL, false);
}

// idt IdListTail
void IdList()
{
    checkNextToken(Token_T::IDENTIFIER, false);
}

// , idt IdListTail | e
void IdListTail()
{
    checkNextToken(Token_T::IDENTIFIER, true);

    if (prev_empty)
    {
        return;
    }
    
    checkNextToken(Token_T::IDENTIFIER, false);
}

// write ( WriteList ) | writeln ( WriteList )
void OutStat()
{
    Token t = token;
    bool writeln = false;
    prev_empty = false;
    
    if (token.m_token == Token_T::WRITELN)
    {
        writeln = true;
    }

    // Token write;
    // write.m_token = Token_T::WRITE;
    // token_stack.push(write);

    // Know it is either WRITE or WRITELN at this point, so check next tokens
    checkNextToken(Token_T::L_SYMBOL, false);
    WriteList();
    checkNextToken(Token_T::R_SYMBOL, false);

    // Not sure what this was for
    // if (token.m_token == Token_T::WRITELN)
    // {
    //     token_stack.push(token);
    // }

    if (writeln == true)
    {
        Token wln;
        wln.m_token = Token_T::WRITELN;
        token_stack.push(wln);
    }
}

// WriteToken WriteListTail
void WriteList()
{
    WriteToken();
    WriteListTail();
}

// , WriteToken WriteListTail | e
void WriteListTail()
{
    checkNextToken(Token_T::COMMA, true);

    if (prev_empty)
    {
        return;
    }

    WriteToken();
    WriteListTail();
}

// idt | numt | literal
void WriteToken()
{
    std::vector<Token_T> types_to_check = {Token_T::IDENTIFIER, Token_T::NUMBER, Token_T::LITERAL};
    checkNextToken(types_to_check, false);

    Token write;
    write.m_token = Token_T::WRITE;
    token_stack.push(write);

    if (token.m_token == Token_T::LITERAL)
    {
        std::string lname = lt.insertLiteral(token.m_literal);

        Token new_literal_for_stack;
        new_literal_for_stack.m_lexeme = lname;
        new_literal_for_stack.m_token = Token_T::LITERAL;

        token_stack.push(new_literal_for_stack);
    }
    else
    {
        token_stack.push(token);
    }
}

// Expr -> Relation
void Expr()
{
    Relation();
}

// Relation -> SimpleExpr
void Relation()
{
    SimpleExpr();
}

// SimpleExpr -> Term MoreTerm
void SimpleExpr()
{
    Term();
    MoreTerm();
}

// MoreTerm -> Addop Term MoreTerm | e
void MoreTerm()
{
    AddOp();

    if (prev_empty)
    {
        return;
    }
    else
    {
        // Token temp_flag;
        // temp_flag.m_token = Token_T::UNKNOWN;
        // token_stack.push(temp_flag); 
    }

    Term();
    MoreTerm();
}

// Term -> Factor MoreFactor
void Term()
{
    Factor();
    MoreFactor();
}

// MoreFactor -> MulOp Factor MoreFactor | e
void MoreFactor()
{
    MulOp();
    
    if (prev_empty == true)
    {
        return;
    }
    else
    {
        // Token temp_flag;
        // temp_flag.m_token = Token_T::UNKNOWN;
        // token_stack.push(temp_flag);
    }

    Factor();
    MoreFactor();
}

// Factor -> idt | numt | ( Expr ) | ~ Factor | SignOp Factor
void Factor()
{
    std::vector<Token_T> types_to_check = { Token_T::IDENTIFIER, Token_T::NUMBER, Token_T::L_SYMBOL,
            Token_T::TILDAE};

    checkNextToken(types_to_check, true);

    Token t = token;

    if (prev_empty)
    {
        // Not sure if this right
        
        SignOp();
        prev_empty = false;
        Factor();
        
        Token temp_flag;
        temp_flag.m_token = Token_T::TEMP_END;
        token_stack.push(temp_flag);

        return;
    }
    
    if (token.m_token == Token_T::IDENTIFIER)
    {
        // Ensure used identifiers have been declared previously
        if (st.Lookup(token.m_lexeme) == nullptr)
        {
            std::cout << "ERROR - SEMANTIC ANALYSIS - LINE " << line_no << ": Identifier '"
                << token.m_lexeme << "' not previously declared" << std::endl;
            exit(101);
        }
        
    }
    else if (token.m_token == Token_T::NUMBER)
    {
        // Maybe add stuff here in later assingments?
        // Token temp_flag;
        // temp_flag.m_token = Token_T::TEMP_BEGIN;
        // token_stack.push(temp_flag);

        token_stack.push(token);

        // temp_flag.m_token = Token_T::TEMP_END;
        // token_stack.push(temp_flag);

        return;
    }
    else if (token.m_token == Token_T::L_SYMBOL)
    {
            Token temp_flag;
            temp_flag.m_token = Token_T::TEMP_BEGIN;
            token_stack.push(temp_flag);

            Expr();
            
            checkNextToken(Token_T::R_SYMBOL, false);

            temp_flag.m_token = Token_T::TEMP_END;
            token_stack.push(temp_flag);
            return;
    }
    else if (token.m_token == Token_T::TILDAE)
    {
            Factor();
    }

    t = token;

    token_stack.push(token);

    return;
}

// AddOp -> + | - | OR
void AddOp()
{
    std::vector<Token_T> types_to_check = { Token_T::ADDOP, Token_T::MINUS };

    checkNextToken(types_to_check, true);

    Token t = token;

    if (!prev_empty)
    {
        token_stack.push(token);
    }
}

// MulOp -> * | / | DIV | MOD | &
void MulOp()
{
    checkNextToken(Token_T::MULOP, true);

    Token t = token;

    if (!prev_empty)
    {
        token_stack.push(token);
    }
}

// SignOp -> -
void SignOp()
{
    Token t = token;

    Token temp_flag;
    temp_flag.m_token = Token_T::TEMP_BEGIN;
    token_stack.push(temp_flag);

    checkNextToken(Token_T::MINUS, true);

    token_stack.push(token);
}




#endif
