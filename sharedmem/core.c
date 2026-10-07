#include "common.h"
#include "cpu.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    /* Open order matters: every process opens the pipes in the same order,
       otherwise two processes could wait for each other forever. */
    int req_q  = open_pipe(REQ_PIPE,  O_RDONLY);   /* we only read commands  */
    int resp_q = open_pipe(RESP_PIPE, O_WRONLY);   /* we only write results  */
    int log_q  = open_pipe(LOG_PIPE,  O_WRONLY);   /* we only write logs     */

    Simulator sim;
    sim_init(&sim);
    send_msg(log_q, MSG_LOG_INFO, "CORE", "Core process started");

    Message m;
    while (1) {
        if (!recv_msg(req_q, &m)) break;     /* wait for a command (0 = UI closed pipe) */
        if (m.type == MSG_QUIT) break;

        char out[TEXT_SIZE];
        const char *component;
        int rc = sim_execute(&sim, m.text, out, sizeof(out), &component);

        /* 1) answer the UI */
        send_msg(resp_q, MSG_RESPONSE, "CORE", out);

        /* 2) tell the Logger what happened */
        char line[320];
        snprintf(line, sizeof(line), "%s -> %s", m.text, out);
        send_msg(log_q, (rc == 0) ? MSG_LOG_INFO : MSG_LOG_ERROR, component, line);
    }

    send_msg(resp_q, MSG_QUIT, "CORE", "bye");          /* lets the UI display thread stop */
    send_msg(log_q, MSG_LOG_INFO, "CORE", "Core process stopped");
    send_msg(log_q, MSG_QUIT, "CORE", "shutdown");     /* tell Logger to stop */

    close(req_q); close(resp_q); close(log_q);
    return 0;
}