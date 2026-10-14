// Structure to store overall performance metrics for an algorithm
struct AlgorithmMetrics {
    char algo_name[30];
    float avg_waiting_time;
    float avg_turnaround_time;
};

// Function to calculate averages and print the final comparison table
void display_performance_comparison(struct AlgorithmMetrics results[], int total_algos) {
    printf("\n==================================================\n");
    printf("         CPU SCHEDULING PERFORMANCE COMPARISON    \n");
    printf("==================================================\n");
    printf("%-20s | %-18s | %-18s\n", "Algorithm", "Avg Waiting Time", "Avg Turnaround Time");
    printf("--------------------------------------------------\n");

    float min_waiting = results[0].avg_waiting_time;
    int best_algo_index = 0;

    for (int i = 0; i < total_algos; i++) {
        printf("%-20s | %-18.2f | %-18.2f\n", 
            results[i].algo_name, 
            results[i].avg_waiting_time, 
            results[i].avg_turnaround_time);

        // Track the algorithm with the lowest average waiting time
        if (results[i].avg_waiting_time < min_waiting) {
            min_waiting = results[i].avg_waiting_time;
            best_algo_index = i;
        }
    }

    printf("==================================================\n");
    printf("🏆 Best Performing Algorithm: %s (Lowest Avg Waiting Time)\n", results[best_algo_index].algo_name);
    printf("==================================================\n");
}