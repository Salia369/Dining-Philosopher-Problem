

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t forks[] = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
};



void take_forks(int *i);
void return_forks(int *i);
void *philosophers(void *id);
int ratio (int);


int ratio(int count) {                  // function to create the ratio of thinking to eating
    
    if (count < 8) {
        return 1;
    }
    return 0;
}



void *philosophers (void *id) {
    
    int count;
    clock_t philTime = 0 ;
    
    
    do {
        printf("thinking, %d\n", *(int *)id);
        usleep(1000 * 500);         //think
        count++;
    } while (ratio(count) == 1);
    
    printf("hungry, %d\n", *(int *)id);
    take_forks((int *)id);      // get a fork
    philTime = clock() - philTime;      // keep track of waiting
    
    
    printf("eating, %d\n", *(int *)id);
    usleep(1000 * 500);         //eat
    return_forks((int *)id);
    
    printf("\tPhilosopher %d , %f seconds\n ",*(int *)id,((float)philTime)/CLOCKS_PER_SEC);     // count time spent hungry
    return NULL;
}

void take_forks(int *i){
    if (*i != 0){           // test for availability
        if (pthread_mutex_lock(&forks[*i]) != 0){
            fprintf(stderr, "Error while locking mutex\n");
            exit(EXIT_FAILURE);}
        sleep(1);
        if (pthread_mutex_lock(&forks[(*i+1) % 7]) != 0){
            fprintf(stderr, "Error while locking mutex\n");
            exit(EXIT_FAILURE);}
    } else {
        if (pthread_mutex_lock(&forks[(*i+1) % 7]) != 0){
            fprintf(stderr, "Error while locking mutex\n");
            exit(EXIT_FAILURE);}
        sleep(1);
        if (pthread_mutex_lock(&forks[*i]) != 0){
            fprintf(stderr, "Error while locking mutex\n");
            exit(EXIT_FAILURE);}
    }
}

void return_forks(int *i){          // release forks
    if (pthread_mutex_unlock(&forks[*i]) != 0){
        fprintf(stderr, "Error while unlocking mutex");
        exit(EXIT_FAILURE);}
    if (pthread_mutex_unlock(&forks[(*i+1) % 7]) != 0){
        fprintf(stderr, "Error while unlocking mutex");
        exit(EXIT_FAILURE);}
}



int main(int argc, char **argv)
{
    pthread_t thePhilosophers[7];
    int id[] = {0,1,2,3,4,5,6};
    int i;
    
    for (i = 0; i < 7; i++)     // creates philosophers with threads
    if (pthread_create( &thePhilosophers[i], NULL, &philosophers, (void *)&id[i]) != 0){
        fprintf(stderr, "Error while creating philosopher threads");
        exit(EXIT_FAILURE);
    }
    
    for (i = 0; i < 7; i++)
    if (pthread_join(thePhilosophers[i], NULL) != 0){
        fprintf(stderr, "Error while joining philosopher threads");
        exit(EXIT_FAILURE);
    }
    
    return 0;
}



