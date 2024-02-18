#include "core/kstring.h"
#include "core/kmemory.h"

#include <string.h>

u64 string_length(const char* str) {
    return strlen(str);
}

char* string_duplicate(const char* string) {
    u64 length = string_length(string);
    char* copy = kallocate(length + 1, MEMORY_TAG_STRING);
    kcopy_memory(copy, string, length + 1);
    return copy;
}

b8 strings_equal(const char* str, const char* other) {
    return strcmp(str, other) == 0;
}