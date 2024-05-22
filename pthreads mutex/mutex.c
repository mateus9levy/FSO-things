#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Prototipo da funcao work
int work(int id);

// Variáveis globais
int count = 0;
pthread_mutex_t mut;

// Estrutura de argumentos para a thread
struct thread_arg {
    int vezes;
};

// Função que será executada por cada thread
void *thread_func(void *arg) {
    struct thread_arg *targ = (struct thread_arg *)arg;

    for (int i = 0; i < targ->vezes; i++) {
        pthread_mutex_lock(&mut); // Lock o mutex antes de acessar o contador
        int id = count++;
        pthread_mutex_unlock(&mut); // Unlock o mutex após acessar o contador

        work(id);
    }

    return NULL;
}

// Função principal do trabalho que cria e gerencia threads
void do_all_work(int n, int num_threads) {
    pthread_t threads[num_threads];
    struct thread_arg targs[num_threads];

    // Inicializar o mutex
    pthread_mutex_init(&mut, NULL);

    // Criar threads
    for (int i = 0; i < num_threads; i++) {
        targs[i].vezes = n / num_threads;
        if (pthread_create(&threads[i], NULL, thread_func, &targs[i]) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }

    // Aguardar todas as threads terminarem
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destruir o mutex
    pthread_mutex_destroy(&mut);
}

// Função de trabalho (exemplo de implementação)
// int work(int id) {
//     printf("Trabalho com id: %d\n", id);
//     return 0;
// }

// Função principal para teste
// int main() {
//     int num_threads = 5;
//     int num_work = 10;

//     do_all_work(num_work, num_threads);

//     return 0;
// }
