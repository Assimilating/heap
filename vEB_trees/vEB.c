#include <stdio.h>
#include <stdlib.h>

#define HIGH(X, W)      ((X) >> (W >> 1))
//operator priority!
#define LOW(X, W)       ((X) & ((1 << (W >> 1)) - 1))

#define INDEX(H, L, W)  (H << (W >> 1) | L)

#define SQRU(W)         (1 << (W >> 1))

#define CSIZE(U, W)     (U >> (W >> 1))

#define POPU(u) do{ \
    u = (0x55555555 & u) + (0x55555555 & (u >> 1)); \
    u = (0x33333333 & u) + (0x33333333 & (u >> 2)); \
    u = (0x0f0f0f0f & u) + (0x0f0f0f0f & (u >> 4)); \
    u = (0x00ff00ff & u) + (0x00ff00ff & (u >> 8)); \
    u = (0x0000ffff & u) + (0x0000ffff & (u >> 16)); \
} while(0) 

#define TRUE    1
#define FALSE   0


struct veb{
    int u;
    int min, max;
    struct veb *summary;
    struct veb **cluster;
};

int population(int x);
void travel_veb(struct veb *root);
void swap(int *a, int *b);

struct veb *build_skeleton(int u);
int member(struct veb *root, int x);
int minimum(struct veb *root);
int maximum(struct veb *root);
int predecessor(struct veb *root, int x);
int successor(struct veb *root, int x);
void empty_insert(struct veb *root, int x);
int insert(struct veb *root, int x);
int delete(struct veb *root, int x);

void travel_veb(struct veb *root)
{
    int i, u, w, csize;

    u = root->u;
    w = population(u - 1);
    csize = CSIZE(u, w);
//    printf("w = %d, csize = %d,", w, csize);

    if (root) {
        
        if (u == 2)
            printf("veb->u = %d, min = %d, max = %d\n", root->u, root->min, root->max);
        else {
            printf("veb->u = %d, min = %d, max = %d\n", root->u, root->min, root->max);
            travel_veb(root->summary);
            for (i = 0; i < csize; i++)
                travel_veb(root->cluster[i]);
        }
        /*  when u == 2, cluster == NULL, but for loop access it. 
            printf("veb->u = %d, min = %d, max = %d\n", root->u, root->min, root->max);
            travel_veb(root->summary);
            for (i = 0; i < csize; i++)
                travel_veb(root->cluster[i]);
        */
    }
}

struct veb *
build_skeleton(int u)
{
    int i, w, csize;
    struct veb *v;


    v = (struct veb *)malloc(sizeof (struct veb));
    v->u = u;
    v->min = v->max = -1;
    w = population(u - 1);
    csize = CSIZE(u, w);
    printf("build test universe = %d, cluster number = %d, cluster universe = %d\n", u, csize, SQRU(w));
    if (v->u == 2) {
        v->summary = NULL;
        v->cluster = NULL;
    } else {
        v->summary = build_skeleton(csize);
        v->cluster = (struct veb **)malloc((sizeof (struct veb)) * csize);
        for (i = 0; i < csize; i++)
            v->cluster[i] = build_skeleton(SQRU(w));
    }
    return v;
}

int member(struct veb *v, int x)
{
    int w;

    w = population(v->u - 1);
    if (v->min == x || v->max == x) 
        return TRUE;
    else if (v->u == 2)
        return FALSE;
    else 
        return member(v->cluster[HIGH(x, w)], LOW(x, w));
}

int 
predecessor(struct veb *v, int x)
{
    int w, min_low, pre_cluster, offset;

    w = population(v->u - 1);
    if (v->u == 2) {
        if (x == 1 && v->min == 0)
            return 0;
        else 
            return -1;
    } else if (v->max != -1 && x > v->max)
        return v->max;
    else {
        min_low = minimum(v->cluster[HIGH(x, w)]);
        if (min_low != -1 && LOW(x, w) > min_low) {
            offset = predecessor(v->cluster[HIGH(x, w)], LOW(x, w));
            return INDEX(HIGH(x, w), offset, w);
        } else {
            pre_cluster = predecessor(v->summary, HIGH(x, w));
            if (pre_cluster !=  -1) {
                offset = maximum(v->cluster[pre_cluster]); 
                return INDEX(pre_cluster, offset, w);
            } else {
               if (v->min != -1 && x > v->min)
                   return v->min;
               else
                   return -1;
            }
        }
    }
}

int 
successor(struct veb *v, int x)
{
    int w, max_low, offset, succ_cluster;

    w = population(v->u - 1);
    if (v->u == 2) {
        if (x == 0 && v->max == 1)
            return 1;
        else
            return -1;
    } else if (v->min != -1 && x < v->min) {
        return v->min;
    } else {
       max_low = maximum(v->cluster[HIGH(x, w)]);
       if (max_low != -1 && LOW(x, w) < max_low) {
           offset = successor(v->cluster[HIGH(x, w)], LOW(x, w));
           return INDEX(HIGH(x, w), offset, w);
       } else {
           succ_cluster = successor(v->summary, HIGH(x, w));
           if (succ_cluster == -1)
               return -1;
           else {
               offset = minimum(v->cluster[succ_cluster]);
               return INDEX(succ_cluster, offset, w);
           }
       }
    }
}


int 
minimum(struct veb *v)
{
    return v->min;
}
int maximum(struct veb *v)
{
    return v->max;
}

void test_macro()
{
    int i;
    long long u = 0x20;
    int w = population(u-1);

    for (i = 0; i < u; i++)
        printf("i = %d\t, word size = %d, high = %d, low = %d, sqru = %d, csize = %d\n",\
                i, w, HIGH(i, w), LOW(i, w), SQRU(w), CSIZE(u, w));

}

int population(int u)
{
    u = (0x55555555 & u) + (0x55555555 & (u >> 1)); \
    u = (0x33333333 & u) + (0x33333333 & (u >> 2)); \
    u = (0x0f0f0f0f & u) + (0x0f0f0f0f & (u >> 4)); \
    u = (0x00ff00ff & u) + (0x00ff00ff & (u >> 8)); \
    u = (0x0000ffff & u) + (0x0000ffff & (u >> 16)); \

    return u;
}

void
empty_insert(struct veb *v, int x)
{
    v->min = x;
    v->max = x;
}

void swap(int *a, int *b)
{
    int c;
    c = *a;
    *a = *b;
    *b = c;
}

int 
insert(struct veb *v, int x)
{
    int w;

    if (v->min == -1) {
        empty_insert(v, x);
    } else {
        if (x < v->min) 
            swap(&x, &(v->min));
        if (v->u > 2) {
            w = population(v->u - 1);
            if (minimum(v->cluster[HIGH(x, w)]) == -1) {
                empty_insert(v->cluster[HIGH(x, w)], LOW(x, w));
                insert(v->summary, HIGH(x, w));
            } else 
                insert(v->cluster[HIGH(x, w)], LOW(x, w));
        }
        if (x > v->max)
            v->max = x;
    }
}

int 
delete(struct veb *v, int x)
{
    int w, csize;
    int first_cluster, offset, summary_max;

    w = population(v->u - 1);
    csize = CSIZE(v->u, w);

    if (v->min == v->max) 
        v->min = v->max = -1;
    else if (v->u == 2) {
        if (x == 0)
            v->min = v->max;
        else
            v->max = v->min;
    } else {
        if (x == v->min) {
            first_cluster = minimum(v->summary);
            offset = minimum(v->cluster[first_cluster]);
            x = INDEX(first_cluster, offset, w);
            v->min = x;
        }
        delete(v->cluster[HIGH(x, w)], LOW(x, w));
        if (minimum(v->cluster[HIGH(x, w)]) == -1) {
            delete(v->summary, HIGH(x, w));
            if (x == v->max) {
                summary_max = maximum(v->summary);
                if (summary_max == -1)
                    v->max = v->min;
                else
                    v->max = INDEX(summary_max, maximum(v->cluster[summary_max]), w);
            }
        } else if (x == v->max)
            v->max = INDEX(HIGH(x, w), maximum(v->cluster[HIGH(x, w)]), w);
    }
}


int 
main()
{
    int u = 16;
    struct veb *root;

    root = build_skeleton(u);
    travel_veb(root);
//    printf("test population: %d\n", population(u - 1));
//    test_macro();
    
    printf("---------------------------insert test---------------------------\n");
    insert(root, 2);
    insert(root, 3);
    insert(root, 4);
    insert(root, 5);
    insert(root, 7);
    insert(root, 14);
    insert(root, 15);
    travel_veb(root);



    printf("---------------------------delete test---------------------------\n");
    delete(root, 2);
    delete(root, 3);
    travel_veb(root);

    printf("---------------------------member test---------------------------\n");
    printf("4 is %d the vebt\n", member(root, 4));
    printf("3 is %d the vebt\n", member(root, 3));

    printf("---------------------------predecessor test---------------------------\n");
    int j;
    for (j = 15; j >= 0; j--)
        printf("predecessor of %d is %d\n", j, predecessor(root, j));
    printf("---------------------------successor test---------------------------\n");
    int i;
    for (i = 0; i < 16; i++)
        printf("successor of %d is %d\n", i, successor(root, i));
    return 0;
}




