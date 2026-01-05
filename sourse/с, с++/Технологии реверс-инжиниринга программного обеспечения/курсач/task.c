#include <stdio.h>
#include <string.h>

int main()
{
  char giveflag[4]; 
  char word[256];
  strcpy(giveflag, "No.");
  printf("What's your favorite word? ");
  scanf("%s", word);
  if ( !strcmp(giveflag, "Yesss!") )
    printf("Flag is: MyCTF{good_net}\n");
  else
    printf("Good, but i won't give you flag.\n");
}