#include "stable.h"
#include <cctype>
#include <iostream>

void STable::init(int size)
{
   Size = size;
   Table = new STList[size];
}
/**
 * @brief STable::STable : Default constructor, call init with DEFAULT_SIZE
 */
STable::STable()
{
    init(DEFAULT_SIZE);
}
/**
 * @brief STable::STable  constructor with Number of slots = size, call init with size
 * @param size
 */
STable::STable(unsigned long size)
{
    init(size);
}
STable::STable(unsigned long size ,int fCase)
{
    foldCase = fCase ;
    init(size);
}
int STable::isFoldCase() {
    return foldCase ;
}
/**
 * @brief STable::Reset   Recreate the Table by calling clear then call init
 * @param size
 */
void STable::Reset(unsigned long size)
{
    Clear();
    init(size);
}
/**
 * @brief STable::ElfHash : Hash Function
 * @param str  : Key which is the Variable Name to be added to the Symbol Table
 * @return
 */
unsigned long STable::ElfHash( char *str )
{
    unsigned long   h = 0, high; // h=0, g
    unsigned char *s = (unsigned char *) str;
    while ( *s )
    {
        h = ( h << 4 ) + *s++;  //1/4 of bits
        if ( high = h & 0xF0000000 )
            h ^= high >> 24; // h = h ^ (g >>24)
        h &= ~high; // in book this is h = h^g
    }
    return h%Size;
}

void toLowerCase(char *str) {
    for (; *str; ++str) *str = tolower(*str);
}

STEntry *STable::GetSymbol(char *str) {
    unsigned long hash = ElfHash(str); // Compute hash value
    STEntry *foundEntry = Table[hash].FindEntry(str,true); // Use FindEntry in STList

    return foundEntry;
}


bool STable::AddEntry(char *name, STE_TYPE type,bool foldCase) {
    char tempName[64];
    strcpy(tempName, name);
    if (foldCase) {
        toLowerCase(tempName);
    }
    unsigned long index = ElfHash(tempName);
    bool Added = Table[index].AddEntry(name, type, foldCase);
    return Added;
}


void STable::FindAndPrintEntry(char *name, FILE *fp) {
    char tempName[64];
    strcpy(tempName, name);
    if (foldCase) {
        toLowerCase(tempName);
    }
    unsigned long index = ElfHash(tempName);
    STEntry *ste = Table[index].FindEntry(tempName, foldCase);
    if (ste) {
        fprintf(fp, "%s: Found = %s\n", name, ste->toString());
    } else {
        fprintf(fp, "%s: not found \n", name);
    }
}


void STable::PrintAll(FILE *fp)
{
    unsigned long i;
    for (i =0; i < Size; i++)
    {
        fprintf(fp,"T[%lu]: %d ste:\t", i, Table[i].Count());
        Table[i].PrintAll(fp);
    }

}
/**
 * @brief STable::Clear : Delete All Elements, use a loop and call the Table[i].clear. Then Delete the Array Table
 */
void STable::Clear()
{
    unsigned long i;
    for (i =0; i < Size; i++)
    {
        Table[i].Clear();
    }
    delete[]Table;

}
void STable::PrintSymbolStats(FILE* fp) {
    unsigned long totalEntries = 0;
    unsigned long freeSlots = 0;
    unsigned long maxChainLength = 0;
    unsigned long totalChainLength = 0;
    unsigned long nonEmptySlots = 0;

    for (unsigned long i = 0; i < Size; ++i) {
        int count = Table[i].Count();
        totalEntries += count;
        if (count == 0) {
            freeSlots++;
        } else {
            nonEmptySlots++;
            if (count > maxChainLength) {
                maxChainLength = count;
            }
            totalChainLength += count;
        }
    }

    double averageChainLength = nonEmptySlots > 0 ? (double)totalChainLength / nonEmptySlots : 0.0;

    fprintf(fp, "Symbol Table Statistics:\n");
    fprintf(fp, "Total Entries: %lu\n", totalEntries);
    fprintf(fp, "Number of Free Slots: %lu\n", freeSlots);
    fprintf(fp, "Longest Chain Length: %lu\n", maxChainLength);
    fprintf(fp, "Average Chain Length: %.2f\n", averageChainLength);
}

int STable::hashFunction(const char* str, int size) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % size;
}


STable::~STable()
{
    Clear();
}


