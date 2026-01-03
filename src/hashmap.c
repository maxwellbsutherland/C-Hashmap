#include <hashmap.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hashmap_t *create_hashmap(const size_t size) {
    hashmap_t *hmap = malloc(sizeof(*hmap));
    if (!hmap)
        return NULL;

    hmap->entries = malloc(size * sizeof(entry_t *));
    if (!hmap->entries) {
        free_hashmap(hmap);
        return NULL;
    }

    for (size_t i = 0; i < size; i++)
        hmap->entries[i] = NULL;

    hmap->num_entries = size;

    return hmap;
}

static void free_entry(entry_t *e) {
    if(!e)
        return;

    free_entry(e->next);
    free(e->value);
    free(e);
}

void free_hashmap(hashmap_t *hmap) {
    if (!hmap)
        return;

    for (size_t i = 0; i < hmap->num_entries; i++) {
        free_entry(hmap->entries[i]);
    }

    free(hmap->entries);
    free(hmap);
}

static int hash(const char *key, const size_t max) {
    unsigned int hashval = 0;
    int c;

    while ((c = *key++)) {
        hashval = c + (hashval * 31);
    }

    return hashval % max;
    return 0;
}

static entry_t *hashmap_bucket_create(entry_t *e, entry_t n) {
    if (e == NULL) {
        entry_t *x = malloc(sizeof(*x));
        if (!x)
            return NULL;

        x->key   = strdup(n.key);
        x->value = strdup(n.value);
        x->next  = NULL;

        return x;
    }

    if (strcmp(e->key, n.key) == 0)
        return e;
    
    e->next = hashmap_bucket_create(e->next, n);
    return e;
}

static entry_t *hashmap_bucket_read(entry_t *e, const char *key) {
    if (!e)
        return NULL;

    return (strcmp(e->key, key) == 0) 
        ? e
        : hashmap_bucket_read(e->next, key);
}

static entry_t *hashmap_bucket_update(entry_t *e, const char *key, char *val) {
    if (!e)
        return NULL;

    if (strcmp(e->key, key) == 0) {
        e->value = strdup(val);
        return e;
    }

    return hashmap_bucket_update(e->next, key, val);
}

static entry_t *hashmap_bucket_delete(entry_t *e, const char *key) {
    if (!e)
        return NULL;

    if (strcmp(e->key, key) == 0) {
        entry_t *next_entry = e->next;

        free(e->value);
        free(e);
        
        return next_entry;
    }

    e->next = hashmap_bucket_delete(e->next, key);
    return e;
}

static int print_entry(entry_t *entry) {
    if (!entry)
        return -1;
    else 
        printf(" -> ");

    printf("[%s,%s]", entry->key, entry->value);

    print_entry(entry->next);

    return 0;
}

entry_t *hashmap_create(hashmap_t *hmap, entry_t new_entry) {
    if (!hmap)
        return NULL;

    const int h = hash(new_entry.key, hmap->num_entries);
    hmap->entries[h] = hashmap_bucket_create(hmap->entries[h], new_entry);

    return hmap->entries[h];
}

entry_t *hashmap_read(hashmap_t *hmap, const char *key) {
    if (!hmap)
        return NULL;

    const int h = hash(key, hmap->num_entries);

    return hashmap_bucket_read(hmap->entries[h], key);
}

entry_t *hashmap_update(hashmap_t *hmap, const char *key, char *val) {
    if (!hmap)
        return NULL;

    const int h = hash(key, hmap->num_entries);

    return hashmap_bucket_update(hmap->entries[h], key, val);
}

int hashmap_delete(hashmap_t *hmap, const char *key) {
    if (!hmap) return -1;

    const int h = hash(key, hmap->num_entries);
    hmap->entries[h] = hashmap_bucket_delete(hmap->entries[h], key);

    return 0;
}

int hashmap_print(hashmap_t *hmap) {
    if (!hmap)
        return -1;

    for(size_t i = 0; i < hmap->num_entries; i++) {
        entry_t *entry = hmap->entries[i];

        if (entry)
            printf("[%lli]", i);

        int r = print_entry(entry);
        if (r == 0)
            printf("\n");
    }

    return 0;
}