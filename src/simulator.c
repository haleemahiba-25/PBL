#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 50
#define STACK_SIZE 10
#define QUEUE_SIZE 10

// Simulated System State
int memory[MEM_SIZE];
int stack[STACK_SIZE];
int top = -1;

int queue[QUEUE_SIZE];
int head = 0;
int tail = 0;
int count = 0;

// Stack Functions
void push(int val) {
    if (top < STACK_SIZE - 1) {
        stack[++top] = val;
        printf("[LOG] CPU: Pushed %d onto the Stack.\n", val);
    } else {
        printf("[LOG] ERROR: Stack Overflow!\n");
    }
}

void pop() {
    if (top >= 0) {
        printf("[LOG] CPU: Popped %d from the Stack.\n", stack[top--]);
    } else {
        printf("[LOG] ERROR: Stack Underflow!\n");
    }
}

// Queue Functions
void enqueue(int val) {
    if (count < QUEUE_SIZE) {
        queue[tail] = val;
        tail = (tail + 1) % QUEUE_SIZE;
        count++;
        printf("[LOG] CPU: Enqueued task ID %d.\n", val);
    } else {
        printf("[LOG] ERROR: Queue is full!\n");
    }
}

void dequeue() {
    if (count > 0) {
        printf("[LOG] CPU: Processed task ID %d (Dequeued).\n", queue[head]);
        head = (head + 1) % QUEUE_SIZE;
        count--;
    } else {
        printf("[LOG] ERROR: Queue is empty!\n");
    }
}

// Main Simulation Loop
int main() {
    printf("=== Starting Basic Simulator Engine ===\n");

    // Simulating some operations
    enqueue(101);
    enqueue(102);
    
    push(50);
    push(75);

    memory[0] = 999; 
    printf("[LOG] MEMORY: Stored value %d at address 0\n", memory[0]);

    dequeue();
    pop();

    printf("=== Simulation Ended Successfully ===\n");
    return 0;
}