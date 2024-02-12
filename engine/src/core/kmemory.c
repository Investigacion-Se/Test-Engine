#include "core/kmemory.h"

#include "core/logger.h"
#include "platform/platform.h"

// TODO: Custom string lib
#include <string.h>
#include <stdio.h>

struct memory_stats
{
    u64 total_allocated;
    u64 tagged_allocations[MEMORY_TAG_MAX_TAGS];
};

static const char* memory_tag_strings[MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN    ",
    "ARRAY      ",
    "DARRAY     ",
    "DICT       ",
    "RING_QUEUE ",
    "BST        ",
    "STRING     ",
    "APPLICATION",
    "JOB        ",
    "TEXTURE    ",
    "MAT_INST   ",
    "RENDERER   ",
    "GAME       ",
    "TRANSFORM  ",
    "ENTITY     ",
    "ENTITY_NODE",
    "SCENE      "};

static struct memory_stats stats;

void initialize_memory() {
    platform_zero_memory(&stats, sizeof(struct memory_stats));
}

void shutdown_memory() {}

void* kallocate(u64 size, memory_tag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        KWARN("kallocate called with MEMORY_TAG_UNKNOWN. Re-class this allocation");
    }

    stats.total_allocated += size;
    stats.tagged_allocations[tag] += size;

    // TODO: memory aligment
    void* block = platform_allocate(size, FALSE);
    platform_zero_memory(block, size);
    return block;
}

void kfree(void* block, u64 size, memory_tag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        KWARN("kfree called with MEMORY_TAG_UNKNOWN. Re-class this allocation");
    }

    stats.total_allocated -= size;
    stats.tagged_allocations[tag] -= size;

    // TODO: memory aligment
    platform_free(block, FALSE);
}

void* kzero_memory(void* block, u64 size) {
    return platform_zero_memory(block, size);
}

void* kcopy_memory(void* dest, const void* src, u64 size) {
    return platform_copy_memory(dest, src, size);
}

void* kset_memory(void* dest, i32 value, u64 size) {
    return platform_set_memory(dest, value, size);
}

char* get_memory_usage_str() {
    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    char buffer[8000] = "System memory use (tagged): \n";
    u64 offset = strlen(buffer);

    for (u32 tag = 0; tag < MEMORY_TAG_MAX_TAGS; ++tag) {
        char uint[4] = "XiB";
        f32 amount = 1.0f;

        if (stats.tagged_allocations[tag] >= gib) {
            uint[0] = 'G';
            amount = stats.tagged_allocations[tag] / (f32)gib;
        } else if (stats.tagged_allocations[tag] >= mib) {
            uint[0] = 'M';
            amount = stats.tagged_allocations[tag] / (f32)mib;
        } else if (stats.tagged_allocations[tag] >= kib) {
            uint[0] = 'K';
            amount = stats.tagged_allocations[tag] / (f32)kib;
        } else {
            uint[0] = 'B';
            uint[1] = 0;
            amount = (f32) stats.tagged_allocations[tag];
        }

        i32 length = snprintf(buffer + offset, 8000, "  %s: %.2f%s\n", memory_tag_strings[tag], amount, uint);
        offset += length;
    }

    char* out_string = _strdup(buffer);
    return out_string;
}