#ifndef UTIL_H
#define UTIL_H

#include <list>
#include <vector>

#define EOW_ID -1 

// TODO: Find better place for this
typedef std::vector<int> IntSet;
typedef struct {
    IntSet X;
    IntSet candExt;
} cand_t;

// ========================== //

int* buildEowMsg(size_t &msgSize);
int* list2Msg(std::list<cand_t> l, int id, size_t& msgSize);
std::list<cand_t> msg2List(int* msg);
int msg2Id(int* msg);

// ========================== //

char* buildEowMsg2();
char* list2Msg2(std::list<IntSet> l, int id);
std::list<IntSet> msg2List2(char* msg);
int msg2Id2(char* msg);

// ========================== //

#endif  /* UTIL_H */

