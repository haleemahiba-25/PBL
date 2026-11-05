# Week 2: Multi-Process Simulator & POSIX IPC Engine

##  Project Overview

This project upgrades the baseline single-process CPU simulator into a modular, **three-process architecture** leveraging POSIX Inter-Process Communication (IPC). Designed as part of the Operating Systems PBL curriculum, the system decouples user interface, execution runtime, and diagnostic logging into isolated Linux processes.

##  Architecture & Component Design

The system is split into three independent process modules connected via POSIX IPC channels (`/tmp/sim_*_pipe`):

```
+-------------------+        Request Pipe        +-------------------+
|                   | -------------------------> |                   |
|   1. UI Process   |                            |  2. Core Process  |
|  (User / Input)   | <------------------------- |   (CPU Engine)    |
+-------------------+       Response Pipe        +-------------------+
                                                           |
                                                           | Log Pipe
                                                           v
                                                 +-------------------+
                                                 |                   |
                                                 | 3. Logging Process|
                                                 |   (Event Logger)  |
                                                 +-------------------+

```

## 👥 Team Responsibilities

| Role | Member Assignment | Responsibilities | 
| ----- | ----- | ----- | 
| **Amal** | User Interface (UI) Process | User prompts, CLI command formatting, menu navigation, and displaying execution state output. | 
| **Rion** | Core Simulator Engine | Simulating hardware registers (`R0`-`R3`), PC/SP pointers, RAM, Stack, Queue, and command execution (`LOAD`, `ADD`, `SHOW`). | 
| **Krithi** | Logging Process | Receiving asynchronously streamed log events, formatting timestamps, and persisting stdout/error logs. | 
| **Hiba** | Integration & Benchmarking | System integration across IPC boundaries, resolving interface issues, performing stress testing, and profiling single-process vs. multi-process performance metrics. | 

##  POSIX IPC Mechanism Justification

* **Selected Mechanism:** **Named Pipes (FIFOs)** (`mkfifo`, `/tmp/sim_*_pipe`)

* **Rationale:**

  1. **Decoupled Lifecycle:** Named pipes exist as filesystem nodes in Linux, allowing processes to launch, attach, and disconnect independently.

  2. **Strict Flow Control:** FIFO semantics ensure blocking/non-blocking FIFO guarantees across message channels without complex user-space mutexes.

  3. **Standardized Struct Protocol:** Unified C `Message` protocol headers (`common.h`) enable predictable serialization across process boundaries.

## File Structure

```
week 4/
├── common.h          # Shared IPC definitions, FIFO helper functions, and CPU Simulator engine logic
├── ui_process.c      # UI Process source file
├── core_process.c    # Core execution simulator engine
├── log_process.c     # Event and error logger module
├── Makefile          # Automated compilation and clean build system
└── README.md         # Project architecture and evaluation documentation

```

##  Compilation & Execution Instructions

### **1. Build all executables using GCC**

```
make
# Or compile manually:
gcc ui_process.c -o ui_process
gcc core_process.c -o core_process
gcc log_process.c -o log_process

```

### **2. Launch the Processes (Separate Terminals)**

* **Terminal 1 (Start Logger):**

  ```
  ./log_process
  
  ```

* **Terminal 2 (Start Core Engine):**

  ```
  ./core_process
  
  ```

* **Terminal 3 (Start User Interface):**

  ```
  ./ui_process
  
  ```

## Standalone vs. Multi-Process Performance Benchmark

| Metric | Standalone (Single Process) | Multi-Process (POSIX IPC Architecture) | 
| ----- | ----- | ----- | 
| **Execution Overhead** | Low (Direct function invocation) | Moderate (Kernel context switches & pipe I/O buffer copies) | 
| **Fault Isolation** | Low (Crash in UI or execution halts entire system) | **High** (UI or logger failure does not crash core engine) | 
| **Scalability & Concurrency** | Restricted to single runtime process | **High** (Parallel non-blocking logging and concurrent processing) | 
| **Memory Footprint** | Single heap/stack boundary | Decoupled dynamic memory per process space | 

##  IPC Test Cases Verified

1. **Pipe Creation:** Verified dynamic creation of `/tmp/sim_req_pipe`, `/tmp/sim_resp_pipe`, and `/tmp/sim_log_pipe` using `mkfifo`.

2. **Command Dispatch:** Successfully parsed assembly instructions (`LOAD R0 10`, `ADD R0 R1`, `SHOW`) across pipe descriptors.

3. **Graceful Shutdown:** Transmitted `MSG_QUIT` payload across pipes to terminate daemon processes cleanly without memory leaks.
