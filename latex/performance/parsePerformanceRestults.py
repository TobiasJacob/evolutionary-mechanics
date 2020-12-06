#%%
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import tikzplotlib

from matplotlib import cm
import matplotlib.colors
cmap = matplotlib.colors.LinearSegmentedColormap.from_list("", ["green","gold","red"])

#%% Create the test plot
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

#%% Import data of Equation solver
df = pd.read_csv('performanceBridgesFull.txt')
df.columns = df.columns.str.strip()
df.columns

# %% Plot equation solver
plt.figure(figsize=(8.27, 5.83))
plt.plot(runTimes.C, runTimes.C, '--', color='gray', label='ideal')
for (N, runTimes) in reversed(list(df.groupby('N'))):
    plt.plot(runTimes.C, runTimes.Time.iloc[0] / runTimes.Time, color=cmap((1 - N / 200) ** 3), label='$N = ' + str(N) + '$')
plt.xlabel("Core count")
plt.ylabel("Speedup")
plt.legend(loc='upper left')

# plt.grid(True)

tikzplotlib.save("../report/graphs/mechanicsSolverResults.tex", axis_width="\\textwidth", axis_height="0.5\\textwidth")
# %% Create latex table

Ns = df.N.unique()
Ns
Cs = df.C.unique()
Cs

# %%
executionSpeedTable = pd.DataFrame(index=['Equations'] + list(Cs), columns=Ns, dtype=int)

for (i, (N, C, Time, EquationSize)) in df.iterrows():
    executionSpeedTable.loc[C, N] = Time / 1000
    executionSpeedTable.loc['Equations', N] = EquationSize

executionSpeedTable.index = ['Equations'] + [f"$C = {C}$" for C in Cs]
executionSpeedTable.columns = [f"$N = {N}$" for N in Ns]

latex = executionSpeedTable.to_latex(
            escape = False,
            float_format="%.0f"
            )

latex_list = latex.splitlines()
latex_list.insert(5, '\\midrule')
latex = '\n'.join(latex_list)
with open('../report/tables/executionSpeedTable.tex', 'w') as f:
    f.write(latex)
executionSpeedTable
# %%
