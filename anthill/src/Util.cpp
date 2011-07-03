#include <string>
#include <sstream>
#include <cstring>
#include <iostream>
#include "Util.h"

// =================== //

int* 
buildEowMsg(size_t& msgSize)
{
    msgSize = 1*sizeof(int);
    int* pMsg = new int(EOW_ID);
    return pMsg;
}

// =================== //

/**
 * Create message from a list of candidates.
 * Format: ID list_size [X.size candExt.size x1 x2 ... candExt1 ...] ...
 * Return the msg size on msgSize parameter
 */
int* 
list2Msg(std::list<cand_t> l, int id, size_t& msgSize)
{
    // Compute list size
    int count  = 0;
    count += 2; // Add space to id and list size.
    std::list<cand_t>::iterator it;
    IntSet::iterator it2;
    for (it = l.begin(); it != l.end(); it++)
    {
        int cand_size = 2; // X size and candExt size
        cand_size += it->X.size();
        cand_size += it->candExt.size();
        count += cand_size;
    }

    // Create message
    int* pMsg = new int [count];

    // Fill id and list size
    pMsg[0] = id;
    pMsg[1] = l.size();
    
    // Fill the values
    int idx = 2;
    for (it = l.begin(); it != l.end(); it++)
    {
        pMsg[idx++] = it->X.size();
        pMsg[idx++] = it->candExt.size();
        for (it2 = it->X.begin(); it2 != it->X.end(); it2++)
        {
            pMsg[idx++] = *it2;
        }
        for (it2 = it->candExt.begin(); it2 != it->candExt.end(); it2++)
        {
            pMsg[idx++] = *it2;
        }
    }
 
    msgSize = sizeof(int)*count;

    return pMsg;
}

// =================== //

/**
 * Create a list of candidates from the message
 */
std::list<cand_t> 
msg2List(int* msg)
{
    int numCands;
    std::list<cand_t> results;

    numCands = msg[1];
    int idx = 2;
    // Grab each candidate and put on the list
    while (numCands-- > 0) 
    {
        cand_t currCand;
        int X_size = msg[idx++];
        int candExt_size = msg[idx++];

        // Grab X elements
        while (X_size-- > 0)
        {
            currCand.X.push_back(msg[idx++]);
        }

        // Grab candExt elements
        while (candExt_size-- > 0)
        {
            currCand.candExt.push_back(msg[idx++]);
        }

        results.push_back(currCand);
    }

    return results;
}

// =================== //

int 
msg2Id(int* msg)
{
    return msg[0];
}

// =================== //
// ================================================================= //
// =================== //

char* 
buildEowMsg2()
{
    char* cMsg = new char [3];
    strcpy(cMsg, "-1");
    std::cout << "=============== buildEoWMsg ===============" << std::endl;
    std::cout << "=> Final MSG: " << cMsg << std::endl;
    return cMsg;
}

// =================== //

char* 
list2Msg2(std::list<IntSet> l, int id)
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
msg2List2(char* msg)
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
msg2Id2(char* msg)
{
    int id;
    std::stringstream ss(msg);
    ss >> id;
    return id;
}

// =================== //

