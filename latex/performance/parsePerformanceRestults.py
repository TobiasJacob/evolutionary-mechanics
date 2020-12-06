#%%
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import tikzplotlib

t = np.arange(0.0, 2.0, 0.1)
s = np.sin(2 * np.pi * t)
s2 = np.cos(2 * np.pi * t)
plt.plot(t, s, "o-", lw=4.1)
plt.plot(t, s2, "o-", lw=4.1)
plt.xlabel("time (s)")
plt.ylabel("Voltage (mV)")
plt.title("Simple plot $\\frac{\\alpha}{2}$")
plt.grid(True)


tikzplotlib.save("../report/graphs/test.tex")

#%%
df = pd.read_csv('performanceBridgesFull.txt')
df.columns = df.columns.str.strip()
df.columns
# %%
plt.figure(figsize=(8.27, 5.83))
for (N, runTimes) in df.groupby('N'):
    plt.plot(runTimes.C, runTimes.Time.max() / runTimes.Time, label='$N = ' + str(N) + '$')
plt.plot(runTimes.C, runTimes.C, '--', label='ideal')
plt.xlabel("Core count")
plt.ylabel("Speedup")
plt.legend(loc='upper left')

# plt.title("Simple plot $\\frac{\\alpha}{2}$")
plt.grid(True)

tikzplotlib.save("../report/graphs/mechanicsSolverResults.tex", axis_width="\\textwidth", axis_height="0.618\\textwidth")
# %%
