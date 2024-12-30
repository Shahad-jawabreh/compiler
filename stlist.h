#ifndef STLIST_H
#define STLIST_H

#include "stentry.h" // Assuming STEntry and STE_TYPE are defined here
#include <cctype>    // For std::tolower

class STList {
private:
    int Counter;    // counts the Nodes in the List
    STEntry *Head;  // Should be Initialized to Null in the Constructor

public:
    STList();
    STEntry* FindEntry(char *name, bool foldCase); // Return NULL if not found, with optional case-folding
    bool AddEntry(char *name, STE_TYPE type, bool foldCase); // Adds an entry if the Node does not exist, with optional case-folding
    void PrintAll(FILE *fp);
    int Count();
    void Clear();
    ~STList();
};

#endif // STLIST_H
