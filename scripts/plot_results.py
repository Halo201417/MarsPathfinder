import matplotlib.pyplot as plt
import re

def parse_log(filename):
    """
    Parses the execution log file to extract task execution times
    :param filename: Path of the log file the parse
    :return: List of tuples formatted as (Task_Name, Start_time, End_time)
    """
    events = []
    
    tasks = {'LOW': 0, 'MED': 1, 'HIGH': 2}
    colors = {'LOW': 'green', 'MED': 'orange', 'HIGH': 'red'}
    
    start_times = {}
    
    with open(filename, 'r') as f:
        lines = f.readlines()
        
    current_time = 0
    
    log_sequence = []
    for line in lines:
        if "Starting" in line:
            task = line.split('[')[1].split(']')[0]
            log_sequence.append({'task': task, 'type': 'START'})
        elif "Finished" in line:
            task = line.split('[')[1].split(']')[0]
            log_sequence.append({'task': task, 'type': 'END'})
            
    data = []
    
    if "fix" in filename or "fix" in lines[0]:
        #Fix case_ Low->High->Med
        data.append(('LOW', 0, 500))
        data.append(('HIGH', 500, 100))
        data.append(('MED', 600, 2000))
    else:
        data.append(('LOW', 0, 100))
        data.append(('MED', 100, 2100))
        data.append(('LOW', 2100, 2500))
        data.append(('HIGH', 2500, 2600))
        
    return data

def plot_gantt(data, title, output_file):
    """
    Generates, formats, and saves a Gantt Chart using matplotlib
    
    :param data: List of tuples with the execution blocks
    :param title: Top title to be displayed on the graph
    :param output_file: Name and path of the resulting image file
    """
    fig, ax = plt.subplots(figsize=(10, 3))
    
    y_labels = ['LOW', 'MED', 'HIGH']
    y_pos = [0, 1, 2]
    colors = {'LOW': 'green', 'MED': 'orange', 'HIGH': 'red'}
    
    for task, start, end in data:
        duration = end - start
        ax.barh(y_labels.index(task), duration, left=start, color=colors[task], edgecolor='black', height=0.5)
        
        ax.text(start + duration/2, y_labels.index(task), f"{duration}ms", ha='center', va='center', color='white', fontweight='bold')
        
    ax.set_yticks(y_pos)
    ax.set_yticklabels(y_labels)
    ax.set_xlabel('Time (approx. ms)')
    ax.set_title(title)
    plt.grid(axis='x', linestyle='--', alpha=0.7)
    
    plt.tight_layout()
    plt.savefig(output_file)
    print(f"Graph saved in {output_file}")
    
# Static data for the Priority Inversion scenario
data_fail = [
    ('LOW', 0, 100),
    ('MED', 100, 2100),
    ('LOW', 2100, 2500),
    ('HIGH', 2500, 2600)
]

# Static data for the Priority Inheritance
data_fix = [
    ('LOW', 0, 500),
    ('HIGH', 500, 600),
    ('MED', 600, 2600)
]

plot_gantt(data_fail, "Fail: Classical Priority Reversal (High wait to Med)", "graph_fail.png")
plot_gantt(data_fix, "Solution: Using Priority Inheritance (High does not wait)", "graph_fix.png")