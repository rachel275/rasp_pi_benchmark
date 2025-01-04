import numpy as np
import matplotlib.pyplot as plt
from enum import Enum

class unit(Enum):
    NS = 1
    US = 2
    MS = 3

UNIT = unit.US 

# N, computation time
data = [
    (16, 656), # us
    (32, 610),
    (64, 700),
    (128, 1709),
    (256, 9502),
    (512, 71753),
    (768, 240545),
    (1024, 570007)
]

for i in range(len(data)):
    data[i] = list(data[i])
    if UNIT == unit.NS:
        data[i] = (data[i][0], data[i][1] / 10e5) # convert to ms
    elif UNIT == unit.US:
        data[i] = (data[i][0], data[i][1] / 10e2)
        
N_values = np.array([d[0] for d in data])
computation_time = np.array([d[1] for d in data])

print(N_values)
print(computation_time)

###### aN^3 + bN^2 + cN + d ######
coefficients = np.polyfit(N_values, computation_time, 3)
model = np.poly1d(coefficients)

N_predict = np.linspace(min(N_values), max(N_values), 100)
computation_time_predict = model(N_predict)
print(f"Cubic Model Coefficients: {coefficients}")

############## CN^3 ##############
# # T_computation(N) = C * N^3, so C = T_computation(N) / N^3
# C = computation_time / (N_values ** 3)
# #The average of these C values is the estimated constant
# C = np.mean(C)
# print(f"Estimated C: {C}")
# N_predict = np.linspace(16, 1024, 100)
# computation_time_predict = C * (N_predict ** 3)


# Plot actual data and the cubit fit
plt.scatter(N_values, computation_time, color='red', label="Actual Time")
# plt.plot(N_predict, computation_time_predict, label=f"Fitted: C = {C:.6f}", color='blue')
plt.plot(N_predict, computation_time_predict, label=f"Fitted Time", color='blue')

plt.xlabel('N')
# unit_label = {
#     unit.NS: "Computation Time (ns)",
#     unit.US: "Computation Time (μs)",
#     unit.MS: "Computation Time (ms)"
# }[UNIT]
plt.ylabel("Computation Time(ms)")
plt.legend()
plt.grid(True)
plt.savefig('./relationship.png')

