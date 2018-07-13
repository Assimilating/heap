#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define ODD(X)          ((X) & 1)
#define EVEN(X)         (! ((X) & 1))

/* High: cluster number in current level
 * X:  index numebr of current level
 * W: word size of current level.  W = 2m or 2m + 1 
 */
#define HIGH(X, W)          ((X) >> (W >> 1))                    
//offset in cluster in next level
#define LOW(X, W)           ((X) & ((1 << (W >> 1)) - 1))       

/* key value in current level
 * ERROR in INEDX!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * because operator priority, that arithematic is upper than shift!!!!!!!!!!!!!!!!!!
 * #define index(H, L, W)   ((H << (W >> 1)) + L)  
 * is also right.
 */

#define INDEX(H, L, W)      (H << (W >> 1) | L)         

/*
 * SQRU: universe size of children 
 * W: word size of current level. W = 2m or 2m + 1 
 */
#define SQRU(W)             (1 << (W >> 1))


/*
 * CLEN: cluster size of current level
 * U: universize of current level
 * W: word size of current level. W = 2m or 2m + 1 
 */
#define CLEN(U, W)         (U >> (W >> 1))

int test_population(int x);
void test_macro(void);
void test_universe(void);

/*
 * when universe size is determined, the skeleton of pro_veb is determined too. 
 */
struct pro_veb *
build_skeleton(int u);


/*
 * determinig wether a value is in the set
 */
int 
member(struct pro_veb *v, int x);


/*
 * inserting an element 
 */
void 
insert(struct pro_veb *v, int x);


/*
 * finding the minimum element
 */
int 
minimum(struct pro_veb *v);

/*
 * finding the successor
 */
int successor(struct pro_veb *v, int x);


struct pro_veb {
    int u;
    struct pro_veb *summary;
    struct pro_veb **cluster;
    int A[2];
};

struct pro_veb *
build_skeleton(int u)
{
    int i;
    int w = test_population(u-1);       //word size
    int clen = CLEN(u, w);              //cluster length 
    printf("u = %d, w == %d, clen = %d\n", u, w, clen);
    struct pro_veb *ptr = (struct pro_veb *)malloc(sizeof (struct pro_veb));

    if (u == 2) {
        ptr->u = 2;
        printf("ptr->u == %d\n", ptr->u);
        ptr->summary = NULL;
        ptr->cluster = NULL;
        ptr->A[0] = ptr->A[1] = 0;
        printf("base unit complete\n");
    } else {
        ptr->u = u;
        printf("ptr->u == %d\n", ptr->u);

        printf("summary\n");
        ptr->summary = build_skeleton(clen);
        ptr->cluster = (struct pro_veb **)malloc(sizeof (struct pro_veb *) * clen);

        for (i = 0; i < clen; i++) {
            printf("ptr->u = %d, cluster[%d]\n", ptr->u, i);
            ptr->cluster[i] = build_skeleton(SQRU(w));
        }
    }
    printf("return ptr->u = %d\n", ptr->u);
    return ptr;
}


void 
travel_veb(struct pro_veb *p)
{
    int i, u, w;
    u = p->u;
    w = test_population(p->u - 1);
    int clen = CLEN(u, w);

    if (p) {
        if (p->u == 2)
            printf("p->u = %d, p->A[0] = %d, p->A[1] = %d\n", p->u, p->A[0], p->A[1]);
        else {
            printf("p->u = %d\n", p->u);
            travel_veb(p->summary);
            for (i = 0; i < clen; i++)
                travel_veb(p->cluster[i]);
        }   
    }
}


void test_macro()
{
    int i;
    int u, w, clen;

    /*
     *  use index i to calculate:
     *  1. cluster number
     *  2. offset in a cluster 
     */
    for (i = 0; i < 16; i++)
        printf("i = %d, ODD(i) = %d, EVEN(i) = %d, HIGH(i) = %d, LOW(i) = %d, INDEX() = %d\n", i, ODD(i), EVEN(i), HIGH(i, 4), LOW(i, 4), INDEX(HIGH(i, 4), LOW(i, 4), 4));

    /*
     * use universe u and word size w to calculate:
     * 1. universe u of its children
     * 2. word size of its children 
     * 3. cluster size of its own 
     */
    u = 32;
    w = test_population(u-1);
    clen = CLEN(u, w);
    while (u != 2) {
        printf("u = %d, w = %d, clen = %d\n", u, w, clen);
        u = SQRU(w);
        w = test_population(u-1);
        clen = CLEN(u, w);
    }
}

void test_universe()
{
    int u;      //size of universe
    int w;      //bits of key value
    int temp;   //counter for w
    /*
    printf("Please enter the size of universe ");
    scanf("%d", u);
    printf("The size of universe = %d\n", u);
    */
    u = 65536;
    for (temp = 1, w = 0; temp < u; temp <<= 1, ++w)
        ;
    printf("for calculate w = %d\n", w);
    w = test_population(u-1);
    printf("population claculate w = %d\n", w);

}

int test_population(int x)
{
   x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
   x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
   x = (x & 0x0f0f0f0f) + ((x >> 4) & 0x0f0f0f0f);
   x = (x & 0x00ff00ff) + ((x >> 8) & 0x00ff00ff);
   x = (x & 0x0000ffff) + ((x >> 16) & 0x0000ffff);
   return x;
}

void
insert(struct pro_veb *v, int x)
{
    int clen = test_population(v->u - 1);

    if (v->u == 2){
        printf("insert %d into %d->A[%d]\n", x, v->u, x);
        v->A[x] = 1;
    } else {
        printf("insert %d into %d->cluster[%d] with %d offset\n", x, v->u, HIGH(x, clen), LOW(x, clen));
        insert(v->cluster[HIGH(x, clen)], LOW(x, clen));
        insert(v->summary, HIGH(x, clen));
    }
}

/*
 * func: find the minimum element in pro_veb v
 * para: a ptr pointing to a pro_veb instance
 * retu: the value of the minimum element
 *       return -1 if the whole pro_veb is empty
 */
int 
minimum(struct pro_veb *v)
{
    int mini_cluster, offset, w;

    if (v->u == 2) {
        printf("get into 2\n");
        if (v->A[0] == 1)
            return 0;
        else if (v->A[1] == 1)
            return 1;
        else
            return -1;
    } else {
        printf("get into %d->summary\n", v->u);
        mini_cluster = minimum(v->summary);
        if (mini_cluster == -1)
            return -1;
        else {
            offset = minimum(v->cluster[mini_cluster]);
            w = test_population(v->u - 1);
            printf("u = %d, w = %d, mini_cluster = %d, offset = %d, INDEX = %d\n", v->u, w, mini_cluster, offset, INDEX(mini_cluster, offset, w));
            return INDEX(mini_cluster, offset, w);
        }
    }
}

int main()
{
    test_macro();
    test_universe();


    int i = 2147483647;
    test_population(i);


    printf("------------------------------build_skeleton--------------------------------\n");
    struct pro_veb  *root = NULL;
    root = build_skeleton(16);
    travel_veb(root);

    printf("------------------------------insert_test--------------------------------\n");
    insert(root, 13);
    travel_veb(root);

    printf("------------------------------minimum_test--------------------------------\n");
    printf("minimum = %d\n", minimum(root));

    return 0;
}
