# cardparse

A tiny C++ parser for a simple contact-card format (`.card`). It reads a
line-based file, fills in a small record, and can export that record back to
disk.

**cardparse is a deliberately vulnerable sample application.** It exists to be
a small, self-contained fuzzing target: easy to build, easy to feed input to,
and easy to crash. Do not use it for anything real.

## The format

A `.card` file is a list of `key:value` lines. Blank lines and lines starting
with `#` are ignored.

```
# a simple card
name:Ada Lovelace
title:Mathematician
note:first programmer
```

Supported keys:

| Key     | Meaning                                                        |
|---------|----------------------------------------------------------------|
| `name`  | Sets the card name.                                            |
| `title` | Sets the card title.                                           |
| `note`  | Sets a free-text note.                                         |
| `data`  | `data:<len>:<bytes>` loads a raw data blob of `<len>` bytes.   |
| `save`  | `save:<path>` writes the current card to `<path>`.            |

## Build

```sh
make            # plain build -> ./cardparse
make asan       # AddressSanitizer build -> ./cardparse-asan
make fuzz       # libFuzzer build (needs clang) -> ./cardparse-fuzz
```

## Run

Read a file:

```sh
./cardparse examples/sample.card
```

Or read from stdin:

```sh
cat examples/sample.card | ./cardparse
```

## Fuzzing

### libFuzzer

```sh
make fuzz
mkdir -p findings
./cardparse-fuzz -artifact_prefix=findings/ corpus/
```

### AFL++

```sh
afl-g++-fast -O1 -g -fno-stack-protector -o cardparse-afl src/cardparse.cpp
afl-fuzz -i corpus -o out -- ./cardparse-afl @@
```

## Known bug classes

These are intentional. A fuzzer should surface them quickly.

- **Stack buffer overflow (fields).** `name`, `title`, and `note` are copied
  into fixed-size struct buffers with no length check, so a long value writes
  past the buffer and smashes adjacent stack memory.
- **Stack buffer overflow (data blob).** `data:<len>:<bytes>` trusts the
  caller-supplied `<len>` and `memcpy`s that many bytes into a 32-byte stack
  buffer, giving an attacker-controlled overflow length.
- **Arbitrary file write / path traversal.** `save:<path>` uses the path
  verbatim, so a card can write to any location the process can reach
  (for example `save:../../tmp/pwned` or an absolute path).

## License

MIT. See [LICENSE](LICENSE).
