import math

earth=[[]]
eert=0.017
Tert=8766
aert=149598261
bert=math.sqrt((1-eert**2)*aert**2)
wert=(2*math.pi)/Tert
uert=23

flag1=0
flag2=0
flag3=0
flag4=0

met=[]
Rfin=[]#итоговый результат
j=0#кол-во астероидов
R=[]#расстояние+время
custast=[]#пользовательский астероид
def metals(costm):
    if (is_num(costm) == False):
        met.append(0)
    else:
        met.append(costm)
def is_num(n):
    try:
        n = float(n)
        return True
    except ValueError:
        return False
aster=[["(16) Психея",0.1367578,43801.944,437235000, 3.09848, 2.27]]
             #1-имя.2-эксцентриситет.3-период.4-наибольшая полуось.5-угол, 6- масса
def pain (j):#функция высчитывающая наименьшее возможное расстояние, и дающаяя итоговый массив с нужными данными
        name = aster[j][0]
        e=aster[j][1]
        T=aster[j][2]
        a=aster[j][3]
        u=aster[j][4]
        m=aster[j][5]
        b=math.sqrt((1-e**2)*a**2)#наименьшая полуось
        w=(2*math.pi)/T#угловая скорость
        fuelcost=5000#стоимость 1 кг топлива
        potr=9000000#потребление топлива за определенное время(кг/ч)
        cos = 60000000# стоимость запуска

        t = 0  # коментарий-время hour
        R = []
        optim=[]#экономическая выгода при двух случаях
        min1=min(met)
        max1=max(met)
        maxec = float(m) / 90 * max1
        minex = float(m) / 90 * min1
        if(T>=Tert):
            Tmax=T
        elif(Tert>T):
            Tmax=Tert

        for i in range(round(Tmax)):
            r = math.sqrt(-b * math.sin(w * t) + bert * math.sin(wert * t) ** 2 + ((a * math.sin(u) * math.cos(w * t)) + (aert * math.sin(uert) * math.cos(wert * t))) ** 2 + ((a * math.cos(u) * math.cos(w * t)) - (aert * math.cos(uert) * math.cos(wert * t))))
            t = t+1
            R.append([r,t])
        R.sort(reverse=True)

        sebstoim=fuelcost*((R[0][1]/potr)+(R[1][1]/potr))+cos#себестоимость
        optim=[minex-sebstoim,maxec-sebstoim]
        tf=(R[0][0]/11.2)*3600
        tf1=(R[1][0]/11.2)*3600
        tpro=tf1+tf+R[0][1]+abs(R[1][1]-R[0][1])#время выполнения проекта
        Rfin.append([name,round(tpro,2),optim[0],optim[1] ])
        for i in range(len(Rfin[j])):#для наименований, думаю можно было бы и сделать лучше, если делать через таблицу
            if (i==1):
                print(Rfin[j][i],"hours")
                print(Rfin[j][i]/24,"days")
            elif(i==2):
                print(Rfin[j][i] , "km")
            else:
                print(Rfin[j][i])

def check(j):#проверка проверенных и общего кол-во астероидов
    while (j!=len(aster)):
            pain(j)
            if(flag1==0 and flag2==0 and flag3==0 and flag4==0 ):
                j = j + 1
print("Please enter the prices of metals($/kg)")

print("aluminum ",end="")
costm=float(input())
metals(costm)

print("copper ",end="")
costm=float(input())
metals(costm)
'''
print("lead ",end="")
costm=float(input())
metals(costm)
print("nickel ",end="")
costm=float(input())
metals(costm)
print("tin ",end="")
costm=float(input())
metals(costm)
print("molybdenum ",end="")
costm=float(input())
metals(costm)
print("cobalt ",end="")
costm=float(input())
metals(costm)
print("gold ",end="")
costm=float(input())
metals(costm)
print("silver ",end="")
costm=float(input())
metals(costm)
print("platinum ",end="")
costm=float(input())
metals(costm)
print("palladium ",end="")
costm=float(input())
metals(costm)
print("rhodium ",end="")
costm=float(input())
metals(costm)
print("iridium ",end="")
costm=float(input())
metals(costm)
print("ruthenium ",end="")
costm=float(input())
metals(costm)
print("osmium ",end="")
costm=float(input())
metals(costm)
'''
check(j)

print("Do you want to add another asteroid?")
ans=input()

if (ans=="1" or ans=="yes" or ans=="Yes" or ans=="YES"):
    print("name"," ")
    custast.append(input())

    while(flag1==0):
        print("eccentricity", " ")
        cuse = str(input())
        n=cuse
        if (is_num(n)==True):
            if(0<=float(cuse) and float(cuse)<1 ):
                custast.append(float(cuse))
                flag1=1
        else:
            flag1=0
    while(flag2==0):
        print("period", " ")
        cust = str(input())
        n=cust
        if (is_num(n) == True):
            custast.append(float(cust))
            flag2 = 1
        else:
            flag2 = 0
    while (flag3 == 0):
        print("the longest half - axis", " ")
        cusa = str(input())
        n=cusa
        if (is_num(n)== True):
            custast.append(float(cusa))
            flag3 = 1
        else:
            flag3 = 0
    while(flag4==0):
        print("inclination", " ")
        cusu = str(input())
        n=cusu
        if (is_num(n) == True):

            flag4 = 1
            while(float(cusu)>360):
                cusu=cusu-360
        else:
            flag4 = 0
        custast.append(float(cusu))
else:
    Rfin.insert(0, ["Name", "Time(hour)", "Max benefit", "Min benefit"])
    for i in range(0, len(Rfin)):
        for i2 in range(0, len(Rfin[i])):
            print(Rfin[i][i2], end=' ')
        print()
    quit()
if(flag1==1 and flag2==1 and flag3==1 and flag4==1 ):
    aster.append(custast)
    j=j+1
    check(j)

