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

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    for (list_ele_t *cur = q->head; cur;) {
        list_ele_t *tmp = cur->next;
        free(cur->value);
        free(cur);
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
    if (!(newh = malloc(sizeof(list_ele_t)))) {
        return false;
    }

    size_t size = strlen(s);
    if (!(newh->value = malloc(sizeof(char) * (size + 1)))) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, size);

    newh->next = q->head;
    q->head = newh;
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
    if (!q) {
        return false;
    }
    list_ele_t *newt;
    if (!(newt = malloc(sizeof(list_ele_t)))) {
        return false;
    }
    size_t size = strlen(s);
    if (!(newt->value = malloc(sizeof(char) * (size + 1)))) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, size);

    q->tail->next = newt;
    q->tail = newt;
    ++(q->size);
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (sp) {
        strncpy(sp, q->head->value, bufsize);
    }
    q->head = q->head->next;
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
    if (!q)
        return;

    char *values[q->size];
    int i = 0;
    for (list_ele_t *cur = q->head; cur; cur = cur->next) {
        values[i++] = cur->value;
    }
    i = q->size - 1;
    for (list_ele_t *cur = q->head; cur; cur = cur->next) {
        cur->value = values[i--];
    }
}

void quicksort(char **v, int lo, int hi)
{
    if (lo >= hi)
        return;

    /* partition */
    int pivot = rand() % (hi - lo + 1) + lo;
    char *tmp = v[pivot];
    v[pivot] = v[hi];
    v[hi] = tmp;

    int i = lo;
    for (int j = lo; j < hi; ++j) {
        if (strcmp(v[j], v[hi]) > 0) {
            tmp = v[j];
            v[j] = v[i];
            v[i++] = tmp;
        }
    }
    return quicksort(v, lo, i - 1), quicksort(v, i, hi);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 * Merge Sort
 */
void q_sort(queue_t *q)
{
    if (!q)
        return;

    char *values[q->size];
    int i = 0;
    for (list_ele_t *cur = q->head; cur; cur = cur->next) {
        values[i++] = cur->value;
    }

    quicksort(values, 0, q->size - 1);
    i = 0;
    for (list_ele_t *cur = q->head; cur; cur = cur->next) {
        cur->value = values[i++];
    }
}
