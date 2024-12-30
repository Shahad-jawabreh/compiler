#include <stdlib.h>
#include "stable.h"
#include <ctime>
#include <iostream>
using namespace std;

int getIdetifier(FILE* fp, char *name, STE_TYPE& type) {
    char s1[64], s2[64], s3[64],s4[64], s5[64];
    int n = fscanf(fp, "%s%s%s%s%s", s1, s2, s3,s4,s5);
    cout << s1 << s2 <<s3 << s4 <<s5 ;
    strcpy(name, s2);
    type = STEntry::getType(s1);
    return n;
}

char getRandChar(bool IsFirst = false) {
    int index = 0;
    char ch;
    if (IsFirst) index = rand() % 26;
    else index = rand() % (26 + 10);
    if (index < 26) ch = 'A' + index;
    else ch = '0' + (index - 26);
    return ch;
}

void getRandIdentifier(char *str, STE_TYPE& type) {
    int i;
    str[0] = getRandChar(true);
    for (i = 1; i < 8; i++) {
        str[i] = getRandChar();
    }
    str[i] = 0;
    type = (STE_TYPE)(rand() % TYPE_SIZE);
}

int main(int argc, char *argv[]) {
    STable ST;

    FILE *fp;
    STE_TYPE type;
    char str[64];

    fp = fopen("t.txt", "r");
    if (!fp) return 0;
    FILE *fout = fopen("out.txt", "w");

    fprintf(fout, "\n**(Test 1)**: Reading from the file t.txt and creating a symbol table with size = 19 (default)\n\n");

    int n;
    bool Added;
    while (!feof(fp)) {
        n = getIdetifier(fp, str, type);
        if (n == 5) {
            Added = ST.AddEntry(str, type, true);
            if (!Added) fprintf(fout, "Entry %s Already Exists\n", str);
        }
    }
    fclose(fp);

    fprintf(fout, "\n==============\n");
    ST.PrintAll(fout);
    fprintf(fout, "\n==============\n");
    ST.PrintSymbolStats(fout);
    fprintf(fout, "\n\n**(Test 2)**: Searching for some Variables in the Symbol Table Created\n\n");
    ST.FindAndPrintEntry("variable1111", fout);
    ST.FindAndPrintEntry("variable1111", fout);
    ST.FindAndPrintEntry("jklm", fout);
    ST.FindAndPrintEntry("x", fout);
    ST.FindAndPrintEntry("xx12", fout);
    
     fprintf(fout, "\n------------ clear ST -----------\n");
     ST.Reset(19);
     ST.PrintAll(fout);
    
//    fprintf(fout, "\n------------ Get Element -----------\n");
//    char searchStr[] = "xc";
//    STEntry *foundEntry = ST.GetSymbol(searchStr);
//
//    if (foundEntry != NULL) {
//        // Symbol found, do something with foundEntry
//        fprintf(fout, "Symbol %s found!", searchStr);
//    } else {
//        // Symbol not found
//        fprintf(fout, "Symbol %s not found!", searchStr);
//    }
 
/*
    fprintf(fout, "\n\n**(Test 3)**: Creating 8 Symbol Tables with different Slot Size and Different Sizes\n");
    fprintf(fout, "            : Random Variable Names are generated\n");
    srand(time(0));

    int k, i;
    int Sizes[10] = { 29, 29, 17, 23, 37, 31, 101, 101, 101, 101 };
    int ElementsCount[10] = { 100, 100, 100, 100, 100, 150, 300, 100, 200, 50 };

    STable ST2(Sizes[8], 0);
    for (k = 0; k < 10; k++) {
        if (k != 0) ST2.Reset(Sizes[k]);
        for (i = 0; i < ElementsCount[k]; i++) {
            getRandIdentifier(str, type);
            Added = ST2.AddEntry(str, type, true);
            if (!Added) fprintf(fout, "Entry %s Already Exists\n", str);
        }
        fprintf(fout, "\n===== Table (%d) =======\n", k);
        ST2.PrintAll(fout);
        
    }
  */ 
    fprintf(fout, "\n------------------done-------------------\n");
    fclose(fout);
    return 0;
}
