# Week 5: Simulation and Comparative Analysis of Page Replacement Algorithms

## Project Overview
This project simulates and compares three fundamental page replacement algorithms used in operating systems virtual memory management:
1. **FIFO (First-In, First-Out)**
2. **LRU (Least Recently Used)**
3. **Optimal Page Replacement**

The program accepts a user-defined page-reference string and frame count, processes each reference step-by-step, counts hits and faults, computes performance ratios, and generates a side-by-side comparative summary.

---

##  Team Task Division (4-Member Distribution)

The project workload is modularized cleanly across a 4-member team:

* **Krithi (`input.c`) — Input & Initialization:**
  * Handles user input for the page-reference string and number of physical frames.
  * Implements input validation (edge cases, non-positive counts, invalid entries).
  * Initializes memory frame data structures.

* **Amal (`fifo.c`) — FIFO Simulation:**
  * Implements the First-In, First-Out page replacement logic using a circular tracking pointer.
  * Generates the step-by-step simulation table displaying frame status changes, hits, and faults.

* **Rion (`replacement.c`) — LRU & Optimal Algorithms:**
  * **LRU:** Tracks page usage history to replace the least recently used page during a fault.
  * **Optimal:** Looks ahead into the reference string to replace the page used farthest in the future.
  * Generates step-by-step tracking output for both algorithms[cite: 8].

* **Hiba(`main.c`) — Metrics, Summary & Integration:**
  * Calculates performance metrics (**Hit Ratio** and **Fault Ratio**).
  * Generates the final comparative summary table comparing FIFO, LRU, and Optimal performance.
  * Implements the master interactive menu loop linking all modules together.

---

##  Repository Structure
```text
week 5/
├── input.c        # Krithi: Input management and frame initialization
├── fifo.c         # Amal  : FIFO algorithm and step-by-step table
├── replacement.c  # Rion  : LRU and Optimal algorithms implementation
├── main.c         # Hiba  : Metrics, summary table, and master menu loop
└── README.md      # Project documentation
