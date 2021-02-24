#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q;
    if (!(q = malloc(sizeof(queue_t)))) {
        return NULL;
    }
    *q = (queue_t){
        .head = NULL,
        .tail = NULL,
        .size = 0,
    };
    return q;
}

void free_node(list_ele_t *n)
{
    free(n->value);
    free(n);
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;
    for (list_ele_t *cur = q->head; cur;) {
        list_ele_t *tmp = cur->next;
        free_node(cur);
        cur = tmp;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh;
    if (!(newh = malloc(sizeof(list_ele_t))))
        return false;

    size_t len = strlen(s);
    if (!(newh->value = malloc(sizeof(char) * (len + 1)))) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, len);
    newh->value[len] = '\0';

    newh->next = q->head;
    q->head = newh;
    if (!(q->tail))
        q->tail = newh;
    ++(q->size);
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newt;
    if (!(newt = malloc(sizeof(list_ele_t))))
        return false;

    size_t len = strlen(s);
    if (!(newt->value = malloc(sizeof(char) * (len + 1)))) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, len);
    newt->value[len] = '\0';

    newt->next = NULL;
    if (q->tail)
        q->tail->next = newt;
    else
        q->head = newt;
    q->tail = newt;
    ++(q->size);
    return true;
}
/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to
 * *sp (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->size)
        return false;
    if (sp) {
        strncpy(sp, q->head->value, bufsize);
        int len = strlen(sp);
        sp[len < bufsize ? len : bufsize - 1] = '\0';
    }
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    free_node(tmp);

    --(q->size);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size < 2)
        return;

    list_ele_t *new = q->head;
    list_ele_t *old = q->head->next;
    for (list_ele_t *tmp = old->next; old; tmp = old->next) {
        old->next = new;
        new = old;
        old = tmp;

        /* this if statement is needed,
         * although the for should do it.
         * I DONT KNOW WHY
         */
        if (!old)
            break;
    }

    q->tail = q->head;
    q->head = new;
    q->tail->next = NULL;
}


/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO */
}
