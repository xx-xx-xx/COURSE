#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#define MAX_NODE 265000    //Must be greater than the nodes number
#define MAX_TEMP_STACK 50  //Used in bin_heap, this means 2^50 - 1 nodes are allowed
typedef enum{false, true}bool;

//This structure stores the information of edges, used in both Fibonacci and adjacent list
typedef struct adj_list_node* ptr_adj_list_node;
struct adj_list_node {
    int to;                         //The start node index
    int weight;                     //The destination node index
    ptr_adj_list_node next;         //The pointer to next adjacent node
};

//This structure maintains a visited array
typedef struct adj_found_node* ptr_adj_found_node;
struct adj_found_node{
    int index;                      //The index of the node
    int distance;                   //The distance from the source node
    bool visited;                   //Whether this node is visited or not
};

//This structure stores the information of edges, used in both Fibonacci and adjacent list
struct {
    int nodes;                                                //The number of nodes in the graph
    int edges;                                                //The number of edges in the graph
    int found_num;                                            //The number of valid items in the visit array
    struct adj_list_node adj_list_node[MAX_NODE];             //Contains each node
    struct adj_found_node adj_found_list[MAX_NODE];           //Contains possible next-nodes, used in Dijkstra using adj list.
}adj_list;

//This structure is a formal one of a binomial heap node
typedef struct bin_node* ptr_bin_node;
struct bin_node {
    int index;                      //The index of the node
    int distance;                   //The distance from the source node
    ptr_bin_node parent;            //The pointer to its parent
    ptr_bin_node left_child;        //The pointer to its child
    ptr_bin_node right_sibling;     //The pointer to its right sibling
};

//This structure is used in binomial method
struct {
    ptr_bin_node bin_temp_node;                     //Only used as a temporary pointer
    ptr_bin_node bin_stack[MAX_TEMP_STACK];         //Pointer array of all trees
    ptr_bin_node bin_temp_stack[MAX_TEMP_STACK];    //Used when merging
    ptr_bin_node bin_node_ptr[MAX_NODE];            //Pointer to every node
    struct bin_node bin_node[MAX_NODE];             //All the binomial nodes
}bin_heap;

//This structure is a formal one of a fibonacci heap node
typedef struct fib_node* ptr_fib_node;
struct fib_node {
    int distance;           //The length of path from the source node to this node
    int degree;             //The number of its children
    int index;              //The index of this node
    bool mark;              //Whether its child had been removed or not.
    ptr_fib_node parent;    //The pointer to its parent
    ptr_fib_node child;     //The pointer to its child
    ptr_fib_node left;      //The pointer to its left sibling
    ptr_fib_node right;     //The pointer to its right sibling
};

//This structure is used in fibonacci method
struct {
    int tree_num;                                   //The number of trees in the fibonacci heap
    ptr_fib_node fib_temp_node;                     //Only used as a temporary pointer in variable deliver
    ptr_fib_node fib_min_node;                      //The pointer to the node with minimum distance
    ptr_fib_node fib_temp_stack[MAX_NODE/2 + 1];    //A temp array, used to reconstruct fib_heap
    struct fib_node fib_node[MAX_NODE];             //All the fibonacci nodes
}fib_heap;

void read_data();

void dijkstra_using_fibonacci_heap();

void fib_construct(int source);

int fib_delete_min();

void fib_reconstruct();

ptr_fib_node fib_combine_two_nodes_in_temp_stack(ptr_fib_node new_node);

void fib_update_distance(int index);

void fib_decrease(int index);

void dijkstra_using_adjacent_list();

void adj_construct(int source);

ptr_adj_found_node adj_find_next();

void adj_update_distance(ptr_adj_found_node found_node);

void dijkstra_using_binomial_heap();

void bin_construct(int source);

void bin_merge();

void bin_decrease(int source);

void bin_combine_two_nodes(ptr_bin_node *node_adder_1, ptr_bin_node *node_adder_2);

int bin_delete_min();

void bin_update_distance(int index);

int main() {
    //load information from xxx.gr. NOTE THAT #define MAX_NODE must be greater than the number of nodes!
    read_data();

    //traverse the whole graph using fibonacci heap
    dijkstra_using_fibonacci_heap();

    //traverse the whole graph using binomial heap
    dijkstra_using_binomial_heap();
    printf("\nPress Enter to exit...\n");
    //traverse the whole graph using adjacent list
//    dijkstra_using_adjacent_list();
    getchar();
    getchar();    
    return 0;
}

/*
 * This function will using one input as source node, traverse the whole graph.
 * After that, the total time will be printed to the console.
 */
void dijkstra_using_binomial_heap() {
    clock_t start, total;
    int source;
    int index;
    printf("Enter Source x (1 <= x <= %d):\n",adj_list.nodes);
    scanf("%d",&source);
    start = clock();
    //Build the initial heap.
    bin_construct(source);
    for (int i = 0; i < adj_list.nodes; i++) {
        //Delete the node that contains the minimal distance.
        index = bin_delete_min();
        //Update that node's neighbors.
        bin_update_distance(index);
    }
    total = clock() - start;
    printf("Using Binomial Heap to do Dijkstra needs %.3f seconds\n",(double)total / (double)CLOCKS_PER_SEC);
}

/*
 * This function will update all the node which are connected to the given node.
 */
void bin_update_distance(int index) {
    int neighbor_index;
    ptr_adj_list_node temp;
    temp = &adj_list.adj_list_node[index];
    //This loop will traverse all the nodes that connect to the given node
    do {
        neighbor_index = temp->to;
        //If the updated distance is smaller than its parent, then swap them iteratively
        if (bin_heap.bin_node_ptr[neighbor_index]->distance > bin_heap.bin_node_ptr[index]->distance + temp->weight) {
            bin_heap.bin_node_ptr[neighbor_index]->distance = bin_heap.bin_node_ptr[index]->distance + temp->weight;
            bin_decrease(neighbor_index);
        }
        temp = temp->next;
    }while (temp != NULL);
}

/*
 * This function will remove the minimal node from the binomial heap,
 * push all its child to temp_stack, and then merge back.
 */
int bin_delete_min() {
    int position = 0;
    int index;
    bin_heap.bin_temp_node = NULL;
    //This loop will save the node with minimal distance in the temp_node
    for (int i = 0; i < MAX_TEMP_STACK; i++) {
        if (bin_heap.bin_stack[i] == NULL) continue;
        //If temp_node is NULL, then fill it with a node in the stack
        if (bin_heap.bin_temp_node == NULL) {
            bin_heap.bin_temp_node = bin_heap.bin_stack[i];
            position = i;
        }
        //This procedure maintains previous property that temp_node saves the minimal node
        if (bin_heap.bin_temp_node->distance > bin_heap.bin_stack[i]->distance) {
            bin_heap.bin_temp_node = bin_heap.bin_stack[i];
            position = i;
        }
    }
    //Count the number of children of this tree
    int count = 0;
    ptr_bin_node temp;
    temp = bin_heap.bin_temp_node->left_child;
    while (temp != NULL) {
        count++;
        temp = temp->right_sibling;
    }
    //Delete minimal node from the stack, push all its children to temp stack, and then merge them.
    temp = bin_heap.bin_temp_node->left_child;
    index = bin_heap.bin_temp_node->index;
    while (count > 0) {
        count--;
        bin_heap.bin_temp_stack[count] = temp;
        temp = temp->right_sibling;
        bin_heap.bin_temp_stack[count]->parent = NULL;
        bin_heap.bin_temp_stack[count]->right_sibling = NULL;
    }
    //Set this slot NULL to remove the minimal node
    bin_heap.bin_stack[position] = NULL;
    bin_merge();
    return index;
}

/*
 * This function will initiate the binomial heap and build up index array in bin_node_ptr
 * By doing so, it will be much more easier to locate one node in key-decrease part.
 */
void bin_construct(int source) {
    for (int i = 1; i <= adj_list.nodes; i++) {
        bin_heap.bin_node[i].index = i;
        bin_heap.bin_node[i].distance = INT_MAX;
        bin_heap.bin_node[i].parent = NULL;
        bin_heap.bin_node[i].left_child = NULL;
        bin_heap.bin_node[i].right_sibling = NULL;
        bin_heap.bin_node_ptr[i] = &bin_heap.bin_node[i];
        bin_heap.bin_temp_stack[0] = &bin_heap.bin_node[i];
        //Use merge to do "insertion"
        bin_merge();
    }
    bin_heap.bin_node[source].distance = 0;
    bin_heap.bin_temp_node = NULL;
    //Find the entance
    bin_decrease(source);
}

/*
 * This function will swap ALL information of two nodes.
 */
void bin_decrease(int source) {
    ptr_bin_node temp;
    temp = bin_heap.bin_node_ptr[source];
    int temp_int_for_swap;
    //This loop will swap a node whose distance is smaller than its parent iteratively
    while (temp->parent != NULL && temp->distance < temp->parent->distance) {
        //Keep the property that bin_node_ptr is an index array
        bin_heap.bin_node_ptr[temp->index] = temp->parent;
        bin_heap.bin_node_ptr[temp->parent->index] = temp;
        //swap data
        temp_int_for_swap = temp->distance;
        temp->distance = temp->parent->distance;
        temp->parent->distance = temp_int_for_swap;

        temp_int_for_swap = temp->index;
        temp->index = temp->parent->index;
        temp->parent->index = temp_int_for_swap;
        //go up to the parent
        temp = temp->parent;
    }
}

/*
 * This function will merge two binomial heaps into one.
 */
void bin_merge() {
    //Denote :
    // bin_heap.bin_stack[i]        => A      Original stack
    // bin_heap.bin_temp_node       => B      Stores temp addition result
    // bin_heap.bin_temp_stack[i]   => C      Another stack, a temp stack
    bin_heap.bin_temp_node = NULL;
    for (int i = 0; i < MAX_TEMP_STACK; i++) {
        if (bin_heap.bin_temp_node == NULL) {
            if (bin_heap.bin_temp_stack[i] == NULL) continue;  //A = null or not null | B = null | C = null;
            if (bin_heap.bin_stack[i] == NULL) {  //A = null | B = null | C = not null
                bin_heap.bin_stack[i] = bin_heap.bin_temp_stack[i];
                bin_heap.bin_temp_stack[i] = NULL;
                continue;
            }
            //A = not null | B = null | C = not null
            bin_combine_two_nodes(&bin_heap.bin_stack[i], &bin_heap.bin_temp_stack[i]);
            bin_heap.bin_stack[i] = NULL;
            bin_heap.bin_temp_stack[i] = NULL;
        } else {
            if (bin_heap.bin_stack[i] == NULL) {
                if (bin_heap.bin_temp_stack[i] == NULL) {  //A = null | B = not null | C = null
                    bin_heap.bin_stack[i] = bin_heap.bin_temp_node;
                    bin_heap.bin_temp_node = NULL;
                } else {  //A = null | B = not null | C = not null
                    bin_combine_two_nodes(&bin_heap.bin_temp_stack[i], &bin_heap.bin_temp_node);
                    bin_heap.bin_temp_stack[i] = NULL;
                }
            } else {
                if (bin_heap.bin_temp_stack[i] == NULL){  //A = not null | B = not null | C = null
                    bin_combine_two_nodes(&bin_heap.bin_stack[i], &bin_heap.bin_temp_node);
                    bin_heap.bin_stack[i] = NULL;
                } else {  //A = not null | B = not null | C = not null
                    bin_combine_two_nodes(&bin_heap.bin_temp_stack[i], &bin_heap.bin_temp_node);
                    bin_heap.bin_temp_stack[i] = NULL;
                }
            }
        }
    }
}

/*
 * This function is used while doing merging in the way of adding two binary numbers.
 */
void bin_combine_two_nodes(ptr_bin_node *node_adder_1, ptr_bin_node *node_adder_2) {
    ptr_bin_node big_node, small_node;
    big_node = ((*node_adder_1)->distance > (*node_adder_2)->distance) ? (*node_adder_1) : (*node_adder_2);
    small_node = ((*node_adder_1)->distance <= (*node_adder_2)->distance) ? (*node_adder_1) : (*node_adder_2);
    //Let the big node become the left_child of the small one.
    big_node->right_sibling = small_node->left_child;
    small_node->left_child = big_node;
    big_node->parent = small_node;
    bin_heap.bin_temp_node = small_node;
}

/*
 * This function will using one input as source node, traverse the whole graph.
 * After that, the total time will be printed to the console.
 */
void dijkstra_using_adjacent_list() {
    clock_t start, total;
    start = clock();
    int source ;
    ptr_adj_found_node next = NULL;
    printf("Enter Source x (1 <= x <= %d):\n",adj_list.nodes);
    scanf("%d",&source);
    adj_construct(source);
    for (int i = 0; i < adj_list.nodes; i++) {
        //Find the tree whose root node contains the minimal distance.
        next = adj_find_next();
        //Update all the nodes that connect to that node.
        adj_update_distance(next);
    }
    total = clock() - start;
    printf("Using Adjacent List to do Dijkstra needs %.3f seconds\n",(double)total / (double)CLOCKS_PER_SEC);
}

/*
 * This function will update all the nodes that connect to the given node.
 */
void adj_update_distance(ptr_adj_found_node found_node) {
    int find;
    int i;
    ptr_adj_list_node temp_node;
    temp_node = &adj_list.adj_list_node[found_node->index];
    do { // The loop will traverse all the nodes that connect to this node.
        find = temp_node->to;
        for (i = 1; i <= adj_list.found_num; i++) {
            if (adj_list.adj_found_list[i].index == find) {
                //If the node that to be updated is already been found, then update in the array.
                if (adj_list.adj_found_list[i].distance > found_node->distance + temp_node->weight)
                    adj_list.adj_found_list[i].distance = found_node->distance + temp_node->weight;
                break;
            }
        }
        //If the node that to be updated hasn't been found before, then add it to the array.
        if (i > adj_list.found_num) {
            adj_list.adj_found_list[i].distance = found_node->distance + temp_node->weight;
            adj_list.adj_found_list[i].index = find;
            adj_list.adj_found_list[i].visited = false;
            adj_list.found_num++;
        }
        temp_node = temp_node->next;
    }while (temp_node != NULL);
}

/*
 * This function will find the node with the minimal distance in the visited list, then return it's pointer.
 */
ptr_adj_found_node adj_find_next() {
    int min = INT_MAX;  //This will make "Find minimum" more convenient.
    int index_in_found_list = -1;
    ptr_adj_found_node index = NULL;
    //This loop will find the minimal distance and corresponding node.
    for (int i = 1; i <= adj_list.found_num; i++) {
        //If the node has been visited before, the skip it.
        if (adj_list.adj_found_list[i].visited == true) continue;
        //Otherwise, record its index.
        if (adj_list.adj_found_list[i].distance <  min) {
            min = adj_list.adj_found_list[i].distance;
            index_in_found_list = i;
        }
    }
    //Mark this newly found node.
    adj_list.adj_found_list[index_in_found_list].visited = true;
    index = &adj_list.adj_found_list[index_in_found_list];
    return index;
}

/*
 * This function is used to initiate the adjacent list
 */
void adj_construct(int source) {
    adj_list.found_num = 1;
    adj_list.adj_found_list[1].visited = false;
    adj_list.adj_found_list[1].index = source;
    adj_list.adj_found_list[1].distance = 0;
}

/*
 * This function will using one input as source node, traverse the whole graph.
 * After that, the total time will be printed to the console.
 */
void dijkstra_using_fibonacci_heap() {
    clock_t start, total;
    int source;
    int index;
    printf("Enter Source x (1 <= x <= %d):\n",adj_list.nodes);
    scanf("%d",&source);
    start = clock();
    //Build the initial heap.
    fib_construct(source);
    for (int i = 0; i < adj_list.nodes; i++) {
        //Delete the node that contains the minimal distance.
        index = fib_delete_min();
        //Update that node's neighbors.
        fib_update_distance(index);
    }
    total = clock() - start;
    printf("Using Fibonacci Heap to do Dijkstra needs %.3f seconds\n",(double)total / (double)CLOCKS_PER_SEC);
}

/*
 * This function will update all the nodes that connect to the previously deleted node.
 */
void fib_update_distance(int index) {
    ptr_adj_list_node temp = &adj_list.adj_list_node[index];
    int fib_index;
    do {
        //Traverse all the neighbors of that node.
        fib_index = temp->to;
        if (fib_heap.fib_node[fib_index].distance > fib_heap.fib_node[index].distance + temp->weight) {
            fib_heap.fib_node[fib_index].distance = fib_heap.fib_node[index].distance + temp->weight;
            //More details will be done in the fib_decrease function
            fib_decrease(fib_index);
        }
        temp = temp->next;
    }while (temp != NULL);
    //Fresh the min_node
    fib_heap.fib_temp_node = fib_heap.fib_min_node;
    for (int i = 0 ;i < fib_heap.tree_num; i++) {
        if (fib_heap.fib_min_node->distance > fib_heap.fib_temp_node->distance) {
            fib_heap.fib_min_node = fib_heap.fib_temp_node;
        }
        fib_heap.fib_temp_node = fib_heap.fib_temp_node->right;
    }
}

/*
 * This function will decrease the distance of the nodes that are connected to the deleted node.
 * Denote the node whose distance is decrease "A"
 * After decrease, if A's distance is smaller than that of A's parent, then A must be move to
 * be a new root instantly, and A's parent should be marked.
 * If A's parent has been marked before, then this procedure will be done cascadingly.
 * All the nodes that are roots are not marked.
 */
void fib_decrease(int index) {
    ptr_fib_node fib_decreased = &fib_heap.fib_node[index];
    ptr_fib_node temp_parent = fib_decreased->parent;
    if (temp_parent == NULL || temp_parent->distance <= fib_decreased->distance) return;
    while(1) {
        //If the node is a root, then we are done.
        if (temp_parent == NULL){
            fib_decreased->mark = false;
            break;
        }
        //Otherwise, this node will be moved to the root chain.
        //Before that, parent's degree should be check.
        if (temp_parent->degree == 1) {
            temp_parent->child = NULL;
        } else {
            //The moved node has some siblings.
            temp_parent->child = fib_decreased->right;
            fib_decreased->right->left = fib_decreased->left;
            fib_decreased->left->right = fib_decreased->right;
        }
        //Common changes.
        temp_parent->degree--;
        fib_decreased->mark = false;
        fib_decreased->right = fib_heap.fib_min_node;
        fib_decreased->left = fib_heap.fib_min_node->left;
        fib_heap.fib_min_node->left->right = fib_decreased;
        fib_heap.fib_min_node->left = fib_decreased;
        fib_decreased->parent = NULL;
        fib_heap.tree_num++;
        if (temp_parent->mark == false) {
            //If it's parent hasn't been marked before, then we are done.
            temp_parent->mark = true;
            break;
        } else {
            //If it's parent has been marked before, then we have to repeat the previous procedure.
            fib_decreased = temp_parent;
            temp_parent = fib_decreased->parent;
        }
    }
    //Find the new min_node.
    fib_heap.fib_temp_node = fib_heap.fib_min_node;
    for (int i = 0 ;i < fib_heap.tree_num; i++) {
        if (fib_heap.fib_min_node->distance > fib_heap.fib_temp_node->distance) {
            fib_heap.fib_min_node = fib_heap.fib_temp_node;
        }
        fib_heap.fib_temp_node = fib_heap.fib_temp_node->right;
    }
}


// This function will delete a tree whose root contains the minimum distance.
int fib_delete_min() {
    if (fib_heap.fib_min_node == NULL) return 0;
    int previous_index = fib_heap.fib_min_node->index;
    //Denote some nodes for convenience.
    ptr_fib_node temp_deleted = fib_heap.fib_min_node;
    ptr_fib_node temp_child = temp_deleted->child;
    ptr_fib_node temp_left = temp_deleted->left;
    if (temp_deleted->left == temp_deleted) {  //Fibonacci heap only has one tree
        fib_heap.fib_min_node = temp_child;
        for (int i = 0; i < temp_deleted->degree; i++) {
            temp_child->parent = NULL;
            temp_child->mark = false;
            temp_child = temp_child->right;
        }
    } else { //The deleted node is not the only node in root chain.
        //This is not really updating fib_min_node. True node will be updated in reconstruct function.
        fib_heap.fib_min_node = temp_left;
        for (int i = 0; i < temp_deleted->degree; i++) {
            //Move all the children to the root chain.
            temp_left->right = temp_child;
            temp_child->left = temp_left;
            temp_child->parent = NULL;
            temp_child->mark = false;
            temp_child = temp_child->right;
            temp_left = temp_left->right;
        }
        temp_left->right = temp_deleted->right;
        temp_deleted->right->left = temp_left;
    }
    //Update the number of nodes in root chain.
    fib_heap.tree_num = fib_heap.tree_num - 1 + temp_deleted->degree;
    fib_reconstruct();
    return previous_index;
}

/*
 * This function is to modify the fibonacci heap after deletion and is VERY important.
 * A temp stack will be used to combine the nodes with the same degree to maintain fibonacci heap's properties
 */
void fib_reconstruct() {
    if (fib_heap.fib_min_node == NULL) return;
    int max_occupy_index = INT_MIN;  //Stores the maximal index of temp stack that are used for faster traversal.
    ptr_fib_node this,next;
    this = fib_heap.fib_min_node;
    //This loop will traverse the root chain and try to put them to the temp stack.
    //If collision happens, then these two will be combined. This will be done in iteration
    for (int i = 0; i < fib_heap.tree_num; i++) {
        next = this->right;
        if (fib_heap.fib_temp_stack[this->degree] == NULL) {
            //This slot hasn't been occupied, then will continue.
            fib_heap.fib_temp_stack[this->degree] = this;
        } else {  //Otherwise, combine them in iteratively
            while (fib_heap.fib_temp_stack[this->degree] != NULL) {
                this = fib_combine_two_nodes_in_temp_stack(this);
            }
            fib_heap.fib_temp_stack[this->degree] = this;
        }
        max_occupy_index = (this->degree > max_occupy_index) ? this->degree : max_occupy_index;
        this = next;
    }
    fib_heap.fib_min_node = NULL;
    fib_heap.tree_num = 0;
    //Combine all the node to form the new fibonacci heap.
    for (int i = 0; i <= max_occupy_index; i++) {
        if (fib_heap.fib_temp_stack[i] == NULL) continue;
        fib_heap.tree_num++;
        if (fib_heap.fib_min_node == NULL) {
            fib_heap.fib_min_node = fib_heap.fib_temp_stack[i];
            fib_heap.fib_min_node->left = fib_heap.fib_min_node;
            fib_heap.fib_min_node->right = fib_heap.fib_min_node;
        } else {
            fib_heap.fib_temp_stack[i]->right = fib_heap.fib_min_node;
            fib_heap.fib_temp_stack[i]->left = fib_heap.fib_min_node->left;
            fib_heap.fib_min_node->left->right = fib_heap.fib_temp_stack[i];
            fib_heap.fib_min_node->left = fib_heap.fib_temp_stack[i];
            fib_heap.fib_min_node = (fib_heap.fib_min_node->distance > fib_heap.fib_temp_stack[i]->distance) ?
                                    fib_heap.fib_temp_stack[i] : fib_heap.fib_min_node;
        }
        //Empty the temp stack
        fib_heap.fib_temp_stack[i] = NULL;
    }
}

/*
 * This function handles collision in the temp stack
 */
ptr_fib_node fib_combine_two_nodes_in_temp_stack(ptr_fib_node new_node) {
    ptr_fib_node old_node;  //The node that is already in the stack
    old_node = fib_heap.fib_temp_stack[new_node->degree];
    ptr_fib_node big_node = (new_node->distance > old_node->distance) ? new_node : old_node;
    ptr_fib_node small_node = (new_node->distance <= old_node->distance) ? new_node : old_node;
    fib_heap.fib_temp_stack[new_node->degree] = NULL;
    //Always combine the greater one to the smaller one.
    if (small_node->child == NULL) {
        small_node->child = big_node;
        big_node->left = big_node;
        big_node->right = big_node;
    } else {
        big_node->right = small_node->child;
        big_node->left = small_node->child->left;
        small_node->child->left->right = big_node;
        small_node->child->left = big_node;
    }
    small_node->degree++;
    big_node->parent = small_node;
    fib_heap.fib_temp_node = small_node;
    return fib_heap.fib_temp_node;
}

/*
 * This function will initiate the fibonacci heap.
 */
void fib_construct(int source) {
    ptr_fib_node temp = &fib_heap.fib_node[1];
    temp->right = temp;
    temp->left = temp;
    temp->parent = NULL;
    temp->child = NULL;
    temp->degree = 0;
    temp->distance = INT_MAX;
    temp->mark = false;
    temp->index = 1;
    for (int i = 2; i <= adj_list.nodes; i++) {
        fib_heap.fib_node[i].right = &fib_heap.fib_node[i - 1];
        fib_heap.fib_node[i - 1].left = &fib_heap.fib_node[i];
        fib_heap.fib_node[i].left = &fib_heap.fib_node[1];
        fib_heap.fib_node[1].right = &fib_heap.fib_node[i];
        fib_heap.fib_node[i].parent = NULL;
        fib_heap.fib_node[i].child = NULL;
        fib_heap.fib_node[i].mark = false;
        fib_heap.fib_node[i].distance = INT_MAX;
        fib_heap.fib_node[i].degree = 0;
        fib_heap.fib_node[i].index = i;
    }
    fib_heap.fib_node[source].distance = 0;
    fib_heap.fib_min_node = &fib_heap.fib_node[source];
    fib_heap.tree_num = adj_list.nodes;
}

/*
 * This function will read data.
 */
void read_data() {
    char in[200]; //Used to absorb nonsense
    clock_t start, total;
    start =clock();
    int from, to, weight;
    freopen("USA-road-d.NY.gr","r",stdin);  //Read data from a file
    for (int i = 0; i < 4; i++) gets(in);  //filter. The first four lines are not needed
    scanf("%s",in);
    scanf("%s",in);
    scanf("%d%d",&adj_list.nodes,&adj_list.edges);
    for (int i = 0; i < 2; i++) gets(in); //filter. The following two lines are not needed
    //Initiate part of data
    for (int i = 0; i < MAX_NODE; i++) {
        adj_list.adj_list_node[i].to = 0;
        adj_list.adj_list_node[i].next = NULL;
        fib_heap.fib_temp_stack[i/2] = NULL;
    }
    for (int i = 0; i < MAX_TEMP_STACK; i++) {
        bin_heap.bin_stack[i] = NULL;
        bin_heap.bin_temp_stack[i] = NULL;
    }
    ptr_adj_list_node temp_node;
    //Read all the possible infomation.
    while (scanf("%s%d%d%d",in,&from,&to,&weight)!=EOF) {
        temp_node = &adj_list.adj_list_node[from];
        while (temp_node->next != NULL) temp_node = temp_node->next;
        if (temp_node->to != 0) {
            temp_node->next = (ptr_adj_list_node)malloc(sizeof(struct adj_list_node));
            temp_node = temp_node->next;
        }
        temp_node->to = to;
        temp_node->weight = weight;
        temp_node->next = NULL;
    }
    freopen("CON","r",stdin);
    total = clock() - start;
    printf("finish read. Running time : %.3f s\n",(double)total/(double)CLOCKS_PER_SEC);
}