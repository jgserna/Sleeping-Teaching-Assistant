#include <pthread.h>        // Create POSIX threads.
#include <time.h>           // Wait for a random time.
#include <unistd.h>         // Thread calls sleep for specified number of seconds.
#include <semaphore.h>      // To create semaphores
#include <stdlib.h>
#include <stdio.h>          // Input Output

#define MAXCHAIRS 3
pthread_t *Students;         // N threads running as Students.
pthread_t TA;                // Separate Thread for TA.

int ChairsCount = 0;
int CurrentIndex = 0;
int studentsLeft = 0;

// Declaration of Semaphores and Mutex Lock
pthread_mutex_t mutex; // To lock and unlock var ChairsCount to increment/decrement its value.
sem_t TA_sleep;        // A semaphore to signal and wait TA's sleep.
sem_t Student_sem;     // A semaphore to signal and wait for TA's next student.
sem_t chairs_sem[MAXCHAIRS];   // An array of 3 semaphores to signal and wait chair to wait for the TA.

// Declared Functions
void *TA_Activity(void *);
void *Student_Activity(void *threadID);

int main(int argc, char* argv[]) {
    int number_of_students;   // variable taken from the user to create student threads
    srand(time(NULL));

    // Initializing Mutex Lock and Semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&TA_sleep, 0, 0);
    sem_init(&Student_sem, 0, 0);
    for (int i = 0; i < MAXCHAIRS; i++) {
        sem_init(&chairs_sem[i], 0, 0);
    }

    // Default # of students or
    if (argc < 2) {
        printf("Number of Students not specified. Using default (5) students.\n");
        number_of_students = 5;
    }
    else {
        number_of_students = atoi(argv[1]);
        printf("Number of Students specified. Creating %d threads.\n", number_of_students);
    }

    studentsLeft = number_of_students;

    // Allocate memory for Students
    Students = (pthread_t*) malloc(sizeof(pthread_t)*number_of_students);

    // Creating one TA thread and N Student threads.
    pthread_create(&TA, NULL, TA_Activity, NULL);
    for (int i = 0; i < number_of_students; i++) {
        int *id = (int *)malloc(sizeof(int));
        *id = i + 1; // create student ids
        pthread_create(&Students[i], NULL, Student_Activity, (void *)id);
    }

    // Waiting for TA thread and N Student threads.
    pthread_join(TA, nullptr);
    for (int i = 0; i < number_of_students; i++) {
        pthread_join(Students[i], NULL);
    }

    //Free allocated memory
    free(Students);
    pthread_mutex_destroy(&mutex);
    sem_destroy(&TA_sleep);
    sem_destroy(&Student_sem);
    for (int i = 0; i < MAXCHAIRS; i++) {
        sem_destroy(&chairs_sem[i]);
    }

    printf("Program finished sucessfully.\n");
    return 0;
}

void *TA_Activity(void *) {
    while(true) {
        printf("TA is sleeping ... zzz.\n");  // TA is currently sleeping
        sem_wait(&TA_sleep);

        while(true) {
                pthread_mutex_lock(&mutex);  // lock to access chairs count

            if (studentsLeft == 0) {   // if no students
                pthread_mutex_unlock(&mutex);
                printf("All students have been helped. TA is going home. Yay!\n");
                return NULL; // exit TA thread
            }

            if (ChairsCount == 0) { // if chairs are empty, break the loop
                pthread_mutex_unlock(&mutex);
                break;
            }

            // TA gets next student on chair
            sem_post(&chairs_sem[CurrentIndex]);   // let student in
            ChairsCount--; // decrement occupied chairs

            printf("TA is helping a student.\n");

            CurrentIndex = (CurrentIndex + 1) % MAXCHAIRS;  // cycle through just like in bounded buffer example
            pthread_mutex_unlock(&mutex);  // unlock after ChairsCount is modified

            sem_wait(&Student_sem);
            printf("TA finished helping student.\n");
            pthread_mutex_lock(&mutex);
            studentsLeft--; // decrement students left waiting
            pthread_mutex_unlock(&mutex);
            }
        }

    return NULL;
}

void *Student_Activity(void *threadID) {
    int studentID = *(int *)threadID;
    free(threadID);

    while (true) {
        // student needs help from TA
        pthread_mutex_lock(&mutex);     // lock to check chair count

        // student tried to sit on chair
        if (ChairsCount < MAXCHAIRS) {  // if chair available
            ChairsCount++;              // student sits
            int chairIndex = (CurrentIndex + ChairsCount - 1) % MAXCHAIRS;
            printf("Student %d is waiting on chair %d.\n", studentID, chairIndex);

            sem_post(&TA_sleep);           // wake TA if sleeping
            pthread_mutex_unlock(&mutex);  // release lock after chair count changes

            sem_wait(&chairs_sem[chairIndex]); // wait for TA
            printf("Student %d is getting help from TA.\n", studentID);

            sem_post(&Student_sem);
            break;  // Exit loop after getting help
        }
        else {   // if student didn't find chair, will return another time
            pthread_mutex_unlock(&mutex);
            printf("Student %d will come back later due to no chairs available.\n", studentID);
            sleep(1);
        }
    }
    return NULL;
}
