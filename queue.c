#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

static list_ele_t* _list_ele_new(char const *s)
{
    list_ele_t *new_ele;
    size_t ssize = strlen(s) + 1;
    new_ele = malloc(sizeof(list_ele_t));
    if (new_ele == NULL) return NULL;
    new_ele->value = malloc(ssize * sizeof(char));
    if (new_ele->value == NULL) 
    {
        free(new_ele);
        return NULL;
    }
    /* Q: What if either call to malloc returns NULL?     */
    /* A: Return NULL to tell user the operation failed.  */
    /*    And don't forget to free the first one if the   */
    /*    second one failed.                              */
    strcpy(new_ele->value, s);
    return new_ele;
}

static void _list_ele_free(list_ele_t* ele)
{
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
    if (q == NULL) return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    list_ele_t *ele_to_free = q->head;
    list_ele_t *next_to_free;
    /* DONE: How about freeing the list elements and the strings? */
    while(ele_to_free)
    {
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
    list_ele_t *newh;
    if (q == NULL) return false;
    
    newh = _list_ele_new(s);
    if (newh == NULL) return false;
    if (q->size) {
        newh->next = q->head;
        q->head = newh;
    } else q->head = q->tail = newh;
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
    if (q == NULL) return false;
    
    newt = _list_ele_new(s);
    if (newt == NULL) return false;
    if (q->size) q->tail = (q->tail->next = newt);
    else q->head = q->tail = newt;
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
    if (!q || !q->size) return false;

    // Copy Head content
    if (!sp)
    {
        strncpy(sp, q->head->value, bufsize-1);
        sp[bufsize-1] = '\0';
    }
    // Remove head
    list_ele_t *next;
    next = q->head->next;
    _list_ele_free(q->head);
    q->head = next;

    // Handle tail
    if (q->size == 1) q->tail = NULL;

    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q->size;
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
    if (!q || !q->size) return;
    // Handle tail
    q->tail = q->head;
    // Reverse linked list
    list_ele_t *pre = NULL; 
    list_ele_t *cur = q->head; 
    list_ele_t *nxt;
    while (cur) {
        nxt = cur->next;
        cur->next = pre;
        pre = cur;
        cur = nxt;
    }
    q->head = pre;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
static void _merge_two_list(list_ele_t *lst1, list_ele_t *lst2)
{
    list_ele_t *newhead = NULL, *newtail = NULL;
    while (lst1 || lst2)
    {
        if (!lst1)  
    }
}


static void _merge_sort(list_ele_t *head, size_t len)
{
    if (len <= 1) return;
    list_ele_t *prevmid, *mid = head;
    for (size_t i = 0; i < len / 2 - 1; i++) mid = mid->next;
    prevmid = mid;
    mid = mid->next;
    prevmid->next = NULL;

    _merge_sort(head, len / 2);
    _merge_sort(mid, len - len / 2);

    // merge

}

void q_sort(queue_t *q)
{
    if (!q || !q->size) return;

    _merge_sort(q->head);
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
}
