// cardparse CLI entry point.
//
// Reads a .card file (path argument) or stdin and parses it. The parser and
// its bugs live in cardparse.cpp; keeping main here means a fuzz harness can
// link the parser functions without colliding with this main().

#include "cardparse.h"

#include <cstdio>

int main(int argc, char **argv) {
    FILE *in = stdin;
    if (argc > 1) {
        in = fopen(argv[1], "r");
        if (!in) {
            fprintf(stderr, "cannot open %s\n", argv[1]);
            return 1;
        }
    }

    parse_stream(in);

    if (in != stdin) {
        fclose(in);
    }
    return 0;
}
