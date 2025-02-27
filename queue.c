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
{
    return NULL;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    return NULL;
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
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

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
