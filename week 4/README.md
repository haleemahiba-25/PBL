# Interactive Deadlock Simulation System (Week 4)

##  Project Overview

Deadlock occurs when a set of processes are blocked because each process holds a resource while waiting for another resource held by another process.

This interactive system simulates:
* **Resource Allocation:** Assigning resources to processes.
* **Resource Requests:** Simulating dynamic requests from active processes.
* **Deadlock Avoidance:** Running Banker's Algorithm to maintain safe states.
* **Deadlock Detection:** Identifying permanently blocked processes.
* **Resource Allocation Graph (RAG):** Visualizing process-resource interactions.
* **Deadlock Recovery:** Terminating processes or preempting resources to resolve deadlocks.

##  Project Structure & Team Division

The project is divided among 4 module roles:

* **Hiba: Data Structures & Input Management**
  * Manages system vectors and matrices: `Available`, `Allocation`, `Max`, and `Need`.
* **Amal: Deadlock Avoidance (Banker's Algorithm)**
  * Runs Banker's Algorithm, generates safe sequences, and tests dynamic resource requests.
* **Krithi: Deadlock Detection, RAG, & Recovery**
  * Detects active deadlocks, prints the Resource Allocation Graph (RAG), and handles recovery.
* **Rion: Integration & Performance Summary**
  * Connects all modules into the main execution flow and builds the performance summary.

##  How to Compile and Run

### Prerequisites
* GCC Compiler installed (works on Linux/Kali Linux, macOS, and Windows via MinGW/WSL).

### Compilation
Navigate to the project source directory and compile all C files together:

```bash
gcc main.c -o deadlock_sim
