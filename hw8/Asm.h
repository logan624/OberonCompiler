#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

struct Procedure
{
    std::string name;
    std::string body = "";
    int size_of_locals;
    int size_of_params;
};

class Asm 
{
    public:
        Asm() { proc_name = ""; }
        Asm(std::string tac_file_name) { tac_name = tac_file_name; }
        void readTACFile();
        static void writeHeader();
        static void writeData(std::vector<std::string> v);
        void setModuleName(std::string s) { module_name = s; }
        void addProc(std::string name);
        void one(std::vector<std::string>);
        void two(std::vector<std::string>);
        void three(std::vector<std::string>);
        void four(std::vector<std::string>);
        void five(std::vector<std::string>);
        Procedure * getProc(std::string name);
        void printProc(std::string pname);
        void writeCode();

    private:
        std::vector<Procedure> procs;
        std::string module_name;
        std::string tac_name;
        std::string proc_name;
        std::vector<std::string> main_temps;
        bool in_module_code = false;
};
