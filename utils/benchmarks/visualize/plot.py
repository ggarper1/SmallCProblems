import pandas as pd
import matplotlib.pyplot as plt
import os

CSV_FILE = "./utils/benchmarks/data/stack1.csv"

CLOCKS_PER_SEC = 1000000.0

def plot_stack_performance(csv_path):
    """
    Loads stack benchmark data from a CSV, converts times to seconds,
    and plots the execution time vs. the number of elements.
    """
    if not os.path.exists(csv_path):
        print(f"🚨 Error: CSV file not found at path: {csv_path}")
        print("Please ensure your C program ran successfully and the path is correct.")
        return

    try:
        df = pd.read_csv(csv_path, 
                         names=['length', 'linkedListStack_ticks', 'arrayStack_ticks'], 
                         skiprows=1) # Skip the header row if it exists
    except Exception as e:
        print(f"🚨 Error reading CSV file: {e}")
        return

    df['linkedListStack_sec'] = df['linkedListStack_ticks'] / CLOCKS_PER_SEC
    df['arrayStack_sec'] = df['arrayStack_ticks'] / CLOCKS_PER_SEC

    plt.figure(figsize=(10, 6))

    plt.plot(df['length'], df['linkedListStack_sec'], 
             label='Linked List Stack (Push Time)', 
             marker='o', 
             linestyle='-')

    plt.plot(df['length'], df['arrayStack_sec'], 
             label='Array Stack (Push Time)', 
             marker='x', 
             linestyle='--')

    plt.title('Stack Performance Comparison: Array vs. Linked List')
    plt.xlabel('Number of Elements Added (N)')
    plt.ylabel('Total Execution Time (Seconds)')
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend()
    
    plt.tight_layout()

    plt.show()

if __name__ == "__main__":
    plot_stack_performance(CSV_FILE)
