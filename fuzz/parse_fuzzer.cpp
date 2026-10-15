// libFuzzer harness for the whole parser.
//
// Build: make fuzz  (links against the parser in src/cardparse.cpp)

#include "../src/cardparse.h"

#include <cstdio>
#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    FILE *in = fmemopen(const_cast<uint8_t *>(data), size, "r");
    if (in) {
        parse_stream(in);
        fclose(in);
    }
    return 0;
}
