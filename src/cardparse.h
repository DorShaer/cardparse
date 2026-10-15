// cardparse - public surface for the deliberately vulnerable .card parser.
//
// These helpers have external linkage so a fuzz harness can call each one
// directly as a compiled function. The memory-safety bugs live in the
// definitions in cardparse.cpp.

#ifndef CARDPARSE_H
#define CARDPARSE_H

#include <cstdio>

struct Card {
    char name[64];
    char title[64];
    char note[256];
};

// Strip a trailing newline (and carriage return) in place.
void trim_newline(char *s);

// Copy a field value into a fixed destination buffer.
// The value length is never checked against the destination size.
void set_field(char *dst, const char *value);

// Decode a "data:<len>:<bytes>" record into a small working buffer.
// The length is read from the record and trusted as-is.
void load_data(const char *spec);

// Write the current card to a path taken from the file.
void save_card(const Card *c, const char *path);

// Parse a stream of "key:value" lines into a single card.
void parse_stream(FILE *in);

#endif // CARDPARSE_H
