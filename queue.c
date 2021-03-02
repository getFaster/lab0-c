#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    for (list_ele_t *tmp = old->next;; tmp = old->next) {
        old->next = new;
        new = old;
        old = tmp;
        if (!old)
            break;
    }

    q->tail = q->head;
    q->head = new;
    q->tail->next = NULL;
}

/*
#ifdef DEBUG
void print_list()
{
    for (list_ele_t *cur = head;; cur = cur->next) {
        printf("%s ", cur->value);
        if (cur == tail)
            break;
    }
    printf("\n----------------------------------------------------\n");
}
#endif
*/

/*
 * return tail of the sorted list,
 * and tail->next = head
 */
list_ele_t *quicksort(list_ele_t *head, list_ele_t *tail)
{
    srand(time(NULL));
    if (rand() & 1) {
        char *tmp = head->value;
        head->value = tail->value;
        tail->value = tmp;
    }

    /* partition into sm, eq, lg */
    list_ele_t *sm, *eq, *lg, *sm_t, *eq_t, *lg_t, *tmp;
    sm = eq = lg = sm_t = eq_t = lg_t = NULL;
    for (list_ele_t *cur = head; cur != tail;) {
        int diff = strcmp(cur->value, tail->value);
        tmp = cur->next;
        if (diff < 0) {
            if (!sm_t)
                sm_t = cur;
            cur->next = sm;
            sm = cur;
        } else if (!diff) {
            if (!eq_t)
                eq_t = cur;
            cur->next = eq;
            eq = cur;
        } else {
            if (!lg_t)
                lg_t = cur;
            cur->next = lg;
            lg = cur;
        }
        cur = tmp;
        if (cur == tail)
            break;
    }
    tail->next = eq;
    eq = tail;
    if (!eq_t)
        eq_t = eq;

    /* sort lg and sm, then put them back together */
    if (lg) {
        tail = quicksort(lg, lg_t);
        eq_t->next = tail->next;
        tail->next = NULL;
    } else {
        tail = eq_t;
    }

    if (sm) {
        tmp = quicksort(sm, sm_t);
        head = tmp->next;
        tmp->next = eq;
    } else {
        head = eq;
    }

    tail->next = head;
    return tail;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size < 2)
        return;
    q->tail = quicksort(q->head, q->tail);
    q->head = q->tail->next;
    q->tail->next = NULL;
}
