#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */

struct list_head *q_new()
{
    // Declare a pointer to the list_head structure and allocate memory
    struct list_head *new_head = malloc(sizeof(struct list_head));

    // Check if memory allocation was successful
    if (new_head) {
        INIT_LIST_HEAD(new_head);  // Initialize the new list head
        return new_head;
    }
    return NULL;  // If memory allocation fails, return NULL
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    // Check if the list pointer is NULL or the list is empty
    if (!head || list_empty(head)) {
        free(head);
        return;
    }

    // Initialize 'pos' to the first element in the list
    struct list_head *pos = head->next;

    // Traverse the list and free each element
    while (pos != head) {
        // Get the structure that contains this list node
        element_t *entry = list_entry(pos, element_t, list);

        // Move to the next element before freeing the current one
        pos = pos->next;

        // Free the memory for the current element
        q_release_element(entry);
    }

    // Free the list head itself after all elements are released
    free(head);
    return;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;

    // Allocate memory for a new element
    element_t *new_ele = malloc(sizeof(element_t));
    if (!new_ele)
        return false;

    // Initialize the list_head of the new element
    INIT_LIST_HEAD(&new_ele->list);

    // Duplicate the string and assign it to the new element's value
    new_ele->value = strdup(s);

    // If string duplication fails, free the allocated memory and return false
    if (!new_ele->value) {
        free(new_ele);
        return false;
    }

    // Add the new element to the head of the list
    list_add(&new_ele->list, head);

    return true;
}


/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;

    element_t *new_ele = malloc(sizeof(element_t));
    if (!new_ele)
        return false;

    INIT_LIST_HEAD(&new_ele->list);
    new_ele->value = strdup(s);

    if (!new_ele->value) {
        free(new_ele);
        return false;
    }

    list_add_tail(&new_ele->list, head);

    return true;
}


/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *first_entry = list_first_entry(head, element_t, list);

    // If sp is not NULL, copy the value of the first entry into the buffer sp
    if (sp != NULL) {
        strncpy(sp, first_entry->value, bufsize - 1);
        sp[bufsize - 1] = '\0';  // Ensure the string is null-terminated
    }

    // Remove the first entry from the list
    list_del(&first_entry->list);

    return first_entry;
}



/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *last_entry = list_last_entry(head, element_t, list);

    if (sp != NULL) {
        strncpy(sp, last_entry->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del(&last_entry->list);

    return last_entry;
}


/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    // If the list doesn't exist, size is 0
    if (!head)
        return 0;

    int len = 0;
    struct list_head *pos;

    list_for_each (pos, head)
        len++;
    return len;
}

/* Delete the middle node in queue */

bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    struct list_head *ptr_1 = head->next;
    struct list_head *ptr_2 = head->next;

    // Traverse the list until fast and its next pointer reach head
    while (ptr_1 != head && ptr_1->next != head) {
        ptr_1 = ptr_1->next->next;
        ptr_2 = ptr_2->next;
    }

    element_t *mid = list_entry(ptr_2, element_t, list);

    list_del(ptr_2);
    free(mid->value);
    free(mid);
    return true;
}


/* Delete all nodes that have duplicate string */

/**
 * Definition for singly-linked list.
 * public class ListNode {
 *     public int val;
 *     public ListNode next;
 *     public ListNode(int val=0, ListNode next=null) {
 *         this.val = val;
 *         this.next = next;
 *     }
 * }
 */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    struct list_head *cur = head->next;
    while (cur != head && cur->next != head) {
        struct list_head *next = cur->next;
        element_t *elem_cur = list_entry(cur, element_t, list);
        element_t *elem_next = list_entry(next, element_t, list);

        if (strcmp(elem_cur->value, elem_next->value) == 0) {
            while (next != head &&
                   strcmp(elem_cur->value, elem_next->value) == 0) {
                list_del(next);
                free(elem_next->value);
                free(elem_next);
                next = cur->next;
                elem_next = list_entry(next, element_t, list);
            }
            list_del(cur);
            free(elem_cur->value);
            free(elem_cur);
            cur = next;
        } else {
            cur = cur->next;
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    q_reverseK(head, 2);
}


/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }
    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head) {
        node->next = node->prev;
        node->prev = safe;
    }
    node->next = node->prev;
    node->prev = safe;
    return;
}

/* Reverse the nodes of the list k at a time */

void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head) {
        return;
    }

    int split_time = q_size(head) / k;
    struct list_head *tail;

    LIST_HEAD(tmp);
    LIST_HEAD(new_head);

    for (int i = 0; i < split_time; i++) {
        int j = 0;
        list_for_each (tail, head) {
            if (j >= k) {
                break;
            }
            j++;
        }
        list_cut_position(&tmp, head,
                          tail->prev);  // Cuts the k nodes from the list and
                                        // stores them in the tmp list.
        q_reverse(&tmp);                // Reverses the k nodes.
        list_splice_tail_init(
            &tmp, &new_head);  // Appends the reversed nodes to the new_head.
    }
    list_splice_init(
        &new_head,
        head);  // Attach the reversed list back to the original list:
}

/* Start of sort */
typedef int (*compare_func_t)(struct list_head *, struct list_head *);

static int cmp_func(struct list_head *a, struct list_head *b)
{
    const element_t *element_a = list_entry(a, element_t, list);
    const element_t *element_b = list_entry(b, element_t, list);
    return strcmp(element_a->value, element_b->value);
}

static struct list_head *merge_list(struct list_head *first,
                                    struct list_head *second,
                                    compare_func_t cmp)
{
    struct list_head dummy;
    struct list_head *tail = &dummy;
    dummy.next = NULL;

    while (first && second) {
        if (cmp(first, second) <= 0) {
            tail->next = first;
            first->prev = tail;
            first = first->next;
        } else {
            tail->next = second;
            second->prev = tail;
            second = second->next;
        }
        tail = tail->next;
    }

    tail->next = (first ? first : second);
    if (tail->next) {
        tail->next->prev = tail;
    }
    return dummy.next;
}

static struct list_head *merge_two_list(struct list_head *head,
                                        compare_func_t cmp)
{
    if (!head || !head->next) {
        return head;
    }

    struct list_head *slow = head;
    struct list_head *fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    struct list_head *second = slow->next;
    slow->next = NULL;
    if (second) {
        second->prev = NULL;
    }

    head = merge_two_list(head, cmp);
    second = merge_two_list(second, cmp);

    return merge_list(head, second, cmp);
}

void merge_sort(struct list_head *head, compare_func_t cmp)
{
    if (!head || list_empty(head) || head->next->next == head) {
        return;
    }

    struct list_head *first = head->next;
    first->prev->next = NULL;
    head->prev->next = NULL;

    first = merge_two_list(first, cmp);

    struct list_head *tail = first;
    while (tail->next) {
        tail = tail->next;
    }
    head->next = first;
    first->prev = head;
    tail->next = head;
    head->prev = tail;
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    merge_sort(head, cmp_func);

    if (descend) {
        q_reverse(head);
    }
}

/* End of sort */


/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    element_t *right = list_entry(head->prev, element_t, list);
    element_t *left = list_entry(head->prev->prev, element_t, list);
    while (&left->list != head) {
        if (strcmp(right->value, left->value) > 0) {
            left = list_entry(left->list.prev, element_t, list);
            right = list_entry(right->list.prev, element_t, list);
        } else {
            list_del(&left->list);
            free(left->value);
            free(left);
            left = list_entry(right->list.prev, element_t, list);
        }
    }
    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    element_t *right = list_entry(head->prev, element_t, list);
    element_t *left = list_entry(head->prev->prev, element_t, list);
    while (&left->list != head) {
        if (strcmp(right->value, left->value) < 0) {
            left = list_entry(left->list.prev, element_t, list);
            right = list_entry(right->list.prev, element_t, list);
        } else {
            list_del(&left->list);
            free(left->value);
            free(left);
            left = list_entry(right->list.prev, element_t, list);
        }
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    if (!head || list_empty(head)) {
        return 0;
    }

    queue_contex_t *base_queue = list_first_entry(head, queue_contex_t, chain);
    if (list_is_singular(head)) {
        return base_queue->size;
    }

    queue_contex_t *queue_to_merge;
    struct list_head *current, *next;

    list_for_each_safe (current, next, head) {
        if (current == &base_queue->chain) {
            continue;
        }
        queue_to_merge = list_entry(current, queue_contex_t, chain);
        list_splice_tail_init(queue_to_merge->q, base_queue->q);
        base_queue->size += queue_to_merge->size;
    }

    q_sort(base_queue->q, descend);
    return base_queue->size;
}
