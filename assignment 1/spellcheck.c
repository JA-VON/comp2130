#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//620068192

//sequential search 
int search(char *list[],char word[])
{	
	int i;
	for(i=0;i<45427;i++)
	{
		if(strcmp(list[i],word)==0)
			return 1;
	}
	return 0;
}

/*
* function to find a suitable replacement
*/
char * findReplacement(char *list[],char word[])
{
	int n,i,numSugg =0;
	char *suggestions[10];
	char *answer;

	printf("The word %s is not found in the dictionary\n",word);
	
 	n = sizeof(word)-1;
	while(n>0)
	{
		char repl[n];
		for(i=0;i<n;i++)
		{
			repl[i] = word[i];
		}
		repl[n] = '\0';
		
		for(i=0;i<45427;i++)
		{
			
			if(numSugg==10)
				break;
			if(strstr(list[i],repl)!=NULL)
			{
				
				suggestions[numSugg] = malloc(strlen(list[i])+1);
				printf("%d - %s\n",numSugg+1,list[i]);
				strcpy(suggestions[numSugg],list[i]);
				numSugg++;
				
			}
			
		}
		
		n--;
	}
	int key;
	printf("Press 0 to use the original word or choose replacement based on the preceding number\n");
	scanf("%d",&key);
	if(key == 0)
		answer = word;
	else
		answer = suggestions[key-1];
	return answer;
	
}

int main(int argc, char *argv[])
{
	char *words[45427];
	char text[80];
	FILE *fpt,*fpt2;
	int i =0;

	if (argc!=3)//Print error if incorrect arguments given
	{
		printf("Incorrect number of inputs.\n");
		exit(1);
	}
	
	fpt = fopen("linux.words","r");
	while(fscanf(fpt,"%s",text)!=EOF)
	{
		words[i] = malloc(strlen(text)+1);	
		strcpy(words[i],text);
		i++;
	}

	fpt = fopen(argv[1],"r");  //opens files for reading and writing
	fpt2 = fopen(argv[2],"w");

	while(fscanf(fpt,"%s",text)!=EOF)
	{
		if(search(words,text))
		{
			fprintf(fpt2,"%s ",text);
		}
		else
		{
			char * answer;
			answer = findReplacement(words,text);
			fprintf(fpt2,"%s ",answer);
		}
	}
}
