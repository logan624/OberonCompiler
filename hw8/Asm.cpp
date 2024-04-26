#include "Asm.h"
#include "LiteralTable.h"
#include "Parser.h"

#include <iostream>
#include <sstream>

extern std::ostringstream asm_file;
extern LiteralTable lt;
extern std::ostringstream tac_file;

int howManyNotBlank(std::string one, std::string two, std::string three, std::string four, std::string five)
{
    int count_filled = 0;

    if (one.length() != 0)
    {
        count_filled++;
    }
    
    if (two.length() != 0)
    {
        count_filled++;
    }
    
    if (three.length() != 0)
    {
        count_filled++;
    }
    
    if (four.length() != 0)
    {
        count_filled++;
    }
    
    if (five.length() != 0)
    {
        count_filled++;
    }

    return count_filled;
}

void Asm::readTACFile() {
    std::ifstream file(tac_name);
    std::string line_raw;
    while (std::getline(file, line_raw))
    {
        std::stringstream line(line_raw);
        int count = 0;
        std::string one = "";
        std::string two = "";
        std::string three = "";
        std::string four = "";
        std::string five = "";

        std::string word = "";

        while (line >> word)
        {
            // std::cout << word << " ";
            if (one.length() == 0)
            {
                one = word;
            }
            else if (two.length() == 0)
            {
                two = word;
            }
            else if (three.length() == 0)
            {
                three = word;
            }
            else if (four.length() == 0)
            {
                four = word;
            }
            else if (five.length() == 0)
            {
                five = word;
            }

            count++;
        }

        int num = howManyNotBlank(one, two, three, four, five);
        std::vector<std::string> vec;

        vec.push_back(one);
        vec.push_back(two);
        vec.push_back(three);
        vec.push_back(four);
        vec.push_back(five);

        switch(num)
        {
            case 0:
                continue;
            case 1:
                this->one(vec);
                break;
            case 2:
                this->two(vec);
                break;
            case 3:
                this->three(vec);
                break;
            case 4:
                this->four(vec);
                break;
            case 5:
                this->five(vec);
                break;
            default:
                ;
        }

        if (in_module_code)
        {
            for (std::string s : vec)
            {
                if (s.length() > 2)
                {
                    if (s[0] == '_' && s[1] == 't')
                    {
                        bool found = false;
                        for (std::string mt : main_temps)
                        {
                            if (mt == s.substr(1, s.length() - 1))
                            {
                                found = true;
                            }
                        }

                        if (!found)
                        {
                            main_temps.push_back(s.substr(1, s.length() - 1));
                        }
                    }
                }
            }
        }

    }

    file.close();
}

void Asm::writeHeader()
{
    asm_file << ".MODEL SMALL" << std::endl;
    asm_file << ".586" << std::endl;
    asm_file << ".STACK 100h" << std::endl;
}

void Asm::writeData(std::vector<std::string> v)
{
    asm_file << ".DATA" << std::endl;

    for (std::string var : v)
    {
        bool is_char = false;

        if (var[0] == '_')
        {
            asm_file << "\t" << var.substr(1, var.size() - 1) << "\tDB\t";

            std::string lit = lt.getLiteral(var);

            asm_file << "\"" << lit << "\"" << ",\"$\"";
        }
        else
        {
            TableRecord * tr = st.LookupAtCurrentDepth(var);
    
            if (tr == nullptr)
            {
                tr = st.Lookup(var);
            }

            if (tr != nullptr)
            {
                if (tr->is_var)
                {
                    if (tr->item.variable.m_type == Var_T::CHAR)
                        is_char = true;
                }
            }

            if (!is_char)
            {
                asm_file << "\t" << var << "\tDW\t" << "?";
            }
            else
            {
                asm_file << "\t" << var << "\tDB\t" << "?";
            }

            is_char = false;
        }

        asm_file << std::endl;
    }
}

void Asm::addProc(std::string name)
{
    Procedure p;
    p.name = name;

    procs.push_back(p);
}

Procedure * Asm::getProc(std::string name)
{
    Procedure * ret = nullptr;

    for(int i = 0; i < procs.size(); i++)
    {
        if (procs[i].name == name)
        {
            ret = &procs[i];
        }
    }

    return ret;
}

bool isRef(std::string s)
{
    if (s[0] == '@')
    {
        return true;
    }

    return false;
}

std::string printVar(std::string v)
{
    bool is_ref = isRef(v);

    if (is_ref)
    {
        v = v.substr(1, v.length() - 1); 
    }

    // Global Var name
    if (v[0] != '_')
    {

    }
    // _bp case
    else
    {
        if (v[1] == 'S')
        {
            v = v.substr(1, v.length() - 1);
        }
        else if (v[1] != 't')
        {
            v = v.substr(1, v.length() - 1);
            v = "[" + v + "]";
        }
        else
        {
            v = v.substr(1, v.length() - 1);
        }
        
    }

    return v;
}

void Asm::one(std::vector<std::string> v)
{
    Procedure * p = nullptr;

    if (proc_name != "")
    {
        p = this->getProc(proc_name);
    }
    else
    {
        p = this->getProc(module_name);
    }

    // Case for wrln
    if (v[0] == "wrln")
    {
        p->body = p->body + "call writeln" + "\n";
    }
}

void Asm::two(std::vector<std::string> v)
{
    Procedure * p = nullptr;

    if (proc_name != "")
    {
        p = this->getProc(proc_name);
    }
    else
    {
        p = this->getProc(module_name);
    }

    // Cases
    //      proc name
    //          procname PROC
    //          push bp
    //          mov bp , sp
    //          sub sp , SIZE OF LOCALS
    if (v[0] == "proc")
    {
        if (v[1] != module_name)
        {
            proc_name = v[1];
            p = this->getProc(proc_name);

            p->body = p->body + v[1] + "    PROC" + "\n";
            p->body = p->body + "push bp" + "\n";
            p->body = p->body + "mov bp , sp" + "\n";
            p->body = p->body + "sub sp , " + std::to_string(p->size_of_locals) + "\n";   
        }
        else
        {
            in_module_code = true;
            proc_name = "";
            p = this->getProc(module_name);
            p->body = p->body + v[1] + "    PROC" + "\n";
            p->body = p->body + "push bp" + "\n";
            p->body = p->body + "mov bp , sp" + "\n";
            p->body = p->body + "sub sp , " + std::to_string(p->size_of_locals) + "\n\n"; 
        }
    }
    //      endp name
    //          add sp , SIZE OF LOCALS
    //          pop bp
    //          ret SIZE OF PARAMS
    //          procname endp
    else if (v[0] == "endp")
    {
        if (proc_name != "")
        {
            p->body = p->body + "\nadd sp , " + std::to_string(p->size_of_locals) + "\n";
            p->body = p->body + "pop bp" + "\n";
            p->body = p->body + "ret " + std::to_string(p->size_of_params) + "\n";
            p->body = p->body + v[1] + "    ENDP" + "\n\n"; 
            proc_name = "";    
        }
        else
        {
            p->body = p->body + "\nadd sp , " + std::to_string(p->size_of_locals) + "\n";
            p->body = p->body + "pop bp" + "\n";
            p->body = p->body + "ret " + std::to_string(p->size_of_params) + "\n";
            p->body = p->body + v[1] + "    ENDP" + "\n\n"; 
        }
    }
    //      push _
    else if (v[0] == "push")
    {
        // Add in "offset" if it is by reference
        p->body = p->body + "push " + printVar(v[1]) + "\n";
    }
    //      call _
    //          call procname
    else if (v[0] == "call")
    {
        p->body = p->body + "call " + v[1] + "\n";
    }
    else if (v[0] == "wri")
    {
        p->body = p->body + "mov ax , " + printVar(v[1]) + "\n";
        p->body = p->body + "call writeint" + "\n";
    }
    else if (v[0] == "wrs")
    {
        p->body = p->body + "mov dx , offset " + printVar(v[1]) + "\n";
        p->body = p->body + "call writestr" + "\n";
    }
    // If I follow through with char
    else if (v[0] == "wrc")
    {
        p->body = p->body + "mov dl , " + printVar(v[1]) + "\n";
        p->body = p->body + "call writech" + "\n";
    }
    else if (v[0] == "readi")
    {
        p->body = p->body + "call readint" + "\n";
        if (isRef(v[1]))
        {
            p->body = p->body + "mov ax , " + printVar(v[1]) + "\n";
            p->body = p->body + "mov [ax] , bx" + "\n";
        }
        else
        {
            p->body = p->body + "mov " + printVar(v[1]) + " , bx" + "\n";
        }
    }
    // If I follow through with char
    else if (v[0] == "readc")
    {
        p->body = p->body + "call readch" + "\n";
        p->body = p->body + "mov " + printVar(v[1]) + ", al\n";
    }
}

void Asm::three(std::vector<std::string> v)
{
    if (v[0] == "start")
    {
        return;
    }

    Procedure * p = nullptr;

    if (proc_name != "")
    {
        p = this->getProc(proc_name);
    }
    else
    {
        p = this->getProc(module_name);
    }
    
    // Cases
    //      start proc name
    //      _ = _
    if (isRef(v[0]))
    {
        p->body = p->body + "\nmov AX , " + printVar(v[2]) + "\n";
        p->body = p->body + "\nmov BX , " + printVar(v[0]) + "\n";
        p->body = p->body + "mov [BX], AX\n";
    }
    else
    {
        p->body = p->body + "\nmov AX , " + printVar(v[2]) + "\n";
        p->body = p->body + "mov " + printVar(v[0]) + " , AX\n";
    }
}

void Asm::four(std::vector<std::string> v)
{

}

void Asm::five(std::vector<std::string> v)
{
    Procedure * p = nullptr;

    if (proc_name != "")
    {
        p = this->getProc(proc_name);
    }
    else
    {
        p = this->getProc(module_name);
    }

    // Cases
    //      _ = _ op _
    if (v[3] == "*")
    {
        p->body = p->body + "mov AX, " + printVar(v[2]) + "\n";
        p->body = p->body + "mov BX, " + printVar(v[4]) + "\n";
        p->body = p->body + "imul BX\n";
        if (isRef(v[0]))
        {
            p->body = p->body + "mov BX, " + printVar(v[0]) + "\n";
            p->body = p->body + "mov [BX], AX" + "\n";
        }
        else
        {
            p->body = p->body + "mov " + printVar(v[0]) + " , AX\n";
        }
    }
    else if (v[3] == "+")
    {
        p->body = p->body + "mov AX, " + printVar(v[2]) + "\n";
        p->body = p->body + "add AX, " + printVar(v[4]) + "\n";
        // p->body = p->body + "mov " + printVar(v[0]) + " , AX\n";
        if (isRef(v[0]))
        {
            p->body = p->body + "mov BX, " + printVar(v[0]) + "\n";
            p->body = p->body + "mov [BX], AX" + "\n";
        }
        else
        {
            p->body = p->body + "mov " + printVar(v[0]) + " , AX\n";
        }
    }
    else if (v[3] == "-")
    {
        p->body = p->body + "mov AX, " + printVar(v[2]) + "\n";
        p->body = p->body + "sub AX, " + printVar(v[4]) + "\n";
        if (isRef(v[0]))
        {
            p->body = p->body + "mov BX, " + printVar(v[0]) + "\n";
            p->body = p->body + "mov [BX], AX" + "\n";
        }
        else
        {
            p->body = p->body + "mov " + printVar(v[0]) + " , AX\n";
        }
    }
}

void Asm::printProc(std::string pname)
{
    Procedure * pr = this->getProc(pname);
    asm_file << pr->body;
}

void Asm::writeCode()
{
    for (std::string mt : main_temps)
    {
        asm_file << "\t" << mt << "\tDW\t?" << std::endl;    
    }

    asm_file << ".CODE" << std::endl;
    asm_file << "include io.asm" << std::endl << std::endl;

    for (Procedure proc : procs)
    {
            asm_file << proc.body;
    }

    asm_file << "main\tPROC\n";
    asm_file << "mov AX, @data\n";
    asm_file << "mov DS, AX\n\n";
    asm_file << "call " << module_name << "\n\n";
    asm_file << "mov ah, 04ch\n";
    asm_file << "int 21h\n";
    asm_file << "main\tENDP\n";
    asm_file << "END\tmain\n" << std::endl;
}
