#include <stdio.h>
#include <stdlib.h>
typedef int elem_type;
struct q_node *ini_node(elem_type elem);
struct queue *ini_queue();
void enque(struct queue *q, elem_type elem);
struct q_node *deque(struct queue *q);


struct q_node {
    elem_type elem;
    struct q_node *next;
};

struct queue {
    int size;
    struct q_node *front;
    struct q_node *rear;
};

struct q_node *
ini_node(elem_type elem)
{
    struct q_node *temp = (struct q_node *)malloc(sizeof (struct q_node));
    temp->elem = elem;
    temp->next = NULL;
    return temp;
}

struct queue *
ini_queue()
{
    struct queue *temp = (struct queue *)malloc(sizeof (struct queue));
    temp->front = temp->rear = NULL;
    temp->size = 0;
    return temp;
}

void 
enque(struct queue *q, elem_type elem)
{
    struct q_node *new;

    new = ini_node(elem);
    if (q->rear == NULL) 
        q->rear = q->front = new;
    else {
        q->rear->next = new;
        q->rear = new;
    }
    (q->size)++;
    printf("q->size = %d  enqueue = %d q->front = %d  q->rear = %d\n", q->size, new->elem, q->front->elem, q->rear->elem);
}

struct q_node *
deque(struct queue *q)
{
    struct q_node *temp;

    if (q->front == NULL)
        temp = NULL;
    else {
        temp = q->front;
        q->front = temp->next;
        temp->next = NULL;
    }
    if (q->front == NULL)
        q->rear = NULL;
    (q->size)--;
    if (q->front != NULL)
        printf("q->size = %d  dequeue = %d q->front = %d  q->rear = %d\n", q->size, temp->elem, q->front->elem, q->rear->elem);
    return temp;
}

int 
main()
{
    int i;
    struct q_node *out;

    struct queue *q = ini_queue();
    for (i = 0; i < 10; i++)
        enque(q, i);
    while (q->rear != NULL)
        out = deque(q);
    return 0;
}
