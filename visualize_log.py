import sys
import numpy as np
import matplotlib.pyplot as plt


def plot_data(xs, ys, deg=1):
    coeff = np.array(np.polyfit(xs, ys, deg)[::-1])

    mx, mn = max(xs), min(xs)
    rng = mx - mn
    xline = np.arange(mn, mx + rng/11, rng/10)
    xmatrix = np.tile(xline, (coeff.shape[0], 1)).transpose()
    powers = np.arange(coeff.shape[0])

    xmatrix = np.power(xmatrix, powers)
    yline = np.dot(xmatrix, coeff)

    fig, ax = plt.subplots()
    ax.plot(xs, ys, 'o')
    ax.plot(xline, yline, "r-")
    ax.set_title('Relationship between steps and time')
    plt.xlabel("Steps")
    plt.ylabel("Time (s)")
    plt.show()


# get data from file
filename = "logs/logs.txt" if len(sys.argv) == 1 else sys.argv[1]
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

plot_data(data_steps, data_time)

# plt.hist(sorted(data_steps))
# plt.show()
