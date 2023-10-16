from matplotlib.ticker import FuncFormatter
from matplotlib.animation import FuncAnimation
from matplotlib.widgets import Button
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import random

def y_axis_formatter(value, _):
    return f'{value:,}'

animation_running = True
# Добавить изменение типов данных
def update_plot(i, ax1, ax2):
    if animation_running:
        data = pd.read_csv("_profiler.csv")
        tests = pd.read_csv("_tests.csv")

        if len(data) > 5:
            closest_x = data["time"].values[np.abs(data["time"].values - tests["time"].iloc[0]).argmin()]
            closest_y = data.loc[data["time"] == closest_x, 'RAM'].values[0]

            data['time'] = pd.to_datetime(data['time'], unit='s')
            tests['time'] = pd.to_datetime(tests['time'], unit='s')
            ax1.clear()
            ax2.clear()

            ax1.plot(data['time'], data['RAM'] - closest_y, linestyle='-')

            ax1.set_xlabel('Время')
            ax1.set_ylabel('RAM (в байтах)')
            ax1.set_title('Потребление RAM')
            ax1.grid()
            ax1.yaxis.set_major_formatter(FuncFormatter(y_axis_formatter))
            ax1.tick_params(axis='x', rotation=45)
            ax1.locator_params(axis='y', nbins=20)

            ax1.minorticks_on()
            # ax1.grid(which="major", color="#444", lw=1)
            ax1.grid(which="minor", color="#aaa", ls=':')

            for line in tests.itertuples():
                random_color = "#{:02x}{:02x}{:02x}".format(random.randint(0, 255),
                                                            random.randint(0, 255), random.randint(0, 255))
                ax1.axvline(x=line.time, linestyle=':', label=line.labels, color=random_color)
                ax1.text(line.time, (data['RAM'].max() + data['RAM'].mean()) / 2, line.labels, color='b', fontsize=10,
                         rotation=90, ha='right', va='bottom')

            legend = ax1.legend()
            ax2.set_position([0.87, 0.1, 0.12, 0.8])
            ax2.axis('off')
            ax2.legend(handles=legend.legend_handles, ncol=3, title="Tests", loc="best")
            legend.remove()


def main():
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 6), gridspec_kw={'width_ratios': [4, 1]})

    def stop_animation(event):
        global animation_running
        animation_running = False

    def start_animation(event):
        global animation_running
        animation_running = True

    axstop = plt.axes([0.19, 0.9, 0.05, 0.03])
    stop_button = Button(axstop, 'Stop')
    stop_button.on_clicked(stop_animation)

    axstart = plt.axes([0.12, 0.9, 0.05, 0.03])
    start_button = Button(axstart, 'Start')
    start_button.on_clicked(start_animation)

    def on_close(event):
        plt.close()
        exit(0)

    fig.canvas.mpl_connect('close_event', on_close)
    ani = FuncAnimation(fig, update_plot, fargs=(ax1, ax2), interval=1000, save_count=1)
    plt.show()

if __name__ == "__main__":
    main()

