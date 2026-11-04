CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: ui core logger simulator

ui: ui.c
	$(CC) $(CFLAGS) ui.c -o ui

core: core.c
	$(CC) $(CFLAGS) core.c -o core

logger: logger.c
	$(CC) $(CFLAGS) logger.c -o logger

simulator: simulator.c
	$(CC) $(CFLAGS) simulator.c -o simulator

clean:
	rm -f ui core logger simulator
