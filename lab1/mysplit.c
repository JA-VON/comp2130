#include <stdio.h>
#include <string.h>

main()
{
	
	char name[50],first[25],last[25];
	printf("What is your name? ");
	scanf("%s",name);

	int i,k;

	for (i=0; i<strlen(name); i++)
	{
		if(i>0 && isupper(name[i]))
			break;
		first[i] = name[i];
	}

	
	for(k=0;k<strlen(name)-i;k++)
	{
		last[k] = name[k+i];
	}

	printf("%s %s\n",first,last);

}
