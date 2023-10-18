from matplotlib.ticker import FuncFormatter
from matplotlib.animation import FuncAnimation
from matplotlib.widgets import Button
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import random
import sys


animation_running = True
tests_dict = {"time": [], "test": [], "color": [], "size": []}

def y_axis_formatter(value, _):
    return f"{round(value, 7):,}"

def update_plot(i, _ax1, _ax2, _converter):

    if animation_running:

        data = pd.read_csv(".profiler.csv")
        tests = pd.read_csv(".tests.csv")

        if len(data) > 100 and len(tests) > 1:

            closest_x = data["time"].values[np.abs(data["time"].values - tests["time"].iloc[0]).argmin()]
            closest_y = data.loc[data["time"] == closest_x, "RAM"].values[0]

            _ax1.clear()
            _ax2.clear()

            _ax1.plot(pd.to_datetime(data["time"], unit='s'), (data["RAM"] - closest_y) / _converter[1], linestyle="-")

            _ax1.set_xlabel("Time")
            _ax1.set_ylabel(f"RAM {_converter[0]}")
            _ax1.set_title("RAM usage")
            _ax1.grid()
            _ax1.yaxis.set_major_formatter(FuncFormatter(y_axis_formatter))
            _ax1.tick_params(axis='x', rotation=45)
            _ax1.locator_params(axis='y', nbins=20)

            _ax1.minorticks_on()
            _ax1.grid(which="minor", color="#aaa", ls=':')

            for line in tests.itertuples():
                if pd.to_datetime(line.time, unit='s') not in tests_dict["time"] and line.time != tests["time"].iloc[-1]:

                    tests_dict["time"].append(pd.to_datetime(line.time, unit='s'))
                    tests_dict["test"].append(line.labels)

                    random_color = "#{:02x}{:02x}{:02x}".format(random.randint(0, 255),
                                                                random.randint(0, 255), random.randint(0, 255))
                    tests_dict["color"].append(random_color)

                    filtered_data = data[data["time"] == data.loc[data["time"] < tests["time"].iloc[line.Index - 1]]["time"].max()]
                    max_RAM_i = 0

                    if not filtered_data.empty:
                        last_time_i = filtered_data.index[0] + 1
                        max_RAM_i = data[(data["time"] >= data.iloc[last_time_i]["time"]) & (data["time"] <= line.time)]["RAM"].max() - data.iloc[last_time_i]["RAM"]

                    if np.isnan(max_RAM_i) or max_RAM_i / _converter[1] == 0:
                        tests_dict["size"].append(0)
                    else:
                        tests_dict["size"].append(max_RAM_i / _converter[1])

            if len(tests_dict["time"]) > 0:
                for k in range(0, len(tests_dict["time"])):

                    _ax1.axvline(x=tests_dict["time"][k], linestyle=":", label=tests_dict["test"][k] + ' ' + str(tests_dict["size"][k]) + ' ' + _converter[0], color=tests_dict["color"][k])

                    _ax1.text(tests_dict["time"][k],
                              (data["RAM"].max() + data["RAM"].mean() - 2 * closest_y) / (2 * _converter[1]),
                              tests_dict["test"][k] + ' ' + str(tests_dict["size"][k]) + ' ' + _converter[0],
                              color="b", fontsize=10, rotation=90, ha="right", va="bottom")

            legend = _ax1.legend()
            _ax2.set_position([0.87, 0.1, 0.12, 0.8])
            _ax2.axis("off")
            _ax2.legend(handles=legend.legend_handles, ncol=3, title="Tests", loc="best")
            legend.remove()


def arg_handler(arg):
    if arg == "bytes":
        return ["bytes", 1]
    elif arg == "kb":
        return ["kb", 1024]
    elif arg == "mb":
        return ["mb", 1024*1024]
    elif arg == "gb":
        return ["gb", 1024*1024*1024]

def main(arg):

    converter = arg_handler(arg)
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 9), gridspec_kw={"width_ratios": [2, 1]})

    def stop_animation(event):
        global animation_running
        animation_running = False

    def start_animation(event):
        global animation_running
        animation_running = True

    axstop = plt.axes([0.19, 0.9, 0.05, 0.03])
    stop_button = Button(axstop, "Stop")
    stop_button.on_clicked(stop_animation)

    axstart = plt.axes([0.12, 0.9, 0.05, 0.03])
    start_button = Button(axstart, "Start")
    start_button.on_clicked(start_animation)

    def on_close(event):
        plt.close()
        exit(0)

    fig.canvas.mpl_connect("close_event", on_close)
    ani = FuncAnimation(fig, update_plot, fargs=(ax1, ax2, converter), interval=1000, save_count=1)
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) > 1:
        main(sys.argv[1])
    else:
        print("Please pass the arguments to the script.")
