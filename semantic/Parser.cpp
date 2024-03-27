#ifndef PARSER_H

#include "Parser.h"

bool prev_empty;
SymbolTable st;
int global_depth;
int curr_scope_offset;

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
    prev_empty = false;
    global_depth = 1;

    TableRecord s_record;
    Token module_name;

    checkNextToken(Token_T::MODULE, false);
    checkNextToken(Token_T::IDENTIFIER, false);

    // Insert the Procedure - FIX IF NEEDED
    TableRecord * p_to_proc;
    st.Insert(token.m_lexeme, token, global_depth);
    p_to_proc = st.Lookup(token.m_lexeme);
    p_to_proc->m_entry = Entry_Type::FUNCTION;

    module_name = token;

    checkNextToken(Token_T::SEMICOLON, false);

    global_depth++;

    DeclarativePart(p_to_proc);
    StatementPart();
    checkNextToken(Token_T::END, false);
    checkNextToken(Token_T::IDENTIFIER, false);
    checkNextToken(Token_T::PERIOD, false);

    while (global_depth >= 1)
    {
        st.WriteTable(global_depth);
        st.DeleteDepth(global_depth);

        std::cout << "Deleted Depth " << global_depth << std::endl; 

        global_depth--;
    }

    checkNextToken(Token_T::EOF_T, false);
}

// DeclarativePart -> ConstPart VarPart ProcPart
void DeclarativePart(TableRecord * p_to_proc)
{
    ConstPart();
    int local_vars_size = VarPart();

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

        if (st.Lookup(constants[i].first.m_lexeme) == nullptr)
        {
            st.Insert(constants[i].first.m_lexeme, constants[i].first, global_depth);
        }
        else
        {
            std::cout << "ERROR - MULTIPLE DECLARATION: '" << constants[i].first.m_lexeme << "' already at depth " << global_depth << std::endl;
            exit(109);
        }

        p_rec = st.Lookup(constants[i].first.m_lexeme);

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

    // Insert the variables as this type into the symbol table
    for (long int i = 0; i < vars_to_insert.size(); i++)
    {
        TableRecord * p_rec;
        TableRecord rec;

        if (st.Lookup(vars_to_insert[i].first.m_lexeme) == nullptr)
        {
            st.Insert(vars_to_insert[i].first.m_lexeme, vars_to_insert[i].first, global_depth);
        }
        else
        {
            std::cout << "ERROR - MULTIPLE DECLARATION: '" << vars_to_insert[i].first.m_lexeme << "' already at depth " << global_depth << std::endl;
            exit(109);
        }

        p_rec = st.Lookup(vars_to_insert[i].first.m_lexeme);

        p_rec->m_entry = Entry_Type::VAR;
        p_rec->m_lexeme = vars_to_insert[i].first.m_lexeme;
        p_rec->m_token = vars_to_insert[i].first;
        p_rec->m_depth = global_depth;
        p_rec->item.variable.m_type = tokenTypeToVarType(vars_to_insert[i].second);
        p_rec->item.variable.m_offset = curr_scope_offset;
        p_rec->item.variable.m_size = typeToSize(p_rec->item.variable.m_type, vars_to_insert[i].first);
        
        curr_scope_offset += p_rec->item.variable.m_size;
        size_of_vars += p_rec->item.variable.m_size;
    }

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

    if (ret.first == true)
    {
        return true;
    }
    checkNextToken(Token_T::SEMICOLON, false);
    ProcBody(ret.second);
    checkNextToken(Token_T::IDENTIFIER, false);
    checkNextToken(Token_T::SEMICOLON, false);
        
    // Do at the end of each procedure
    st.WriteTable(global_depth);
    st.DeleteDepth(global_depth);
    std::cout << "Deleted Depth " << global_depth << std::endl;
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

    // Insert the Procedure - FIX IF NEEDED
    TableRecord * p_to_proc;
    // st.Insert(token.m_lexeme, token, global_depth);
    if (st.Lookup(token.m_lexeme) == nullptr)
        {
            st.Insert(token.m_lexeme, token, global_depth);
        }
        else
        {
            std::cout << "ERROR - MULTIPLE DECLARATION: '" << token.m_lexeme << "' already at depth " << global_depth << std::endl;
            exit(109);
        }
    p_to_proc = st.Lookup(token.m_lexeme);

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

        prev = node;
        
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

    // ParameterInfo* node = head;
    // while(node != nullptr) // Changed to check node itself, not node->next_node
    // {
    //     std::cout << "Node: " << node->m_token.m_lexeme << std::endl; // Added std::endl for clarity in output
    //     node = node->next_node;
    // }

    // Increment the global depth, and insert the parameters
    global_depth++;

    curr_scope_offset = 0;
    
    for (int i = 0; i < params_to_insert.size(); i++)
    {
        Token p_token_info = params_to_insert[i].m_token;

        TableRecord * p_rec;
        TableRecord rec;

        if (st.Lookup(p_token_info.m_lexeme) == nullptr)
        {
            st.Insert(p_token_info.m_lexeme, p_token_info, global_depth);
        }
        else
        {
            std::cout << "ERROR - MULTIPLE DECLARATION: '" << p_token_info.m_lexeme << "' already at depth " << global_depth << std::endl;
            exit(109);
        }

        p_rec = st.Lookup(p_token_info.m_lexeme);

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
    
    // TODO - Set the size appropriately by calculating the sum of all vars sizes
    // int params_size = 0;
    // for(ParameterInfo info : params_to_insert)
    // {
    //     params_size += typeToSize(info.m_type, info.m_token);
    // }

    // Set the number of parameters
    p_to_proc->item.procedure.num_params = params_to_insert.size();

    p_to_proc->item.procedure.param_info = head;

    ret.second = p_to_proc;
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
        return;
    }

    SeqOfStatements();
}

// SeqOfStatements -> e
void SeqOfStatements()
{
    // while(token.m_token != Token_T::END)
    // {
    //     LexicalAnalyzer::GetNextToken();

    //     if (token.m_token == Token_T::EOF_T)
    //     {
    //         break;
    //     }
    // }

    // prev_empty = true;
    
    return;

}


#endif
