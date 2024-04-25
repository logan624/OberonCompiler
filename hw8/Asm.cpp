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
        if (var[0] == '_')
        {
            asm_file << "\t" << var << "\tDB\t";

            std::string lit = lt.getLiteral(var);

            asm_file << "\"" << lit << "\"" << ",\"$\"";
        }
        else
        {
            asm_file << "\t" << var << "\tDW\t" << "?";
        }

        asm_file << std::endl;
    }

    asm_file << ".CODE" << std::endl;
    asm_file << "include io.asm" << std::endl << std::endl;
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

void Asm::one(std::vector<std::string> v)
{

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
        
    }
    //      call _
    //          call procname
    else if (v[0] == "call")
    {
        p->body = p->body + "call " + v[1] + "\n";
    }

}

void Asm::three(std::vector<std::string> v)
{
    // Cases
    //      start proc name
    //      _ = _
}

void Asm::four(std::vector<std::string> v)
{

}

void Asm::five(std::vector<std::string> v)
{
    // Cases
    //      _ = _ op _
}

void Asm::printProc(std::string pname)
{
    Procedure * pr = this->getProc(pname);
    asm_file << pr->body;
}

void Asm::writeCode()
{
    asm_file << "main\tPROC\n";
    asm_file << "mov ax, @data\n\n";
    asm_file << "call " << module_name << "\n\n";
    asm_file << "mov ah, 04ch\n";
    asm_file << "int 21h\n";
    asm_file << "main\tENDP\n\n";

    for (Procedure proc : procs)
    {
            asm_file << proc.body;
    }

    asm_file << "END\tmain\n" << std::endl;
}
