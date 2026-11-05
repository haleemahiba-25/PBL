# Week 4: Interactive Deadlock Simulation System (GUI Edition)

##  Project Overview
This project presents an interactive **Deadlock Simulation System** built to demonstrate how an Operating System manages resource allocation, evaluates safety states, detects deadlocks, and recovers blocked processes. The simulator provides dynamic visualization tools including auto-computing matrix tables and a real-time **Resource Allocation Graph (RAG)** engine.

---

##  Key Objectives & Features
1. **System Initialization**: Define custom process counts ($P_0 \dots P_n$) and resource type vectors ($R_0 \dots R_m$).
2. **Matrix Management**: Real-time entry for Allocation and Maximum matrices with automatic computation of the **Need Matrix** ($Need = Max - Allocation$).
3. **Banker's Algorithm (Deadlock Avoidance)**: Evaluates system safety and generates explicit safe execution sequences.
4. **Deadlock Detection**: Identifies circular wait conditions and flags specific deadlocked processes.
5. **Resource Allocation Graph (RAG)**: Visualizes process-to-resource graph edges ($R \to P$ allocation and $P \to R$ request lines) using an HTML5 Canvas engine.
6. **Dynamic Resource Requests (Choice 7)**: Simulates real-time process requests with Banker's safety check validation and automatic rollback for unsafe requests.
7. **Deadlock Recovery (Choice 10)**: Terminates deadlocked processes and releases their allocated instances back to the system's `Available` pool.

---

## Project Structure & Tech Stack
* **Frontend/Interface**: HTML5, Tailwind CSS, FontAwesome icons.
* **Logic & Engine**: JavaScript (ES6+), Canvas API.
* **Execution Environment**: Cross-platform (Windows, macOS, Linux) — runs in any standard web browser (Chrome, Edge, Firefox, Safari).

```text
.
├── index.html       # Combined Single-File Interactive Dashboard (GUI Engine)
└── README.md        # Project documentation and team delegation breakdown
```

---

## Execution Instructions

Because the GUI simulator is standard HTML/JavaScript, no compilation, server setup, or special OS environment is required.

### Windows / macOS / Linux (File Explorer)
1. Locate `index.html` in your file manager.
2. Double-click `index.html` to open it in your default browser.

##  Verification & Test Presets

### 1. Safe State Verification
* Click the **"Load Safe Preset"** button at the top header.
* Click **"Run Banker's Safety"**.
* **Expected Output**: System state badge highlights **SAFE STATE** and outputs a valid sequence (e.g., `P1 ➔ P0 ➔ P2`).

### 2. Deadlock Condition Verification
* Click **"Load Deadlock Preset"**.
* Click **"Detect Deadlock"**.
* **Expected Output**: System state highlights **DEADLOCK DETECTED** and flags the deadlocked processes.
* Click **"Terminate Process"** (Recovery) to select a process, release its resources, and resolve the deadlock.

---

## Team Work Allocation (4-Member Division)

| Member | Assigned Role | Core Responsibilities & Deliverables |
| :--- | :--- | :--- |
| **Member 1** | **System Architecture & Data Entry** | - System dimensions logic (`numProcesses`, `numResources`).<br>- State vector inputs (`Available` vector, `Allocation` matrix, `Max` matrix).<br>- Automated computation of the **Need Matrix**. |
| **Member 2** | **Banker's Algorithm Engine** | - Implementation of Banker's Safety algorithm.<br>- Safe sequence vector calculation and validation.<br>- Handling **Dynamic Resource Requests** (Safety verification & Rollback). |
| **Member 3** | **Deadlock Detection & Recovery** | - Detection algorithm implementation to scan for circular wait conditions.<br>- Identifying specific deadlocked processes.<br>- Implementing process termination & resource release recovery logic. |
| **Member 4** | **Visual RAG & UI Integration** | - HTML5 Canvas rendering for the **Resource Allocation Graph (RAG)**.<br>- Color-coded nodes (Processes/Resources) and directional request/allocation edges.<br>- Dashboard UX, presets, and diagnostic execution logger. |
