//----------------------------------structers----------------------------------

struct TrieNode {
    TrieNode* children[33];
    bool end_of_word;
    char letter;
    TrieNode() {
        end_of_word = false;
        for (int i = 0; i < 33; i++) {
            children[i] = 0;
        }
        letter = '\0';
    }
};

struct for_stat {
    for_stat* children_ss[33];
    for_stat* parent_ss[33];
    char children[33] = { 0 };
    char parents[33] = { 0 };
    int chislo_c[33];
    int chislo_p[33];
    char letter;
    //int i;
    for_stat() {
        for (int i = 0; i < 33; i++) {
            children_ss[i] = NULL;
            parent_ss[i] = NULL;
        }
        memset(chislo_c, 0, 33 * sizeof(int));
        memset(chislo_p, 0, 33 * sizeof(int));
        strncpy(children, (char*)"àáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ¸", 33);
        strncpy(parents, (char*)"àáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ¸", 33);
        letter = '\0';
    }
};
struct for_root {
    //public:
    for_stat* children[33];
    // Character stored in this node
    //char letter;
    //int i;
    for_root() {
        for (int i = 0; i < 32; i++) {
            //children[i] = NULL;
            children[i] = new for_stat;
            children[i]->letter = i - 32;

        }
        children[32] = new for_stat;
        children[32]->letter = -72;
        //letter = '\0';
    }
};

TrieNode root;
for_root root_stat;

void Insert(char str[17]) {
    TrieNode* current = &root;
    for_stat* cur_stat = NULL;
    char c;
    //--------------------------------------------for_stat--------------------------------------
    for (size_t i = 0; i < strlen(str) - 1; i++) {
        c = str[i];
        //printf("%c %d\n ", c,c);
        if (str[i] == -72) {
            c = 0;
        }
        cur_stat = (&root_stat)->children[c + 32];//áóêâà, ïî êîòîðîé ñîáèðàåì ñòàòèñòèêó
        if (i > 0)
            if (str[i - 1] == -72) {
                (cur_stat->chislo_p[32])++;
                if (cur_stat->parent_ss[32] == NULL) {
                    cur_stat->parent_ss[32] = new for_stat;
                    cur_stat->parent_ss[32]->letter = str[i];
                }
            }
            else {
                (cur_stat->chislo_p[str[i - 1] + 32])++;
                if (cur_stat->parent_ss[str[i - 1] + 32] == NULL) {
                    cur_stat->parent_ss[str[i - 1] + 32] = new for_stat;
                    cur_stat->parent_ss[str[i - 1] + 32]->letter = str[i];
                }
            }
        if (i < strlen(str) - 2)
            if (str[i + 1] == -72) {
                (cur_stat->chislo_c[32])++;
                if (cur_stat->children_ss[32] == NULL) {
                    cur_stat->children_ss[32] = new for_stat;
                    cur_stat->children_ss[32]->letter = str[i];
                }
                //continue;
            }
            else {
                (cur_stat->chislo_c[str[i + 1] + 32])++;
                if (cur_stat->children_ss[str[i + 1] + 32] == NULL) {
                    cur_stat->children_ss[str[i + 1] + 32] = new for_stat;
                    cur_stat->children_ss[str[i + 1] + 32]->letter = str[i];
                }
            }
    }


    //-------------------------------------for_slovar--------------------------------
    for (size_t i = 0; i < strlen(str) - 1; i++) {
        int c = str[i];
        //printf("%c", c);
        if (str[i] == -72) {
            c = 0;
        }
        if (current->children[c + 32] == NULL) {
            current->children[c + 32] = new TrieNode;
            current->children[c + 32]->letter = str[i];
        }
        current = current->children[c + 32];
    }
    current->end_of_word = true;
    // printf("\n");
}

TrieNode* Search(char str[16]) {
    TrieNode* current = &root;
    for (size_t i = 0; i < strlen(str); i++) {
        short c = str[i];
        if (str[i] == -51)
        {
            printf("%c!!!!!!!!!!!!!!\n", -51);
        }
        if (str[i] == -72) {
            c = 0;
        }
        if (current->children[c + 32]) {
            current = current->children[c + 32];
        }
        else {
            current = NULL;
            break;
        }
    }
    return current;
}

TrieNode* current2;
void slovar() {
    fopen_s(&slov, "slovar.txt", "r");
    char dop[17] = "";
    while (!feof(slov)) {
        fgets(dop, 17, slov);
        if (dop[0] != 120)
            Insert(dop);
    }
    fclose(slov);
}