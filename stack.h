/*
#include <stdio.h>
#include <stdlib.h>
*/
#define for_each_node(head, p) \
    for (p = head->next; p != NULL; p = p->next)
typedef struct fibo_node *selem_type;

struct stack *ini_stack_node(selem_type elem);
struct stack *ini_stack();
void push(struct stack *head, selem_type elem);
struct stack *pop(struct stack *head);

struct stack {
    selem_type elem;
    struct stack *next;
};

struct stack *
ini_stack_node(selem_type elem)
{
    struct stack *temp = (struct stack *)malloc(sizeof (struct stack));
    temp->elem = elem;
    temp->next = NULL;
    return temp;
}

struct stack *
ini_stack()
{
    struct stack *temp = (struct stack *)malloc(sizeof (struct stack));
    temp->next = NULL;
    return temp;
}

void
push(struct stack *head, selem_type elem)
{
    struct stack *temp = ini_stack_node(elem);
    printf("push :%d\n", temp->elem);
    if (!head->next)
        head->next = temp;
    else {
        temp->next = head->next;
        head->next = temp;
    }
}

struct stack *
pop(struct stack *head)
{
    struct stack *top = NULL;
    if (!head->next)
        printf("error: empty stack\n");
    else {
        top = head->next;
        head->next = head->next->next;
        top->next = NULL;
    }
    return top;
}
int 
isempty(struct stack *h)
{
    if (!h->next)
        return 1;
    else
        return 0;
}
/*
int main()
{
    int i;
    struct stack *p = NULL;
    struct stack *head = ini_stack();
    for (i = 0; i < 10; i++)
        push(head, i);
    for_each_node(head, p)
        printf(" %d ", p->elem);
    putchar('\n');
    for (i = 0; i < 10; i++) {
        p = pop(head);
        printf(" %d ", p->elem);
    }
    return 0;
}
*/
