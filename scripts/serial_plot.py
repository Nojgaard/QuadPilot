from collections import deque
from ctypes import Array
import serial
import matplotlib.pyplot as plt
import matplotlib.animation
from threading import Thread

class SerialPlot:
    def __init__(self, port: str, labels: Array[str], maxlen: int):
        self.serial = serial.Serial(port, 9600)
        self.x_values = deque(maxlen = maxlen)
        self.y_values = [deque(maxlen = maxlen) for _ in labels]

        fig, ax = plt.subplots()
        self.fig = fig
        self.ax = ax
        self.plots = [ax.plot([], [], label=l)[0] for l in labels]
        self.anim = matplotlib.animation.FuncAnimation(fig=fig, func=self._update, interval = 30)
        self.serial.reset_input_buffer()
        self._last_line = ""
        self._finished = False
        self._read_serial_task = Thread(target=self._read_serial)
        self._read_serial_task.start()


    def _read_serial(self):
        while not self._finished:
            self._last_line = self.serial.readline().strip()
    

    # clean up
    def close(self):
        self._finished = True
        self._read_serial_task.join()
        # close serial
        self.serial.flush()
        self.serial.close()


    def _update_xy_values(self):
        #line = self.serial.readline().strip()
        line = self._last_line
        could_parse = True
        try:
            data = [float(val) for val in line.split()]
        except ValueError:
            could_parse = False
        
        if not could_parse or len(data) - 1 != len(self.y_values):
            return
        
        self.x_values.append(data[0])
        for ys, v in zip(self.y_values, data[1:]):
            ys.append(v)

    

    def _update(self, frame_num):
        self._update_xy_values()

        for i in range(len(self.plots)):
            self.plots[i].set_xdata(self.x_values)
            self.plots[i].set_ydata(self.y_values[i])
        if (len(self.x_values) >= 2):
            self.ax.set_xlim(min(self.x_values), max(self.x_values))
        return self.plots