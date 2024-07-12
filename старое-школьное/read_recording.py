from csv import reader
import matplotlib.pyplot as plt
import numpy as np
from time import time


class DataReader:
    def __init__(self, filename):
        self.file = open(filename, mode='r', encoding='utf-8')
        self.reader = reader(self.file, delimiter=',')
        self.global_heading = next(self.reader)
        self.headings = next(self.reader)

    def get_headings(self):
        return self.headings

    def close_file(self):
        self.file.close()

    def get_global_heading(self):
        return self.global_heading

    def get_reader(self):
        return self.reader

    def read_from_to(self, from_time, to_time, sensors_indexes,
                     show_plot=False, return_numpy_array=True):
        sensor_data = []
        for i in range(len(sensors_indexes)):
            sensor_data.append([])
        timestamps = []
        # get first data line
        row = next(self.reader)
        start_timestamp, stop_timestamp, *sensors, cq = float(row[0]), float(row[1]), *row[2:17], row[17]
        for row in self.reader:
            relative_time = stop_timestamp - start_timestamp
            if relative_time < start_timestamp:
                continue
            timestamps.append(relative_time)
            for value_index in range(len(sensors_indexes)):
                sensor_data[value_index].append(float(row[sensors_indexes[value_index]]))
            if relative_time >= to_time:
                break
        sensor_data = np.array(sensor_data)
        timestamps = np.array(timestamps)
        if show_plot:
            plt.figure('CSV data plot')
            for plt_data in sensor_data:
                plt.plot(timestamps, plt_data)
            plt.grid(True)
            plt.show()
        return timestamps, sensor_data

    def get_spectr(self, sensor_data, show_plot=False):
        power, freqs = plt.psd(sensor_data)
        power = 10 * np.log10(power[:80])
        freqs = freqs[:80] * 64
        if show_plot:
            plt.grid(True)
            plt.show()
        return freqs, power


INTERPOLATED_CHN = 2
AF_3 = 3
F_7 = 4
F_3 = 5
FC_5 = 6
T_7 = 7
P_7 = 8
O_1 = 9
O_2 = 10
P_8 = 11
T_8 = 12
FC_6 = 13
F_4 = 14
F_8 = 15
AF_4 = 16
RAW_CQ = 17

#sensors=[AF_3,F_7, F_3, FC_5, T_7, P_7, O_1, O_2, P_8,T_8, FC_6, F_4, F_8, AF_4]
interface = DataReader(r'C:\Users\user\Downloads\main_record.csv')
try:
    print(interface.get_global_heading())
    print(interface.get_headings())
    timestamps, sensor_data = interface.read_from_to(20., 40., sensors, show_plot=True)
    print(*sensor_data[0])
    plt.figure('EEG signal')
    plt.plot(timestamps, sensor_data[1])
    plt.figure('spectrograph')

    freqs, power = interface.get_spectr(sensor_data[1], show_plot=True)

finally:
    interface.close_file()
