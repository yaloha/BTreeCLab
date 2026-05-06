#include "BTree.c"

void clean_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    struct BTreeNode *root = NULL;
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
                insert(&root, key, val);
                break;
            case 2:
                printf("provide the key value of which you want to delete\n");
                if ((scanf("%s", key)) != 1){
                    printf("incorrect input format\n");
                    clean_input();
                    continue;
                };
                delete_key(&root, key);
                break;
            case 3:
                printB(root);
                break;
            case 4:
                printf("provide the key value of which you want to find\n");
                if ((scanf("%s", key)) != 1){
                    printf("incorrect input format\n");
                    clean_input();
                    continue;
                };
                double result;
                int s = value_by_key(root, key, &result);
                if (s)
                    printf("The value for the key %s is %lf\n", key, result);
                else
                    printf("wasn't able to find the key\n");
                break;
            case 5:
                n = -1;
                printf("bye!");
                free_tree(&root);
                break;
            default:
                printf("FROM 1 TO 5! >:(\n");
        }
    }
    return 0;
}