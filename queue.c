#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new_qhead = malloc(sizeof(struct list_head));
    if (new_qhead) {
        INIT_LIST_HEAD(new_qhead); /* 初始化 list_head，使其成為空的鏈表 */
        return new_qhead;          /* 返回創建並初始化的佇列首 */
    }
    return NULL;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    // 如果佇列不存在或為空，直接釋放佇列本身的記憶體並返回
    if (!l || list_empty(l)) {
        free(l);
        return;
    }

    // 使用指標 pos 來遍歷佇列中的每個元素
    struct list_head *pos = l->next;
    while (pos != l) {
        // 透過 list_entry 取得目前元素的指標
        element_t *entry = list_entry(pos, element_t, list);

        // 移動到下一個節點之前先保存 next，以防止記憶體釋放後失效
        pos = pos->next;

        // 釋放目前元素的記憶體
        q_release_element(entry);
    }

    // 最後釋放佇列本身的記憶體
    free(l);
    return;
}

// Insert an element at head of queue
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;  // 如果 head 是空的，返回 false

    // 分配記憶體給新的元素
    element_t *new_ele = malloc(sizeof(element_t));
    if (!new_ele)
        return false;

    // 初始化 list 項目
    INIT_LIST_HEAD(&new_ele->list);
    new_ele->value = strdup(s);

    if (!new_ele->value) {
        free(new_ele);  // 如果字符串複製失敗，釋放 new_ele 記憶體
        return false;
    }

    // 將新元素插入到佇列的首部
    list_add(&new_ele->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;  // 如果 head 是空的，返回 false

    // 分配記憶體給新的元素
    element_t *new_ele = malloc(sizeof(element_t));
    if (!new_ele)
        return false;

    // 初始化 list 項目
    INIT_LIST_HEAD(&new_ele->list);
    new_ele->value = strdup(s);

    if (!new_ele->value) {
        free(new_ele);  // 如果字符串複製失敗，釋放 new_ele 記憶體
        return false;
    }

    // 將新元素插入到佇列的尾部
    list_add_tail(&new_ele->list, head);

    return true;
}


/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
// 從一個 head 指向的鏈表中刪除首部元素，並將該元素的 value
// （假設是字符串）複製到 sp，最多複製 bufsize - 1 字符。
{
    if (head == NULL || list_empty(head))
        return NULL;

    // 取得鏈表首部的元素
    element_t *first_entry = list_first_entry(head, element_t, list);

    // 如果提供了有效的 sp 且 sp 不是 NULL，將元素的 value 複製到 sp
    if (sp != NULL) {
        // 使用 strncpy 複製元素的 value 到 sp，確保不會溢出
        sp = strncpy(sp, first_entry->value, bufsize - 1);

        // 確保 sp 是一個有效的 C 字符串，並添加字符串結束符 '\0'
        sp[bufsize - 1] = '\0';
    }

    // 從鏈表中刪除首部的元素
    list_del(&first_entry->list);  // first_entry->list: first_entry
                                   // 節點中用來鏈接到其他節點的鏈表指針。
    return first_entry;
}



/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    // 如果 head 為 NULL 或鏈表為空，返回 NULL
    if (head == NULL || list_empty(head))
        return NULL;

    // 取得鏈表尾部的元素
    element_t *last_entry = list_last_entry(head, element_t, list);

    // 如果提供了有效的 sp 且 sp 不是 NULL，將元素的 value 複製到 sp
    if (sp != NULL) {
        // 使用 strncpy 複製元素的 value 到 sp，確保不會溢出
        sp = strncpy(sp, last_entry->value, bufsize - 1);

        // 確保 sp 是一個有效的 C 字符串，並添加字符串結束符 '\0'
        sp[bufsize - 1] = '\0';
    }

    // 從鏈表中刪除尾部的元素
    list_del(&last_entry->list);  // last_entry->list: last_entry
                                  // 節點中用來鏈接到其他節點的鏈表指針。

    return last_entry;
}


/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    struct list_head *fast = head->next;
    struct list_head *slow = head->next;
    while (fast != head && fast->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }
    element_t *mid = list_entry(slow, element_t, list);
    list_del(slow);
    free(mid->value);
    free(mid);
    return true;
}


/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head)) {
        return false;
    }

    struct list_head *node, *safe;
    bool dup = false;

    list_for_each_safe (node, safe, head) {
        element_t *first = list_entry(node, element_t, list);
        const element_t *second = list_entry(safe, element_t, list);
        if (safe != head && !strcmp(first->value, second->value)) {
            list_del(node);
            free(first->value);
            free(first);
            dup = true;
        } else if (dup) {
            element_t *tmp = list_entry(node, element_t, list);
            list_del(node);
            free(tmp->value);
            free(tmp);
            dup = false;
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
        list_cut_position(&tmp, head, tail->prev);
        q_reverse(&tmp);
        list_splice_tail_init(&tmp, &new_head);
    }
    list_splice_init(&new_head, head);
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
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
