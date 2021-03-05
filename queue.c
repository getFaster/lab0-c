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
 * navie mergesort
 * return head pointer and set tail pointer in **t
 * the list will end with NULL
 */
list_ele_t *mergesort(list_ele_t *h, list_ele_t **t, int size)
{
    if (size == 1) {
        h->next = NULL;
        *t = h;
        return h;
    }

    /* devide into half and sort them */
    list_ele_t *mid = h;
    for (int i = 0; i < size / 2; ++i)
        mid = mid->next;

    h = mergesort(h, t, size / 2);

    list_ele_t *t_cand;
    mid = mergesort(mid, &t_cand, size / 2 + (size & 1));

    /* merge now */
    list_ele_t *new, *new_t;
    if (strcmp(h->value, mid->value) <= 0) {
        new = new_t = h;
        h = h->next;
    } else {
        new = new_t = mid;
        mid = mid->next;
    }
    while (h && mid) {
        if (strcmp(h->value, mid->value) <= 0) {
            new_t->next = h;
            new_t = h;
            h = h->next;
        } else {
            new_t->next = mid;
            new_t = mid;
            mid = mid->next;
        }
    }
    if (!h) {
        new_t->next = mid;
        *t = t_cand;
    } else {
        new_t->next = h;
    }
    return new;
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
    q->head = mergesort(q->head, &(q->tail), q->size);
}
