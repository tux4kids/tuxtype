#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
	char input_files[400];
	char command[1000];
	int f,d,s,j,k,l;
	int i;
	int count=0;
	
	for(l=0;l<=1;l++)
	{
		for(k=0;k<=1;k++)
		{
			for(j=0;j<=1;j++)
			{
				for(s=0;s<=1;s++)
				{
					for(d=0;d<=1;d++)
					{
						for(f=0;f<=1;f++)
						{
							i = 0;
							input_files[0] = '\0';
							strcpy(input_files,"convert ");
							
							if (f==1)
							{
								strcat(input_files,"f.png ");
								i++;
							} 

							if (d==1)
							{
								strcat(input_files,"d.png ");
								i++;
							} 
							if (s==1)
							{
								strcat(input_files,"s.png ");
								i++;
							} 
							if (j==1)
							{
								strcat(input_files,"j.png ");
								i++;
							} 	
							if (k==1)
							{
								strcat(input_files,"k.png ");
								i++;
							} 																											
							if (l==1)
							{
								strcat(input_files,"l.png ");
								i++;
							} 
							
							printf("\nI = %d",i);
							
							sprintf(command,"%s-coalesce %d.png",input_files,count);
							system(command);
							printf("\n%s",command);

							sprintf(command,"cp %d-%d.png %d.png",count,i-1,count);
							system(command);
							
							i=i-1;
							while(i>=0)
							{
								sprintf(command,"rm %d-%d.png",count,i);
								system(command);
								i--;
							}
							count++;
						}
					}
				}
			}
		}
	}
	
	return 0;
}
