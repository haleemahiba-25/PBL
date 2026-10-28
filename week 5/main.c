#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_REF_LEN 100
#define MAX_FRAMES 10

// Unified Simulation Structure
struct PageSimulation {
    int ref_string[MAX_REF_LEN];
    int ref_len;
    int num_frames;
    int frames[MAX_FRAMES];
};

// --- MEMBER 1 LOGIC: Input & Initialization ---
void initialize_simulation(struct PageSimulation *sim) {
    char buffer[256];
    
    printf("\n--- Input Management & Initialization ---\n");
    do {
        printf("Enter the number of page frames (1 to %d): ", MAX_FRAMES);
        if (scanf("%d", &sim->num_frames) != 1) {
            printf("Invalid input format. Enter an integer.\n");
            while(getchar() != '\n');
            sim->num_frames = 0;
        } else if (sim->num_frames <= 0 || sim->num_frames > MAX_FRAMES) {
            printf("Error: Frame count must be between 1 and %d.\n", MAX_FRAMES);
        }
    } while (sim->num_frames <= 0 || sim->num_frames > MAX_FRAMES);

    while(getchar() != '\n');

    printf("Enter the page-reference string (space-separated, e.g., 7 0 1 2 0 3 0 4 2 3): \n> ");
    fgets(buffer, sizeof(buffer), stdin);

    sim->ref_len = 0;
    char *token = strtok(buffer, " \t\n");
    while (token != NULL && sim->ref_len < MAX_REF_LEN) {
        sim->ref_string[sim->ref_len++] = atoi(token);
        token = strtok(NULL, " \t\n");
    }

    if (sim->ref_len == 0) {
        printf("Error: Reference string cannot be empty!\n");
        return;
    }

    for (int i = 0; i < sim->num_frames; i++) {
        sim->frames[i] = -1;
    }
    printf("-> Loaded %d references and %d frames successfully!\n", sim->ref_len, sim->num_frames);
}

// --- MEMBER 2 LOGIC: FIFO Algorithm ---
void run_fifo(int ref_string[], int ref_len, int num_frames, int *hits, int *faults) {
    int frames[MAX_FRAMES];
    for (int i = 0; i < num_frames; i++) frames[i] = -1;
    int pointer = 0;
    *hits = 0; *faults = 0;

    printf("\n--- FIFO Simulation ---\n");
    for (int i = 0; i < ref_len; i++) {
        int page = ref_string[i];
        bool found = false;
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) { found = true; (*hits)++; break; }
        }
        if (!found) {
            frames[pointer] = page;
            pointer = (pointer + 1) % num_frames;
            (*faults)++;
        }
    }
}

// --- MEMBER 3 LOGIC: LRU & Optimal Algorithms ---
void run_lru(int ref_string[], int ref_len, int num_frames, int *hits, int *faults) {
    int frames[MAX_FRAMES], recent[MAX_FRAMES];
    for (int i = 0; i < num_frames; i++) { frames[i] = -1; recent[i] = -1; }
    *hits = 0; *faults = 0;

    for (int t = 0; t < ref_len; t++) {
        int page = ref_string[t];
        bool found = false;
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) { found = true; recent[j] = t; (*hits)++; break; }
        }
        if (!found) {
            int idx = -1;
            for (int j = 0; j < num_frames; j++) {
                if (frames[j] == -1) { idx = j; break; }
            }
            if (idx == -1) {
                int min_time = t + 1;
                for (int j = 0; j < num_frames; j++) {
                    if (recent[j] < min_time) { min_time = recent[j]; idx = j; }
                }
            }
            frames[idx] = page;
            recent[idx] = t;
            (*faults)++;
        }
    }
}

void run_optimal(int ref_string[], int ref_len, int num_frames, int *hits, int *faults) {
    int frames[MAX_FRAMES];
    for (int i = 0; i < num_frames; i++) frames[i] = -1;
    *hits = 0; *faults = 0;

    for (int i = 0; i < ref_len; i++) {
        int page = ref_string[i];
        bool found = false;
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == page) { found = true; (*hits)++; break; }
        }
        if (!found) {
            int idx = -1;
            for (int j = 0; j < num_frames; j++) {
                if (frames[j] == -1) { idx = j; break; }
            }
            if (idx == -1) {
                int farthest = -1;
                for (int j = 0; j < num_frames; j++) {
                    int next_use = -1;
                    for (int k = i + 1; k < ref_len; k++) {
                        if (frames[j] == ref_string[k]) { next_use = k; break; }
                    }
                    if (next_use == -1) { idx = j; break; }
                    if (next_use > farthest) { farthest = next_use; idx = j; }
                }
            }
            frames[idx] = page;
            (*faults)++;
        }
    }
}

// --- MEMBER 4 LOGIC: Comparative Metrics & Integration Menu ---
void display_comparison_summary(struct PageSimulation *sim) {
    if (sim->ref_len == 0) {
        printf("Please initialize inputs first (Option 1)!\n");
        return;
    }

    int fifo_h, fifo_f, lru_h, lru_f, opt_h, opt_f;
    run_fifo(sim->ref_string, sim->ref_len, sim->num_frames, &fifo_h, &fifo_f);
    run_lru(sim->ref_string, sim->ref_len, sim->num_frames, &lru_h, &lru_f);
    run_optimal(sim->ref_string, sim->ref_len, sim->num_frames, &opt_h, &opt_f);

    printf("\n================ COMPARATIVE PERFORMANCE SUMMARY ================\n");
    printf("Total Page References: %d\n", sim->ref_len);
    printf("Number of Frames: %d\n\n", sim->num_frames);
    
    printf("Algorithm\tPage Hits\tPage Faults\tHit Ratio\tFault Ratio\n");
    printf("-----------------------------------------------------------------\n");
    printf("FIFO\t\t%d\t\t%d\t\t%.2f%%\t\t%.2f%%\n", 
           fifo_h, fifo_f, ((float)fifo_h/sim->ref_len)*100, ((float)fifo_f/sim->ref_len)*100);
    printf("LRU\t\t%d\t\t%d\t\t%.2f%%\t\t%.2f%%\n", 
           lru_h, lru_f, ((float)lru_h/sim->ref_len)*100, ((float)lru_f/sim->ref_len)*100);
    printf("Optimal\t\t%d\t\t%d\t\t%.2f%%\t\t%.2f%%\n", 
           opt_h, opt_f, ((float)opt_h/sim->ref_len)*100, ((float)opt_f/sim->ref_len)*100);
    printf("=================================================================\n");
}

int main() {
    struct PageSimulation sim = {0};
    int choice;

    do {
        printf("\n================ PAGE REPLACEMENT SIMULATOR ================\n");
        printf("1. Enter Reference String and Frame Count\n");
        printf("2. Run FIFO Algorithm Simulation\n");
        printf("3. Run LRU & Optimal Simulations\n");
        printf("4. Display Comparative Summary (All Algorithms)\n");
        printf("5. Exit\n");
        printf("Enter your choice (1-5): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                initialize_simulation(&sim);
                break;
            case 2: {
                if (sim.ref_len == 0) { printf("Initialize inputs first!\n"); break; }
                int h, f;
                run_fifo(sim.ref_string, sim.ref_len, sim.num_frames, &h, &f);
                printf("FIFO Finished -> Hits: %d, Faults: %d\n", h, f);
                break;
            }
            case 3: {
                if (sim.ref_len == 0) { printf("Initialize inputs first!\n"); break; }
                int h1, f1, h2, f2;
                run_lru(sim.ref_string, sim.ref_len, sim.num_frames, &h1, &f1);
                run_optimal(sim.ref_string, sim.ref_len, sim.num_frames, &h2, &f2);
                printf("LRU Finished -> Hits: %d, Faults: %d\n", h1, f1);
                printf("Optimal Finished -> Hits: %d, Faults: %d\n", h2, f2);
                break;
            }
            case 4:
                display_comparison_summary(&sim);
                break;
            case 5:
                printf("Exiting Page Replacement Simulator. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Choose between 1 and 5.\n");
        }
    } while (choice != 5);

    return 0;
}
