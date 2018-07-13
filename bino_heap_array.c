#include <stdio.h>
#include <stdlib.h>
#define LEN(X)  (sizeof (X) / sizeof (X[0]))
#define BINSIZE 20


typedef int elem_type;

struct bin_node {
    elem_type elem;
    struct bin_node *left_child;
    struct bin_node *next_sibling;
};

struct bin_heap {
    struct bin_node *forest[BINSIZE];
    int current_size;
};

struct bin_node *
ini_bin_node(elem_type elem)
{
    struct bin_node *temp;
    temp = (struct bin_node *)malloc(sizeof (struct bin_node));
    temp->elem = elem;
    temp->left_child = NULL;
    temp->next_sibling = NULL;
    return temp;
}

struct bin_heap *
ini_bin_heap()
{
    struct bin_heap *temp = (struct bin_heap *)malloc(sizeof (struct bin_heap));
    int i, len;
    len = LEN(temp->forest);
    for (i = 0; i < len; i++)
        temp->forest[i] = NULL;
    temp->current_size = 0;
    return temp;
}

/* func: combine two binominal trees
 * para: t1 is a binominal tree with smaller elem
 *       t2 is a binominal tree with bigger elem
 * retu: a new binominal tree merged from t1 and t2
 */

struct bin_node *
combine_trees(struct bin_node *t1, struct bin_node *t2)
{
    if (t1->elem > t2->elem)
        return combine_trees(t2, t1);
    t2->next_sibling = t1->left_child;
    t1->left_child = t2;
    return t1;
}

/*func: merge two binomial heaps, h1 and h2, saved into h1
 *para: h1 is a binomial heap, h2 is anoter one 
 *retu: h1
 */

struct bin_heap *
merge(struct bin_heap *h1, struct bin_heap *h2)
{
    int i, j;
    struct bin_node *t1, *t2, *carry;

    carry = NULL;
    h1->current_size += h2->current_size;    
    for (i = 0, j = 1; j <= h1->current_size; i++, j *= 2) {
        t1 = h1->forest[i], t2 = h2->forest[i];
        switch ((!!t1) + 2 * (!!t2) + 4 * (!!carry)) {
            case 0:
            case 1:
                break;
            case 2:
                h1->forest[i] = t2;
                h2->forest[i] = NULL;
                break;
            case 3:
                carry = combine_trees(t1, t2);
                h1->forest[i] = h2->forest[i] = NULL;
                break;
            case 4:
                h1->forest[i] = carry;
                carry = NULL;
                break;
            case 5:
                carry = combine_trees(t1, carry);
                h1->forest[i] = NULL;
                break;
            case 6:
                carry = combine_trees(t2, carry);
                h2->forest[i] = NULL;
                break;
            case 7:
                h1->forest[i] = carry;
                carry = combine_trees(t1, t2);
                h2->forest[i] = NULL;
                break;
        }
    }
    return h1;
}

void insert(struct bin_heap *bh, elem_type elem)
{
   struct bin_node *new_node;
   struct bin_heap *new_heap;

   new_node = ini_bin_node(elem);
   new_heap = ini_bin_heap();
   new_heap->forest[0] = new_node;
   new_heap->current_size++;
   bh = merge(bh, new_heap);
}

void not_test()
{
    int i, j, k;
    i = 4;
    j = 0x31233;
    k = 0;
    printf("i = %d\t!i = %d\t!!i = %d\n", i, !i, !!i);
    printf("j = %d\t!j = %d\t!!j = %d\n", j, !j, !!j);
    printf("k = %d\t!k = %d\t!!k = %d\n", k, !k, !!k);
}

void dfs_bin_tree(struct bin_node *root)
{
    while (root != NULL) {
        printf("%d\n", root->elem);
        dfs_bin_tree(root->left_child);
        root = root->next_sibling;
    }
}

void bino_heap_test()
{
    int i; 
    struct bin_heap *bheap;

    bheap = ini_bin_heap();
    for (i = 0; i <= 7; i++)
        insert(bheap, i);
    for (i = 0; i < BINSIZE; i++) {
        if (bheap->forest[i] != NULL) {
            printf("bheap->forest[%d] != NULL\n", i);
            dfs_bin_tree(bheap->forest[i]);            
        } else {
            printf("bheap->forest[%d] == NULL\n", i);
        }
    }
}

int main()
{
    int a, b, c;
    int *d, *e, *f;
    a = 1;
    b = 2;
    c = 3;
    d = &a;
    e = &b;
    f = &c;
    printf("a = %d, b = %d, c = %d\n", a, b, c);
    printf("*d = %d, *e = %d, *f = %d\n", *d, *e, *f);
    bino_heap_test();

    return 0;
}
