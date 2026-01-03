#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include <hashmap.h>

#define UNUSED(x) (void)(x)

#define MAX_ARGS 8
#define MAX_LINE 128
#define NUM_FUNCTIONS 6
#define HASHMAP_SIZE 1024

typedef int (*command_fn_t)(hashmap_t *hmap, int argc, char **argv);

typedef struct {
    const char *name;
    command_fn_t func;
} command_t;

static int cmd_create(hashmap_t *hmap, int argc, char **argv);
static int cmd_read(hashmap_t *hmap, int argc, char **argv);
static int cmd_update(hashmap_t *hmap, int argc, char **argv);
static int cmd_delete(hashmap_t *hmap, int argc, char **argv);
static int cmd_print(hashmap_t *hmap, int argc, char **argv);
static int cmd_help(hashmap_t *hmap, int argc, char **argv);

const command_t commands[NUM_FUNCTIONS] = {
    { .name = "-c", .func = cmd_create },
    { .name = "-r",   .func = cmd_read   },
    { .name = "-u", .func = cmd_update },
    { .name = "-d", .func = cmd_delete },
    { .name = "-p",  .func = cmd_print  },
    { .name = "-h",  .func = cmd_help  },
};

static int cmd_create(hashmap_t *hmap, int argc, char **argv) {
    if (argc < 3) // [FUNC] [KEY] [VALUE]
        return 1;

    const char *key = strdup(argv[1]);
    char *val = strdup(argv[2]);

    entry_t new_entry = {.key = key, .value = val};

    entry_t *e = hashmap_create(hmap, new_entry);
    if (e == NULL)
        return -1;

    return 0;
}

static int cmd_read(hashmap_t *hmap, int argc, char **argv) {
    if (argc < 2) // [FUNC] [KEY]
        return 1;

    const char *key = strdup(argv[1]);

    entry_t *e = hashmap_read(hmap, key);
    if (e == NULL)
        return -1;

    printf("{Key: \"%s\", Value: \"%s\"}\n", e->key, e->value);

    return 0;
}

static int cmd_update(hashmap_t *hmap, int argc, char **argv) {
    if (argc < 3) // [FUNC] [KEY] [VALUE]
        return 1;

    const char *key = strdup(argv[1]);
    char *val = strdup(argv[2]);

    entry_t *e = hashmap_update(hmap, key, val);
    if (e == NULL)
        return -1;

    return 0;
}

static int cmd_delete(hashmap_t *hmap, int argc, char **argv) {
    if (argc < 2)
        return 1; // [FUNC] [KEY]

    const char *key = strdup(argv[1]);

    return hashmap_delete(hmap, key);
}

static int cmd_print(hashmap_t *hmap, int argc, char **argv) {
    UNUSED(argc);
    UNUSED(argv);

    return hashmap_print(hmap);
}

static int cmd_help(hashmap_t *hmap, int argc, char **argv) {
    UNUSED(hmap);
    UNUSED(argc);
    UNUSED(argv);

    printf("Create\t-c [KEY] [VALUE]\tCreate and add Entry to Hashmap.\n");
    printf("Read\t-r [KEY]\t\tRead and display the value of a Hashmap entry.\n");
    printf("Update\t-u [KEY] [VALUE]\tUpdate the value of an existing Hashmap entry.\n");
    printf("Delete\t-d [KEY]\t\tDelete an entry from the Hashmap.\n");
    printf("Print\t-p\t\t\tPrint all entries in the Hashmap.\n");
    printf("Help\t-h\t\t\tList of all commands.\n");

    return 0;
}

int parse_command(int *argc, char ***argv) {
    static char line[MAX_LINE];
    char **args;

    *argc = 0;
    *argv = NULL;

    if (!fgets(line, sizeof(line), stdin)) {
        return -1;
    }

    line[strcspn(line, "\n")] = '\0';

    args = malloc(sizeof(char *) * MAX_ARGS);
    if (!args)
        return -1;

    char *tok = strtok(line, " \t");
    while (tok && *argc < MAX_ARGS) {
        args[*argc] = _strdup(tok);
        if (!args[*argc]) {
            for (int i = 0; i < *argc; i++)
                free(args[i]);
            free(args);
            return -1;
        }
        (*argc)++;
        tok = strtok(NULL, " \t");
    }

    *argv = args;
    return 0;
}

void terminal(hashmap_t *hmap) {
    int argc;
    char **argv;

    for (;;) {
        printf("> ");
        fflush(stdout);

        if (parse_command(&argc, &argv) == -1) {
            printf("\n");
            break;
        }

        int handled = 0;

        for (size_t i = 0; i < NUM_FUNCTIONS; i++) {
            if (strcmp(argv[0], commands[i].name) == 0) {
                handled = 1;

                int r = commands[i].func(hmap, argc, argv);

                switch (r) {
                    case 0: {
                        fprintf(stdout, "Operation completed successfully.\n");
                        break;
                    }
                    case 1: {
                        fprintf(stderr, "Error: Missing required arguments.\n");
                        break;
                    }
                    case -1: {
                        fprintf(stderr, "Error: Operation failed.\n");
                        break;
                    }
                    default: {
                        fprintf(stderr, "Error: Unknown return code %d.\n", r);
                        break;
                    }
                }

                break;
            }
        }

        if (!handled) {
            fprintf(stderr, "Error: Unknown command.\n");
        }

        for (int i = 0; i < argc; i++)
            free(argv[i]);
        free(argv);
    }
}

int main(void) {
    hashmap_t *hmap = create_hashmap(HASHMAP_SIZE);
    printf("Hashmap Test Terminal\n");
    printf("Help\t-h\tList of all commands.\n");
    terminal(hmap);
    free_hashmap(hmap);
    printf("Exiting.\n");
    return 0;

}
