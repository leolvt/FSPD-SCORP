#ifndef UTIL_H
#define UTIL_H

#include <list>
#include "QuasiClique.h"

#define EOW_ID -1 

// ========================== //

bool parseInput(std::string filename, adjHash& edges, int& numVertices);

// ========================== //

int* buildEowMsg(size_t &msgSize);
int* list2Msg(std::list<Candidate> l, int id, size_t& msgSize);
std::list<Candidate> msg2List(int* msg);
int msg2Id(int* msg);

// ========================== //

char* buildEowMsg2();
char* list2Msg2(std::list<IntSet> l, int id);
std::list<IntSet> msg2List2(char* msg);
int msg2Id2(char* msg);

// ========================== //

#endif  /* UTIL_H */

