#include "BTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clean_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void run_tests() {
    printf("\nrunning tests...\n");
    int result = system("../build/btree_tests");
    if (result != 0) {
        printf("test execution failed!\n");
    }
}

int parse_line(char *line, int *op, char *key, double *value) {
    line[strcspn(line, "\r\n")] = '\0';
    while (*line == ' ' || *line == '\t') line++;
    if (*line == '\0') return -1;
    char *tk = strtok(line, " \t");
    if (tk == NULL) return -1;
    *op = atoi(tk);
    if (*op < 1 || *op > 4) return -1;

    if (*op != 3) {
        tk = strtok(NULL, " \t");
        if (tk == NULL) return -1;
        memset(key, 0, 7);
        strncpy(key, tk, 6);
        key[6] = '\0';

        if (*op == 1) {
            tk = strtok(NULL, " \t");
            if (tk == NULL) return -1;
            *value = atof(tk);
        }
    }
    return 0;
}

int process_command(FILE *out, BTreeNode **root, int op, const char *key, double value) {
    switch (op) {
        case 1:
            if (key == NULL || strlen(key) == 0) {
                fprintf(out, "error: no key for insert\n");
                return -1;
            }
            insert(root, key, value);
            fprintf(out, "OK\n");
            break;
        case 2:
            if (key == NULL) {
                fprintf(out, "error: no key for delete\n");
                return -1;
            }
            if (delete_key(root, (char*)key)) {
                fprintf(out, "OK\n");
            } else {
                fprintf(out, "key not found\n");
            }
            break;
        case 3:
            printB_to_file(out, *root);
            break;
        case 4:
            if (key == NULL) {
                fprintf(out, "error: no key for search\n");
                return -1;
            }
            double result;
            if (value_by_key(*root, (char*)key, &result))
                fprintf(out, "value: %lf\n", result);
            else
                fprintf(out, "key not found\n");
            break;
        default:
            fprintf(out, "error: unknown operation %d\n", op);
            return -1;
    }
    return 0;
}

void print_help(const char *prog) {
    printf("Usage: %s [OPTIONS]\n", prog);
    printf("Options:\n");
    printf("  --input <file>   Read commands from file\n");
    printf("  --output <file>  Write results to file (default: stdout)\n");
    printf("  --help           Show this help\n");
    printf("if no input is set, runs interactively.\n");
}

void interative_mode(BTreeNode **root){
    int n = 0;
    while(n != -1) {
        printf("MENU\n");
        printf("1: add a new value by key to the Btree\n");
        printf("2: delete a value by key\n");
        printf("3: print out the Btree\n");
        printf("4: find a value using a key\n");
        printf("5: exit the program (data wont be saved)\n");
        printf("Choose a number from 1 to 5:\n");
        if ((scanf("%d", &n)) != 1){
            printf("incorrect input format\n");
            clean_input();
            continue;
        };
        char key[7]; double val;
        switch (n) {
            case 1:
                printf("provide the key (length = 6) and value you want to add\n");
                if ((scanf("%s %lf", key, &val)) != 2){
                    printf("incorrect input format\n");
                    clean_input();
                    continue;
                };
                insert(root, key, val);
                break;
            case 2:
                printf("provide the key value of which you want to delete\n");
                if ((scanf("%s", key)) != 1){
                    printf("incorrect input format\n");
                    clean_input();
                    continue;
                };
                delete_key(root, key);
                break;
            case 3:
                printB(*root, 0);
                break;
            case 4:
                printf("provide the key value of which you want to find\n");
                if ((scanf("%s", key)) != 1){
                    printf("incorrect input format\n");
                    clean_input();
                    continue;
                };
                double result;
                int s = value_by_key(*root, key, &result);
                if (s)
                    printf("The value for the key %s is %lf\n", key, result);
                else
                    printf("wasn't able to find the key\n");
                break;
            case 5:
                n = -1;
                printf("bye!");
                free_tree(root);
                return;
            default:
                printf("FROM 1 TO 5! >:(\n");
        }
    }
}

int main(int argc, char *argv[]) {
    char *input_file = NULL;
    char *output_file = NULL;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--test") == 0) {
            run_tests();
            return 0;
        }
         else if (strcmp(argv[i], "--input") == 0 && i + 1 < argc) {
            input_file = argv[++i];
        } else if (strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        } else {
            printf("unknown option: %s\n", argv[i]);
            print_help(argv[0]);
            return 1;
        }
    }
    if (input_file != NULL) {
        FILE *file_in = fopen(input_file, "r");
        if (file_in == NULL) {
            fprintf(stderr, "Error: cannot open input file %s\n", input_file);
            return 1;
        }
        FILE *file_out = stdout;
        if (output_file != NULL) {
            file_out = fopen(output_file, "w");
            if (file_out == NULL) {
                fprintf(stderr, "Error: cannot open output file %s\n", output_file);
                fclose(file_in);
                return 1;
            }
        }

        BTreeNode *root = NULL;
        char line[256];
        int line_num = 0;
        while (fgets(line, sizeof(line), file_in)) {
            line_num++;
            char original[256];
            strcpy(original, line);
            original[strcspn(original, "\n")] = '\0';

            int op;
            char key[7];
            double value = 0.0;
            if (parse_line(line, &op, key, &value) != 0) {
                fprintf(file_out, "%s\n", original);
                fprintf(file_out, "error: invalid command format\n");
                continue;
            }
            fprintf(file_out, "%s\n", original);
            process_command(file_out, &root, op, key, value);
        }
        free_tree(&root);
        fclose(file_in);
        if (output_file != NULL) fclose(file_out);
        return 0;
    }
    BTreeNode *root = NULL;
    interative_mode(&root);
    return 0;
}