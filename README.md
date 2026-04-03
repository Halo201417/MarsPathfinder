<h1 align="center">Mars Pathfinder Priority Inversion</h1>

<p align="center">
  <a href="https://github.com/Halo201417/marspathfinder/stargazers">
    <img src="https://img.shields.io/github/stars/Halo201417/marspathfinder.svg?colorA=363a4f&colorB=b7bdf8&style=for-the-badge&v=1" alt="Stars">
  </a>
  <a href="https://github.com/Halo201417/MarsPathfinder/issues">
    <img src="https://img.shields.io/github/issues/Halo201417/MarsPathfinder.svg?colorA=363a4f&colorB=f5a97f&style=for-the-badge&v=1" alt="Issues">
  </a>
  <a href="https://github.com/Halo201417/MarsPathfinder/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/Halo201417/MarsPathfinder.svg?colorA=363a4f&colorB=a6da95&style=for-the-badge&v=2" alt="License">
  </a>
</p>

<p align="center">
  <b>A Real-Time Systems simulation in C demonstrating the classic Priority Inversion problem that occurred on the Mars Pathfinder, and its solution using Priority Inheritance.</b>
</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png" alt="line">
</p>

## 📖 Table of Contents

- [📖 Table of Contents](#-table-of-contents)
- [📋 Prerequisites](#-prerequisites)
- [⚙️ Installation and Setup](#️-installation-and-setup)
  - [1. Clone the repository](#1-clone-the-repository)
  - [2. Compile the C Project](#2-compile-the-c-project)
  - [3. Install Python Dependencies (For graphs)](#3-install-python-dependencies-for-graphs)
- [🚀 System Usage](#-system-usage)
  - [1. Classical Priority Reversal (Failure Mode)](#1-classical-priority-reversal-failure-mode)
  - [2. Priority Inheritance (Solution Mode)](#2-priority-inheritance-solution-mode)
  - [3. Generate Visual Gantt Charts](#3-generate-visual-gantt-charts)
- [📁 Code Structure](#-code-structure)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)

<p align="center">
  <img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png" alt="line">
</p>

## 📋 Prerequisites

This project simulates real-time operating system task scheduling and requires specific environment configurations.

* **Operating System:** Debian/Ubuntu or derivative Linux distributions.
* **Compiler:** `gcc` and `make`.
* **Privileges:** `sudo` access (Required to set POSIX `SCHED_FIFO` real-time priorities and CPU affinity).
* **Python (Optional):** Python 3 and `matplotlib` for generating the result graphs.

<p align="center">
  <img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png" alt="line">
</p>

## ⚙️ Installation and Setup

### 1. Clone the repository
```bash
git clone https://github.com/Halo201417/MarsPathfinder.git
cd ./MarsPathfinder
```

### 2. Compile the C Project
Use the provided Makefile to compile the source code:
```bash
make
```
This will generate the executable inside the `bin/` directory.

### 3. Install Python Dependencies (For graphs)
```bash
pip install matplotlib
```

<p align="center">
  <img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png" alt="line">
</p>

## 🚀 System Usage

The project simulates three tasks (LOW, MED, HIGH) competing for CPU time and a shared Mutex. It must be run with `sudo` to allow real-time thread priorities.

### 1. Classical Priority Reversal (Failure Mode)
Run the default simulation where unbounded priority inversion occurs:
```bash
sudo ./bin/rt_project
```
*Observation: The HIGH priority task will be blocked indefinitely while the MED priority task hijacks the CPU.*

### 2. Priority Inheritance (Solution Mode)
Run the simulation applying the Priority Inheritance Protocol (PIP) to the Mutex:
```bash
sudo ./bin/rt_project fix
```
*Observation: The LOW priority task inherits the HIGH priority, preventing MED from interrupting, thus solving the inversion.*

### 3. Generate Visual Gantt Charts
Generate graphical representations of both executions:
```bash
python3 scripts/plot_results.py
```
This will create `graph_fail.png` and `graph_fix.png`.

<p align="center">
  <img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png" alt="line">
</p>

## 📁 Code Structure

* `src/main.c` - Entry point, thread creation, and scheduler policy configuration.
* `src/tasks.c` - Definition of the LOW, MED, and HIGH priority tasks and their critical sections.
* `src/utils.c` - CPU burning and real-time visual terminal output logic.
* `include/` - Header files (`config.h`, `task.h`, `utils.h`).
* `scripts/plot_results.py` - Python script to plot Gantt charts using matplotlib.
* `Makefile` - Compilation instructions.

<p align="center">
  <img src="https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png" alt="line">
</p>

## 🤝 Contributing

Contributions, issues, and pull requests are welcome! 
Feel free to check the [issues page](https://github.com/Halo201417/MarsPathfinder/issues).

## 📄 License

This project is licensed under the **MIT** license. You can freely use, modify, and distribute it.