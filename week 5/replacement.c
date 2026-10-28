#include <stdio.h>
#include <stdbool.h>

#define MAX_REF_LEN 100
#define MAX_FRAMES 10

// 1. LRU (Least Recently Used) Algorithm Implementation
void simulate_lru(int ref_string[], int ref_len, int num_frames) {
    int frames[MAX_FRAMES];
    int recent_time[MAX_FRAMES]; // Tracks the timestamp/last used index of each page
    
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
        recent_time[i] = -1;
    }

    int page_hits = 0;
    int page_faults = 0;

    printf("\n================ LRU SIMULATION TABLE ================\n");
    printf("Page\t");
    for (int i = 0; i < num_frames; i++) printf("Frame %d\t", i + 1);
    printf("Status\n");
    printf("-------------------------------------------------------\n");

    for (int t = 0; t < ref_len; t++) {
        int current_page = ref_string[t];
        bool found = false;

        // Check for Hit
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == current_page) {
                found = true;
                recent_time[j] = t; // Update recent use time
                page_hits++;
                break;
            }
        }

        // If Fault
        if (!found) {
            int replace_idx = -1;
            // First look for any empty frame
            for (int j = 0; j < num_frames; j++) {
                if (frames[j] == -1) {
                    replace_idx = j;
                    break;
                }
            }

            // If no empty frame, find the least recently used page (minimum recent_time)
            if (replace_idx == -1) {
                int lru_time = t + 1;
                for (int j = 0; j < num_frames; j++) {
                    if (recent_time[j] < lru_time) {
                        lru_time = recent_time[j];
                        replace_idx = j;
                    }
                }
            }

            frames[replace_idx] = current_page;
            recent_time[replace_idx] = t;
            page_faults++;
        }

        // Print row
        printf("%d\t", current_page);
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == -1) printf("—\t");
            else printf("%d\t", frames[j]);
        }
        printf(found ? "Hit\n" : "Fault\n");
    }
    printf("=======================================================\n");
    printf("Total Page Hits (LRU): %d | Total Page Faults (LRU): %d\n", page_hits, page_faults);
}

// 2. Optimal Algorithm Implementation
void simulate_optimal(int ref_string[], int ref_len, int num_frames) {
    int frames[MAX_FRAMES];
    for (int i = 0; i < num_frames; i++) frames[i] = -1;

    int page_hits = 0;
    int page_faults = 0;

    printf("\n================ OPTIMAL SIMULATION TABLE ================\n");
    printf("Page\t");
    for (int i = 0; i < num_frames; i++) printf("Frame %d\t", i + 1);
    printf("Status\n");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < ref_len; i++) {
        int current_page = ref_string[i];
        bool found = false;

        // Check for Hit
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == current_page) {
                found = true;
                page_hits++;
                break;
            }
        }

        // If Fault
        if (!found) {
            int replace_idx = -1;
            // First look for any empty frame
            for (int j = 0; j < num_frames; j++) {
                if (frames[j] == -1) {
                    replace_idx = j;
                    break;
                }
            }

            // If no empty frame, look ahead into the future
            if (replace_idx == -1) {
                int farthest = -1;
                for (int j = 0; j < num_frames; j++) {
                    int next_use = -1;
                    // Find when frames[j] is used next in the reference string
                    for (int k = i + 1; k < ref_len; k++) {
                        if (frames[j] == ref_string[k]) {
                            next_use = k;
                            break;
                        }
                    }
                    // If a page is never used again in the future, it's the best candidate to replace
                    if (next_use == -1) {
                        replace_idx = j;
                        break;
                    }
                    // Otherwise, pick the one used farthest in the future
                    if (next_use > farthest) {
                        farthest = next_use;
                        replace_idx = j;
                    }
                }
            }

            frames[replace_idx] = current_page;
            page_faults++;
        }

        // Print row
        printf("%d\t", current_page);
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == -1) printf("—\t");
            else printf("%d\t", frames[j]);
        }
        printf(found ? "Hit\n" : "Fault\n");
    }
    printf("==========================================================\n");
    printf("Total Page Hits (Optimal): %d | Total Page Faults (Optimal): %d\n", page_hits, page_faults);
}

// Test main to verify LRU and Optimal independently
int main() {
    int ref_string[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3};
    int ref_len = 10;
    int num_frames = 3;

    simulate_lru(ref_string, ref_len, num_frames);
    simulate_optimal(ref_string, ref_len, num_frames);

    return 0;
}