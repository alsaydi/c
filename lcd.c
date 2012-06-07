/**
*	AB
*	08/22/2007
*	Display digits in LCD format
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void fill_rect(char ** rect_ptr,int s,int is_last_digit);
void print_rect(char *** rect_ptr,int len, int s);
void turn_off_areas(char ** rect_ptr, int s,int areas[], int how_many);
int areas_to_turn_off(int areas[], char c);
int main(){

	FILE * fptr = fopen("data.txt","r");
	char * _line = (char *) malloc(sizeof(char)*101);
	char * line = _line;
	int s = 0;
	int areas[5]={0,0,0,0,0};
	int areas_len = 0;
	int rows = 0;
	int cols = 0;
	char *** the_ptr = NULL;
	int i = 0;
	int j = 0;
	int line_len = 0;
	int bytes_read = 0;
	if(!fptr)
	{
		fprintf(stderr,"cannot open data.txt file");
		return 1;
	}
	if(!line)return 1;
	while(!feof(fptr) &&  fgets(line,100,fptr))
	{		
		line[100]=0;
		s = atoi(&line[0]);
		rows = 2*s + 3;		
		cols = s + 2;
		line += 2; 
		line_len = strlen(line)-1;
		the_ptr = (char ***)malloc(sizeof(char **)*line_len);
		for(i=0;i<line_len;i++)
		{
			the_ptr[i] = (char **) malloc(sizeof(char*)*rows);
			for(j=0;j<rows;j++)
				the_ptr[i][j] = (char *) malloc(sizeof(char)*cols+1);
		}
		for(i=0;i<line_len-1;i++)
		{
			fill_rect(the_ptr[i],s,0);
			areas_len = areas_to_turn_off(areas,line[i]);
			turn_off_areas(the_ptr[i],s,areas,areas_len);			
		}
		fill_rect(the_ptr[i],s,1);
		areas_len = areas_to_turn_off(areas,line[i]);
		turn_off_areas(the_ptr[i],s,areas,areas_len);			
		print_rect(the_ptr,line_len,s);	

		for(i=0;i<line_len;i++)
		{
			for(j=0;j<rows;j++)
				free(the_ptr[i][j]);
			free(the_ptr[i]);
		}
		free(the_ptr);
		line = _line;
	}	
	fclose(fptr);
	free(_line);
	return 0;
}

void fill_rect(char ** rect_ptr,int s,int is_last_digit)
{
	int rows = 2*s+3;
	int cols = s+2;
	int i=0;
	int j=0;	
	char delim = ' ';
	if(is_last_digit) 
		delim = '\n';
	rect_ptr[0][0] = ' ';
	for(j=1;j<cols-1;j++)
	{
		rect_ptr[0][j] = '-';
	}
	rect_ptr[0][cols-1] = ' ';
	rect_ptr[0][cols] = delim;
	for(i=1;i<rows-1;i++)
	{
		if(i == (int)rows/2)
		{
			rect_ptr[i][0] = ' ';
			for(j=1;j<cols-1;j++)
			{
				rect_ptr[i][j] = '-';
			}
			rect_ptr[i][cols-1] = ' ';
			rect_ptr[i][cols] = delim;
			continue;
		}
		rect_ptr[i][0]='|';
		for(j=1;j<cols-1;j++)
		{
			rect_ptr[i][j]=' ';
		}
		rect_ptr[i][cols-1]='|';
		rect_ptr[i][cols]=delim;
	}
	rect_ptr[i][0] = ' ';
	for(j=1;j<cols-1;j++)
	{
		rect_ptr[i][j] = '-';
	}
	rect_ptr[j][cols-1] = ' ';
	rect_ptr[j][cols] = delim;
	rect_ptr[rows-1][cols-1]=0;
	rect_ptr[rows-1][cols]=0;
}
int areas_to_turn_off(int areas[], char c)
{
	int count = 0;
	switch(c){
		case '1':
			areas[0] = 1;
			areas[1] = 2;
			areas[2] = 4;
			areas[3] = 5;
			areas[4] = 6;
			count = 5;
			break;
		case '2':
			areas[0] = 1;
			areas[1] = 7;
			count = 2;
			break;
		case '3':
			areas[0] = 1;
			areas[1] = 5;
			count = 2;
			break;
		case '4':
			areas[0] = 2;
			areas[1] = 5;
			areas[2] = 6;
			count = 3;
			break;
		case '5':
			areas[0] = 3;
			areas[1] = 5;
			count = 2;
			break;
		case '6':
			areas[0] = 3;
			count = 1;
			break;
		case '7':
			areas[0] = 1;
			areas[1] = 4;
			areas[2] = 5;
			areas[3] = 6;
			count = 4;
			break;
		case '9':
			areas[0] = 5;
			count = 1;
			break;
		case '0':
			areas[0]= 4;
			count =1;
			break;
		default:
			count = 0;
			break;
	}
	return count;
}
void turn_off_areas(char ** rect_ptr, int s,int areas[], int how_many)
{
	int i=0;
	int max_col = s+2-1;
	int max_row = 2*s+3;
	int half_way = max_row /2 ;
	int j=0;
	int k=0;
	
	for(i=0;i<how_many;i++)
	{
		switch(areas[i])
		{
			case 1:
				for(j=1;j<=s;j++)
					rect_ptr[j][0]=' ';
				break;
			case 2:
				for(k=1;k<=s;k++)
					rect_ptr[0][k]=' ';
				break;
			case 3:
				for(j=1;j<=s;j++)
					rect_ptr[j][max_col]=' ';
				break;
			case 4:
				for(k=1;k<=s;k++)
					rect_ptr[half_way][k]= ' ';
				break;
			case 5:
				for(j=1;j<=s;j++)
					rect_ptr[half_way+j][0]=' ';
				break;
			case 6:
				for(k=1;k<=s;k++)
					rect_ptr[max_row-1][k] = ' ';
				break;
			case 7:
				for(j=1;j<=s;j++)
					rect_ptr[half_way+j][max_col]=' ';
				break;
			default:
				break;
		}
	}
}

void print_rect(char *** rect_ptr,int len, int s)
{
	int rows=2*s+3;
	int cols = s+2;
	int i=0;
	int j=0;
	int k=0;
	for(i=0;i<rows;i++){
		for(j=0;j<len;j++)
			for(k=0;k<=cols;k++)			
				printf("%c",rect_ptr[j][i][k]);	//print the current cell of the current matrix in the current row		
	}
	printf("\n");
}
