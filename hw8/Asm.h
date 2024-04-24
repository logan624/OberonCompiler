#pragma once

#include <string>
#include <vector>
#include <fstream>

struct Procedure
{
    std::string name;
    std::stringstream body;
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

    private:
        std::vector<Procedure> procs;
        std::string module_name;
        std::string tac_name;
        std::string proc_name;
};
