#include <stdio.h>
#include <stdlib.h>
#define MAXSIZE      100

#define PARENT(i)   ((i-1) / 2)      
#define LEFT        ((i << 1) + 1)
#define RIGHT       ((i << 1) + 2)

typedef int elem_type;

struct minheap{
    elem_type *array;
    int current_size;
    int capacity;
};

struct minheap *
ini_minheap(int max_size)
{   
    struct minheap *mh = (struct minheap *)malloc(sizeof (struct minheap));

    mh->array = (elem_type *)malloc(sizeof (elem_type) * max_size);
    mh->current_size = 0;
    mh->capacity = max_size;
    return mh;
}

void 
sift_down(struct minheap *mh, int pos)
{   
    int i = pos;
    int j = 2 * i + 1;
    elem_type temp = mh->array[i];

    while (j < mh->current_size) {                                              //left exists
        if (j < mh->current_size-1 && mh->array[j+1] < mh->array[j])            //right exists and right is smaller
            j++;
        if (temp > mh->array[j]) {                                              //break min-heap order
            mh->array[i] = mh->array[j];
            i = j;
            j = 2 * j + 1;
        } else
            break;
    }
    mh->array[i] = temp;    //must be here!! for while may exit from conditon.
}

void 
sift_up(struct minheap *mh, int pos)
{
    int j = pos;
    int i = (j - 1) / 2;
    elem_type temp = mh->array[j];

    while (j > 0 && mh->array[i] > temp) {      //i >= 0 is wrong!!! when temp is smaller than mh->array[0], j == 1 or 2, 
        mh->array[j] = mh->array[i];            //while block makes j == 0 and i == 0, but mh->array[0] didn't change, while conditon always is true. loop forever.
//        printf("sift up little child , big root moves down. big root  = %d\n", mh->array[i]);
        j = i;
        i = (i - 1) / 2;
    }
    mh->array[j] = temp;
}

int 
insert(struct minheap *mh, elem_type ele)
{
    if (mh->current_size >= mh->capacity)
        return -1;
    else {
        mh->array[mh->current_size] = ele;
//        printf("insert test: cur_size = %d\t ele = %d\n", mh->current_size, ele);
        sift_up(mh, mh->current_size);
        mh->current_size++;
        return 0;
    }
}

elem_type 
delete_min(struct minheap *mh)
{
    if (mh->current_size <= 0)
        return mh->array[0];
    else {
        elem_type mini = mh->array[0];
        mh->array[0] = mh->array[mh->current_size - 1];
        --(mh->current_size);
        sift_down(mh, 0);
        return mini;
    }
}

void test_delmini(struct minheap *mh)
{
    int i;
    elem_type mini;

    for (i = 5; i >= 0; i--)
        insert(mh, i);
    for (i = 0; i <= 5; i++)
        printf("mh->array[%d] = %d\n", i, mh->array[i]);
    printf("mh->current_size = %d\n", mh->current_size);
    mini = delete_min(mh);
    printf("minimum elem = %d\n", mini);
    printf("mh->current_size = %d\n", mh->current_size);
    for (i = 0; i < mh->current_size; i++)
        printf("after delmin mh->array[%d] = %d\n", i, mh->array[i]);

}
void test_insert(struct minheap *mh)
{
    int i;
    for (i = 5; i >= 0; i--)
        insert(mh, i);
    for (i = 0; i <= 5; i++)
        printf("mh->array[%d] = %d\n", i, mh->array[i]);
    printf("mh->current_size = %d\n", mh->current_size);
}
void test_siftup(struct minheap *mh)
{
    int i;
    struct minheap *h = mh;
    for (i = 5; i >= 0; i--) {
        h->array[i] = (5-i);
        h->current_size++;
    }

    for (i = 0; i <= 5; i++)
        printf("mh->array[%d] = %d\n", i, h->array[i]);
    printf("mh->current_size = %d\n", h->current_size);

    for (i = (h->current_size - 1); i > 0; i--)
        sift_up(h, i);

    for (i = 0; i <= 5; i++)
        printf("mh->array[%d] = %d\n", i, h->array[i]);
    insert(h, -1);
    for (i = 0; i < h->current_size; i++)
        printf("mh->array[%d] = %d\n", i, h->array[i]);

}
void test_siftdown(struct minheap *mh)
{
    int i;
    struct minheap *h = mh;
    for (i = 5; i >= 0; i--) {
        h->array[i] = (5-i);
        h->current_size++;
    }

    for (i = 0; i <= 5; i++)
        printf("mh->array[%d] = %d\n", i, h->array[i]);
    printf("mh->current_size = %d\n", h->current_size);

    for (i = (h->current_size / 2 - 1); i >= 0; i--)
        sift_down(h, i);

    for (i = 0; i <= 5; i++)
        printf("mh->array[%d] = %d\n", i, h->array[i]);
}

int main()
{
    int i = 0;
    int j = (i-1)/2;

    struct minheap *mh;
    mh = ini_minheap(MAXSIZE);
//    test_siftdown(mh);
//    test_insert(mh);
    test_delmini(mh);
    return 0;

}
