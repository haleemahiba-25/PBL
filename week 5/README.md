# Page Replacement Algorithms Simulator (Virtual Memory GUI)

##  Project Overview
This project is an interactive **Page Replacement Algorithm Simulator** designed to demonstrate virtual memory management concepts in Operating Systems. The application models how physical memory frames handle incoming page requests using **FIFO (First-In-First-Out)**, **LRU (Least Recently Used)**, and **Optimal Page Replacement** algorithms, while calculating page hits, page faults, and performance ratios in real time.

---

## Key Features & Objectives
1. **Dynamic Inputs**: Accepts user-defined page-reference strings and physical page frame counts ($1 \dots 7$).
2. **Algorithm Engine**:
   - **FIFO**: Replaces the oldest loaded page using a circular pointer queue.
   - **LRU**: Tracks historical access timestamps to evict the least recently referenced page.
   - **Optimal**: Inspects future references to replace the page that will not be used for the longest time.
3. **Step-by-Step Memory Execution Trace**: Renders a color-coded table displaying memory frame slots after every reference step, clearly marking **Hits** (green) and **Page Faults** (red).
4. **Performance Metrics & Ratios**:
   $$\text{Hit Ratio} = \left(\frac{\text{Page Hits}}{\text{Total Page References}}\right) \times 100$$
   $$\text{Fault Ratio} = \left(\frac{\text{Page Faults}}{\text{Total Page References}}\right) \times 100$$
5. **Comparative Visual Dashboard**: Side-by-side metric tables and performance comparison bars.
6. **PDF Sample Preset**: Includes a quick button to load the document sample test case (`7 0 1 2 0 3 0 4 2 3` with `3` frames).

---

## Repository Structure

```text
.
├── page_replacement_simulator.html   # Interactive HTML5/JS Visual Dashboard
└── README.md                         # Documentation & Team Division
```

---

##  Execution Instructions

The project is built as a stand-alone, client-side web dashboard. No external web server, compilers, or Linux packages are required.

### Simple Steps to Run:
1. Save `page_replacement_simulator.html` to your local folder.
2. Double-click `page_replacement_simulator.html` to open it in any modern browser (Chrome, Firefox, Edge, Safari).
3. Click **"Load PDF Sample Preset"** or enter custom inputs, then click **"Run Simulation"**.

---

##  Verification Test Case

Using the sample data from the project specification document:
* **Page-Reference String**: `7 0 1 2 0 3 0 4 2 3`
* **Page Frames**: `3`

### Expected Output Summary:
| Algorithm | Page Hits | Page Faults | Hit Ratio | Fault Ratio |
| :--- | :---: | :---: | :---: | :---: |
| **FIFO** | 1 | 9 | $10.0\%$ | $90.0\%$ |
| **LRU** | 2 | 8 | $20.0\%$ | $80.0\%$ |
| **Optimal** | 3 | 7 | $30.0\%$ | $70.0\%$ |

---

##  Team Work Allocation (4-Member Division)

| Member | Assigned Role | Primary Responsibilities & Deliverables |
| :--- | :--- | :--- |
| **Member 1** | **Virtual Memory Core & Input Parser** | - Manages reference string parsing, array tokenization, and frame count validation.<br>- Handles edge cases (empty strings, single frame, zero frame errors).<br>- Introduces virtual memory concepts, demand paging, and page table basics during presentation. |
| **Member 2** | **FIFO & LRU Algorithm Logic** | - Implements the **FIFO** circular queue pointer replacement algorithm.<br>- Implements the **LRU** timestamp/stack tracking eviction logic.<br>- Explains queue mechanics vs. recency tracking during the lab viva. |
| **Member 3** | **Optimal Algorithm & Performance Analytics** | - Implements the **Optimal** look-ahead algorithm (inspecting future page requests).<br>- Calculates Hit Ratio ($\%$) and Fault Ratio ($\%$) metrics.<br>- Analyzes Belady's Anomaly and comparative performance across algorithms. |
| **Member 4** | **GUI Engine & Visualization Interface** | - Constructs the HTML5/Tailwind layout, status badges, and dynamic data tables.<br>- Renders step-by-step memory frame execution trace tables and hit/fault badges.<br>- Integrates test presets and visual comparative bar charts. |
