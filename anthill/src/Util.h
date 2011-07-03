#ifndef UTIL_H
#define UTIL_H

#include <list>
#include <vector>

// TODO: Find better place for this
typedef std::vector<int> IntSet;

char* buildEowMsg();
char* list2Msg(std::list<IntSet> l, int id);
std::list<IntSet> msg2List(char* msg);
int msg2Id(char* msg);

#endif  /* UTIL_H */

