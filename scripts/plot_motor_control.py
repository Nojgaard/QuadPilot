from serial_plot import SerialPlot
import matplotlib.pyplot as plt

def main():
    ypr_plot = SerialPlot("COM3", ["M1", "M2", "M3", "M4"], maxlen = 200)
    ypr_plot.ax.set(xlim=[0,5], ylim=[0, 100], xlabel='Time [s]', ylabel='Motor Percentage')
    ypr_plot.ax.legend()

    plt.show()
    ypr_plot.close()

if __name__ == "__main__":
    main()