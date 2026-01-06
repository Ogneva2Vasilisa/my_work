int function(unsigned long a, int n)
{
    int i = 0;
    unsigned long b = a;
    while (a>0)
    {
        a = a/n;
        i = i+1;
    }
    int k = 0;
    while (b>0)
    {
        b = b/10;
        k = k + 1;
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
    printf("Enter num1: \n");
    scanf("%ld", &a);
    int n;
    printf("Enter num2: \n");
    scanf("%d", &n);
    int bl = function(a, n);
    printf("Result: %d", bl);
}
