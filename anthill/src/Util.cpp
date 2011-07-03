#include <string>
#include <sstream>
#include <cstring>
#include <iostream>
#include "Util.h"

// =================== //

char* 
buildEowMsg()
{
    char* cMsg = new char [3];
    strcpy(cMsg, "-1");
    std::cout << "=============== buildEoWMsg ===============" << std::endl;
    std::cout << "=> Final MSG: " << cMsg << std::endl;
    return cMsg;
}

// =================== //

char* 
list2Msg(std::list<IntSet> l, int id)
{
    std::stringstream ss;
    std::string sMsg;

    std::cout << "============= list2MSg =============" << std::endl;

    ss << id << " ";
    ss << l.size() << " ";
    std::cout << "> Id: " << id << std::endl;
    std::cout << "> List Size: " << l.size() << std::endl;

    std::list<IntSet>::iterator it;
    for (it = l.begin(); it != l.end(); it++)
    {
        std::cout << "> IS size: " << it->size() << std::endl;
        std::cout << "> Elements: ";
        ss << it->size() << " ";
        IntSet::iterator it2;
        for (it2 = it->begin(); it2 != it->end(); it2++)
        {
            ss << *it2 << " ";
            std::cout << *it2 << " ";
        }
        std::cout << std::endl;
    }
  
    sMsg = ss.str();
    char* msg = new char [sMsg.length()+1];
    strcpy(msg, sMsg.c_str());

    std::cout << "> Final MSG: " << msg << std::endl;

    return msg;
}

// =================== //

std::list<IntSet> 
msg2List(char* msg)
{
    int i;
    int numSets;
    int setSize;
    std::stringstream ss(msg);
    std::list<IntSet> results;

    std::cout << "============= msg2List =============" << std::endl;
    std::cout << "==> Original MSG: " << msg << std::endl;
    ss >> i; // Ignore pId
    std::cout << " ==> Id: " << i << std::endl;
    ss >> numSets;
    std::cout << " ==> NumSets: " << numSets << std::endl;

    while (numSets-- > 0) 
    {
        IntSet set;
        ss >> setSize;
        std::cout << " ==> SetSize: " << setSize << std::endl;
        std::cout << " ==> Elements: "; 
        while (setSize-- > 0)
        {
            ss >> i;
            set.push_back(i);
            std::cout << i << " ";
        }
        results.push_back(set);
        std::cout << std::endl;
    }

    return results;
}

// =================== //

int 
msg2Id(char* msg)
{
    int id;
    std::stringstream ss(msg);
    ss >> id;
    return id;
}

// =================== //

