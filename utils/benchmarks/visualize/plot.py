import pandas as pd
import matplotlib.pyplot as plt
import os

CSV_FILE = "./utils/benchmarks/data/boolStack.csv"
CLOCKS_PER_SEC = 1000000.0

def plot_csv_data(csv_path, convert_to_seconds=True, clocks_per_sec=CLOCKS_PER_SEC):
    """
    Generic function to plot CSV data with three columns.

    Parameters:
    -----------
    csv_path : str
        Path to the CSV file
    convert_to_seconds : bool
        Whether to convert the y-axis values from clock ticks to seconds
    clocks_per_sec : float
        Conversion factor for clock ticks (only used if convert_to_seconds=True)

    The CSV must have exactly 3 columns:
    - Column 1: X-axis values (e.g., length, size, iterations)
    - Column 2: First dataset Y-values
    - Column 3: Second dataset Y-values
    """
    if not os.path.exists(csv_path):
        print(f"🚨 Error: CSV file not found at path: {csv_path}")
        print("Please ensure the file exists and the path is correct.")
        return

    try:
        df = pd.read_csv(csv_path)
    except Exception as e:
        print(f"🚨 Error reading CSV file: {e}")
        return

    if len(df.columns) != 3:
        print(f"🚨 Error: Expected 3 columns, but found {len(df.columns)}")
        print(f"Columns found: {list(df.columns)}")
        return
    
    x_col, y1_col, y2_col = df.columns

    if convert_to_seconds:
        df[f'{y1_col}_sec'] = df[y1_col] / clocks_per_sec
        df[f'{y2_col}_sec'] = df[y2_col] / clocks_per_sec
        y1_data = df[f'{y1_col}_sec']
        y2_data = df[f'{y2_col}_sec']
        y_label = 'Time (Seconds)'
    else:
        y1_data = df[y1_col]
        y2_data = df[y2_col]
        y_label = 'Value'

    plt.figure(figsize=(10, 6))
    plt.plot(df[x_col], y1_data, 
             label=y1_col, 
             marker='o', 
             linestyle='-')
    plt.plot(df[x_col], y2_data, 
             label=y2_col, 
             marker='x', 
             linestyle='--')
    
    plt.title(f'Performance Comparison: {y1_col} vs {y2_col}')
    plt.xlabel(x_col)
    plt.ylabel(y_label)
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.legend()
    
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    # Example usage with time conversion
    plot_csv_data(CSV_FILE, convert_to_seconds=True)
    
    # If you have a CSV that doesn't need time conversion, use:
    # plot_csv_data("path/to/data.csv", convert_to_seconds=False)
