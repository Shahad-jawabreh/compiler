#ifndef STABLE_H
#define STABLE_H
#include "stlist.h"
#define DEFAULT_SIZE 19
class STable
{
private:
    unsigned long Size;
    STList *Table;   // Dynamic Array of size = Size
    void init(int size);
    unsigned long ElfHash(char *str);
    int foldCase = 0;
    int number_entries; // Number of entries in table
    int number_probes = 0; // Number of probes into table
    int number_hits = 0; // Number of probes that immediately found entry
    int max_search_dist = -1; // Maximum entries searched
public:
    STable();
    ~STable();
    STable(unsigned long size);
    STable( unsigned long size , int foldCase);
    void Reset(unsigned long size);
    bool AddEntry(char *name, STE_TYPE type , bool foldCase);
    void FindAndPrintEntry(char *name, FILE *fp);//finds and prints the Entry if it exist
    void PrintAll(FILE *fp);
    void PrintSymbolStats (FILE* fp);
    int isFoldCase();
    STEntry *GetSymbol(char *str); 
    int hashFunction(const char* str, int size);
    void Clear();
};

#endif // STABLE_H
