#include <stdio.h>
#include <stdlib.h>

#define MAX_THREADS 50

typedef struct {
    int p_id;
    int arr_time;
    int burst_time;
    int waiting_time;
    int turn_around_time;
} threadInfo;

int main() {
    FILE *file = fopen("sample_in_schedule.txt", "r");
    if (file == NULL) {
        printf("Unable to open file!\n");
        return 1;
    }

    threadInfo threads[MAX_THREADS];
    int i = 0;
    int num_threads = 0;

    // Read thread data from file
    while (fscanf(file, "%d,%d,%d", &threads[num_threads].p_id, &threads[num_threads].arr_time, &threads[num_threads].burst_time) != EOF) {
        threads[num_threads].waiting_time = 0;
        threads[num_threads].turn_around_time = 0;
        num_threads++;
    }
    fclose(file);

    // Sort threads by arrival time
    for (i = 0; i < num_threads; i++) {
        for (int j = i + 1; j < num_threads; j++) {
            if (threads[i].arr_time > threads[j].arr_time) {
                threadInfo temp = threads[i];
                threads[i] = threads[j];
                threads[j] = temp;
            }
        }
    }

    // Calculate waiting time, turn around time for each thread
    int total_time = 0;
    for (i = 0; i < num_threads; i++) {
        threads[i].waiting_time = total_time - threads[i].arr_time;
        if (threads[i].waiting_time < 0)
            threads[i].waiting_time = 0;
        total_time += threads[i].burst_time;
        threads[i].turn_around_time = threads[i].waiting_time + threads[i].burst_time;
    }

    // Calculate average waiting time and average turn around time
    double avg_waiting_time = 0;
    double avg_turn_around_time = 0;
    for (i = 0; i < num_threads; i++) {
        avg_waiting_time += threads[i].waiting_time;
        avg_turn_around_time += threads[i].turn_around_time;
    }
    avg_waiting_time /= num_threads;
    avg_turn_around_time /= num_threads;

    // Print result
    printf("Thread ID\tArrival Time\tBurst Time\tWaiting Time\tTurn-Around Time\n");
    for (i = 0; i < num_threads; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", threads[i].p_id, threads[i].arr_time, threads[i].burst_time, threads[i].waiting_time, threads[i].turn_around_time);
    }
    printf("\nThe average waiting time: %.2f\n", avg_waiting_time);
    printf("The average turn-around time: %.2f\n", avg_turn_around_time);

    return 0;
}
