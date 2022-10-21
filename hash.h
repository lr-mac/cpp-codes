#include<iostream>
#include<filesystem>
#include<fstream>
#include<string>

#define A 54059
#define B 76063
#define C 86969 
#define FIRSTH 37

class Hash
{
public:
    unsigned hash_str(const char* s);
};

std::string get_from_file(std::filesystem::path p);

