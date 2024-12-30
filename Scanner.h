#ifndef SCANNER_H
#define SCANNER_H

#include "fd.h"

typedef enum {
    /* Literals */
    lx_identifier, lx_integer, lx_string, lx_float,
    /* Keywords */
    kw_program, kw_var, kw_constant, kw_integer, kw_boolean, kw_string, kw_float,
    kw_true, kw_false, kw_if, kw_fi, kw_then, kw_else,
    kw_while, kw_do, kw_od,
    kw_and, kw_or,
    kw_read, kw_write,
    kw_for, kw_from, kw_to, kw_by,
    kw_function, kw_procedure, kw_return, kw_not, kw_begin, kw_end,

    lx_lparen, lx_rparen, lx_lbracket, lx_rbracket,
    lx_colon, lx_dot, lx_semicolon, lx_comma, lx_colon_eq,
    lx_plus, lx_minus, lx_star, lx_slash,
    lx_eq, lx_neq, lx_lt, lx_le, lx_gt, lx_ge, lx_rbrace,
    lx_eof, lx_error, lx_lbrace, unknown
} LEXEM_TYPE;

struct TOKEN {
    LEXEM_TYPE type;
    int value;
    float float_value;
    char* str_ptr;
};

class SCANNER {
private:
    FileDescriptor* fd;
    int line_number;
    void  skip_comments();
    TOKEN* get_id();
    TOKEN* get_float(char* initialBuffer, int initialLength);
    void skip_short_comment();
    TOKEN* get_string();
    TOKEN* get_int();
    bool check_keyword(const char* str);

public:
    SCANNER();
    SCANNER(FileDescriptor* fd);
    TOKEN* Scan();
};

static const char* TOKEN_NAMES[] = {
    "lx_identifier", "lx_integer", "lx_string", "lx_float", "kw_program",
    "kw_var", "kw_constant", "kw_integer", "kw_boolean", "kw_string",
    "kw_float", "kw_true", "kw_false", "kw_if", "kw_fi", "kw_then",
    "kw_else", "kw_while", "kw_do", "kw_od", "kw_and", "kw_or", "kw_read",
    "kw_write", "kw_for", "kw_from", "kw_to", "kw_by", "kw_function",
    "kw_procedure", "kw_return", "kw_not", "kw_begin", "kw_end",
    "lx_lparen", "lx_rparen", "lx_lbracket", "lx_rbracket", "lx_colon",
    "lx_dot", "lx_semicolon", "lx_comma", "lx_colon_eq", "lx_plus",
    "lx_minus", "lx_star", "lx_slash", "lx_eq", "lx_neq", "lx_lt",
    "lx_le", "lx_gt", "lx_ge", "lx_eof","lx_rbrace","lx_minus","unknown"
};

static const int keys = 30; // number of keywords
static const char* keyword[] = {
    "and", "begin", "boolean", "by", "constant", "do", "else", "end",
    "false", "fi", "float", "for", "from", "function", "if", "integer",
    "not", "od", "or", "procedure", "program", "read", "return", "string",
    "then", "to", "true", "var", "while", "write"
};

static const LEXEM_TYPE key_type[] = {
    kw_and, kw_begin, kw_boolean, kw_by, kw_constant, kw_do, kw_else,
    kw_end, kw_false, kw_fi, kw_float, kw_for, kw_from, kw_function,
    kw_if, kw_integer, kw_not, kw_od, kw_or, kw_procedure, kw_program,
    kw_read, kw_return, kw_string, kw_then, kw_to, kw_true, kw_var,
    kw_while, kw_write
};

#endif // SCANNER_H
