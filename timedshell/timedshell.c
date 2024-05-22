#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main() {
    char path[256], arg[256];
    struct timeval start, end;
    double total_time = 0.0;

    while (scanf("%255s %255s", path, arg) == 2) {
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // Child process
            execl(path, path, arg, (char *)NULL);
            // If execl fails
            fprintf(stderr, "> Erro: %s\n", strerror(errno));
            exit(errno);
        } else {
            // Parent process
            gettimeofday(&start, NULL);
            int status;
            waitpid(pid, &status, 0);
            gettimeofday(&end, NULL);

            double elapsed_time = (end.tv_sec - start.tv_sec) + 
                                  (end.tv_usec - start.tv_usec) / 1000000.0;
            total_time += elapsed_time;

            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                printf("> Demorou %.1f segundos, retornou %d\n", elapsed_time, exit_code);
            } else {
                printf("> Demorou %.1f segundos, retornou %d\n", elapsed_time, -1);
            }
        }
    }
    
    printf(">> O tempo total foi de %.1f segundos\n", total_time);
    
    return 0;
}
