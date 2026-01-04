int function(unsigned long a)
{
    int x[50];
    int i = 0;
    while (a>0)
    {
        x[i] = (a%10);
        a = a/10;
        i = i+1;
    }
    int k = 0;
    for (int j = 0; j<i; j++)
    {
        if (x[j]==x[i-j-1])
        {
            k = k+1;
        }
    }
    if (k==i)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int main(void)
{
    unsigned long a;
    printf("Enter num: \n");
    scanf("%ld", &a);
    int bl = function(a);
    printf("Result: %d", bl) ;
}
