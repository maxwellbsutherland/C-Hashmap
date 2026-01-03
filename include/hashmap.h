#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

typedef struct entry entry_t;

typedef struct {
    entry_t **entries;
    size_t num_entries;
} hashmap_t;

typedef struct entry {
    const char *key;
    char *value;

    struct entry *next;
} entry_t;

hashmap_t *create_hashmap(const size_t size);
void free_hashmap(hashmap_t *hmap);

entry_t *hashmap_create(hashmap_t *hmap, entry_t new_entry);
entry_t *hashmap_read(hashmap_t *hmap, const char *key);
entry_t *hashmap_update(hashmap_t *hmap, const char *key, char *val);
int hashmap_delete(hashmap_t *hmap, const char *key);
int hashmap_print(hashmap_t *hmap); 

#endif