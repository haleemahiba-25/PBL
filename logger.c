/* logger.c - LOGGING PROCESS (Student 3)
   Two threads (producer / consumer):
     receiver thread : reads messages from the log pipe -> puts them in a buffer
     writer thread   : takes messages from the buffer    -> writes them to the file
   This way a slow disk write never blocks the Core process. */
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define BUF_SIZE 64

static Message buffer[BUF_SIZE];     /* circular buffer shared by both threads */
static int head = 0, tail = 0, count = 0;

static pthread_mutex_t lock      = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  not_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t  not_full  = PTHREAD_COND_INITIALIZER;

static int log_q;
static FILE *logfile;

/* Thread 1: pipe -> buffer */
static void *receiver_thread(void *arg) {
    (void)arg;
    Message m;
    while (1) {
        if (!recv_msg(log_q, &m)) m.type = MSG_QUIT;   /* pipe closed = stop */

        pthread_mutex_lock(&lock);
        while (count == BUF_SIZE)                    /* buffer full: wait */
            pthread_cond_wait(&not_full, &lock);
        buffer[tail] = m;
        tail = (tail + 1) % BUF_SIZE;
        count++;
        pthread_cond_signal(&not_empty);             /* wake the writer */
        pthread_mutex_unlock(&lock);

        if (m.type == MSG_QUIT) break;
    }
    return NULL;
}

/* Thread 2: buffer -> file */
static void *writer_thread(void *arg) {
    (void)arg;
    while (1) {
        pthread_mutex_lock(&lock);
        while (count == 0)                           /* buffer empty: wait */
            pthread_cond_wait(&not_empty, &lock);
        Message m = buffer[head];
        head = (head + 1) % BUF_SIZE;
        count--;
        pthread_cond_signal(&not_full);              /* wake the receiver */
        pthread_mutex_unlock(&lock);

        if (m.type == MSG_QUIT) break;

        char timestr[16];
        time_t now = time(NULL);
        struct tm tmv;
        localtime_r(&now, &tmv);
        strftime(timestr, sizeof(timestr), "%H:%M:%S", &tmv);

        const char *level = (m.type == MSG_LOG_ERROR) ? "ERROR" : "INFO";
        fprintf(logfile, "[%s] [%-5s] [%s] %s\n", timestr, level, m.source, m.text);
        fflush(logfile);                             /* write to disk now */
    }
    return NULL;
}

int main(void) {
    log_q   = open_pipe(LOG_PIPE, O_RDONLY);
    logfile = fopen("simulator.log", "w");
    if (!logfile) { perror("fopen"); return 1; }

    pthread_t rx, wr;
    pthread_create(&rx, NULL, receiver_thread, NULL);
    pthread_create(&wr, NULL, writer_thread, NULL);
    pthread_join(rx, NULL);
    pthread_join(wr, NULL);

    fclose(logfile);
    close(log_q);
    return 0;
}