#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <limits.h>
#define LEN(X)  (sizeof X / sizeof X[0])
#define PHI 1.618
#define INFINITY INT_MIN
#define MINITEST    -1
// #define DEGREE_UPPER_BOUND(N)   

typedef int elem_type;

enum mark{
    TRUE,
    FALSE
};

struct fibo_node{
    struct fibo_node *p, *c, *left, *right;
    int degree;
    int mark;
    elem_type elem;
};

struct fibo_heap{
    int n;
    struct fibo_node *min;
};

struct fibo_node *make_fibo_node(elem_type elem);
struct fibo_heap *make_fibo_heap();
void fibo_heap_insert(struct fibo_heap *h, elem_type elem);
struct fibo_node *fibo_minimum(struct fibo_heap *h);
struct fibo_node *merge_root_list(struct fibo_node *a, struct fibo_node *b);
struct fibo_heap *fibo_heap_union(struct fibo_heap *h1, struct fibo_heap *h2);
void fibo_heap_link(struct fibo_heap *h, struct fibo_node *y, struct fibo_node *x);
struct fibo_node *fibo_heap_extract_min(struct fibo_heap * h);
void fibo_decrease_key(struct fibo_heap *h, struct fibo_node *x, elem_type elem);
void cut(struct fibo_heap *h, struct fibo_node *x, struct fibo_node *y);
void cascading_cut(struct fibo_heap *h, struct fibo_node *y);
void print_root_list(struct fibo_node *min);
int fibo_max_degree(struct fibo_heap *h);
void fibo_heap_travel(struct fibo_heap *h);
void fibo_heap_delete(struct fibo_heap *h, struct fibo_node *x);
void root_list(struct fibo_node *node);


int 
fibo_max_degree(struct fibo_heap *h)
{
    int power, n;
    double i;

    n = h->n;
    i = 1;
    power = 0;
    while (i < n) {
        i *= PHI;
        power++;
    }
    power--;
    return power;
}

struct fibo_node *
make_fibo_node(elem_type elem)
{
     struct fibo_node *temp = (struct fibo_node *)malloc(sizeof (struct fibo_node));
     temp->p = temp->c = NULL;
     temp->left = temp->right = temp;
     temp->degree = 0;
     temp->mark = FALSE;  //CLRS-3ed,p520
     temp->elem =elem;
     return temp;
}

struct fibo_heap *
make_fibo_heap()
{
    struct fibo_heap *temp = (struct fibo_heap *)malloc(sizeof (struct fibo_heap));
    temp->n = 0;
    temp->min = NULL;
    return temp;
}


//struct fibo_heap * 
void
fibo_heap_insert(struct fibo_heap *h, elem_type elem)
{
   struct fibo_node *new = make_fibo_node(elem);

   if (!h->min) 
       h->min = new;
   else {
       new->left = h->min;
       new->right = h->min->right;
       h->min->right->left = new;
       h->min->right = new;
       if (h->min->elem > new->elem)
           h->min = new;
   }
   (h->n)++;
//   return h;
}

struct fibo_node*
fibo_minimum(struct fibo_heap *h)
{
    return h->min;
}

/*
 * func: merge two double linked circle lists 
 * para: two root lists
 * retu: the address of minimum node in new merged list
 */
struct fibo_node *
merge_root_list(struct fibo_node *a, struct fibo_node *b)
{
    struct fibo_node *ar = a->right;
    struct fibo_node *bl = b->left;
    
    a->right = b;
    b->left = a;
    ar->left = bl;
    bl->right = ar;
    if (a->elem < b->elem)
        return a;
    else
        return b;
}


struct fibo_heap *
fibo_heap_union(struct fibo_heap *h1, struct fibo_heap *h2)
{
    struct fibo_heap *h = make_fibo_heap();
    if (!h1)
        return h2;
    if (!h2)
        return h1;
    h->min = merge_root_list(h1->min, h2->min);
    h->n = h1->n + h2->n;
    return h;
}

/*
 * func: remove y from root list of fibo_heap h, add y to child list of x
 * para: fibo_heap h, fibo_tree x will be parent, fibo_tree y will be child
 * retu: void
 */
void 
fibo_heap_link(struct fibo_heap *h, struct fibo_node *y, struct fibo_node *x)
{

    printf("before link parent: x->degree = %d\tx->left = %d\tx->right = %d\n", x->degree, x->left->elem, x->right->elem);
    printf("before link child: y->degree = %d\ty->left = %d\ty->right = %d\n", y->degree, y->left->elem, y->right->elem);
    y->left->right = y->right;
    y->right->left = y->left;

    y->left = y->right = y;

    if (!x->c) 
        x->c = y;
    else 
       x->c = merge_root_list(y, x->c); 
    y->p = x;
    printf("link test:");
    print_root_list(x->c);

    (x->degree)++;
    y->mark = FALSE;
    printf("after link parent: x->degree = %d\tx->left = %d\tx->right = %d\n", x->degree, x->left->elem, x->right->elem);
    printf("after link child: y->degree = %d\ty->left = %d\ty->right = %d\n", y->degree, y->left->elem, y->right->elem);
}

static inline void 
exchange(struct fibo_node **a, struct fibo_node **b)
{
    struct fibo_node *c;
    c = *a;
    *a = *b;
    *b = c;
}
/*
void 
pre_travel(struct fibo_node *node)
{
    while (node) {
        printf("%d\n", node->elem);
        pre_travel(node->c);
        pre_travel(node->right);
        if (node == node->right)
            return;
    }
}
*/

void 
consolidata(struct fibo_heap *h)
{
    int i, d, size;
    struct fibo_node *w, *x, *y, *wr;   //wr is the w->right
    size = fibo_max_degree(h);
    printf("consolidate root table size %d\n", size);
    struct fibo_node *rt[size];         //root table
    printf("rt size = %d\n", LEN(rt));

    for (i = 0; i < size; i++)
        rt[i] = NULL;

    d = h->min->degree;
    rt[d] = h->min;
    root_list(h->min);
    print_root_list(h->min);
    for (w = h->min->right; w != rt[w->degree]; w = wr) {
        wr = w->right; 
        printf("before w->elem = %d\tw->degree = %d\tw->left = %d\tw->right = %d\n", w->elem, w->degree, w->left->elem, w->right->elem);
        x = w;
        d = x->degree;
        printf("test w scan the root list: x elem = %d\tx degree = %d\n", x->elem, x->degree);  
        if (rt[d])
            printf("test w scan the root list: r elem = %d\tr degree = %d\n", rt[d]->elem, rt[d]->degree);  
        
        while (rt[d]) {
            y = rt[d];
            if (y->elem < x->elem) 
                exchange(&x, &y); 
            fibo_heap_link(h, y, x);
            rt[d] = NULL;
            d = d+1;
        }
        
        rt[d] = x;
        printf("after w->elem = %d\tw->degree = %d\tw->left = %d\tw->right = %d\n", w->elem, w->degree, w->left->elem, w->right->elem);
        putchar('\n');
    }

    h->min = NULL;
    for (i = 0; i < size; i++) {
        if (rt[i]) {
            if (!h->min)
                h->min = rt[i];
            else if (rt[i]->elem < h->min->elem)
                h->min = rt[i];
        }
    }
    printf("achivevd consolidate h->min = %d\n", h->min->elem);
}
/*
 * func: extract min node in fibo heap
 * para: fibo heap ptr
 * retu: min noe ptr
 */
struct fibo_node *
fibo_heap_extract_min(struct fibo_heap * h)
{
    struct fibo_node *z = h->min;
    printf("extract min test: min z = %d\n", z->elem);
    struct fibo_node *zc = z->c;        //children
    struct fibo_node *cc = NULL;        //children cursor

    if (z) {
        if (zc) {
            printf("extract min test: min zc = %d\n", zc->elem);
            zc->p = NULL;
            for (cc = zc->right; cc != zc; cc = cc->right)
                cc->p = NULL;

            h->min = merge_root_list(zc, h->min);
            printf("before remove z from root list:");
            root_list(h->min);
        }
        z->c = NULL;
        printf("z = %d z->left = %d z->right = %d\n", z->elem, z->left->elem, z->right->elem);
        z->right->left = z->left;
        z->left->right = z->right;
        printf("after remove z from root list:");
        root_list(z->right);

         
        if (z == z->right)
            h->min = NULL;
        else {
            h->min = z->right;
            consolidata(h);
        }
        h->n = h->n - 1;
    }
    return z;
}


void 
cut(struct fibo_heap *h, struct fibo_node *x, struct fibo_node *y)
{
    if (x->right == x) {
        printf("cut the node %d which has no siblings\n", x->elem);
        y->c = NULL;
    }
    else {
        printf("cut the node %d which has right sibling %d\n", x->elem, x->right->elem);
        y->c = x->right;
    }

    x->right->left = x->left;
    x->left->right = x->right;
    x->right = x->left = x;
    y->degree--;

    h->min = merge_root_list(x, h->min);

    x->p = NULL;
    x->mark = FALSE;
}



void cascading_cut(struct fibo_heap *h, struct fibo_node *y)
{
    struct fibo_node *z;
    z = y->p;
    if (z) {
        if (y->mark == FALSE)
            y->mark = TRUE;
        else {
            cut(h, y, z);
            cascading_cut(h, z);
        }
    }
}


void
fibo_decrease_key(struct fibo_heap *h, struct fibo_node *x, elem_type elem)
{
    struct fibo_node *y;
    y = x->p;

    x->elem = elem;
    if (y && (x->elem < y->elem)) {
        printf("decrease key = %d it's parent = %d\n", x->elem, y->elem);
        cut(h, x, y);
        cascading_cut(h, y);
    }
    if (x->elem < h->min->elem)
        h->min = x;
}
/*
 * func: print the whole fibo_heap
 * para: the ptr pointing to fibo min node
 * retu: void
 */
void print_root_list(struct fibo_node *min)
{
    struct fibo_node *end, *x;

    if (min) {
        end = min->left;
        for (x = min; x != end; x = x->right) {
            if (x->p)
                printf("%d is parent of %d\n", x->p->elem, x->elem);
            else
                printf("%d is in root list\n", x->elem);

            if (x->c)
                print_root_list(x->c);
        
        }   
        if (x->p)
            printf("%d is parent of %d\n", x->p->elem, x->elem);
        else
            printf("%d is in root list\n", x->elem);
        if (x->c)
            print_root_list(x->c);
    }
}

void fibo_heap_delete(struct fibo_heap *h, struct fibo_node *x)
{
    fibo_decrease_key(h, x, INFINITY);
    printf("________________delete-1: decrease______________\n");
    print_root_list(h->min);
    printf("________________delete-2: extract______________\n");
    root_list(h->min);
    print_root_list(h->min);
    fibo_heap_extract_min(h);
}

void fibo_heap_travel(struct fibo_heap *h)
{
    struct fibo_node *p; 
    struct stack *sh = ini_stack();     //stack head
    struct stack *snode;                //stack node
    printf(" %d ", h->min->elem);
    if (h->min->c)
        push(sh, h->min->c);
    for (p = h->min->right; p != h->min; p = p->right) {
        printf(" %d ", p->elem);
        if (p->c)
            push(sh, p->c);
    }
    while (!isempty(sh)) {
        snode = pop(sh);
        p = snode->elem;
        printf(" %d ", p->elem);
        for (p = h->min->right; p != h->min; p = p->right) {
            printf(" %d ", p->elem);
            if (p->c)
                push(sh, p->c);
        }
    }
}

void
root_list(struct fibo_node *node)
{
    struct fibo_node *start = node;
    struct fibo_node *x;

    printf(" %d ", start->elem);
    for (x = start->right; x != start; x = x->right)
        printf(" %d ", x->elem);
    putchar('\n');
}


void test()
{
    int i;

    printf("_________________________insert-test_____________________________\n");
    struct fibo_heap *h = make_fibo_heap();
    for (i = 2; i >= 0; i--)
        fibo_heap_insert(h, i);
    print_root_list(h->min);
    printf("h->min->elem = %d\n", h->min->elem);
    printf("h->n = %d\n",h->n);

    printf("_________________________minimum-test______________________________\n");
    struct fibo_node *tmin;
    tmin = fibo_minimum(h);
    printf("fibo_minimum = %d\n", tmin->elem);

    printf("_________________________union-test______________________________\n");
    struct fibo_heap *fh = make_fibo_heap();
    for (i = 15; i > 5; i--)
        fibo_heap_insert(fh, i);
    print_root_list(fh->min);
    printf("fh->min->elem = %d\n", fh->min->elem);
    printf("fh->n = %d\n",fh->n);

    fh = fibo_heap_union(h, fh);
    print_root_list(fh->min);
    printf("fh->min->elem = %d\n", fh->min->elem);
    printf("fh->n = %d\n",fh->n);

    printf("_________________________extract-min-test______________________________\n");
    struct fibo_node *min;
    min = fibo_heap_extract_min(fh);
    printf("extract min test: %d\n", min->elem);
    print_root_list(fh->min);
    printf("fh->n = %d\n", fh->n);
    if (fh->min->c) {
        printf("fh->min->c = %d\n", fh->min->c->elem);
        print_root_list(fh->min->c);
    }
/*
    printf("_________________________link-test________________________________\n");
    printf("fh->min = %d\n", fh->min->elem);
    printf("fh->min->right = %d\n", fh->min->right->elem);
    
    fibo_heap_link(fh, fh->min->right, fh->min);

    print_root_list(fh->min);
    printf("fh->min->c->elem = %d\n", fh->min->c->elem);
    print_root_list(fh->min->c);
    printf("fh->min->degree = %d\n", fh->min->degree);
*/


    printf("_________________________decreasing-key-test______________________________\n");
    printf("fh->min = %d\n", fh->min->elem);
    struct fibo_node *x = fh->min->left->c->c;
    printf("decrease key = %d, it' right = %d, it's left = %d, it's parent = %d\n", x->elem, x->right->elem, x->left->elem, x->p->elem);
    fibo_decrease_key(fh, x, 5);
    printf("after decrease key fh->min = %d\n", fh->min->elem);
    print_root_list(fh->min);
    x = fh->min->right->c->c;
    printf("x = %d, x->mark = %d, x->p->mark = %d\n", x->elem, x->mark, x->p->mark);
    fibo_decrease_key(fh, x, 7);
    print_root_list(fh->min);

    
    printf("_________________________fibo-max-degree-test______________________________\n");
    printf("fh->n = %d\n", fh->n);
    printf("max degree in fibo_heap = %d\n", fibo_max_degree(fh));

    printf("_________________________fibo-delete-test______________________________\n");
    root_list(fh->min);
    x = fh->min->right;
    printf("x = %d\n", x->elem);
    fibo_heap_delete(fh, x);
    root_list(fh->min);
    print_root_list(fh->min);


}

int main()
{
    printf("test enum: %d %d\n", TRUE, FALSE);
    printf("test NULL = %d\n", NULL);
    test();
    return 0;
}
