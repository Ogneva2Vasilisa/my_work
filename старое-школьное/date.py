import time, calendar, datetime, csv, datetime
from matplotlib import pyplot as plt
import numpy as np
from sklearn.ensemble import RandomForestRegressor as ThatModel
from re import compile, fullmatch


def main():
    x, y = [], []
    with open('train.csv') as file:
        data = np.array(list(csv.reader(file, delimiter=','))[1:])
        i = 0
        for row in data:
            try:
                if int(row[1]) < 90000: row[1] = str((int(data[i - 1][1]) + int(data[i + 1][1])) // 2)
            except:
                row[1] = 90306
            i = i + 1
            s = [int(row[0].split()[0][:2]), int(row[0].split()[0][3:5]), int(row[0].split()[1][:2])]
            x.append(s)
            y.append(int(row[1]))
        file.close()

    print(data.shape)

    pat = compile(r"(\d?\d)\.(\d?\d)\.(\d\d\d\d) (\d\d):00:00")
    converted_data = np.array([list(fullmatch(pat,i).groups())+[j] for i,j in data],dtype=np.int32)

    def train_by_date(dataset):
        model = ThatModel()
        data_array = list()

        for d,m,y,h,x in dataset:
            today = datetime.datetime(year=y,month=m,day=d,hour=h)
            data_array.append([[d,today.weekday(),m, y, h],x])

        model.fit([i[0] for i in data_array],[i[1] for i in data_array])


        return model

    def predict(model):
        start = int(datetime.datetime(year=2009,month=1,day=1,hour=0).timestamp())
        end = int(datetime.datetime(year=2009,month=6,day=30,hour=23).timestamp())
        timeline = list()
        for t in range(start,end+3600, 3600):
            today = datetime.datetime.fromtimestamp(t)
            timeline.append((today.day,today.weekday(),today.month, today.year, today.hour))

        return model.predict(timeline)



    predictions = predict(train_by_date(converted_data))
    # with open("OTVET2.txt", "w") as my_file:
    #     my_file.write('\n'.join(map(str,predictions)))
    #     my_file.close()

    print(predictions)
    plt.figure(figsize=(10, 5))
    dat = converted_data[:,4]
    print(len(predictions))
    print(predictions)
    plt.plot(tuple(range(len(dat))), tuple(map(int,dat)))
    plt.plot(list(range(len(dat),len(dat)+len(predictions))), predictions)

    plt.show()


if __name__ == "__main__":
    main()