#include "Scanner.h"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <regex> // Include regex library for pattern matching

SCANNER::SCANNER() : fd(nullptr), line_number(1) {}

SCANNER::SCANNER(FileDescriptor* fdd) : fd(fdd), line_number(1) {}

bool SCANNER::check_keyword(const char* str) {
    for (int i = 0; i < keys; i++) {
        if (std::strcmp(str, keyword[i]) == 0) {
            return true;
        }
    }
    return false;
}

bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
}

bool is_delimiter(char c) {
    return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == '.' ||
        c == ';' || c == ',' || c == '+' || c == '-' || c == '*' || c == '/' || c == '=' ||
        c == ':' || c == '!' || c == '<' || c == '>';
}
bool is_dot(char c) {
    return  c == '.';
}
TOKEN* SCANNER::get_float(char* initialBuffer, int initialLength) {
    TOKEN* token = new TOKEN();
    char buffer[256];
    int l = initialLength;
    std::memcpy(buffer, initialBuffer, l); // Copy the initial buffer contents
    int c = fd->getChar();

    if (initialLength == 0 && is_digit(c)) {
        buffer[l++] = c;
        c = fd->getChar();
    }

    while (is_digit(c)) {
        if (l < 255) {
            buffer[l++] = c;
        }
        c = fd->getChar();
    }

    if (c == '.') {
        buffer[l++] = c;
        c = fd->getChar();
        if (!is_digit(c)) {
            fd->reportError("Invalid floating-point number: no digits after dot");
            delete token;
            return nullptr;
        }

        while (is_digit(c)) {
            if (l < 255) {
                buffer[l++] = c;
            }
            c = fd->getChar();
        }
    }

    buffer[l] = '\0';
    fd->ungetChar(c);

    if (buffer[l - 1] == '.') {
        fd->reportError("Invalid floating-point number: ends with dot");
        delete token;
        return nullptr;
    }

    token->type = lx_float;
    token->float_value = std::atof(buffer);
    return token;
}

TOKEN* SCANNER::get_int() {
    TOKEN* token = new TOKEN();
    char buffer[256] = {};
    int l = 0; // length of string
    int c = fd->getChar();

    if (is_digit(c)) {
        buffer[l++] = c;
        c = fd->getChar();

        while (is_digit(c)) {
            if (l < 255) {
                buffer[l++] = c;
            }
            c = fd->getChar();
        }

        if (c == '.') {
            buffer[l++] = c;
            return get_float(buffer, l); // Handle as float if there is a dot
        }

        buffer[l] = '\0';
        if (is_letter(c) || c == '_') {
            fd->reportError("Invalid identifier starting with a number");
            delete token;

            while (!is_space(c) && !is_delimiter(c) && c != EOF) {
                c = fd->getChar();
            }
            return nullptr;
        }

        fd->ungetChar(c);
        token->type = lx_integer;
        token->value = std::atoi(buffer);
        return token;
    }
    else {
        delete token;
        return nullptr;
    }
}


TOKEN* SCANNER::get_id() {
    TOKEN* token = new TOKEN();
    char buffer[256]; // to store the identifier
    int l = 0; // length of string
    int c = fd->getChar();
    if (c == '_' || is_letter(c)) {
        buffer[l++] = c;
        c = fd->getChar();
        while (is_letter(c) || is_digit(c) || c == '_') {
            if (l < 255) {
                buffer[l++] = c;
            }
            c = fd->getChar();
        }
        buffer[l] = '\0';
        fd->ungetChar(c);
        if (check_keyword(buffer)) {
            for (int i = 0; i < keys; ++i) {
                if (std::strcmp(buffer, keyword[i]) == 0) {
                    token->type = key_type[i];
                    token->str_ptr = nullptr;
                    return token;
                }
            }
        }
        else {
            token->type = lx_identifier;
            token->str_ptr = new char[l + 1];
            strcpy_s(token->str_ptr, l + 1, buffer); // Use strcpy_s
            return token;
        }
    }
    else {
        fd->reportError("Invalid identifier");
        delete token;
        return nullptr;
    }
    return token;
}

void SCANNER::skip_short_comment() {
    char ch = fd->getChar();
    while (ch != EOF) {
        if (ch == '#' && fd->getChar() == '#') {
            break;
        }
        ch = fd->getChar();
    }
}
void SCANNER::skip_comments() {
    int count = 0;
    char ch = fd->getChar();
    
    while (ch != EOF) {
        if(ch == '#') {
            count++;
        }
        if(count==2) {
            count =0 ;
            break ;
        }
        if (ch == '\n') {
            line_number++;
            break;
        }
        ch = fd->getChar();
    }
}
TOKEN* SCANNER::get_string() {
    TOKEN* token = new TOKEN();
    char buffer[256] = {};
    int l = 0; // length of string
    int c = fd->getChar();

    if (c == '"') {
        c = fd->getChar(); // move past the initial "

        while (c != '"' && c != EOF) {
            if (l < 255) {
                buffer[l++] = c;
            }
            c = fd->getChar();
        }

        if (c == '"') {
            buffer[l] = '\0';
            token->type = lx_string;
            token->str_ptr = new char[l + 1];
            strcpy_s(token->str_ptr, l + 1, buffer);
            return token;
        }
        else {
            fd->reportError("String not terminated properly");
            delete token;
            return nullptr;
        }
    }
    else {
        delete token;
        return nullptr;
    }

    return token;
}
TOKEN* SCANNER::Scan() {
    TOKEN* token = nullptr;
    int ch = fd->getChar();
    while (ch != EOF) {

        if (is_space(ch)) {
            if (ch == '\n') {
                line_number++;
            }
            ch = fd->getChar();
            continue;
        }

        if (ch == '#') {
            ch = fd->getChar();
            if (ch == '#') {
                skip_comments();
                ch = fd->getChar();
                continue;
            }
            else {
                fd->reportError("Unknown character");
                continue;
            }
        }


        if (is_digit(ch)) {
            fd->ungetChar(ch);
            token = get_int();
            if (token) {
                return token;
            }
        }
        else if (ch == '"') {
            fd->ungetChar(ch);
            token = get_string();
            if (token) {
                return token;
            }
        }
        else if (is_letter(ch) || ch == '_') {
            fd->ungetChar(ch);
            token = get_id();
            if (token) {
                return token;
            }
        }
        else if (ch == '"') {
            fd->ungetChar(ch);
            token = get_string();
            if (token) {
                return token;
            }
        }
        else if (is_letter(ch) || ch == '_') {
            fd->ungetChar(ch);
            token = get_id();
            if (token) {
                return token;
            }
        }
        else {
            token = new TOKEN();
            switch (ch) {
            case ':':
                if (fd->getChar() == '=') {
                    token->type = lx_colon_eq;
                }
                else {
                    token->type = lx_colon;
                    fd->ungetChar(ch);
                }
                break;
            case '!':
                if (fd->getChar() == '=') {
                    token->type = lx_neq;
                }
                else {
                    fd->ungetChar(ch);
                    token->type = unknown;
                }
                break;
            case '<':
                if (fd->getChar() == '=') {
                    token->type = lx_le;
                }
                else {
                    token->type = lx_lt;
                    fd->ungetChar(ch);
                }
                break;
            case '>':
                if (fd->getChar() == '=') {
                    token->type = lx_ge;
                }
                else {
                    token->type = lx_gt;
                    fd->ungetChar(ch);
                }
                break;
            case '(':
                token->type = lx_lparen;
                break;
            case ')':
                token->type = lx_rparen;
                break;
            case '[':
                token->type = lx_lbracket;
                break;
            case ']':
                token->type = lx_rbracket;
                break;
            case '{':
                token->type = lx_lbrace;
                break;
            case '}':
                token->type = lx_rbrace;
                break;
            case '.':
                token->type = lx_dot;
                break;
            case ';':
                token->type = lx_semicolon;
                break;
            case ',':
                token->type = lx_comma;
                break;
            case '+':
                token->type = lx_plus;
                break;
            case '-':
                token->type = lx_minus;
                break;
            case '*':
                token->type = lx_star;
                break;
            case '/':
                token->type = lx_slash;
                break;
            case '=':
                token->type = lx_eq;
                break;
            case '?':
            case '$':
            case '%':
                fd->reportError("Unknown character");
                delete token;
                ch = fd->getChar();
                continue;
            default:
                fd->reportError("Unrecognized character");
                delete token;
                ch = fd->getChar();
                continue;
            }
            return token;
        }

        ch = fd->getChar();
    }

    if (ch == EOF) {
        std::cerr << "Reached end of file." << std::endl;
        token = new TOKEN();
        token->type = lx_eof;
        token->value = 0;
        token->float_value = 0.0;
        return nullptr;
    }

    return token;
}