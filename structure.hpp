#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#define SIZE_NAME 15

struct task {
    char name[SIZE_NAME];
    float burst_time;
    float vruntime;
    int nice_value;
};

struct list_node {
    task* mytask;
    list_node* next;
};

#endif