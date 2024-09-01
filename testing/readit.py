import matplotlib.pyplot as plt
import pandas as pd

# Read the FM signal data from the CSV file
data = pd.read_csv('fm_signal.csv')

# Plot the data
plt.figure(figsize=(10, 6))
plt.plot(data['Time'], data['FM_Signal'], label='FM Signal')
plt.title('FM Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.legend()
plt.grid(True)
plt.show()
