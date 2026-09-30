# PBL
## Inter-Process Communication (IPC)

**Inter-Process Communication (IPC)** is a way for processes in an operating system to communicate and share data with each other.

Each process normally has its own memory space, so one process cannot directly access another process’s data. IPC provides safe methods for processes to exchange information, share resources, and work together.

## Why Is IPC Used?

IPC is used to:

- Share data between processes
- Coordinate different tasks
- Synchronize process execution
- Share system resources
- Build faster and more organized applications

IPC is commonly used in web browsers, databases, client-server applications, operating system services, and distributed systems.

## Common IPC Methods

| IPC Method | Simple Description |
| :--- | :--- |
| **Pipes** | Send data between related processes |
| **Message Queues** | Exchange messages between processes |
| **Shared Memory** | Allow processes to use the same memory area |
| **Sockets** | Enable communication between processes on the same or different computers |

## Main Types of IPC

### Message Passing

Processes communicate by sending and receiving messages. This method is simple and safe because processes do not directly share memory.

### Shared Memory

Processes communicate by using a common memory area. It is very fast, but synchronization is needed to prevent data conflicts.

## Example

```text
Client Process  ─── Request ───>  Server Process
Client Process  <── Response ───  Server Process
```

The client sends a request, and the server processes it and sends back a response.

## Advantages of IPC

- Helps processes communicate safely
- Supports multitasking
- Allows resource sharing
- Improves application performance
- Makes applications more modular

## Challenges

IPC must be handled carefully to avoid:

- Race conditions
- Deadlocks
- Data inconsistency
- Unauthorized access


IPC is an important operating-system concept that allows independent processes to communicate and work together. 

In simple words, IPC is a communication bridge between processes.
