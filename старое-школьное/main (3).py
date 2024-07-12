n = int(input())
s=['']# i= 1,2
ai=0
si=''
a=[]
a.append(0)# i=0
a.append(0)# i=1
#a.append(1)# i=2
for i in range(2, n+1):
    if i%3==0:
        if int( a [i-1]) <int( a[i//3]):
            ai=1 + int(a[i-1])
            si=s[i-1-1]+'1'
        else:
            ai=1 + int(a[i//3])
            si=s[i//3-1]+'3'
    elif i%2==0:
        if int(a [i-1])<int( a[i//2]):
            ai=1 + int( a[i-1])
            si=s[i-1-1]+'1'
        else:
            ai=1 + int( a[i//2])
            si=s[i//2 -1]+'2'
    else:
        ai = 1+ int( a[i-1])
        si=s[i-1-1]+'1'
    a.append(si)
    s.append(si)
print( s[n-1][::])