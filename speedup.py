# Data from the user
import pandas as pd

data = [
    ["H", 0.0250114, 0.00884671, 0.0197088, 0.0135897, 0.382765, 0.133179],
    ["S", 0.0470269, 0.010485, 0.0195339, 0.0192594, 0.316794, 0.130987],
    ["F", 1.23299, 0.17562, 0.512161, 0.0829778, 22.8921, 1.21],
    ["I", 0.205097, 0.046731, 0.060855, 0.0556747, 1.92002, 0.398969],
    ["W", 1.44896, 0.675358, 0.717722, 0.68062, 28.4636, 3.89],
    ["OI", 1.95402, 0.804647, 0.943568, 0.528624, 18.9123, 5.40631],
    ["A", 3.87561, 0.820683, 0.205726, 0.184983, 16.2168, 6.25],
    ["DL", 12.0184, 3.37199, 3.09039, 2.79892, 158.813, 18.2291],
    ["FG", 9.83388, 2.58772, 2.71322, 2.47104, 63.2159, 21.3831],
    ["SO", 80.918, 10.6393, 7.08493, 6.3429, 380.711, 45.763],
    ["HW1", 474.287, 47.7908, 41.1798, 20.7488, 948.958, 160.379],
    ["HW2", 2006.5, 120.511, 130.735, 57.3035, 2790.92, 240.177],
]

df = pd.DataFrame(data, columns=["Graph", "llist", "ParList", "ParCore", "ParMix", "CoreIndex", "ParCoreIndex"])

# Calculate speedups
df["speedup_PCIndex_over_ParCore"] = df["ParCoreIndex"] / df["ParCore"]
df["speedup_PCIndex_over_ParList"] = df["ParCoreIndex"] / df["ParList"]
df["speedup_PCIndex_over_ParMix"] = df["ParCoreIndex"] / df["ParMix"]

# Get max, min, avg
results = {
    "ParCoreIndex/ParCore": {
        "max": df["speedup_PCIndex_over_ParCore"].max(),
        "min": df["speedup_PCIndex_over_ParCore"].min(),
        "avg": df["speedup_PCIndex_over_ParCore"].mean(),
    },
    "ParCoreIndex/ParList": {
        "max": df["speedup_PCIndex_over_ParList"].max(),
        "min": df["speedup_PCIndex_over_ParList"].min(),
        "avg": df["speedup_PCIndex_over_ParList"].mean(),
    },
    "ParCoreIndex/ParMix": {
        "max": df["speedup_PCIndex_over_ParMix"].max(),
        "min": df["speedup_PCIndex_over_ParMix"].min(),
        "avg": df["speedup_PCIndex_over_ParMix"].mean(),
    },
}

print(results)
