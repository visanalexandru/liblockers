//
// Created by kath on 18.01.2023.
//
#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<mutex.h>

#define NUM_THREADS 500


int global_counter = 0;
//numara cate threaduri au ajuns la bariera


mutex mtx;
semaphore sem;

void barrier_point() {

    mutex_acquire(&mtx);
    //sectiunea criica in care numaram cate threaduri au ajuns la bariera
    //daca nu e ultimul iesim din sectiunea critica
    //si ne folosim de un semafor ca sa astepte pana
    //va ajunge ultimul thread
    global_counter += 1;
    if (global_counter != NUM_THREADS) {
        mutex_release(&mtx);
        semaphore_wait(&sem);
    } else {
        mutex_release(&mtx);
        //cand ultimul thread ajunge la bariera elibereaza toate threaurile
        //deja existente la bariera
        for (int i = 0; i < NUM_THREADS - 1; ++i) {
            semaphore_signal(&sem);
        }
    }
}

//folosim functia data in cerinta
void *tfun(void *v) {

    int *tid = (int *) v;
    printf("%d reached the barrier \n", *tid);
    barrier_point();
    printf("%d passed the barrier \n", *tid);

    free(tid);

    return NULL;
}

int main() {

    //initializam mutexul
    mutex_init(&mtx);

    //initializam semaforul
    //daca initializarea esueaza afisam o eroare corespunzatoare
    semaphore_init(&sem, 0);

    //definim vectorul de threaduri
    //in argumrnt retin nr threadul i
    //si creez threadurile daca avem eraoare la creare
    //afisam un mesaj corespunzator
    pthread_t thr[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; ++i) {
        int *arg = malloc(sizeof(int));
        *arg = i;
        if (pthread_create(&thr[i], NULL, tfun, arg)) {
            printf("Eroare la creare thread.\n");

        }
    }

    //asteptam pana la finalizarea executiei ultimului thread
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (pthread_join(thr[i], NULL)) {
            printf("Eroare la join\n");
        }
    }

    //eliberam mutexul si semaforul
    semaphore_delete(&sem);
    mutex_delete(&mtx);

    return 0;
}