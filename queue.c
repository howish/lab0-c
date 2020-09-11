#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "harness.h"
#include "queue.h"

static list_ele_t *_list_ele_new(char const *s)
{
    list_ele_t *new_ele;
    size_t ssize = strlen(s) + 1;
    new_ele = malloc(sizeof(list_ele_t));
    if (new_ele == NULL)
        return NULL;
    new_ele->value = malloc(ssize * sizeof(char));
    if (new_ele->value == NULL) {
        free(new_ele);
        return NULL;
    }
    strncpy(new_ele->value, s, ssize);
    new_ele->next = NULL;
    return new_ele;
}

static void _list_ele_free(list_ele_t *ele)
{
    if (ele != NULL)
        free(ele->value);
    free(ele);
}

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* DONE: What if malloc returned NULL? */
    if (q == NULL)
        return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;
    list_ele_t *ele_to_free = q->head;
    list_ele_t *next_to_free;
    /* DONE: How about freeing the list elements and the strings? */
    while (ele_to_free != NULL) {
        next_to_free = ele_to_free->next;
        _list_ele_free(ele_to_free);
        ele_to_free = next_to_free;
    }
    /* Free queue structure */
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
    if (q == NULL)
        return false;
    list_ele_t *newh;

    newh = _list_ele_new(s);
    if (newh == NULL)
        return false;
    if (q->size) {
        newh->next = q->head;
        q->head = newh;
    } else
        q->head = q->tail = newh;
    q->size++;
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
    list_ele_t *newt;
    if (q == NULL)
        return false;

    newt = _list_ele_new(s);
    if (newt == NULL)
        return false;
    if (q->size) {
        q->tail->next = newt;
        q->tail = q->tail->next;
    } else
        q->head = q->tail = newt;
    q->size++;
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
    if (q == NULL || q->size == 0)
        return false;

    // Copy Head content
    if (sp) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    // Remove head
    list_ele_t *next = q->head->next;
    _list_ele_free(q->head);
    q->head = next;

    // Handle tail
    if (q->size == 1)
        q->tail = NULL;

    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q == NULL ? 0 : q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->size == 0)
        return;
    // Handle tail
    q->tail = q->head;
    // Reverse linked list
    list_ele_t *pre = NULL;
    list_ele_t *cur = q->head;
    list_ele_t *nxt;
    while (cur != NULL) {
        nxt = cur->next;
        cur->next = pre;
        pre = cur;
        cur = nxt;
    }
    q->head = pre;
}

/*
 * Quick sort on list node
 * Implement recursively
 */

static queue_t _q_cat(queue_t q, queue_t p)
{
    if (q.size == 0)
        return p;
    if (p.size == 0)
        return q;

    q.tail->next = p.head;
    q.tail = p.tail;
    q.size += p.size;
    return q;
}

static void _quick_sort(queue_t *q)
{
    if (q->size <= 1)
        return;
    // Get pivot
    srand(time(NULL));
    list_ele_t *piv_ele = q->head;
    list_ele_t *itr;
    int piv_idx = rand() % q->size;
    if (piv_idx == 0)
        itr = piv_ele->next;
    else {
        for (piv_idx--; piv_idx; piv_idx--, piv_ele = piv_ele->next)
            ;
        itr = piv_ele->next;
        piv_ele->next = itr->next;
        piv_ele = itr;
        itr = q->head;
    }
    piv_ele->next = NULL;
    queue_t piv = {piv_ele, piv_ele, 1};

    // Split
    queue_t les = {0}, mor = {0};
    queue_t *ref;

    while (itr) {
        int cmp = strcmp(itr->value, piv.head->value);
        ref = cmp == 0 ? &piv : cmp < 0 ? &les : &mor;
        if (ref->head == NULL) {
            ref->head = ref->tail = itr;
        } else {
            ref->tail->next = itr;
            ref->tail = ref->tail->next;
        }
        itr = itr->next;
        ref->tail->next = NULL;  // Maybe not needed? Nah, it's needed.
        ref->size++;
    }

    _quick_sort(&les);
    _quick_sort(&mor);
    *q = _q_cat(_q_cat(les, piv), mor);
}
/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

void q_sort(queue_t *q)
{
    if (q == NULL || !q->size)
        return;

    // Implement sorting by quick sort
    _quick_sort(q);
}
