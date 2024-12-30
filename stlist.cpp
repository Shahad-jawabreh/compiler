#include "stlist.h"
#include <cstring> // For strcpy, strcmp
#include <cctype>  // For std::tolower

STList::STList() {
    Counter = 0;
    Head = nullptr; // Initialize Head to nullptr
}

STEntry* STList::FindEntry(char *name, bool foldCase) {
    STEntry *ste = Head;

    while (ste != nullptr) {
        if (foldCase) {
            // Convert both name and ste->Name to lowercase for case-insensitive comparison
            char tempName[64];
            char tempSteName[64];
            strcpy(tempName, name);
            strcpy(tempSteName, ste->Name);
            for (int i = 0; tempName[i]; ++i) tempName[i] = std::tolower(tempName[i]);
            for (int i = 0; tempSteName[i]; ++i) tempSteName[i] = std::tolower(tempSteName[i]);
            if (strcmp(tempName, tempSteName) == 0) return ste;
        } else {
            // Use regular case-sensitive comparison
            if (strcmp(name, ste->Name) == 0) return ste;
        }
        ste = ste->Next;
    }

    return nullptr; // Return nullptr if not found
}

bool STList::AddEntry(char *name, STE_TYPE type, bool foldCase) {
    STEntry *ste = FindEntry(name, foldCase);
    if (ste) {
        printf("Entry %s Already Exists\n", name);
        return false; // Entry already exists
    }

    // Entry does not exist, create a new STEntry
    ste = new STEntry(name, type);
    ste->Next = Head;
    Head = ste;
    Counter++;
    return true;
}

void STList::PrintAll(FILE *fp) {
    STEntry *ste = Head;
    while (ste != nullptr) {
        ste->print(fp);
        ste = ste->Next;
    }
    fprintf(fp, "\n");
}

int STList::Count() {
    return Counter;
}

void STList::Clear() {
    STEntry *ste;
    while (Head != nullptr) {
        ste = Head;
        Head = Head->Next;
        delete ste;
    }
    Counter = 0;
}

STList::~STList() {
    Clear();
}
