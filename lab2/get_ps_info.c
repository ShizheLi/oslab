# include <stdio.h>
# include <unistd.h>
# include <sys/syscall.h>
# include <sys/time.h>
# include <stdlib.h>

# define MAXSIZE 64  // max size of array
# define MAX_COMM_LEN 16  // comm length

int main(void) {
    pid_t pid_start[MAXSIZE], pid_end[MAXSIZE], pid_list[MAXSIZE], pid;
    char name_start[MAXSIZE][MAX_COMM_LEN], name_end[MAXSIZE][MAX_COMM_LEN], name_list[MAXSIZE][MAX_COMM_LEN], name;
    long state_start[MAXSIZE], state_end[MAXSIZE], state_list[MAXSIZE], state;
    unsigned long long cpu_start[MAXSIZE], cpu_end[MAXSIZE], cpu_list[MAXSIZE], cpu;
    double consume; /* time difference in main*/

    while(1) {
        for (int i = 0; i < MAXSIZE; i++) { /* initialize */
            pid_start[i] = 0;
            state_start[i] = -1;
            state_end[i] = -1;
        }

        struct timeval start, end, duration;  /* get timespec */

        /* get current time */
        gettimeofday(&start, NULL);

        syscall (333, pid_start, &name_start[0][0], state_start, cpu_start);
        syscall (333, pid_end, &name_end[0][0], state_end, cpu_end);

        /* get current time */
        gettimeofday(&end, NULL);

        /* get time difference */
        duration.tv_sec = end.tv_sec - start.tv_sec;  /* seconds */
        duration.tv_usec = end.tv_usec - start.tv_usec;  /* microseconds */
        if (duration.tv_usec < 0) {
            duration.tv_sec--;
            duration.tv_usec += 1e6;
        }
        consume = 1e12 * (duration.tv_sec + (1.0 * duration.tv_usec) / 1000000); /* nanoseconds */

        for (int i = 0; i < MAXSIZE; i++) {
            for (int j = 0; j < MAXSIZE; j++) {
                if ((pid_end[j] == pid_start[i]) && pid_start[i] != 0) {
                    pid_list[i] = pid_start[i];
                    for (int k = 0; k < MAX_COMM_LEN; k++) {
                        name_list[i][k] = name_start[i][k];
                    }
                    state_list[i] = state_start[i];
                    cpu_list[i] = cpu_end[j] - cpu_start[i];
                    break;
                }
            }
        }    

        /* sort by cpu utilization */

        int LEN = 0;  /* number of processes */
        for (int i = 0; i < MAXSIZE; i++) {
            if (pid_list[i] != 0) LEN++;
        }

        for (int i = 0; i < LEN - 1; i++) {
            int max = i;
            for (int j = i + 1; j < LEN; j++) {
                if (cpu_list[j] > cpu_list[max]) { /* currenr max */
                    max = j;
                }
            }
            if (max != i) {
                /* swap pid_list */
                pid = pid_list[max];
                pid_list[max] = pid_list[i];
                pid_list[i] = pid;
                /* swap name_list */
                for (int k = 0; k < MAX_COMM_LEN; k++) {
                    name = name_list[max][k];
                    name_list[max][k] = name_list[i][k];
                    name_list[i][k] = name;
                }
                /* swap state_list */
                state = state_list[max];
                state_list[max] = state_list[i];
                state_list[i] = state;
                /* swap cpu_list */
                cpu = cpu_list[max];
                cpu_list[max] = cpu_list[i];
                cpu_list[i] = cpu;
            }
        }

        printf ("%4s%20s%10s%10s\n", "PID", "COMM", "CPU", "ISRUNNING");
        for (int i = 0; i < 20; i++) {  // display top 20 processes

            if (state_list[i] == 0) {
                /* determine whether the process is running */
                /* 1 is running */
                /* 0 is not */
                state_list[i] = 1;
            } else {
                state_list[i] = 0;
            }

            printf ("%4d%20s%10.2lf%%%10ld\n", pid_list[i], name_list[i], (double) 100 * cpu_list[i] / consume, state_list[i]);
        }

        sleep(1);
        system("clear");
    }    
    
    return 0;
}