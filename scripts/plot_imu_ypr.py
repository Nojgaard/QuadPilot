from serial_plot import SerialPlot
import matplotlib.pyplot as plt

def main():
    ypr_plot = SerialPlot("COM3", ["Yaw", "Pitch", "Roll"], maxlen = 200)
    ypr_plot.ax.set(xlim=[0,5], ylim=[-180, 180], xlabel='Time [s]', ylabel='Degrees')
    ypr_plot.ax.legend()

    plt.show()
    ypr_plot.close()

if __name__ == "__main__":
    main()