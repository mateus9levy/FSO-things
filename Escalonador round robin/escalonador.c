#include <stdio.h>
#include <stdlib.h>

// Estrutura para armazenar informações do processo
typedef struct {
    int pid;            // ID do processo
    int burst_time;     // Tempo de execução em segundos
    int remaining_time; // Tempo restante em milissegundos
    int finish_time;    // Tempo de conclusão em milissegundos
} Process;

// Função auxiliar para imprimir os resultados
void print_results(Process processes[], int finished_order[], int num_processes) {
    for (int i = 0; i < num_processes; i++) {
        int idx = finished_order[i];
        printf("%d (%d)\n", processes[idx].pid, processes[idx].finish_time);
    }
}

int main() {
    int num_processes, quantum;
    
    // Leitura do número de processos
    scanf("%d", &num_processes);
    if (num_processes < 1 || num_processes > 100) {
        return 0;
    }
    
    // Leitura do quantum
    scanf("%d", &quantum);
    if (quantum < 1 || quantum > 1000) {
        return 0;
    }
    
    Process processes[num_processes];
    int finished_order[num_processes]; // Array para armazenar a ordem dos processos que terminaram
    int finished_idx = 0;
    
    // Leitura dos processos
    for (int i = 0; i < num_processes; i++) {
        scanf("%d %d", &processes[i].pid, &processes[i].burst_time);
        if (processes[i].pid < 1 || processes[i].pid > 200 || processes[i].burst_time < 1 || processes[i].burst_time > 60000) {
            return 0;
        }
        processes[i].remaining_time = processes[i].burst_time * 1000; // Converter segundos para milissegundos
        processes[i].finish_time = 0;
    }
    
    int elapsed_time = 0;
    int processes_completed = 0;
    
    // Fila para gerenciar os processos ativos
    int queue[num_processes];
    int front = 0, rear = 0;
    
    // Inicializar a fila com todos os processos
    for (int i = 0; i < num_processes; i++) {
        queue[rear++] = i;
    }
    
    // Algoritmo Round-Robin otimizado
    while (processes_completed < num_processes) {
        int current_process_index = queue[front];
        front = (front + 1) % num_processes;
        Process *current_process = &processes[current_process_index];
        
        if (current_process->remaining_time > quantum) {
            elapsed_time += quantum;
            current_process->remaining_time -= quantum;
            queue[rear] = current_process_index; // Reenfileirar o processo
            rear = (rear + 1) % num_processes;
        } else {
            elapsed_time += current_process->remaining_time;
            current_process->remaining_time = 0;
            current_process->finish_time = elapsed_time;
            finished_order[finished_idx++] = current_process_index; // Armazenar a ordem de término
            processes_completed++;
        }
    }
    
    // Imprimir resultados
    print_results(processes, finished_order, num_processes);
    
    return 0;
}
