#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

volatile sig_atomic_t signal_count = 0;
pid_t child_pid = -1;

void signal_handler(int signo) {
    signal_count++;
    if (signal_count == 1) {
        // Create a zombie process
        child_pid = fork();
        if (child_pid == 0) {
            // Child process
            exit(0);
        }
    } else if (signal_count == 2) {
        // Wait for the zombie process to be reaped
        if (child_pid > 0) {
            waitpid(child_pid, NULL, 0);
        }
    } else if (signal_count == 3) {
        // Exit the program
        exit(0);
    }
}

int main() {
    // Setup signal handlers for SIGUSR1 and SIGUSR2
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    // Wait for signals
    while (1) {
        pause(); // Wait for signal
    }

    return 0;
}
