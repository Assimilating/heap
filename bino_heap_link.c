#include <stdio.h>
#include <stdlib.h>
typedef int elem_type;
#define INFINITY    9999999

struct bino_node {
    struct bino_node *parent;
    struct bino_node *left_child;
    struct bino_node *right_sibling;

    unsigned int degree;
    elem_type elem;
};

struct bino_heap {
    struct bino_node *head;
    struct bino_node *mini;
};

struct bino_node *
make_bino_node(elem_type elem)
{
    struct bino_node *temp =  (struct bino_node *)malloc(sizeof (struct bino_node));
    temp->degree = 0;
    temp->elem = elem;
    temp->parent = temp->left_child = temp->right_sibling = NULL;
    return temp;
}

struct bino_heap *
make_bino_heap()
{
    struct bino_heap *temp = (struct bino_heap *)malloc(sizeof (struct bino_heap));
    temp->head = NULL;
    temp->mini = NULL;
    return temp;
}

/*func: find the minimum key in a binomial heap 
 *para: binomial heap h
 *retu: return a pointer pointed to the node in binomial heap with minimum key  
 */
struct bino_node *
minimum(struct bino_heap *h)
{
    struct bino_node *x = h->head;
    struct bino_node *y = NULL;
    int mini = INFINITY;

    while (x != NULL) {
        if (x->elem < mini)
            y = x;
        else
            x = x->right_sibling;
    }
    return y;
}


/* func: combine two binominal trees
 * para: parent is a binominal tree with smaller elem
 *       child is a binominal tree with bigger elem
 * retu: a new binominal tree merged from t1 and t2
 */
struct bino_node *
combine_tree(struct bino_node *parent, struct bino_node *child)
{

    child->right_sibling = parent->left_child;
    child->parent = parent;

    parent->left_child = child;
    parent->degree += 1;
    return parent;
}



struct bino_node *
copy_node(struct bino_node *node)
{
    struct bino_node *temp =  (struct bino_node *)malloc(sizeof (struct bino_node));
    temp->degree = node->degree;
    temp->elem = node->elem;
    temp->parent = node->parent;
    temp->left_child = node->left_child;
    temp->right_sibling = node->right_sibling;
    return temp;
}
/* func: merge the root lists of two binomial heaps 
 *       with a increasing order of degree of each root
 * para: h1 is a binomial heap
 *       h2 is another one
 * retu: the merged root list
 */
struct bino_node *
merge_root_list_1(struct bino_node *t1, struct bino_node *t2)
{
    struct bino_node *root_link = make_bino_node(INFINITY);
    struct bino_node *new = NULL;

    while (t1 != NULL && t2 != NULL) {
        if (t1->degree < t2->degree) {
           new = copy_node(t1); 
           root_link->right_sibling = new;
           free((void *)t1);
           t1 = t1->right_sibling;
        } else {
            new = copy_node(t2);
            root_link->right_sibling = new;
            free((void *)t2);
            t2 = t2->right_sibling;
        }
        root_link = root_link->right_sibling;

    }
    while (t1 != NULL) {
        new = copy_node(t1); 
        root_link->right_sibling = new;
        free((void *)t1);
        t1 = t1->right_sibling;
        root_link = root_link->right_sibling;
    }
    while (t2 != NULL) {
        new = copy_node(t2); 
        root_link->right_sibling = new;
        free((void *)t2);
        t2 = t2->right_sibling;
        root_link = root_link->right_sibling;
    }
    return root_link->right_sibling;
}


struct bino_node *
merge_root_list_2(struct bino_node *a, struct bino_node *b)
{
    struct bino_node *head = NULL;
    struct bino_node **pos = &head;

    while (a && b) {
        if (a->degree < b->degree) {
            *pos = a;
            a = a->right_sibling;
        } else {
            *pos = b;
            b = b->right_sibling;
        }
        pos = &(*pos)->right_sibling;
    }
    if (a) 
        *pos = a;
    else
        *pos = b;
    return head;
}
void root_list_test(struct bino_node *node)
{
    while (node) {
        printf("root ist test:  %d\n", node->elem);
        node = node->right_sibling;
    }
    putchar('\n');
}

struct bino_heap *
union_bino_heap(struct bino_heap *h1, struct bino_heap *h2)
{

//    printf("union h1->head = %d\n",h1->head->elem);

    struct bino_heap *h = make_bino_heap();
    struct bino_node *prev, *x, *next;

    if (!h1->head) {
        printf("union h1->head == NULL  ");
        printf("union h2->head == %d\n",h2->head->elem);
        return h2;
    }
    if(!h2->head)
        return h1;

    prev = NULL;
    x = merge_root_list_2(h1->head, h2->head);
    root_list_test(x);
    next = x->right_sibling;
    h->head = x;
   /*
    * loop invariant: merge x and next.
    * 
    */
    while (next) {
        /*
         * match failed or carry successful
         */
        if ((x->degree != next->degree) ||
           (next->right_sibling && next->right_sibling->degree == x->degree)) {
            prev = x;
            x = next;
//            next = x->right_sibling;
        } else if (x->elem < next->elem) {  /*match successful and carry failed*/
            x->right_sibling = next->right_sibling;
            x = combine_tree(x, next);
//            next = x->right_sibling;
        } else {
            if (prev)
                prev->right_sibling = next;
            else
                h->head = next;
            next = combine_tree(next, x);
            x = next;
//            next = x->right_sibling;
        }        
        next = x->right_sibling; //extract the same statement from each block, write in once
    }
    return h;
}



struct bino_node *
bino_heap_reverse(struct bino_heap *heap)
{
    struct bino_node *prev, *h, *next;

    prev = NULL;
    h = heap->head;

   /*
    * loop invariant: h->right_sibling = prev
    * initialization: prev = NULL, h = heap->head;
    * maintenance: next = h->right_sibling, prev = h, h = hext;
    * termination: h->right_sibling == NULL
    */

    while (h->right_sibling) {
        next = h->right_sibling;
        h->right_sibling = prev;
        h->parent = NULL;
        prev = h;
        h = next;
    }
    h->right_sibling = prev;
    return h;
}

void two_level_pointer_test()
{
    struct bino_node **p;
    struct bino_node *a = make_bino_node(INFINITY);
    struct bino_node *b = make_bino_node(INFINITY+1);
    struct bino_node *c = make_bino_node(INFINITY+2);
    a->left_child = b;
    a->right_sibling = c;
    p = &a;
    
    printf("a->elem = %d\t(*p)->elem = %d\n", a->elem, (*p)->elem);
    printf("&(a->elem) = %d\t&(*p)->elem = %d\n", &(a->elem), &(*p)->elem);


    /* two level pointer p point to a struct member witch is pointer type
     * p DO NOT POINT THE STRUCT INSTANCE POINTED BY A->LEFT_CHILD
     * p point to the a->left_child struct field.
     */


    p = &(*p)->left_child;      
    printf("&(a->leftchild) = %d\tb = %d\tp = %d\t", &(a->left_child), b, p);

}


void 
insert(struct bino_heap **heap, elem_type elem)
{
    struct bino_node *node = make_bino_node(elem);
    struct bino_heap *temp = make_bino_heap();
    temp->head = node;
//    printf("insert test: %d\n", temp->head->elem);

    *heap = union_bino_heap(*heap, temp);
    printf("insert test: heap->head = %d\n", (*heap)->head->elem);
}

void
dfs_travel(struct bino_node *node)
{
    if (node != NULL) {
        printf("%d\n", node->elem);
        dfs_travel(node->left_child);
        dfs_travel(node->right_sibling);
    }
}


void test()
{
    int i;

    struct bino_heap *heap = make_bino_heap();
    for (i = 0; i < 8; i++)
        insert(&heap, i);
    dfs_travel(heap->head);
}


int main()
{
//    two_level_pointer_test();
    test();
    return 0;
}
