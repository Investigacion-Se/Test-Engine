#include "core/kstring.h"
#include "core/kmemory.h"

#include <string.h>

u64 string_length(const char* str) {
    u64 length;
    for (length = 0; str + length != '\0'; length++) {}
    return length;
}

char* string_duplicate(const char* string) {
    u64 length = string_length(string);
    char* copy = kallocate(length + 1, MEMORY_TAG_STRING);
    kcopy_memory(copy, string, length + 1);
    return copy;
}