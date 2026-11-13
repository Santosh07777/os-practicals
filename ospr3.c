/*
To implement the Producer-Consumer Problem using Counting Semaphores and
Mutex Locks for thread synchronization in Linux using POSIX threads.
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty, full;
pthread_mutex_t mutex;

/* Producer Thread */
void *producer(void *arg)
{
    int item;

    for(int i = 1; i <= 10; i++)
    {
        item = rand() % 100;

        sem_wait(&empty);

        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Producer produced: %d at position %d\n",
                item, in);

        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);

        sem_post(&full);

        sleep(1);
    }

    pthread_exit(NULL);
}

/* Consumer Thread */
void *consumer(void *arg)
{
    int item;

    for(int i = 1; i <= 10; i++)
    {
        sem_wait(&full);

        pthread_mutex_lock(&mutex);

        item = buffer[out];

        printf("Consumer consumed: %d from position %d\n",
                item, out);

        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);

        sem_post(&empty);

        sleep(2);
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t prod, cons;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);

    pthread_mutex_destroy(&mutex);

    return 0;
    
    /*
    sw@sw-HP-Pro-Tower-280-G9-PCI-Desktop-PC:~$ gcc pc.c
sw@sw-HP-Pro-Tower-280-G9-PCI-Desktop-PC:~$ ./a.out
Producer produced: 83 at position 0
Consumer consumed: 83 from position 0
Producer produced: 86 at position 1
Consumer consumed: 86 from position 1
Producer produced: 77 at position 2
Producer produced: 15 at position 3
Consumer consumed: 77 from position 2
Producer produced: 93 at position 4
Producer produced: 35 at position 0
Consumer consumed: 15 from position 3
Producer produced: 86 at position 1
Producer produced: 92 at position 2
Consumer consumed: 93 from position 4
Producer produced: 49 at position 3
Producer produced: 21 at position 4
Consumer consumed: 35 from position 0
Consumer consumed: 86 from position 1
Consumer consumed: 92 from position 2
Consumer consumed: 49 from position 3
Consumer consumed: 21 from position 4
sw@sw-HP-Pro-Tower-280-G9-PCI-Desktop-PC:~$ ^C
*/
}
