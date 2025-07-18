# -*- coding: utf-8 -*-
"""Копия ynj 13.37. 7.4.22

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1PqeAQ2YQYrjAPjIp9DICyfN7roa_TO0S
"""

'''import numpy as np
from sklearn import linear_model
from sklearn import svm
import keras
import matplotlib.pyplot as plt
from tensorflow.keras.layers import Dense, Conv2D, MaxPooling2D, Flatten, Input, Dropout, BatchNormalization, LeakyReLU

classifiers = [
    svm.SVR(),
    linear_model.SGDRegressor(),
    linear_model.BayesianRidge(),
    linear_model.LassoLars(),
    linear_model.ARDRegression(),
    linear_model.PassiveAggressiveRegressor(),
    linear_model.TheilSenRegressor(),
    linear_model.LinearRegression()]

trainingData    = np.array([ [2.3, 4.3, 2.5],  [1.3, 5.2, 5.2],  [3.3, 2.9, 0.8],  [3.1, 4.3, 4.0]  ])
trainingScores  = np.array( [3.4, 7.5, 4.5, 1.6] )
predictionData  = np.array([ [2.5, 2.4, 2.7],  [2.7, 3.2, 1.2] ])

for item in classifiers:
    print(item)
    clf = item
    clf.fit(trainingData, trainingScores)
    print(clf.predict(predictionData),'\n')
  '''

pip list

import numpy as np
import pandas as pd
import csv
import matplotlib.pyplot as plt
from tensorflow.keras.layers import Dense, Conv2D, MaxPooling2D, Flatten, Input, Dropout, BatchNormalization, LeakyReLU
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression

x,y=[], []
with open('train.csv') as file:
  data = list(csv.reader(file, delimiter =','))[1:]
  i=0
  for row in data:
    try:
      if int(row[1])<90000: row[1]=str((int(data[i-1][1])+int(data[i+1][1]))//2)
    except: row[1]=90306
    i=i+1
  for row in data:
    #день месяц часы
    s=[int(row[0].split()[0][:2]), int(row[0].split()[0][3:5]), int(row[0].split()[0][6:]), int(row[0].split()[1][:2])]
    x.append(s)
    #значение
    y.append(int(row[1]))
x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.2)
print(len(data))
print(x)
print(y)
print(len(y[1095:]))

a=[]
for n in range(24):
  a.append([])
  for j in range(4):  
    x1,y1,y2,x2=[], [], [], []
    if j==3:
      for i in range(8760*j,len(data)):
        if i%(24)==n:
          if len(y2)==366:break
          y2.append(y[i])
          x2.append(i)
          #print(x[i],end=' ')
    elif j>0:
      for i in range(8760*j,8760*(j+1)):
        if i%(24)==n:
          if len(y2)==365:break
          y2.append(y[i])
          x2.append(i)
         # print(i, x[i],end=' ')
    else:
      for i in range(0,8760):
        if i%(24)==n:
          if len(y2)==365:break
          y2.append(y[i])
          x2.append(i)
          #print(i, x[i],end=' ')
    print(len(y2),n,j)

    """plt.figure(figsize=(10, 5))
    plt.title(str([n,j]))
    plt.plot(x2,y2)
    plt.show()"""

    a[n].append(y2)
#print(a[0])
print(len(a),len(a[0]),len(a[0][3]))
for i in range (24):
  print(a[i][3][59], end=' ')
  a[i][3].pop(59)
print()
print(len(a[0][3]))

print(len(a),len(a[0]),len(a[0][0]),a[0][3][0])

#@title добавляем в массив данные с коэфицентом
p=[]
for i in (0,1,2):
  c=[]
  for n in range(24):
    c.append([])
    for d in range(365):
        #print(i+1,i)
        #print(len(a[n][i+1]))
        #print(a[n][i][d])
        c[n].append(a[n][i+1][d]-a[n][i][d])
    #print(c[n],len(c[n]))
  p.append(c)
print(p)

print(len(p),len(p[0]),len(p[0][0]),p[2][0][0])

#@title усредняем коэфицент по годам(или применяем регрессию)

sr=[]
for n in range(24):
  sr.append([])
  for d in range(365):
      #print(i+1,i)
      #print(len(a[n][i+1]))
      #print(p[0][i][d])
      sr[n].append((p[0][n][d]+p[1][n][d]+p[2][n][d])/3)
  #print(c[n],len(c[n]))

print(len(sr),len(sr[0]),sr[4][0])

for n in range(24):
  a[n].append([0for i in range(181)])
  for d in range(181):
    #print(n,d)
    a[n][4][d]=a[n][3][d]+sr[n][d]
print(len(a[n]))
print(a[0][4][0])

#@title перекладываем данные в нужном формате
otv=[]
for d in range(181):
  for n in range(24):
    otv.append(a[n][4][d])
print(len(otv),otv)

x1=[i for i in range(len(y))]
x2=[len(y)+i for i in range(len(otv))]
plt.figure(figsize=(20, 5))
plt.plot(x1,y)
plt.plot(x2,otv)

my_file = open("OTVET.txt", "w")
for i in range (len(otv)):
  my_file.write(str(otv[i])+'\n')
my_file.close()

"""/
/
/
///

///
/

/
///
/
/
/
/
/
/

"""

Model = LinearRegression()
Model.fit(x_train,y_train)

y_test1 = Model.predict(x_test)
# show the inputs and predicted outputs
for i in range(len(x_test)):
  print(y_test[i],end=' ')
  print("X=%s, Predicted=%s" % (x_test[i], y_test1[i]) )

x_t=[i for i in range(0,len(x_test))]
plt.figure(figsize=(60, 40))
plt.plot(x_t, y_test, label='real')
plt.plot(x_t, y_test1, label='train',color='orange')
plt.legend()
plt.show()

from keras.backend import dropout
import keras
model = keras.Sequential([
    Dense(16, input_dim=4),
    Dropout(0.5),
    Dense(16, activation='relu'),
    LeakyReLU(),
    Dense(1,  activation='softmax')

])

model.summary()

model.compile(optimizer='adam', 
              loss='categorical_crossentropy',
              metrics=['accuracy'])

model.fit(x, y, epochs = 10, batch_size = 60, shuffle=True)