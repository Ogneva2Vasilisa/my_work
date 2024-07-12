n, m = map(int, input().split())
a=[[0]*(m+1) for i in range (n+1)]
s=''
#-----------ввод массива----------------------
for i in range(1,n+1): 
    x=list(map(int, input().split()))
    for j in range(len(x)):
        a[i][j+1] = x[j]
#a= [[int(j) for j in input().split()] for i in range(1,n+1)]

#print( *a)

#-----------------------основной код----------------------------------
#-------------первая часть
b= a
for i in range (1,n+1):
    for j in range (1,m+1):
        
        if b[i-1][j]> b[i][j-1]:
                b[i][j]= b[i-1][j]+a[i][j]
        else:
                b[i][j] = b[i][j-1]+a[i][j]
        #print(*b)        
                
                
#----------вторая часть                
for i in range (n,0,-1):
    for j in range (m,0,-1):
        if b[i-1][j]> b[i][j-1]:
            s=s+' D'
            break
        else:
            s=s+' R'
        
print (b[n][m])
print (s[::-1])
