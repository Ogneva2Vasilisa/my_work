int Prime(int num)
{
  int c = 0;
  for(int i = 2; i*i<=num; ++i)
  {
    if(num%i==0)
    {
      return(0);
      c = c + 1;
      break;
    }
  }
  if(c==0)
  {
    return(1);
  }
}

int main(void)
{
  int a;
  printf("Input a ");
  scanf("%d", &a);
  double k = sqrt(a);
  for(int i = 2; i<k; ++i)
  {
    int x = Prime(i);
    if((x==1) && (a%i==0))
    {
      printf("%d, ", i);
    }
  }
}
