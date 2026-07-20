// HY-345 Operating Systems - Assignment 2
// Bus route simulation using pthreads and POSIX semaphores
//
// Name: Papadakis Ioannis Titos
// AM: 5200

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define N 12 // bus capacity (as suggested in the pdf)
#define DEPARTMENTS 4 // math, physics, chemistry, csd
#define CAP_PER_DEPT (N / DEPARTMENTS) // max students per department, per trip
#define T 10 // travel time between the two stops, in sec
#define DWELL 3 // how long the bus waits at a stop before leaving
#define MAX_STU 200

char *dept_name[DEPARTMENTS] = {"math", "physics", "chemistry", "csd"};

typedef enum { STOP_A, ON_BUS, UNIVERSITY, STOP_B, HOME } student_state;

typedef struct student {
    int am;
    int department;
    int study_time;
    student_state state;
    pthread_t tid;

    sem_t sem_board;   // bus posts this when the student is allowed in
    sem_t sem_arrived; // bus posts this once it reaches the next stop
} student;

// FIFO list of waiting students, one for each stop
typedef struct node {
    student *s;
    struct node *next;
} node;

typedef struct queue {
    node *head;
    node *tail;
    pthread_mutex_t lock;
} queue;

queue stopA = { NULL, NULL, PTHREAD_MUTEX_INITIALIZER };
queue stopB = { NULL, NULL, PTHREAD_MUTEX_INITIALIZER };

typedef struct bus {
    int dept_count[DEPARTMENTS];
    int total_in;
    student *passengers[N];
    int passenger_count;
    pthread_t tid;
} bus;

int total_students;
int students_home = 0;
pthread_mutex_t home_lock = PTHREAD_MUTEX_INITIALIZER;

void queue_push(queue *q, student *s)
{
    node *n = malloc(sizeof(node));
    n->s = s;
    n->next = NULL;

    pthread_mutex_lock(&q->lock);
    if (q->tail == NULL)
        q->head = q->tail = n;
    else {
        q->tail->next = n;
        q->tail = n;
    }
    pthread_mutex_unlock(&q->lock);
}

// goes through the queue in order and boards whoever still fits.
// if a student's department is already full for this trip we just
// skip him (he stays in the list) and check the next one, so one
// full department doesn't block everybody behind it
void board_students(bus *b, queue *q, char *stop_name)
{
    pthread_mutex_lock(&q->lock);

    node *cur = q->head;
    node *prev = NULL;

    while (cur != NULL && b->total_in < N) {
        student *s = cur->s;

        if (b->dept_count[s->department] < CAP_PER_DEPT) {
            node *done = cur;
            cur = cur->next;
            if (prev == NULL)
                q->head = cur;
            else
                prev->next = cur;
            if (done == q->tail)
                q->tail = prev;
            free(done);

            b->dept_count[s->department]++;
            b->total_in++;
            b->passengers[b->passenger_count++] = s;

            s->state = ON_BUS;
            printf("[bus] student %d (%s) boards at %s\n", s->am, dept_name[s->department], stop_name);
            sem_post(&s->sem_board);
        } else {
            printf("[bus] student %d (%s) can't board at %s, %s is full for this trip, keeps waiting\n",
                   s->am, dept_name[s->department], stop_name, dept_name[s->department]);
            prev = cur;
            cur = cur->next;
        }
    }

    pthread_mutex_unlock(&q->lock);
}

void dropoff(bus *b, char *stop_name)
{
    for (int i = 0; i < b->passenger_count; i++)
        sem_post(&b->passengers[i]->sem_arrived);

    printf("[bus] arrived at %s, %d students get off\n", stop_name, b->passenger_count);

    b->passenger_count = 0;
    b->total_in = 0;
    for (int i = 0; i < DEPARTMENTS; i++)
        b->dept_count[i] = 0;
}

void *bus_func(void *arg)
{
    bus *b = (bus *)arg;
    int at_a = 1; // bus starts at stop A

    sleep(5); // let the first students reach stop A before the bus checks it

    while (1) {
        char *stop_name = at_a ? "stop A" : "stop B";

        if (b->passenger_count > 0)
            dropoff(b, stop_name);
        else
            printf("[bus] arrived at %s (empty)\n", stop_name);

        pthread_mutex_lock(&home_lock);
        int all_home = (students_home == total_students);
        pthread_mutex_unlock(&home_lock);
        if (all_home) {
            printf("[bus] everybody made it home, done for today\n");
            break;
        }

        board_students(b, at_a ? &stopA : &stopB, stop_name);

        sleep(DWELL);
        printf("[bus] leaving %s with %d students\n", stop_name, b->total_in);
        sleep(T);

        at_a = !at_a;
    }

    return NULL;
}

void *student_func(void *arg)
{
    student *s = (student *)arg;

    printf("student %d created, department %s, study time %ds\n", s->am, dept_name[s->department], s->study_time);

    // going to uni
    s->state = STOP_A;
    printf("student %d arrived at stop A\n", s->am);
    queue_push(&stopA, s);

    sem_wait(&s->sem_board);   // wait until the bus lets us on
    sem_wait(&s->sem_arrived); // wait until the bus gets to stop B

    s->state = UNIVERSITY;
    printf("student %d entered the university, studying for %ds\n", s->am, s->study_time);
    sleep(s->study_time);

    // going back home
    s->state = STOP_B;
    printf("student %d arrived at stop B\n", s->am);
    queue_push(&stopB, s);

    sem_wait(&s->sem_board);
    sem_wait(&s->sem_arrived);

    s->state = HOME;
    printf("student %d arrived home\n", s->am);

    pthread_mutex_lock(&home_lock);
    students_home++;
    pthread_mutex_unlock(&home_lock);

    sem_destroy(&s->sem_board);
    sem_destroy(&s->sem_arrived);

    return NULL;
}

student *make_student(int am)
{
    student *s = malloc(sizeof(student));
    s->am = am;
    s->department = rand() % DEPARTMENTS;
    s->study_time = (rand() % 11) + 5; // 5 - 15 sec
    s->state = STOP_A;
    sem_init(&s->sem_board, 0, 0);
    sem_init(&s->sem_arrived, 0, 0);
    return s;
}

int main(void)
{
    srand(time(NULL));

    printf("Enter the number of students: ");
    if (scanf("%d", &total_students) != 1 || total_students <= 0 || total_students > MAX_STU) {
        printf("invalid number of students (1-%d)\n", MAX_STU);
        return 1;
    }

    bus b;
    b.total_in = 0;
    b.passenger_count = 0;
    for (int i = 0; i < DEPARTMENTS; i++)
        b.dept_count[i] = 0;
    pthread_create(&b.tid, NULL, bus_func, &b);

    student **students = malloc(sizeof(student *) * total_students);
    for (int i = 0; i < total_students; i++) {
        students[i] = make_student(i);
        pthread_create(&students[i]->tid, NULL, student_func, students[i]);
    }

    for (int i = 0; i < total_students; i++)
        pthread_join(students[i]->tid, NULL);
    pthread_join(b.tid, NULL);

    for (int i = 0; i < total_students; i++)
        free(students[i]);
    free(students);

    return 0;
}
