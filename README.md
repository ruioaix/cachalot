# Cachalot

An error-handling library for C program.

Cachalot is more like a protocol rather than a library.

Here is the philosophy behind cachalot:
1. simple: only 8 APIs.
2. safe: if something wrong happens inside cachalot, the caller program will not be affected.

Here is the protocol behind cachalot:
1. If an error happens somewhere in the caller program, the caller program should call `CACHALOT_CAUSE` (and `CACHALOT_MSG`) to insert the error into cachalot and then return err to the upper calling function.
2. Depending on the context, the upper calling function should call `CACHALOT_TRACE`, `CACHALOT_WRAP` or `CACHALOT_MSG` and then return err to the upper calling function.
3. Finally, `main` function receive the error and call `CACHALOT_STRINGLIZE` to generate the error stack string, print or logging, and exit.
