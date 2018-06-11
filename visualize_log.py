import sys
import numpy as np
import matplotlib.pyplot as plt


def plot_data(xs, ys, deg=1, name="logfile"):
    coeff = np.array(np.polyfit(xs, ys, deg)[::-1])

    mx, mn = max(xs), min(xs)
    rng = mx - mn
    xline = np.arange(mn, mx + rng/11, rng/10)
    xmatrix = np.tile(xline, (coeff.shape[0], 1)).transpose()
    powers = np.arange(coeff.shape[0])

    xmatrix = np.power(xmatrix, powers)
    yline = np.dot(xmatrix, coeff)

    ax.plot(xs, ys, 'o', label="Points " + name)
    ax.plot(xline, yline, '-', label="Polyfit " + name)
    ax.set_title('Relationship between steps and time')

    plt.xlim([0, 30000])
    plt.ylim([0, 1.2])
    plt.xlabel("Steps")
    plt.ylabel("Time (s)")
    plt.legend()


global ax
fig, ax = plt.subplots()

# get data from file
global files
files = ["logs/logs.txt"]

if (len(sys.argv) > 1):
    files = sys.argv[1:]

for filename in files:

    with open(filename) as f:
        read_data = f.read()

    # first line contains info about the run itself
    lines = read_data.split("\n")[1:]
    data_steps = np.array([int(line.split(" ")[0])
                           for line in lines if line != ""])

    data_time = np.array([float(line.split(" ")[1])
                          for line in lines if line != ""])

    data_empty = np.array([float(line.split(" ")[2])
                           for line in lines if line != ""])

    plot_data(data_steps, data_time, name=filename)

plt.show()
# plt.hist(sorted(data_steps))
# plt.show()
