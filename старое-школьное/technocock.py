import numpy as np
from math import pi, sqrt
from sklearn.linear_model import LinearRegression
m=(12,24,168,672)



#########################
lin1 = LinearRegression()
lin2 = LinearRegression()
#########################

epoch = 10
train_x = np.empty((epoch,84),dtype=np.ndarray)
train_xy = np.empty((epoch,84),dtype=np.ndarray)
train_y = np.empty((epoch,84),dtype=np.ndarray)

for J in range(epoch):
    np.random.seed(J)
    A = np.random.random_sample(5)*[7000,1500,1500,1500,1500]
    B = np.random.randint(50,size=4)
    freq1 = A[1] * np.cos([(t + B[0]/2)*pi / 6 for t in range(168)])
    freq2 = A[2] * np.cos([(t + B[1])*pi / 12 for t in range(168)])
    freq3 = A[3] * np.cos([(t/7 + B[2])*pi / 12 for t in range(168)])
    freq4 = A[4] * np.cos([(t/28 + B[3])*pi / 12 for t in range(168)])
    Yreal = freq1 + freq2 + freq3 + freq4 + A[0]
    cos3 = Yreal[:84]+Yreal[84:]
    train_x[J], train_xy[J], train_y[J] = cos3, freq4[:84], freq4[84:]


lin1.fit(train_x, train_xy)
lin2.fit(train_xy, train_y)


########################################
#Yreal = list(map(int,input().split()))
Yreal = np.empty(168)
for i in range(168):
    Yreal[i] =  int(input())
######################################
cos3pred = Yreal[:84] + Yreal[84:]
pred1 = lin1.predict([cos3pred])[0]
pred2 = lin2.predict([pred1])[0]
pred3 = lin2.predict([pred2])[0]
predsub = np.append(pred1,pred2)
predfinal = np.append(pred3, lin2.predict([pred3])[0])

#print('\n'.join(Yreal-np.append(pred1,pred2)+predfinal))
for i in (Yreal-np.append(pred1,pred2)+predfinal):
    print(i)

