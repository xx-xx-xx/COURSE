#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CODING_LENGTH 65    // Slightly larger than 63, which is the maximal length of input
#define ASCII 128               // Use ascii code for indexing, reducing time

// This structure describes a huffman node in the huffman tree
typedef struct huffman_node* ptr_huffman_node ;
struct huffman_node {
    char chr;                   // The letter of A-Z,a-z,_
    int frequency;              // The number of this letter's appearances
    ptr_huffman_node left;      // The pointer to the left child
    ptr_huffman_node right;     // The pointer to the right child
};

int weight = 0;                                             // The weight of coding using huffman tree
int node_num = 0;                                           // The number of nodes in the heap
int frequency[ASCII];                                       // Helps in finding the frequency quickly
ptr_huffman_node heap[MAX_CODING_LENGTH] = {NULL};          // The heap that stores huffman nodes
char test_table[MAX_CODING_LENGTH][MAX_CODING_LENGTH];      // The input case in each test

/**
 * @Author      : C01dkit
 * @Date        : 21:10   2020/4/25
 * @Param       : node
 * @return      : void
 * @Output      : none
 * @Description : This function will insert a pointer in the heap
 */
void heap_insert(ptr_huffman_node node);

/**
 * @Author      : C01dkit
 * @Date        : 21:35   2020/4/25
 * @Param       : smaller_index   -------- The index of the node whose frequency is smaller in the heap
 *                larger_index    -------- The index of the node whose frequency is larger in the heap
 * @return      : void
 * @Output      : node
 * @Description : This function will swap two pointers in the heap using their index.
 */
void swap(int smaller_index, int larger_index);

/**
 * @Author      : C01dkit
 * @Date        : 21:41   2020/4/25
 * @Param       : none
 * @return      : void
 * @Output      : none
 * @Description : This function will delete heap[1] and return nothing
 */
void heap_delete();

/**
 * @Author      : C01dkit
 * @Date        : 22:11   2020/4/25
 * @Param       : node    -------- The pointer to the root of huffman tree
 *                level   -------- Current depth of the tree, start at 0
 * @return      : void
 * @Output      : none
 * @Description : This function will do a post-order traversal and calculate total weight of the huffman tree
 */
void calculate_weight(ptr_huffman_node node,int level);

/**
 * @Author      : C01dkit
 * @Date        : 22:39   2020/4/25
 * @Param       : test_groups_num      -------- The number of strings going to check
 * @return      : int
 * @Output      : none
 * @Description : This function will check current group of test.
 *                If it obeys prefix rule, then the function will return 1, otherwise 0.
 */
int check_prefix(int test_groups_num);

int main () {
    int N;
    ptr_huffman_node temp;
    freopen("testin.txt", "r", stdin);
    freopen("testout.txt", "w", stdout);
    scanf("%d",&N);
    getchar();
    // This loop will restore the initial data and construct a heap
    for (int i = 0; i < N; i++) {
        temp = (ptr_huffman_node)malloc(sizeof(struct huffman_node));
        if (i == 0) scanf("%c %d",&temp->chr,&temp->frequency);
        else scanf(" %c %d",&temp->chr,&temp->frequency);
        temp->left = NULL;
        temp->right = NULL;
        frequency[(int)temp->chr] = temp->frequency;
        heap_insert(temp);
    }
    // This loop will construct a huffman tree using the heap
    while (node_num > 1) {  // If the heap only contains one node, then it is the root of the huffman tree
        temp = (ptr_huffman_node)malloc(sizeof(struct huffman_node));
        temp->chr = '#';    // Marking this node a non-leaf node
        temp->frequency = heap[1]->frequency;
        temp->left = heap[1];
        heap_delete();      // Pop one min-node
        temp->frequency += heap[1]->frequency;
        temp->right = heap[1];
        heap_delete();      // Pop another min-node
        // Since the minimal 2 nodes have been popped and connected to the temp node
        // Then push the node back into the heap for the next loop
        heap_insert(temp);
    }
    calculate_weight(heap[1],0);    // Calculate the weight using huffman algorithm
    int M, test_weight;
    char test_char,test_string[MAX_CODING_LENGTH];
    scanf("%d",&M);
    // This loop will read M test cases, the answer will be given right away
    for (int i = 0; i < M; i++) {
        test_weight = 0;
        for (int j = 0; j < N; j++) {
            getchar();
            scanf("%c %s",&test_char,test_string);
            strcpy(test_table[j], test_string);    // This makes checking prefix more quickly
            test_weight += strlen(test_string) * frequency[(int)test_char];    // Calculate the weight of this case
        }
        // After input, there are two test.

        // If test result cost more, then it fails immediately.
        if (test_weight != weight) printf("No\n");

        // Otherwise, check if one string starts at the beginning of any other one
        else if (check_prefix(N) == 0) printf("No\n");

        // If this case is able to pass the previous two tests, then it is OK.
        else printf("Yes\n");
    }
     fclose(stdin);
     fclose(stdout);
    return 0;
}

int check_prefix(int test_groups_num) {
    char* str_result;
    for (int i = 0; i < test_groups_num; i++) {
        for (int j = 0; j < test_groups_num; j++) {
            if (j == i) continue;   // No need to test itself
            str_result = strstr(test_table[i], test_table[j]);  // Check1 : substring
            if (str_result == NULL) continue;  // If it is not its substring, then continue
            if (test_table[i] == str_result) { // Check2 : prefix
                return 0;  // If it starts from the same address as the other one, then it doesn't obey prefix rule
            }
        }
    }
    return 1;
}

void calculate_weight(ptr_huffman_node node, int level) {
    if (node == NULL) return;
    calculate_weight(node->left,level + 1);
    calculate_weight(node->right,level + 1);
    if (node->chr != '#') weight += node->frequency * level;    // Since I use '#' to mark it non-leaf
}

void heap_delete() {
    heap[1] = heap[node_num--];  // Move the last one to the top, node_num counts down
    // This loop will check the property of min-heap from top
    for (int i = 2; i <= node_num; i *= 2) {
        if (i + 1 <= node_num && heap[i]->frequency > heap[i + 1]->frequency) i++;
        if (heap[i / 2]->frequency > heap[i]->frequency) swap(i, i / 2);
        else break;
    }
}

void heap_insert(ptr_huffman_node node) {
    heap[++node_num] = node;   // Insert one node to the last, node_num counts up
    int this = node_num;
    // This loop will check the property of min-heap from bottom
    while (this != 1 && heap[this]->frequency < heap[this/2]->frequency) {
        swap(this, this/2);
        this /= 2;
    }
}

void swap(int smaller_index, int larger_index) {
    // Simply swap two nodes
    ptr_huffman_node temp;
    temp = heap[smaller_index];
    heap[smaller_index] = heap[larger_index];
    heap[larger_index] = temp;
}