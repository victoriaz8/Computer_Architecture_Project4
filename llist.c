#include <stdio.h>
#include "ldata.h"
#include "llist.h"
#include <stdlib.h>

// Creates and returns an new node
struct LNode *create_new_node(struct LData *data) {
	struct LNode *ptr;
	ptr = (struct LNode*) malloc(sizeof(struct LNode));
	ptr->data = data;
	ptr->next = NULL;
	return ptr;
}

// Creates and returns an empty list
struct LList *create_empty_list(){
	struct LList *ptr;
	ptr = (struct LList*) malloc(sizeof(struct LList));
	ptr->size = 0;
	struct LData *y = create_empty_data();
	ptr->sentinal = create_new_node(y);
	ptr->root = NULL;
	return ptr;
}

// Clears (empties) the specified list
void clear_list(struct LList *list) {
    struct LNode *ptr;
    struct LNode *temp;
    ptr = list->root->next;

    while (ptr->next != NULL) {
        temp = ptr;
        free(ptr);
        ptr = temp->next;
    }
    list->root = list->sentinal;
    list->size = 0;
}

// Swaps the elements of a list
void swap_elems(struct LList *list, int idx1, int idx2) {
    struct LData *data1;
    struct LData *data2;
    struct LData tempdata;
    struct LNode *itrtemp = list->root;
    int i;
    if (is_empty(list)==1) { return; }
    for (i = 0; i <= list->size; i++) {
                if (idx2==i) {
            data2 = itrtemp->data;
        }
        if (idx1==i) {
            data1 = itrtemp->data;
        }
        itrtemp = itrtemp->next;
    }
    tempdata = *data1;
    *data1 = *data2;
    *data2 = tempdata;
}

// Prints the list to standard output in the form [elem_1 elem_2 ... elem_n]
void print_list(struct LList *list) {
    struct LNode *temp;
    int size = (list->size);
    temp = list->root;
    printf("[");
    if (temp == NULL) printf("]");
    else {
        while(temp != list->sentinal) {
            print_data(temp->data);
            temp = (temp->next);
            printf(" ");
        }
        if (list->size > 0) printf("\b");
        printf("]\n");
    }
    
}

// ==============================================================
// Query functions (no side effects)
// ==============================================================

// Checks if the list is empty
int is_empty(struct LList *list) {
    return ((list->root == NULL) ? 1 : 0);
}

// Returns the size of the list
int size(struct LList *list) {
    return list->size;
}

// Checks of the list contains the specified data
// Uses the are_equal operation of LData
/*int contains(struct LList *list, struct LData *data) {
    int i,k;
    struct LNode *temp = list->root;
    for (i = 0; i <= list->size; i++) {
        k = are_equal(temp->data, data);
        if (k == 1) return 1;
        temp = temp->next;
    }
    return 0;
}*/

// ==============================================================
// Add operations
// ==============================================================

// Adds the specified element to the specified position
void add_elem(struct LList *list, struct LData *data, int idx) {
    struct LNode *curr = list->root;
    struct LNode *prev;
    struct LNode *newnode = create_new_node(data);
    struct LNode *temp2;
    int i;
    if (is_empty(list) == 1) {
        printf("empty list");
        return;
    }
    if (idx == 0) add_first(list, data);
    else {
        for (i = 1; i <= list->size; i++) {
            prev = curr;
            curr = curr->next;
            if (idx == i) {
                newnode->next = curr;
                prev->next = newnode;
                return;
            }
        }
    }
    return;
}

// Adds the specified data to the front of the list
void add_first(struct LList *list, struct LData *data) {
    struct LNode *temp;
    temp = create_new_node(data);
    if (list->root != NULL) {
        struct LNode *temp2 = list->root;
        temp->next = temp2;
        list->root = temp;
    }
    else {
        list->root = temp;
        temp->next = list->sentinal;
    }
    list->size++;
    //print_data(data);
}

// Adds the specified data to the end of the list
void add_last(struct LList *list, struct LData *data) {
    struct LNode *temp;
    struct LNode *newNode;
    temp = list->root;
    while(temp->next != list->sentinal) {
        temp = temp->next;
    }
    newNode = create_new_node(data);
    newNode->next = list->sentinal;
    temp->next = newNode;
    list->size++;
}


// ==============================================================
// Get operation
// ==============================================================

// Returns a handle (pointer) to the element of the list at the specified position
struct LData *get_elem(struct LList *list, int idx) {
    struct LNode *temp = list->root;
    int i;
    for (i = 0; i <= list->size; i++) {
        if (idx == i) {
            return temp->data;
        }
        temp = temp->next;
    }
    return 0;
}

// ==============================================================
// Remove operations
// ==============================================================

// Removes and returns the specified element from the specified position
struct LData *rem_elem(struct LList *list, int idx) {
    struct LNode *curr = list->root;
    struct LNode *prev;
    struct LData *tempdata;
    struct LNode *tofree;
    int i;
    if (idx == 0) { rem_first(list); }
    else {
        for (i = 1; i <= list->size; i++) {
            prev = curr;
            curr = curr->next;
            if (idx == i) {
                tempdata = curr->data;
                tofree = curr;
                prev->next = curr->next;
                free(tofree);
                return tempdata;
            }
        }
    }
    return;
}

// Removes and returns the first element in the list
// Assumes that the list is not empty
struct LData *rem_first(struct LList *list) {
    struct LNode *first = list->root;
    struct LData *tempdata = first->data;
    struct LNode *tofree = list->root;
    list->root = first->next;
    free(first);
    return tempdata;
}

// Removes and returns the last element in the list
// Assumes that the list is not empty
struct LData *rem_last(struct LList *list) {
    struct LNode *temp;
    temp = list->root;
    while(temp->next->next != list->sentinal) {
        temp = temp->next;
    }
    struct LNode *last = temp->next;
    struct LData *ret = last->data;
    temp->next = list->sentinal;
    free(last);
    list->size--;
    return ret;
}

// ==============================================================
// Sort operations
// ==============================================================

// Sorts the list according to the are_ordered operation of LData
/*void insertion_sort(struct LList *list) {
    struct LData *curr;
    struct LData *prev;
    int i, j;
    for (i = 1; i <= list->size; i++) {
        j = i;
        curr = get_elem(list, j);
        prev = get_elem(list, j-1);
        while ((j > 0) && (!(are_ordered(prev, curr)==1))) {
            swap_elems(list, j, j-1);
            j = j-1;
            curr = get_elem(list, j);
            prev = get_elem(list, j-1);
        }
    }
}*/
// NOTE: Use the pseudocode at Wikipedia as a template for your insertion sort:
// for i ← 1 to length(A) - 1
//     j ← i
//     while j > 0 and A[j-1] > A[j]
//         swap A[j] and A[j-1]
//         j ← j - 1
