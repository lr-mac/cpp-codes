#include "hash.h"

std::string mess;

std::string get_from_file(std::filesystem::path p)
{
    std::ifstream fmess;
    
    fmess.open(p);

    if(!fmess)
    std::cout << "Failed to open" << std::endl;
    else
    {
        std::getline(fmess, mess);
        //std::cout << mess << std::endl;
    }
    fmess.close();

    return mess;
}

unsigned Hash::hash_str(const char* s)
{
    unsigned h = FIRSTH;
    while(*s)
    {
        h = (h * A) ^ (s[0] * B);
        s++;
    }
    return h;
}


