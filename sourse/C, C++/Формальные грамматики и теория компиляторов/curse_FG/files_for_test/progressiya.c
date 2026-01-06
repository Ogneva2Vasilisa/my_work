void summa(char *str1, char *str2, char *g)
{
    int x;
    int y;

    if (strlen(str1)>strlen(str2))
        {
            x = strlen(str1);
            y = strlen(str2);
            int b[x+1];
            int j = 0;
            int t = 0;
            for (j=0; j<x+1; j++)
                {
                    if (j==0)
                    {
                        b[j] = 0;
                        t = 1;
                    }
                    else
                    {b[j] = str1[j-t] - 48;}
                    //printf("%d", b[j]);
                }
    
            int a[x+1];
            j = 0;
            for (j=0; j<x+1; j++)
                {   if (j<(x-y+1))
                        a[j] = 0;
                    else
                        a[j] = str2[j-x-1+y] - 48;
                    //printf("%d", a[j]);
                }

            int length;
            int i;
            length = x + 1;
            
            for (int ix = length-1; ix>-1; ix--)
                        {
                        int p = b[ix];
                        int q = a[ix];
                        //printf("p - %d q- %d", p, q);
                        if (p+q>9)
                        {
                            int k = (p+q)%10;
                            b[ix] = k;
                            b[ix - 1] = (b[ix-1]) + 1;
                        }
                        else
                        {
                            b[ix] += a[ix];
                            b[ix - 1] += (b[ix] / 10);
                            b[ix] %= 10;
                        }
                        }
            
            int r = 0;
            for (i = 0; i<x+1; i++)
                {
                    if (i==0)
                    {
                        if (b[i]!=0)
                            {
                                int q = b[i];
                                g[r] = q+48;
                                r = r + 1;
                            }
                    }
                    else
                        {
                            int q = b[i];
                            g[r] = q+48;
                            r = r + 1;
                        }
                    //printf("%d \n", b[i]);
                }
            g[r] = '\0';
        }
    else
        {
            x = strlen(str2);
            y = strlen(str1);
            int b[x+1];
            int j = 0;
            int t = 0;
            for (j=0; j<x+1; j++)
                {
                    if (j==0)
                    {
                        b[j] = 0;
                        t = 1;
                    }
                    else
                    {b[j] = str2[j-t] - 48;}
                    //printf("%d", b[j]);
                }
    
            int a[x+1];
            j = 0;
            for (j=0; j<x+1; j++)
                {   if (j<(x+1-y))
                        a[j] = 0;
                    else
                        a[j] = str1[j-x-1+y] - 48;
                    //printf("%d", a[j]);
                }

            int length;
            int i;
            length = x + 1;
            
            for (int ix = length-1; ix>-1; ix--)
                        {
                        int p = b[ix];
                        int q = a[ix];
                        //printf("p - %d q- %d", p, q);
                        if (p+q>9)
                        {
                            int k = (p+q)%10;
                            b[ix] = k;
                            b[ix - 1] = (b[ix-1]) + 1;
                        }
                        else
                        {
                            b[ix] += a[ix];
                            b[ix - 1] += (b[ix] / 10);
                            b[ix] %= 10;
                        }
                        }
            
            int r = 0;
            for (i = 0; i<x+1; i++)
                {
                    if (i==0)
                    {
                        if (b[i]!=0)
                            {
                                int q = b[i];
                                g[r] = q+48;
                                r = r + 1;
                            }
                    }
                    else
                        {
                            int q = b[i];
                            g[r] = q+48;
                            r = r + 1;
                        }
                    //printf("%d \n", b[i]);
                }
            g[r] = '\0';
        }
}

void div2(char *str6, char *str7)
{
    int x = strlen(str6);
    int a[x];
    for (int j=0; j<x; j++)
                {
                    a[j] = str6[j] - 48;
                }
    int size = x;
    int c[size];
    for (int t = 0; t<size; t++)
    {
        c[t]=0;
        //printf("%d", c[j]);
    }
    int i = 0;
    int u = 0;
    int z = 0;
    int k = 0;
    int j = 0;
    if (a[0]<2)
    {
        z = (10*(a[0])) + a[1];
        j = z/2;
        if (z%2!=0)
            {k = 1;}
        c[0] = j;
        u = u+1;
        i = i + 2;
    }
    else
    {
        z = a[0];
        j = z/2;
        if (z%2!=0)
            {k = 1;}
        c[0] = j;
        u = u+1;
        i = i + 1;
    }
    while (i<size)
    {
        z = k*10 + a[i];
        j = z/2;
        if (z%2!=0)
            {k = 1;}
        c[u] = j;
        u = u+1;
        i = i + 1;
    }
    for (int y = 0; y<u; y++)
        {
            int q = c[y];
            str7[y] = q+48;
        }
    str7[u] = '\0';
}

int max(int a, int b)
{
  if (a > b) return a;
  else return b;
}

void mult(char *str3, char *str6, char *str8)
{
  int size_b = strlen(str3);
  int size_a = strlen(str6);
  int length = size_a + size_b + 1;
  int c[length];
  for (int i=0; i<length; i++)
    c[i] = 0;
  int maxi = max(size_a, size_b);
  int a[maxi];
  int b[maxi];
  for (int i = 0; i<maxi; i++)
  {
      if (maxi==size_a)
          {
              a[i] = str6[size_a-i-1] - 48;
              if (i<size_b)
              {
                  b[i] = str3[size_b-i-1] - 48;
              }
              else
              {
                  b[i] = 0;
              }
          }
      else
          {
              b[i] = str3[size_b-i-1] - 48;
              if (i<size_a)
              {
                  a[i] = str6[size_a-i-1] - 48;
              }
              else
              {
                  a[i] = 0;
              }
          }
  }
  for (int ix = 0; ix < size_a; ix++)
      for (int jx = 0; jx < size_b; jx++)
          c[ix + jx] += a[ix] * b[jx];

  for (int ix = 0; ix < length; ix++)
  {
      c[ix + 1] +=  c[ix] / 10;
      c[ix] %= 10;
  }
  int p;
  int r = 0;
  int dd[length];
  for (p = 0; p<length; p++)
    {
      dd[p] = c[length-1-p];
    }
  int i;
  for (i = 0; i<length; i++)
                {
                    if ((i==0)||(i==1))
                    {
                        if (dd[i]!=0)
                            {
                              int q = dd[i];
                              str8[r] = q+48;
                              r = r + 1;
                            }
                    }
                    else
                        {
                            int q = dd[i];
                            str8[r] = q+48;
                            r = r + 1;
                        }
                }
  str8[r] = '\0';
  //printf("%s", str8);
}

void subst(char *str2, char *str1, char *str4)
{
    int x = strlen(str2);
    int y = strlen(str1);
    int b[x];
    int j = 0;
    for (j=0; j<x; j++)
        {
            b[j] = str2[j] - 48;
        }

    int a[x];
    j = 0;
    for (j=0; j<x; j++)
        {   if (j<(x-y))
                a[j] = 0;
            else
                a[j] = str1[j-x+y] - 48;
        }

    int length;
    int i;
    length = x + 1;
    
    for (int ix = length-2; ix>-1; ix--)
                {
                int p = b[ix];
                int q = a[ix];
                //printf("p - %d q- %d", p, q);
                if (p-q<0)
                {
                    int k = (10+p-q);
                    b[ix] = k;
                    b[ix - 1] = (b[ix-1]) - 1;
                }
                else
                {
                    b[ix] -= a[ix];
                    b[ix - 1] -= (b[ix] / 10);
                    b[ix] %= 10;
                }
                }

    int r = 0;
    for (i = 0; i<x; i++)
                {
                    if ((i==0)||(i==1))
                    {
                        if (b[i]!=0)
                            {
                              int q = b[i];
                              str4[r] = q+48;
                              r = r + 1;
                            }
                    }
                    else
                        {
                            int q = b[i];
                            str4[r] = q+48;
                            r = r + 1;
                        }
                }
  str4[r] = '\0';
}

void sub(char str1[1001], char str2[1001])
{
    char str3[2001];
    char str4[2001];
    char str5[] = "1";
    char str6[2001];
    char str7[2001];
    char str8[2001];
    summa(str1, str2, str3);
    subst(str2, str1, str4);
    summa(str5, str4, str6);
    mult(str3, str6, str8);
    div2(str8, str7);
    printf("%s", str7);
}

int main(void)
{
    char str1[1001];
    printf("Enter start: ");
    gets(str1);
    char str2[1001];
    printf("Enter end: ");
    gets(str2);
    sub(str1, str2);
}
