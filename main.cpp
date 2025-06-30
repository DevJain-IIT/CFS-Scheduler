#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "menu.hpp"
#include "redblack.hpp"
#include "structure.hpp"

// Forward declarations
void run(int n); // Definition needed
void readFile();

// Read tasks from input.txt and populate TaskList
void readFile() {
    std::ifstream file("input.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open input.txt" << std::endl;
        // Consider exiting or throwing an exception if file is critical
        return;
    }

    int n;
    file >> n;
    No_Of_Tasks = n;

    for (int i = 0; i < n; ++i) {
        task* t = new task;
        file >> t->name >> t->burst_time >> t->nice_value;
        t->vruntime = 0; // Initialize vruntime

        list_node* node = new list_node;
        node->mytask = t;
        node->next = nullptr;

        if (TaskList == nullptr) {
            TaskList = node;
        } else {
            list_node* temp = TaskList;
            while (temp->next != nullptr)
                temp = temp->next;
            temp->next = node;
        }
    }

    file.close();
}

// *** CRITICAL MISSING LOGIC: CFS Scheduler 'run' function ***
// This function needs to implement the core CFS scheduling algorithm:
// 1. Select the task with the minimum vruntime from the Red-Black tree (root of the tree or its left-most child).
// 2. Calculate the time slice for the selected task based on its nice value and the total number of runnable tasks.
// 3. Update the task's burst_time and vruntime.
// 4. Reinsert the task into the Red-Black tree (its vruntime will have increased).
// 5. Repeat for the given N quantum time.
void run(int n) {
    std::cout << "Running scheduler for " << n << " quantum time." << std::endl;

    if (TaskTree->root == nullptr) {
        std::cout << "No processes to run." << std::endl;
        return;
    }

    // Example rudimentary logic (needs to be replaced with actual CFS)
    // This is NOT a CFS implementation, but shows where it would go.
    for (int i = 0; i < n; ++i) {
        if (TaskTree->root == nullptr) {
            std::cout << "All processes finished." << std::endl;
            break;
        }

        // In CFS, you'd pick the left-most node (minimum vruntime)
        rb_node* min_vruntime_node = minValueNode(TaskTree->root);
        if (min_vruntime_node == nullptr) { // Should not happen if root is not null
            break;
        }

        task* current_task = min_vruntime_node->mytask;

        // Simulate running for a quantum (e.g., 1 unit of time)
        float quantum_slice = 1.0; // This should be calculated based on total tasks and nice values in actual CFS

        if (current_task->burst_time > 0) {
            float time_to_run = std::min(quantum_slice, current_task->burst_time);
            current_task->burst_time -= time_to_run;

            // Update vruntime (simplified for example, actual CFS has weight calculations)
            // vruntime += (time_to_run * NICE_0_WEIGHT) / weight_of_this_task;
            current_task->vruntime += time_to_run;

            std::cout << "Running process: " << current_task->name
                      << ", remaining burst: " << current_task->burst_time
                      << ", new vruntime: " << current_task->vruntime << std::endl;

            // Remove from tree and reinsert with new vruntime (if not finished)
            // This is crucial: the node's data (vruntime-based priority) changes, so it must be re-positioned.
            int old_priority = min_vruntime_node->data;
            deleteNode(TaskTree->root, old_priority); // Delete old position

            if (current_task->burst_time > 0) {
                // Calculate new priority (e.g., based on new vruntime)
                int new_priority = static_cast<int>(current_task->vruntime + current_task->nice_value); // Simplified
                rb_node* new_node = create_node(new_priority, current_task);
                insert(new_node); // Insert at new position
            } else {
                std::cout << "Process " << current_task->name << " finished." << std::endl;
                // You might want to delete the task* itself here if it's heap allocated and no longer needed.
                // However, 'taskList' still holds pointers to these, so manage carefully.
            }
        }
    }
    std::cout << "\nRemaining processes in tree:" << std::endl;
    printTree(TaskTree->root); // Display current state of the tree
}


int main() {
    TaskTree = create_tree();
    readFile(); // Reads tasks from input.txt into TaskList

    list_node* process = TaskList;
    while (process != nullptr) { // Iterate through the linked list to insert tasks
        // The initial preference for CFS is based on vruntime, not just nice_value + burst_time.
        // A common starting vruntime is min_vruntime of existing tasks, or 0.
        // For simplicity, let's use vruntime + nice_value as a 'priority' for the RB tree.
        int initial_priority = static_cast<int>(process->mytask->vruntime + process->mytask->nice_value); // Initial priority for RB tree
        rb_node* node = create_node(initial_priority, process->mytask);
        insert(node);
        process = process->next;
    }

    int choice;
    do {
        afterReadingFile(); // Displays menu options
        std::cin >> choice;
        switch (choice) {
            case 1: { // Add new process
                std::cout << "Enter name, burst time, nice value: ";
                task* t = new task;
                std::cin >> t->name >> t->burst_time >> t->nice_value;
                t->vruntime = 0; // Initialize vruntime for new process

                list_node* new_list_node = new list_node;
                new_list_node->mytask = t;
                new_list_node->next = nullptr;

                // Add to TaskList
                if (TaskList == nullptr) {
                    TaskList = new_list_node;
                } else {
                    list_node* temp = TaskList;
                    while (temp->next != nullptr) temp = temp->next;
                    temp->next = new_list_node;
                }
                No_Of_Tasks++;

                // Insert into Red-Black Tree
                int new_priority = static_cast<int>(t->vruntime + t->nice_value);
                rb_node* new_rb_node = create_node(new_priority, t);
                insert(new_rb_node);
                std::cout << "Process added." << std::endl;
                printTree(TaskTree->root);
                break;
            }
            case 2: { // Delete a process
                std::cout << "Enter name of process to delete: ";
                char name_to_delete[SIZE_NAME];
                std::cin >> name_to_delete;
                search(TaskTree->root, name_to_delete); // This calls deleteNode internally
                std::cout << "Process deletion attempted." << std::endl;
                printTree(TaskTree->root);
                break;
            }
            case 3: { // Run processes
                std::cout << "Enter N Quantum time: ";
                int quantum_time;
                std::cin >> quantum_time;
                run(quantum_time);
                break;
            }
            case 4: { // Change priority
                std::cout << "Changing priority is complex as it affects vruntime and tree position." << std::endl;
                std::cout << "Enter name of process to change priority: ";
                char name_to_change[SIZE_NAME];
                std::cin >> name_to_change;

                // To change priority (nice value), you would:
                // 1. Find the task in the RB tree.
                // 2. Remove it from the tree.
                // 3. Update its nice_value.
                // 4. Recalculate its vruntime (this is the tricky part in CFS, as vruntime depends on weights).
                // 5. Reinsert it into the tree with the new vruntime/priority.
                // This would be a significant addition to the 'run' logic.
                std::cout << "Priority change not fully implemented." << std::endl;
                break;
            }
            case 5: { // Exit
                std::cout << "Exiting." << std::endl;
                // TODO: Proper memory cleanup for TaskList and TaskTree nodes
                break;
            }
            default: {
                std::cout << "Invalid option. Please try again." << std::endl;
                break;
            }
        }
    } while (choice != 5);

    // TODO: Add cleanup code to deallocate memory for TaskList and TaskTree
    // (iterate through TaskList and delete task* and list_node*, then delete rb_tree nodes)

    return 0;
}