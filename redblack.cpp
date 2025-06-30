#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm> // For std::swap
#include "redblack.hpp"

// No using namespace std; here, but used in .cpp files is fine.

list_node* TaskList = nullptr;
int No_Of_Tasks = 0;
rb_tree* TaskTree = nullptr;

rb_node* create_node(int value, task* cur_task) {
    rb_node* node = new rb_node;
    node->color = 'R'; // New nodes are always red initially
    node->right = nullptr;
    node->left = nullptr;
    node->parent = nullptr;
    node->data = value;
    node->mytask = cur_task;
    return node;
}

rb_tree* create_tree() {
    rb_tree* tree = new rb_tree;
    tree->root = nullptr;
    tree->size = 0;
    return tree;
}

void display(rb_node* root) {
    if (root != nullptr) {
        display(root->left);
        // Added check for root->mytask to prevent nullptr dereference
        std::cout << root->data << ":" << root->color << " (Task: " << (root->mytask ? root->mytask->name : "N/A") << ") ";
        display(root->right);
    }
}

// Helper function for left rotation
void leftRotate(rb_tree* tree, rb_node* x) {
    rb_node* y = x->right;
    x->right = y->left;
    if (y->left != nullptr) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// Helper function for right rotation
void rightRotate(rb_tree* tree, rb_node* y) {
    rb_node* x = y->left;
    y->left = x->right;
    if (x->right != nullptr) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == nullptr) {
        tree->root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }
    x->right = y;
    y->parent = x;
}

// This function needs to handle all Red-Black Tree insertion cases
void recolor(rb_node* node) {
    // If the node is the root or its parent is black, no violation.
    // If parent is nullptr, it's the root.
    if (node == TaskTree->root || node->parent == nullptr || node->parent->color == 'B') {
        return;
    }

    rb_node* parent = node->parent;
    rb_node* grandparent = parent->parent;

    // Case 2: Grandparent is null (shouldn't happen if parent is red and not root)
    if (grandparent == nullptr) {
        return;
    }

    // Determine uncle
    rb_node* uncle = nullptr;
    if (parent == grandparent->left) {
        uncle = grandparent->right;
    } else {
        uncle = grandparent->left;
    }

    // Case 1: Uncle is Red
    if (uncle != nullptr && uncle->color == 'R') {
        parent->color = 'B';
        uncle->color = 'B';
        grandparent->color = 'R';
        recolor(grandparent); // Recursively fix from grandparent
    } else { // Case 2 & 3: Uncle is Black (or nullptr)
        // Case 2: Triangle case (LR or RL)
        if (parent == grandparent->left && node == parent->right) { // LR Case
            leftRotate(TaskTree, parent);
            node = parent; // Now 'node' points to the new parent (old 'node' is now left child of 'parent')
            parent = node->parent; // Update parent to grandparent
        } else if (parent == grandparent->right && node == parent->left) { // RL Case
            rightRotate(TaskTree, parent);
            node = parent; // Similar to LR case
            parent = node->parent; // Update parent to grandparent
        }

        // Case 3: Line case (LL or RR) - after potential Case 2 transformation
        parent->color = 'B';
        grandparent->color = 'R';
        if (node == parent->left) { // LL Case (or transformed LR)
            rightRotate(TaskTree, grandparent);
        } else { // RR Case (or transformed RL)
            leftRotate(TaskTree, grandparent);
        }
    }
    TaskTree->root->color = 'B'; // Always ensure root is black
}

// BST insert, sets up parent pointers
rb_node* BSTinsert(rb_node* parent, rb_node* root, rb_node* node) {
    if (root == nullptr) {
        node->parent = parent;
        return node;
    }
    if (node->data > root->data) {
        root->right = BSTinsert(root, root->right, node);
    } else if (node->data < root->data) {
        root->left = BSTinsert(root, root->left, node);
    } else {
        // Handle duplicate data if necessary (e.g., allow, update, or ignore)
        // For CFS, vruntime might be the same for different tasks
        // A common approach for ties is to add to the right or left consistently,
        // or add an additional tie-breaking key. Here, we'll just add to the right.
        root->right = BSTinsert(root, root->right, node);
    }
    return root;
}

void insert(rb_node* node) {
    if (TaskTree->root == nullptr) {
        TaskTree->root = node;
        TaskTree->root->color = 'B';
    } else {
        // BSTinsert returns the new root of the subtree, which might be the overall root if rotations occur
        TaskTree->root = BSTinsert(nullptr, TaskTree->root, node);
        recolor(node); // Fix Red-Black properties after insertion
    }
    TaskTree->size++;
    TaskTree->root->color = 'B'; // Ensure root is always black after any operation
}

rb_node* BSTsearch(rb_node* root, int data) {
    if (root == nullptr || root->data == data) return root;
    if (data < root->data) return BSTsearch(root->left, data);
    return BSTsearch(root->right, data);
}

rb_node* minValueNode(rb_node* node) {
    rb_node* current = node;
    while (current && current->left != nullptr) current = current->left;
    return current;
}

// Placeholder for full Red-Black Tree deletion.
// This is a complex function and needs a full implementation of
// `fixDoubleBlack` and careful handling of cases.
rb_node* deleteNode(rb_node* root, int data) {
    // Standard BST delete
    if (root == nullptr) return root;

    if (data < root->data) {
        root->left = deleteNode(root->left, data);
    } else if (data > root->data) {
        root->right = deleteNode(root->right, data);
    } else {
        // Node with only one child or no child
        if (root->left == nullptr) {
            rb_node* temp = root->right;
            // Before deleting root, handle its color and potential double black
            if (root->color == 'B' && temp != nullptr) { // If black node deleted, and child is black or null, need fix
                // This is where fixDoubleBlack would be called for the child or effective child.
                // For a proper Red-Black Tree, this requires careful handling of cases.
                // For simplicity here, we assume 'fixDoubleBlack' handles it.
                // A full implementation would identify the successor/predecessor and the "double black" condition.
                // This part is highly conceptual for brevity.
            }
            delete root;
            TaskTree->size--;
            return temp;
        } else if (root->right == nullptr) {
            rb_node* temp = root->left;
            if (root->color == 'B' && temp != nullptr) {
                // Similar fixDoubleBlack call
            }
            delete root;
            TaskTree->size--;
            return temp;
        }

        // Node with two children: Get the inorder successor (smallest in the right subtree)
        rb_node* temp = minValueNode(root->right);

        // Copy the inorder successor's content to this node
        root->data = temp->data;
        root->mytask = temp->mytask; // Make sure to copy the task pointer as well!

        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->data);
    }
    // After deletion, the tree might be unbalanced or violate Red-Black properties.
    // The actual rebalancing for Red-Black trees is quite intricate, especially for deletion.
    // This simplified BST deletion will not maintain RB properties.
    // A full RB-delete would return the actual root of the modified subtree after rotations/recolors.
    if (TaskTree->root != nullptr) {
        TaskTree->root->color = 'B'; // Always ensure root is black
    }
    return root;
}

// This function needs to be carefully implemented to handle Red-Black deletion properties.
// It is typically called after a node is removed to fix double black violations.
rb_node* fixDoubleBlack(rb_node* node) {
    // This is a highly complex function in Red-Black tree implementation.
    // It involves several cases based on the sibling's color and its children's colors,
    // and potentially rotations and recoloring.
    // A complete implementation would be many lines of code.
    // For now, it's just a placeholder.
    return node;
}

void search(rb_node* root, char* name) {
    if (root == nullptr) return;
    search(root->left, name);
    // Use strcmp for string comparison
    if (strcmp(name, root->mytask->name) == 0) {
        deleteNode(TaskTree->root, root->data);
        return; // Important: return after deleting to avoid use-after-free or double delete if multiple matches
    }
    search(root->right, name);
}

void printTree(rb_node* root) {
    if (root != nullptr) {
        printTree(root->left);
        // Added check for TaskTree->root to prevent nullptr dereference if tree is empty or root changes
        std::cout << "Name=" << root->mytask->name << "  Priority " << root->data
                  << "  Root Value= " << (TaskTree->root ? TaskTree->root->data : -1) // Handle null root
                  << " Color=" << root->color << std::endl;
        printTree(root->right);
    }
}

// This function is generally part of the Red-Black tree deletion fix-up
// and should contain the logic for rotations and color changes after a node is removed.
// It's usually called by fixDoubleBlack.
rb_node* restructure(rb_node* root) {
    // This is also a placeholder. This function needs to contain the logic
    // for specific rotations and recoloring to fix Red-Black properties
    // after a deletion. It's often intertwined with the fixDoubleBlack logic.
    return root;
}