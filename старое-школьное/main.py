n = int(input())
s=''
while n!=1:
    if n==3:
        n=n//3
        s=s+'3'
    elif n%2==0:
        n=n//2
        s=s+'2'
    else:
        n=n-1
        s=s+'1'
print(s[::-1])
