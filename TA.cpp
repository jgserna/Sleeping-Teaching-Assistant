#include <pthread.h>       // Create POSIX threads.
#include <time.h>          // Wait for a random time.
#include <unistd.h>        // Thread calls sleep for specified number of seconds.
#include <semaphore.h>     // To create semaphores
#include <stdlib.h>
#include <stdio.h>         // Input Output

#define MAXCHAIRS 3

pthread_t *Students;            // N threads running as Students.
pthread_t TA;                   // Separate Thread for TA.

int ChairsCount = 0;
int CurrentIndex = 0;
int studentsLeft = 0;

// Declaration of Semaphores and Mutex Lock.
// Semaphores used:
        // 1) A semaphore to signal and wait TA's sleep.
        // 2) An array of 3 semaphores to signal and wait chair to wait for the TA.
        // 3) A semaphore to signal and wait for TA's next student.
// Mutex Lock used: To lock and unlock variable ChairsCount to increment and decrement its value.

pthread_mutex_t mutex;            // mutex lock to protect ChairsCount
sem_t TA_sleep;                   // semaphore for TA sleep/wake
sem_t Student_sem;                // semaphore for the TA to help next student
sem_t chairs_sem[MAXCHAIRS];      // semaphores for each chair

// Declared Functions
void *TA_Activity(void *);
void *Student_Activity(void *threadID);

int main(int argc, char* argv[]) {
	int number_of_students; // a variable taken from the user to create student threads
        int id;
        srand(time(NULL));

        //Initializing Mutex Lock and Semaphores
        pthread_mutex_init(&mutex, NULL);
        sem_init(&TA_sleep, 0, 0);
        sem_init(&Student_sem, 0, 0);

        for (int i = 0; i < MAXCHAIRS; i++) {
                sem_init(&chairs_sem[i], 0, 0);
        }

        if (argc < 2) {
                printf("Number of Students not specified. Using default (5) students.\n");
                number_of_students = 5;   // default students = 5
        }
        else {
        	number_of_students = atoi(argv[1]);
                printf("Number of Students specified. Creating %d threads.\n", number_of_students);
        }

        // Allocate memory for Students
        Students = (pthread_t*) malloc(sizeof(pthread_t) * number_of_students);

        // Create one TA thread and N student threads
        pthread_create(&TA, NULL, TA_Activity, (void *)(long)number_of_students);
        for (id = 0; id < number_of_students; id++) {
                pthread_create(&Students[id], NULL, Student_Activity, (void *)(long)id);
        }

        // Wait for TA thread and N student threads
        pthread_join(TA, NULL);
        for(id = 0; id < number_of_students; id++){
                pthread_join(Students[id], NULL);
        }

        // Free allocated memory and destory semaphores and mutex
        free(Students);
        pthread_mutex_destroy(&mutex);
        sem_destroy(&TA_sleep);
	sem_destroy(&Student_sem);
	for (int i = 0; i < MAXCHAIRS; i++) {
        	sem_destroy(&chairs_sem[i]);
    	}

        printf("Program finished successfully.\n");
        return 0;
}

void *TA_Activity(void *arg) {
        int students_left = (long)arg;
        // TA is currently sleeping.
        // lock
    	// if chairs are empty, break the loop.
        // TA gets next student on chair.
    	// unlock
        // TA is currently helping the student
        while(students_left > 0) {
                printf("TA is sleeping ... zzz.\n");
                sem_wait(&TA_sleep);        // wait until a student wakes up the TA

                while(1) {
                        pthread_mutex_lock(&mutex);  // lock to access chairs count

                        if (ChairsCount == 0){
                                pthread_mutex_unlock(&mutex);
                                break;
                        }

                        sem_post(&chairs_sem[CurrentIndex]);  // let next student in
                        ChairsCount--;   // decrement occupied chairs
                        printf("TA is helping a student.\n");

                        CurrentIndex = (CurrentIndex + 1) % MAXCHAIRS;  // cycle through just like in bounded buffer example
                        pthread_mutex_unlock(&mutex);   // unlock after ChairsCount is modified

                        sem_wait(&Student_sem);  // wait until student gets help
                        printf("TA finished helping student.\n");
                        students_left--;  // decrement students left waiting
                }
        }

        printf("All students have been helped. TA is done for the day. Yay!\n");
        return NULL;
}

void *Student_Activity(void *threadID) {
        // Student  needs help from the TA
        // Student tried to sit on a chair.
        // wake up the TA.
    	// lock
        // unlock
    	// Student leaves his/her chair.
        // Student  is getting help from the TA
        // Student waits to go next.
        // Student left TA room
    	// If student didn't find any chair to sit on.
    	// Student will return at another time
        int studentID = (int)(long)threadID;
        printf("Students %d needs help.\n", studentID);

        while (1) {
                pthread_mutex_lock(&mutex); // lock to check chair count

                if(ChairsCount < MAXCHAIRS) { // if chair available
                        int chairIndex = (CurrentIndex + ChairsCount) % MAXCHAIRS;
                        ChairsCount++;  // student sits
                        printf("Student %d is waiting on chair %d.\n", studentID, chairIndex);

                        pthread_mutex_unlock(&mutex); // release lock after chair count changes

                        sem_post(&TA_sleep);   // wake TA if sleeping
                        sem_wait(&chairs_sem[chairIndex]);   // wait for TA

                        printf("Student %d is getting help from TA.\n", studentID);
                        usleep(rand() % 300000);    // simulate time taken for help

                        sem_post(&Student_sem); // signal that TA can help next student
                        printf("Student %d leaves after getting help.\n", studentID);
                        break; // Exit loop after getting help
                }
                else {
                        printf("Student %d will come back later due to no chairs available.\n", studentID);
                        pthread_mutex_unlock(&mutex);
                        usleep(rand() % 300000);  // wait before trying again
                }
        }
        return NULL;
}
