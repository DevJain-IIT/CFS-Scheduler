#ifndef REDBLACK_HPP
#define REDBLACK_HPP

#include "structure.hpp" // Includes task and list_node structures

struct rb_node {
    char color; // 'R' for Red, 'B' for Black
    rb_node* right;
    rb_node* left;
    rb_node* parent;
    task* mytask; // Pointer to the task associated with this node
    int data;     // The 'data' field is likely used for the vruntime or priority in the RB tree
};

struct rb_tree {
    rb_node* root;
    int size;
};

// Global declarations (defined in redblack.cpp)
extern list_node* TaskList;
extern int No_Of_Tasks;
extern rb_tree* TaskTree;

// Function declarations
rb_node* create_node(int value, task* cur_task);
rb_tree* create_tree();
void insert(rb_node* node);
rb_node* deleteNode(rb_node* root, int data); // Needs a complete implementation
void recolor(rb_node* node); // Needs a complete implementation for all cases
rb_node* BSTinsert(rb_node* parent, rb_node* root, rb_node* node);
rb_node* BSTsearch(rb_node* root, int data);
rb_node* restructure(rb_node* root); // Critical for deletion, needs implementation
rb_node* minValueNode(rb_node* node);
rb_node* fixDoubleBlack(rb_node* root); // Critical for deletion, needs implementation
void search(rb_node* root, char* name); // This search is used for deletion by name
void printTree(rb_node* root);
void display(rb_node* root);

#endif