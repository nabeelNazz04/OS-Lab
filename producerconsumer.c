#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX 5

sem_t empty;
sem_t full;
pthread_mutex_t mutex;
int in = 0, out = 0;
int n;
int buffer[MAX];

void *producer(void *args) {
  for (int i = 0; i < n; i++) {
    int item;
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    item = rand() % 100;
    buffer[in] = item;
    in = (in + 1) % MAX;
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    printf("Producer: %d\n", item);
    sleep(1);
  }
}

void *consumer(void *args) {
  for (int i = 0; i < n; i++) {
    int item;
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    item = buffer[out];
    out = (out + 1) % MAX;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    printf("Consumer: %d\n", item);
    sleep(2);
  }
}
int main() {
  int i;
  printf("Enter no. of process:");  // Actually asks for number of operations
  scanf("%d", &n);
  
  // Initialize synchronization primitives
  sem_init(&empty, 0, MAX);  // Empty slots = buffer size
  sem_init(&full, 0, 0);     // Full slots start at 0
  pthread_mutex_init(&mutex, NULL);
  
  // Create threads
  pthread_t prod, cons;
  pthread_create(&prod, NULL, producer, NULL);
  pthread_create(&cons, NULL, consumer, NULL);
  
  // Wait for threads to complete
  pthread_join(prod, NULL);
  pthread_join(cons, NULL);
  
  // Cleanup
  sem_destroy(&empty);
  pthread_mutex_destroy(&mutex);
  return 0;
}