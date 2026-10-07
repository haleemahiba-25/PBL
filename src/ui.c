/* ui.c - UI PROCE
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

static int req_q, resp_q;
static sem_t reply_ready;

static void print_help(void) {
    printf("\n=== Simulator instructions (registers R0-R3) ===\n");
    printf("  LOAD R0 5     put number 5 in R0\n");
    printf("  ADD  R0 R1    R0 = R0 + R1\n");
    printf("  SUB  R0 R1    R0 = R0 - R1\n");
    printf("  STORE 10 R0   MEM[10] = R0      (memory: 0-63)\n");
    printf("  FETCH R1 10   R1 = MEM[10]\n");
    printf("  PUSH R0       push R0 on stack\n");
    printf("  POP  R1       pop stack into R1\n");
    printf("  ENQ  R0       put R0 in queue\n");
    printf("  DEQ  R1       take from queue into R1\n");
    printf("  SHOW          show registers, PC, SP, queue size\n");
    printf("  HELP / QUIT\n\n");
}

/* Thread: print every answer that comes from the Core */
static void *display_thread(void *arg) {
    (void)arg;
    Message m;
    while (1) {
        if (!recv_msg(resp_q, &m)) break;    /* pipe closed */
        if (m.type == MSG_QUIT) break;       /* Core is shutting down */
        printf("  Core: %s\n", m.text);
        fflush(stdout);
        sem_post(&reply_ready);              /* tell main thread: answer printed */
    }
    return NULL;
}

int main(void) {
    req_q  = open_pipe(REQ_PIPE,  O_WRONLY);
    resp_q = open_pipe(RESP_PIPE, O_RDONLY); /* we only read answers */
    sem_init(&reply_ready, 0, 0);

    pthread_t disp;
    pthread_create(&disp, NULL, display_thread, NULL);

    print_help();

    char line[TEXT_SIZE];
    while (1) {
        printf("sim> ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;      /* Ctrl-D */
        line[strcspn(line, "\n")] = '\0';                  /* remove newline */

        if (line[0] == '\0') continue;
        if (strcasecmp(line, "QUIT") == 0 || strcasecmp(line, "EXIT") == 0) break;
        if (strcasecmp(line, "HELP") == 0) { print_help(); continue; }

        send_msg(req_q, MSG_COMMAND, "UI", line);          /* UI -> Core */
        sem_wait(&reply_ready);                            /* wait for the answer */
    }

    send_msg(req_q, MSG_QUIT, "UI", "quit");   /* stop the Core (Core then stops our display thread) */
    pthread_join(disp, NULL);

    close(req_q);
    close(resp_q);
    sem_destroy(&reply_ready);
    printf("Goodbye!\n");
    return 0;
}