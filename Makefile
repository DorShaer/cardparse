CXX ?= g++
CLANG ?= clang++

# The parser (with the bugs) is a standalone unit; the CLI main lives separately
# so a fuzz harness can link the parser without a duplicate main().
LIB := src/cardparse.cpp
CLI := src/cli.cpp

# Plain build. Stack protector and _FORTIFY_SOURCE are disabled so the overflow
# bugs are easy to trigger and study rather than being turned into clean aborts.
# Keep optimisation low so stack layout stays predictable.
CXXFLAGS ?= -O1 -g -fno-stack-protector -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -Wno-deprecated-declarations

.PHONY: all asan fuzz clean

all: cardparse

cardparse: $(LIB) $(CLI) src/cardparse.h
	$(CXX) $(CXXFLAGS) -o $@ $(LIB) $(CLI)

# AddressSanitizer build for crash triage from the fuzzer.
asan: $(LIB) $(CLI) src/cardparse.h
	$(CXX) -O1 -g -fsanitize=address -fno-omit-frame-pointer -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -o cardparse-asan $(LIB) $(CLI)

# libFuzzer build. Requires clang. Links the harness against the parser.
fuzz: $(LIB) fuzz/parse_fuzzer.cpp src/cardparse.h
	$(CLANG) -O1 -g -fsanitize=address,fuzzer -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0 -o cardparse-fuzz $(LIB) fuzz/parse_fuzzer.cpp

clean:
	rm -f cardparse cardparse-asan cardparse-fuzz
	rm -rf crash-* leak-* oom-*
