#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_REF_LEN 100
#define MAX_FRAMES 10

// System Data Structure for Page Replacement
struct PageSimulation {
    int ref_string[MAX_REF_LEN];
    int ref_len;
    int num_frames;
    int frames[MAX_FRAMES];
};

// Function to handle inputs, validation, and initialization
void initialize_simulation(struct PageSimulation *sim) {
    char buffer[256];
    
    printf("\n--- Member 1: Input Management & Initialization ---\n");
    
    // 1. Input and Validate Number of Page Frames
    do {
        printf("Enter the number of page frames (1 to %d): ", MAX_FRAMES);
        if (scanf("%d", &sim->num_frames) != 1) {
            printf("Invalid input format. Please enter a valid integer.\n");
            while(getchar() != '\n'); // Clear input buffer
            sim->num_frames = 0;
        } else if (sim->num_frames <= 0 || sim->num_frames > MAX_FRAMES) {
            printf("Error: Frame count must be between 1 and %d[span_3](start_span)[span_3](end_span).\n", MAX_FRAMES);
        }
    } while (sim->num_frames <= 0 || sim->num_frames > MAX_FRAMES);

    // Clear buffer leftover from scanf
    while(getchar() != '\n');

    // 2. Input and Parse Page-Reference String
    printf("Enter the page-reference string (space-separated, e.g., 7 0 1 2 0 3 0 4 2 3)[span_4](start_span)[span_4](end_span)[span_5](start_span)[span_5](end_span):\n> ");
    fgets(buffer, sizeof(buffer), stdin);

    sim->ref_len = 0;
    char *token = strtok(buffer, " \t\n");
    while (token != NULL && sim->ref_len < MAX_REF_LEN) {
        sim->ref_string[sim->ref_len++] = atoi(token);
        token = strtok(NULL, " \t\n");
    }

    // Edge Case Validation: Empty reference string[span_6](start_span)[span_6](end_span)
    if (sim->ref_len == 0) {
        printf("Error: Reference string cannot be empty! Please restart input.\n");
        return;
    }

    // 3. Initialize Memory Frames (Setting empty slots to -1)
    for (int i = 0; i < sim->num_frames; i++) {
        sim->frames[i] = -1;
    }

    printf("\n-> SUCCESS: Loaded %d page references and initialized %d frames.\n", sim->ref_len, sim->num_frames);
}

// Optional test main to verify Member 1 independently
int main() {
    struct PageSimulation sim;
    initialize_simulation(&sim);

    printf("\n--- Verification Summary ---\n");
    printf("Total References: %d\n", sim.ref_len);
    printf("Reference String: ");
    for(int i = 0; i < sim.ref_len; i++) {
        printf("%d ", sim.ref_string[i]);
    }
    printf("\nInitial Frames: ");
    for(int i = 0; i < sim.num_frames; i++) {
        printf("[ ] ");
    }
    printf("\n");

    return 0;
}
