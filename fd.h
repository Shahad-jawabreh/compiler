#ifndef FD_H
#define FD_H

#include <cstdio>  // for FILE, fopen, fgetc, fseek, etc.
#include <cstring> // for strcpy_s, strcat_s

class FileDescriptor {
public:
    FileDescriptor(const char* fileName);
    FileDescriptor();
    ~FileDescriptor();

    char* getFileName();
    bool isOpen() const;
    char* getCurrLine();
    int getLineNum() const;
    int getCharNum() const;
    void close();

    char getChar();
    void reportError(const char* msg);
    void ungetChar(char c);
    int getBufferSize() const;

private:
    FILE* fp;
    char* buffer;
    char* file;
    char* error;
    int buf_size;
    int char_number;
    int line_number;

    static const int BUFFER_SIZE = 256;
};

#endif // FD_H
