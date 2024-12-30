#include "fd.h"
#include <iostream> 
FileDescriptor::FileDescriptor(const char* fileName) {
    buf_size = BUFFER_SIZE;  
    buffer = new char[buf_size];
    file = new char[FILENAME_MAX];
    error = new char[buf_size];

    strcpy_s(file, FILENAME_MAX, fileName);  
    char_number = 0;
    line_number = 0;

    char* filePath = new char[strlen(fileName) + 1]; // Add 1 for the null terminator
    strcpy_s(filePath, strlen(fileName) + 1, fileName);

    // Use fopen_s for safer file opening
    if (fopen_s(&fp, filePath, "r") != 0) {
        fp = nullptr;
        strcpy_s(error, buf_size, "Please open the file first");
    }
    else {
        error[0] = '\0';  // No error
    }

    delete[] filePath;  
}

FileDescriptor::FileDescriptor() {
    const char* defaultFileName = "main";
    buf_size = BUFFER_SIZE;  // Initialize buf_size before using it
    buffer = new char[buf_size];
    file = new char[FILENAME_MAX];
    error = new char[buf_size];

    strcpy_s(file, FILENAME_MAX, defaultFileName);  
    char_number = 0;
    line_number = 0;

    char* filePath = new char[strlen(defaultFileName) + 5]; 
    strcpy_s(filePath, strlen(defaultFileName) + 5, defaultFileName);
    strcat_s(filePath, strlen(defaultFileName) + 5, ".txt");

    if (fopen_s(&fp, filePath, "r") != 0) {
        fp = nullptr;
        strcpy_s(error, buf_size, "Please open the file first");
    }
    else {
        error[0] = '\0';  // No error
    }

    delete[] filePath;
}

FileDescriptor::~FileDescriptor() {
    if (fp != nullptr) {
        fclose(fp);
    }
    delete[] buffer;
    delete[] file;
    delete[] error;
}
char* FileDescriptor::getFileName() {
    if (isOpen())
        return file;
    else {
        strcpy_s(error, buf_size, "Please open the file first");
        return error;
    }
}

bool FileDescriptor::isOpen() const {
    return fp != nullptr;
}

char* FileDescriptor::getCurrLine() {
    return buffer;
}

int FileDescriptor::getLineNum() const {
    return line_number;
}
int FileDescriptor::getCharNum() const {
    return char_number;
}

void FileDescriptor::close() {
    if (fp != nullptr) {
        fclose(fp);
        fp = nullptr;
    }
}

char FileDescriptor::getChar() {
    if (fp == nullptr || feof(fp)) {
        return EOF;
    }

    char currentChar = (char)fgetc(fp);
    if (currentChar == EOF) {
        return EOF;
    }

    buffer[char_number] = currentChar;

    if (currentChar == '\n') {
        char_number = 0;
        line_number++;
    }
    else {
        char_number++;
    }
    buffer[char_number] = '\0';
    return currentChar;
}

void FileDescriptor::ungetChar(char c) {
    if (fp != nullptr && fseek(fp, -1, SEEK_CUR) == 0) {
        if (c == '\n') {
            line_number--;
        }
        char_number--;
    }
}

void FileDescriptor::reportError(const char* msg) {
    std::cerr << "Error: " << msg << " in line " << line_number << std::endl;

    std::cerr << "Error -> " << buffer << std::endl;

    for (int i = 0; i < char_number - 1; i++) {
        std::cerr << " ";
    }
    std::cerr << "        ^" << std::endl;
}


int FileDescriptor::getBufferSize() const {
    return buf_size;
}