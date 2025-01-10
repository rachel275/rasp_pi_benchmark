import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

data = pd.read_csv('results/perf_para_results.csv')

# Display the first 5 rows
print(data.head())

# Create a DataFrame from the provided dataset
df = pd.DataFrame(data)

# Cache sizes in bytes 
L1_cache_size = 64 * 1024  # 64 KB for L1
L2_cache_size = 512 * 1024  # 512 KB for L2
L3_cache_size = 2 * 1024 * 1024  # 2 MB for L3

bytes_per_element = 8 

L1_max_K = int(np.sqrt(L1_cache_size / (3 * bytes_per_element)))
L2_max_K = int(np.sqrt(L2_cache_size / (3 * bytes_per_element)))
L3_max_K = int(np.sqrt(L3_cache_size / (3 * bytes_per_element)))
# Filter the data for M = 128
filtered_df = df[df["M"] == df["K"]]
dbl_filtered_df = filtered_df[filtered_df["M"] == filtered_df["N"]]

dbl_filtered_df.loc[dbl_filtered_df['Command'] == 'para_blis', 'M'] *= 2

dbl_filtered_df['Throughput'] = ((2 * dbl_filtered_df['M']^3) - dbl_filtered_df['M']^2) / dbl_filtered_df['Elapsed Time']
#dbl_filtered_df['Throughput'] = dbl_filtered_df['Instructions'] / dbl_filtered_df['Elapsed Time']

#dbl_filtered_df['Throughput'] = dbl_filtered_df['Instructions'] -((2 * dbl_filtered_df['M']^3) - dbl_filtered_df['M']^2)
# Plot the data
plt.figure(figsize=(10, 7))

subset = dbl_filtered_df[dbl_filtered_df["Command"] == "blis"]
plt.plot(subset["K"], (subset["Throughput"] / 1000), marker='o', linestyle='-', label=f'Single task thread')
print(subset["K"], subset["Throughput"])

#Set plot labels and scales
#plt.xscale('log')
plt.xlabel('N')
plt.ylabel('Throughput (thousand instructions/second)')
plt.title('Instruction Throughput of N^2 Matrix Multiplication')
plt.grid(True)

# Add vertical lines for cache thresholds
plt.axvline(L1_max_K, color='r', linestyle='--', label=f'L1 Cache Threshold (N={L1_max_K})')
plt.axvline(L2_max_K, color='b', linestyle='--', label=f'L2 Cache Threshold (N={L2_max_K})')
plt.axvline(L3_max_K, color='g', linestyle='--', label=f'L3 Cache Threshold (N={L3_max_K})')

plt.legend()
plt.show()
