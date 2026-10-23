#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

typedef struct {
    int num_processes;
    int num_resources;
    int available[MAX_RESOURCES];
    int max_matrix[MAX_PROCESSES][MAX_RESOURCES];
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int request[MAX_PROCESSES][MAX_RESOURCES];
} SystemState;

void reset_system(SystemState *sys) {
    memset(sys, 0, sizeof(*sys));
    printf("System reset successfully.\n");
}

bool is_valid_system(SystemState *sys) {
    return sys->num_processes > 0 && sys->num_resources > 0;
}

void compute_need_matrix(SystemState *sys) {
    for (int i = 0; i < sys->num_processes; i++) {
        for (int j = 0; j < sys->num_resources; j++) {
            sys->need[i][j] =
                sys->max_matrix[i][j] - sys->allocation[i][j];
        }
    }
}

void create_processes(SystemState *sys) {
    int count;

    printf("Enter number of processes (1-%d): ", MAX_PROCESSES);
    if (scanf("%d", &count) != 1 ||
        count < 1 || count > MAX_PROCESSES) {
        printf("Invalid process count.\n");
        return;
    }

    sys->num_processes = count;

    memset(sys->allocation, 0, sizeof(sys->allocation));
    memset(sys->max_matrix, 0, sizeof(sys->max_matrix));
    memset(sys->need, 0, sizeof(sys->need));
    memset(sys->request, 0, sizeof(sys->request));

    printf("Processes created successfully.\n");
}

void create_resources(SystemState *sys) {
    int count;

    printf("Enter number of resource types (1-%d): ", MAX_RESOURCES);
    if (scanf("%d", &count) != 1 ||
        count < 1 || count > MAX_RESOURCES) {
        printf("Invalid resource count.\n");
        return;
    }

    int values[MAX_RESOURCES];

    for (int j = 0; j < count; j++) {
        printf("Available instances of R%d: ", j + 1);

        if (scanf("%d", &values[j]) != 1 || values[j] < 0) {
            printf("Invalid resource quantity.\n");
            return;
        }
    }

    sys->num_resources = count;

    memset(sys->available, 0, sizeof(sys->available));
    memset(sys->allocation, 0, sizeof(sys->allocation));
    memset(sys->max_matrix, 0, sizeof(sys->max_matrix));
    memset(sys->need, 0, sizeof(sys->need));
    memset(sys->request, 0, sizeof(sys->request));

    for (int j = 0; j < count; j++) {
        sys->available[j] = values[j];
    }

    printf("Resources created successfully.\n");
}

void enter_allocation_matrix(SystemState *sys) {
    if (!is_valid_system(sys)) {
        printf("Create processes and resources first.\n");
        return;
    }

    int values[MAX_PROCESSES][MAX_RESOURCES];

    printf("\n--- Enter Allocation Matrix ---\n");

    for (int i = 0; i < sys->num_processes; i++) {
        for (int j = 0; j < sys->num_resources; j++) {
            printf("Allocation for P%d, R%d: ", i, j + 1);

            if (scanf("%d", &values[i][j]) != 1 ||
                values[i][j] < 0) {
                printf("Invalid allocation value.\n");
                return;
            }
        }
    }

    for (int i = 0; i < sys->num_processes; i++) {
        for (int j = 0; j < sys->num_resources; j++) {
            if (values[i][j] > sys->max_matrix[i][j] &&
                sys->max_matrix[i][j] != 0) {
                printf("Allocation cannot exceed maximum claim.\n");
                return;
            }
        }
    }

    memcpy(sys->allocation, values, sizeof(values));
    compute_need_matrix(sys);

    printf("Allocation matrix saved.\n");
}

void enter_max_matrix(SystemState *sys) {
    if (!is_valid_system(sys)) {
        printf("Create processes and resources first.\n");
        return;
    }

    int values[MAX_PROCESSES][MAX_RESOURCES];

    printf("\n--- Enter Maximum Matrix ---\n");

    for (int i = 0; i < sys->num_processes; i++) {
        for (int j = 0; j < sys->num_resources; j++) {
            printf("Maximum claim for P%d, R%d: ", i, j + 1);

            if (scanf("%d", &values[i][j]) != 1 ||
                values[i][j] < sys->allocation[i][j]) {
                printf("Maximum claim must be >= allocation.\n");
                return;
            }
        }
    }

    memcpy(sys->max_matrix, values, sizeof(values));
    compute_need_matrix(sys);

    printf("Maximum matrix and need matrix updated.\n");
}

void display_need_matrix(SystemState *sys) {
    if (!is_valid_system(sys)) {
        printf("System not initialized.\n");
        return;
    }

    printf("\nNEED MATRIX\nPID\t");

    for (int j = 0; j < sys->num_resources; j++)
        printf("R%d\t", j + 1);

    printf("\n");

    for (int i = 0; i < sys->num_processes; i++) {
        printf("P%d\t", i);

        for (int j = 0; j < sys->num_resources; j++)
            printf("%d\t", sys->need[i][j]);

        printf("\n");
    }
}

void display_system_status(SystemState *sys) {
    if (!is_valid_system(sys)) {
        printf("System not initialized.\n");
        return;
    }

    printf("\nAvailable: ");

    for (int j = 0; j < sys->num_resources; j++)
        printf("R%d=%d ", j + 1, sys->available[j]);

    printf("\n\nPID\tAllocation\tMax\t\tNeed\n");

    for (int i = 0; i < sys->num_processes; i++) {
        printf("P%d\t", i);

        for (int j = 0; j < sys->num_resources; j++)
            printf("%d ", sys->allocation[i][j]);

        printf("\t");

        for (int j = 0; j < sys->num_resources; j++)
            printf("%d ", sys->max_matrix[i][j]);

        printf("\t");

        for (int j = 0; j < sys->num_resources; j++)
            printf("%d ", sys->need[i][j]);

        printf("\n");
    }
}

bool run_bankers_algorithm(SystemState *sys, int safe_seq[]) {
    if (!is_valid_system(sys))
        return false;

    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};

    for (int j = 0; j < sys->num_resources; j++)
        work[j] = sys->available[j];

    int count = 0;

    while (count < sys->num_processes) {
        bool found = false;

        for (int i = 0; i < sys->num_processes; i++) {
            if (finish[i])
                continue;

            bool can_finish = true;

            for (int j = 0; j < sys->num_resources; j++) {
                if (sys->need[i][j] > work[j]) {
                    can_finish = false;
                    break;
                }
            }

            if (can_finish) {
                for (int j = 0; j < sys->num_resources; j++)
                    work[j] += sys->allocation[i][j];

                safe_seq[count++] = i;
                finish[i] = true;
                found = true;
            }
        }

        if (!found)
            return false;
    }

    return true;
}

void request_resources_menu(SystemState *sys) {
    if (!is_valid_system(sys)) {
        printf("Initialize the system first.\n");
        return;
    }

    int pid;
    int req[MAX_RESOURCES];

    printf("Enter process ID (0-%d): ", sys->num_processes - 1);

    if (scanf("%d", &pid) != 1 ||
        pid < 0 || pid >= sys->num_processes) {
        printf("Invalid process ID.\n");
        return;
    }

    for (int j = 0; j < sys->num_resources; j++) {
        printf("Request for R%d: ", j + 1);

        if (scanf("%d", &req[j]) != 1 || req[j] < 0) {
            printf("Invalid request.\n");
            return;
        }

        if (req[j] > sys->need[pid][j] ||
            req[j] > sys->available[j]) {
            printf("Request exceeds need or available resources.\n");
            return;
        }
    }

    int safe_seq[MAX_PROCESSES];

    for (int j = 0; j < sys->num_resources; j++) {
        sys->available[j] -= req[j];
        sys->allocation[pid][j] += req[j];
        sys->need[pid][j] -= req[j];
    }

    if (run_bankers_algorithm(sys, safe_seq)) {
        for (int j = 0; j < sys->num_resources; j++)
            sys->request[pid][j] = req[j];

        printf("Request granted. Safe sequence: ");

        for (int i = 0; i < sys->num_processes; i++)
            printf("P%d ", safe_seq[i]);

        printf("\n");
    } else {
        for (int j = 0; j < sys->num_resources; j++) {
            sys->available[j] += req[j];
            sys->allocation[pid][j] -= req[j];
            sys->need[pid][j] += req[j];
        }

        printf("Request denied. System would become unsafe.\n");
    }
}

void display_rag(SystemState *sys) {
    if (!is_valid_system(sys)) {
        printf("System not initialized.\n");
        return;
    }

    printf("\nRESOURCE ALLOCATION GRAPH (TEXT VIEW)\n");

    for (int i = 0; i < sys->num_processes; i++) {
        printf("P%d: Allocated: ", i);

        for (int j = 0; j < sys->num_resources; j++)
            if (sys->allocation[i][j] > 0)
                printf("R%d(%d) ", j + 1, sys->allocation[i][j]);

        printf("| Requested: ");

        for (int j = 0; j < sys->num_resources; j++)
            if (sys->request[i][j] > 0)
                printf("R%d(%d) ", j + 1, sys->request[i][j]);

        printf("\n");
    }
}

bool detect_deadlock(SystemState *sys, int deadlocked[], int *d_count) {
    *d_count = 0;

    if (!is_valid_system(sys))
        return false;

    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES];

    for (int j = 0; j < sys->num_resources; j++)
        work[j] = sys->available[j];

    for (int i = 0; i < sys->num_processes; i++) {
        bool has_allocation = false;

        for (int j = 0; j < sys->num_resources; j++)
            if (sys->allocation[i][j] > 0)
                has_allocation = true;

        finish[i] = !has_allocation;
    }

    bool progress;

    do {
        progress = false;

        for (int i = 0; i < sys->num_processes; i++) {
            if (finish[i])
                continue;

            bool can_finish = true;

            for (int j = 0; j < sys->num_resources; j++) {
                if (sys->request[i][j] > work[j]) {
                    can_finish = false;
                    break;
                }
            }

            if (can_finish) {
                for (int j = 0; j < sys->num_resources; j++)
                    work[j] += sys->allocation[i][j];

                finish[i] = true;
                progress = true;
            }
        }
    } while (progress);

    for (int i = 0; i < sys->num_processes; i++) {
        if (!finish[i])
            deadlocked[(*d_count)++] = i;
    }

    return *d_count > 0;
}

void detect_deadlock_menu(SystemState *sys) {
    int deadlocked[MAX_PROCESSES];
    int count = 0;

    if (detect_deadlock(sys, deadlocked, &count)) {
        printf("Deadlock detected in: ");

        for (int i = 0; i < count; i++)
            printf("P%d ", deadlocked[i]);

        printf("\n");
    } else {
        printf("No deadlock detected, or system is not initialized.\n");
    }
}

void recover_deadlock_menu(SystemState *sys) {
    int deadlocked[MAX_PROCESSES];
    int count = 0;

    if (!detect_deadlock(sys, deadlocked, &count)) {
        printf("No deadlock detected.\n");
        return;
    }

    int pid = deadlocked[0];

    printf("Terminating P%d to recover resources.\n", pid);

    for (int j = 0; j < sys->num_resources; j++) {
        sys->available[j] += sys->allocation[pid][j];
        sys->allocation[pid][j] = 0;
        sys->max_matrix[pid][j] = 0;
        sys->need[pid][j] = 0;
        sys->request[pid][j] = 0;
    }

    printf("Recovery completed.\n");
}

int main(void) {
    SystemState sys = {0};
    int choice;

    do {
        printf("\n===== DEADLOCK SIMULATION SYSTEM =====\n");
        printf("1. Create Processes\n");
        printf("2. Create Resources\n");
        printf("3. Enter Allocation Matrix\n");
        printf("4. Enter Maximum Matrix\n");
        printf("5. Display Need Matrix\n");
        printf("6. Display Resource Allocation Graph\n");
        printf("7. Request Resources\n");
        printf("8. Run Banker's Algorithm\n");
        printf("9. Detect Deadlock\n");
        printf("10. Recover from Deadlock\n");
        printf("11. Display System Status\n");
        printf("12. Reset System\n");
        printf("13. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
            break;

        switch (choice) {
            case 1:
                create_processes(&sys);
                break;

            case 2:
                create_resources(&sys);
                break;

            case 3:
                enter_allocation_matrix(&sys);
                break;

            case 4:
                enter_max_matrix(&sys);
                break;

            case 5:
                display_need_matrix(&sys);
                break;

            case 6:
                display_rag(&sys);
                break;

            case 7:
                request_resources_menu(&sys);
                break;

            case 8: {
                int safe_seq[MAX_PROCESSES];

                if (run_bankers_algorithm(&sys, safe_seq)) {
                    printf("System is SAFE. Sequence: ");

                    for (int i = 0; i < sys.num_processes; i++)
                        printf("P%d ", safe_seq[i]);

                    printf("\n");
                } else {
                    printf("System is UNSAFE or not initialized.\n");
                }

                break;
            }

            case 9:
                detect_deadlock_menu(&sys);
                break;

            case 10:
                recover_deadlock_menu(&sys);
                break;

            case 11:
                display_system_status(&sys);
                break;

            case 12:
                reset_system(&sys);
                break;

            case 13:
                printf("Exiting system.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 13);

    return 0;
}