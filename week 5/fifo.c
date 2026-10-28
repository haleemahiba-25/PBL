#include <stdio.h>
#include <stdbool.h>

#define MAX_REF_LEN 100
#define MAX_FRAMES 10

// Function to simulate FIFO Page Replacement Algorithm
void simulate_fifo(int ref_string[], int ref_len, int num_frames) {
    int frames[MAX_FRAMES];
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1; // Initialize frames as empty
    }

    int fifo_pointer = 0; // Points to the oldest page in the frame
    int page_hits = 0;
    int page_faults = 0;

    printf("\n================ FIFO SIMULATION TABLE ================\n");
    printf("Page\t");
    for (int i = 0; i < num_frames; i++) {
        printf("Frame %d\t", i + 1);
    }
    printf("Status\n");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < ref_len; i++) {
        int current_page = ref_string[i];
        bool found = false;

        // Check if page is already present in memory (Hit)
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == current_page) {
                found = true;
                page_hits++;
                break;
            }
        }

        // If not found, it's a Page Fault
        if (!found) {
            frames[fifo_pointer] = current_page;
            fifo_pointer = (fifo_pointer + 1) % num_frames; // Circular pointer update
            page_faults++;
        }

        // Print current row matching expected output style
        printf("%d\t", current_page);
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == -1) {
                printf("—\t");
            } else {
                printf("%d\t", frames[j]);
            }
        }
        if (found) {
            printf("Hit\n");
        } else {
            printf("Fault\n");
        }
    }
    printf("=======================================================\n");
    printf("Total Page Hits (FIFO): %d\n", page_hits);
    printf("Total Page Faults (FIFO): %d\n", page_faults);
}

// Test main to verify FIFO independently using sample input
int main() {
    int ref_string[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3};
    int ref_len = 10;
    int num_frames = 3;

    printf("Running FIFO Simulation with Sample Input:\n");
    printf("Reference String: 7 0 1 2 0 3 0 4 2 3\n");
    printf("Frame Count: %d\n", num_frames);

    simulate_fifo(ref_string, ref_len, num_frames);

    return 0;
}
