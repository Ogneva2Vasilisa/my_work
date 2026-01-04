/* combined_tricks.c
   Комбинирует enum-ловушки, designated initializers, комментарии, hex/binary и метки. */


enum Flags {
    FLAG_NONE  = 0,
    FLAG_READ  = 1 << 0,
    FLAG_WRITE = 1 << 1,
    FLAG_EXEC  = 0x4,       /* hex constant */
    FLAG_ALL   = FLAG_READ | FLAG_WRITE | FLAG_EXEC
};

struct Node {
    int id;
    unsigned flags;
    const char *name;
};

int main(void) {
    struct Node nodes[] = {
        [0] = { .id = 100, .flags = FLAG_READ,  .name = "first" },
        [1] = { .id = 200, .flags = FLAG_WRITE, .name = "second" },
        [2] = { .id = 300, .flags = FLAG_ALL,   .name = "third" }
    };

    for (int i = 0; i < 3; ++i) {
        printf("node %d: id=%d flags=0x%X name=%s\n", i, nodes[i].id, nodes[i].flags, nodes[i].name);
    }

    // метка + пустые операторы + комментарии между токенами
start_label: ; ; /*ready*/ 
    printf("After start_label\n");

    // бинарные литералы в условиях (GCC extension)
    if ((0b1010 & 0b1000) != 0) {
        printf("binary test passed\n");
    }

    // хамелеон: комментарий между '.' и идентификатором в designator (некорректно),
    // но корректно: . /*comment*/ name = "split-designator"
    struct Node n = { . /* comment */ id = 999, .flags = 0x1, .name = "split" };
    printf("split node: id=%d flags=0x%X name=%s\n", n.id, n.flags, n.name);

    return 0;
}
