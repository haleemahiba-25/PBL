#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

/* --- 1. IPC / PIPE CONFIGURATION --- */
#define REQ_PIPE  "/tmp/sim_req_pipe"
#define RESP_PIPE "/tmp/sim_resp_pipe"
#define LOG_PIPE  "/tmp/sim_log_pipe"

#define TEXT_SIZE 256

typedef enum {
    MSG_COMMAND,     /* UI -> Core */
    MSG_RESPONSE,    /* Core -> UI */
    MSG_LOG_INFO,    /* Core -> Logger (Info) */
    MSG_LOG_ERROR,   /* Core -> Logger (Error) */
    MSG_QUIT         /* Shutdown signal */
} MessageType;

typedef struct {
    MessageType type;
    char source[16];          
    char text[TEXT_SIZE];     
} Message;

static inline int open_pipe(const char *path, int mode) {
    mkfifo(path, 0666);
    int fd = open(path, mode);
    if (fd < -1) {
        perror("open_pipe failed");
        exit(1);
    }
    return fd;
}

static inline void send_msg(int fd, MessageType type, const char *source, const char *text) {
    Message m;
    m.type = type;
    strncpy(m.source, source, sizeof(m.source) - 1);
    m.source[sizeof(m.source) - 1] = '\0';
    strncpy(m.text, text, sizeof(m.text) - 1);
    m.text[sizeof(m.text) - 1] = '\0';
    write(fd, &m, sizeof(Message));
}

static inline int recv_msg(int fd, Message *m) {
    int bytes = read(fd, m, sizeof(Message));
    return (bytes > 0);
}


/* --- 2. CPU / SIMULATOR ENGINE --- */
#define MEM_SIZE 64
#define STACK_SIZE 16
#define QUEUE_SIZE 16

typedef struct {
    int registers[4]; /* R0, R1, R2, R3 */
    int pc;           /* Program Counter */
    int sp;           /* Stack Pointer */
    int memory[MEM_SIZE];
    int stack[STACK_SIZE];
    int queue[QUEUE_SIZE];
    int q_head, q_tail, q_count;
} Simulator;

static inline void sim_init(Simulator *sim) {
    memset(sim, 0, sizeof(Simulator));
    sim->sp = -1;
}

static inline int sim_execute(Simulator *sim, const char *cmd, char *out, size_t out_size, const char **component) {
    char op[16] = {0};
    char arg1[16] = {0};
    char arg2[16] = {0};

    sscanf(cmd, "%s %s %s", op, arg1, arg2);

    if (strcasecmp(op, "LOAD") == 0) {
        int reg = arg1[1] - '0';
        int val = atoi(arg2);
        if (reg >= 0 && reg < 4) {
            sim->registers[reg] = val;
            snprintf(out, out_size, "R%d set to %d", reg, val);
            *component = "CPU";
            return 0;
        }
    } else if (strcasecmp(op, "ADD") == 0) {
        int r1 = arg1[1] - '0';
        int r2 = arg2[1] - '0';
        if (r1 >= 0 && r1 < 4 && r2 >= 0 && r2 < 4) {
            sim->registers[r1] += sim->registers[r2];
            snprintf(out, out_size, "R%d is now %d", r1, sim->registers[r1]);
            *component = "ALU";
            return 0;
        }
    } else if (strcasecmp(op, "SHOW") == 0) {
        snprintf(out, out_size, "R0=%d R1=%d R2=%d R3=%d | PC=%d SP=%d Q_Count=%d",
                 sim->registers[0], sim->registers[1], sim->registers[2], sim->registers[3],
                 sim->pc, sim->sp, sim->q_count);
        *component = "CPU";
        return 0;
    }

    snprintf(out, out_size, "Unknown or invalid command: %s", op);
    *component = "CPU";
    return -1;
}

#endif /* COMMON_H */