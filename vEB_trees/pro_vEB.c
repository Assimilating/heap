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
/*
 * delete the element x 
 */
int delete(struct pro_veb *c, int x);


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
        printf("i = %d, ODD(i) = %d, EVEN(i) = %d, HIGH(i) = %d, LOW(i) = %d, INDEX() = %d\n",  
                i, ODD(i), EVEN(i), HIGH(i, 4), LOW(i, 4), INDEX(HIGH(i, 4), LOW(i, 4), 4));

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


/*
 *func: calculate the number of 1 in an integer.
 *      for the universe size, u, is always exp of 2, like 2^w
 *      so the number of 1 in (u - 1) equals the exp w.
 *para: the int number x
 *retu: the number of 1 the para x. 
 */
int test_population(int x)
{
   x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
   x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
   x = (x & 0x0f0f0f0f) + ((x >> 4) & 0x0f0f0f0f);
   x = (x & 0x00ff00ff) + ((x >> 8) & 0x00ff00ff);
   x = (x & 0x0000ffff) + ((x >> 16) & 0x0000ffff);
   return x;
}


/*
 * func: determinig wether a value is in the set
 * para: the ptr point to the pro_veb, the value x
 * retu: if x is a member of v, return 1, otherwise return 0
 *
 */
int member(struct pro_veb *v, int x)
{
    int w;

    w = test_population(v->u - 1);
    if (v->u == 2) 
        return v->A[x];
    else 
        return member(v->cluster[HIGH(x, w)], LOW(x, w));
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

int
maximum(struct pro_veb *v)
{
    int max_cluster, offset, w;
    
    if (v->u == 2) {
        if (v->A[1] == 1)
            return 1;
        else if (v->A[0] == 1)
            return 0;
        else
            return -1;
    } else {
        max_cluster = maximum(v->summary);
        if (max_cluster == -1)
            return -1;
        else {
            offset = maximum(v->cluster[max_cluster]);
            w = test_population(v->u - 1);
            return INDEX(max_cluster, offset, w);
        }
    }
}

int 
successor(struct pro_veb *v, int x)
{
    int mini_cluster, offset, w;

    if (v->u == 2) {
        if (x == 0 && v->A[1] == 1)
            return 1;
        else
            return -1;
    } else {
        w = test_population(v->u - 1);
        offset = successor(v->cluster[HIGH(x, w)], LOW(x, w));      //make assumptions that sx is in the same cluster with x
        if (offset != -1)                                           //when offset != -1, the assumption of the next recursive level was right.
            return INDEX(HIGH(x, w), offset, w);
        else {                                                      //when offset == -1, the assumptin of the next recursive level was wrong. 
            mini_cluster = successor(v->summary, HIGH(x, w));
            if (mini_cluster == -1)
                return -1;
            else {
                offset = minimum(v->cluster[mini_cluster]);
                return INDEX(mini_cluster, offset, w);
            }
        }
    }
}

int 
predecessor(struct pro_veb *v, int x)
{
    int pre_cluster, offset, w;

    if (v->u == 2) {
        if (x == 1 && v->A[0] == 1)
            return 0;
        else
            return -1;
    } else {
        w = test_population(v->u - 1);
        offset = predecessor(v->cluster[HIGH(x, w)], LOW(x, w));
        if (offset != -1)
            return INDEX(HIGH(x, w), offset, w);
        else {
            pre_cluster = predecessor(v->summary, HIGH(x, w));
            if (pre_cluster == -1)
                return -1;
            else {
                offset = maximum(v->cluster[pre_cluster]);
                return INDEX(pre_cluster, offset, w);
            }
        }
    }
}



int delete(struct pro_veb *v, int x)
{
    int cluster_count, w;

    w = test_population(v->u - 1);
    if (v->u == 2) {
        v->A[x] = 0;
        if (v->A[0] == v->A[1])
            return 0;
        else
            return 1;
    } else {
        cluster_count = delete(v->cluster[HIGH(x, w)], LOW(x, w));
        if (cluster_count == 0)
            return delete(v->summary, HIGH(x, w));
        else
            return cluster_count;
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
    insert(root, 2);
    insert(root, 3);
    insert(root, 4);
    insert(root, 5);
    insert(root, 7);
    insert(root, 14);
    insert(root, 15);
    travel_veb(root);

    printf("------------------------------minimum_test--------------------------------\n");
    printf("minimum = %d\n", minimum(root));
    printf("------------------------------maximum_test--------------------------------\n");
    printf("minimum = %d\n", maximum(root));

    printf("------------------------------successor_test--------------------------------\n");
    int x, sx;
    x = 0;
    while(x < 16) {
        sx = successor(root, x);
        printf("x = %d, sx = %d\n", x, sx);
        x++;
    }
    printf("------------------------------predecessor_test--------------------------------\n");
    int y, py;
    y = 15;
    while(y > 0) {
        py = predecessor(root, y);
        printf("y = %d, py = %d\n", y, py);
        y--;
    }

    printf("------------------------------member_test--------------------------------\n");
    int condition;
    for (x = 0; x < 16; x++)
        if (condition = member(root, x))
            printf("%d is in the pro_veb\n", x);
        else
            printf("%d is absent\n", x);
    printf("------------------------------delete_test--------------------------------\n");
    delete(root, 7);
    travel_veb(root);

    delete(root, 4);
    delete(root, 5);
    travel_veb(root);

    delete(root, 2);
    delete(root, 3);
    travel_veb(root);

    delete(root, 14);
    delete(root, 15);
    travel_veb(root);

    return 0;
}
