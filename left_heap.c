#include <stdio.h>
#include <stdlib.h>
typedef int ele_type;

struct leftheap_node {
    ele_type elem;
    int npl;
    struct leftheap_node *kid[2];
};


void 
swap_child(struct leftheap_node *h)
{
    struct leftheap_node *temp;
    temp = h->kid[0];
    h->kid[0] = h->kid[1];
    h->kid[1] = temp;
}

struct leftheap_node *
heap_alloc()
{
    return (struct leftheap_node *)malloc(sizeof (struct leftheap_node));
}

struct leftheap_node *
merge(struct leftheap_node *h1, struct leftheap_node *h2)
{
    if (h1 == NULL)
        return h2;
    if (h2 == NULL)
        return h1;
    if (h1->elem < h2->elem) {
        if (h1->kid[0] == NULL)
            h1->kid[0] = h2;        //the npl of h1 and  h2 are the same after merged.
        else {
            h1->kid[1] = merge(h1->kid[1], h2);
            if (h1->kid[0]->npl < h1->kid[1]->npl)
                swap_child(h1);
            h1->npl = h1->kid[1]->npl + 1;
        }
        return h1;
    } else {
        if (h2->kid[0] == NULL)
            h2->kid[0] == h1;
        else {
            h2->kid[1] = merge(h2->kid[1], h1);
            if (h2->kid[0]->npl < h2->kid[1]->npl)
                swap_child(h2);
            h2->npl = h2->kid[1]->npl + 1;
        }
        return h2;
    }
}

struct leftheap_node *
ini_leftheap_node(ele_type elem)
{
    struct leftheap_node *temp;
    temp = heap_alloc();
    temp->elem = elem;
    temp->npl = 0;
    temp->kid[0] = temp->kid[1] = NULL;
    return temp;
}

int 
heap_insert(struct leftheap_node *heap, ele_type elem)
{
        struct leftheap_node *new;
        new = ini_leftheap_node(elem);
        if (new == NULL)
            return -1;
        merge(heap, new);
        return 0;
}

void mid_travel(struct leftheap_node *root)
{
    if (root != NULL) {
        mid_travel(root->kid[0]);
        printf("%d\n", root->elem);
        mid_travel(root->kid[1]);
    }
}
void pre_travel(struct leftheap_node *root)
{
    if (root != NULL) {
        printf("%d\n", root->elem);
        pre_travel(root->kid[0]);
        pre_travel(root->kid[1]);
    }
}

int
main()
{
    struct leftheap_node *root;
    root = ini_leftheap_node(3);
    heap_insert(root, 10);
    heap_insert(root, 8);
    heap_insert(root, 21);
    heap_insert(root, 14);
    heap_insert(root, 23);
    heap_insert(root, 17);
    heap_insert(root, 26);

    printf("mid order\n");
    mid_travel(root);
    printf("pre order\n");
    pre_travel(root);

    return 0;
}

