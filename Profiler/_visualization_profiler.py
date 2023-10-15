from matplotlib.ticker import FuncFormatter
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.widgets import Button

def y_axis_formatter(value, _):
    return f'{int(value):,}'

animation_running = True
# Добавить изменение типов данных
def update_plot(i, ax):
    if animation_running:
        data = pd.read_csv("_profiler.csv")
        if len(data) > 0:
            data['time'] = pd.to_datetime(data['time'], unit='s')

            ax.clear()
            ax.plot(data['time'], data['RAM'], linestyle='-')

            ax.set_xlabel('Время')
            ax.set_ylabel('RAM (в байтах)')
            ax.set_title('Потребление RAM')
            ax.grid()
            ax.yaxis.set_major_formatter(FuncFormatter(y_axis_formatter))
            ax.tick_params(axis='x', rotation=45)

def main():
    fig, ax = plt.subplots(figsize=(10, 6))

    def stop_animation(event):
        global animation_running
        animation_running = False

    def start_animation(event):
        global animation_running
        animation_running = True

    axstop = plt.axes([0.85, 0.9, 0.05, 0.03])
    stop_button = Button(axstop, 'Stop')
    stop_button.on_clicked(stop_animation)

    axstart = plt.axes([0.78, 0.9, 0.05, 0.03])
    start_button = Button(axstart, 'Start')
    start_button.on_clicked(start_animation)

    def on_close(event):
        plt.close()
        exit(0)

    fig.canvas.mpl_connect('close_event', on_close)
    ani = FuncAnimation(fig, update_plot, fargs=(ax,), interval=1000, save_count=1)

    plt.show()

if __name__ == "__main__":
    main()

