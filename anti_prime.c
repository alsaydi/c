/**
 12 Ramadan 1431
 08/22/2010
 ***/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <memory.h>
int init_array(int array[],int n,int m,int d);
int find_anti_prime_sequence(int array[],int i,int candidate_to_not_use,int n,int m,int d,int length);
int find_next_solution(int array[],int index,int used,int n,int m,int d,int length);
int is_anti_prime(int array[],int from,int to,int d);
int is_prime(int n);
void dump_array(const char * arrayname,int * array, int length); 
//int * numbers_ptr = NULL;
int main (int argc, const char * argv[]) {
    int i = 0;
	int n = 1;
	int m = 1000;
	int d = 10;
	int length = m - n  + 1;
	int result = 0;
	int * array = NULL;
	int temp = 0;
	//printf("enter n, m and d (0 0 0 to quit:)");
	scanf("%d%d%d",&n, &m, &d);
	while (n>0) {
		length = m - n +1;
		result = 0;
		array = (int *) malloc(sizeof(int)*length);
		temp = n;
		for (i=0; i< length; i++) {
			array[i] = temp++;
		}
		for (i=0; i< length; i++) {
			array[i] = 0;
		}
		array[0] = n;
		result = find_anti_prime_sequence(array, 0, 0, n, m, d, length);
		if (result) {
			fprintf(stdout,"%d", array[0]);
			for (i=1; i< length; i++) {
					fprintf(stdout,",%d" , array[i]);
				}
				fprintf(stdout,"\n");
			}
		else{
			fprintf(stdout,"No anti-prime sequence exists.\n");
		}		
		fflush(stdout);
		fflush(stderr);		
		free(array);
		n = 0;m=0;d=0;
		//printf("enter n, m and d (0 0 0 to quit): ");
		scanf("%d%d%d",&n, &m, &d);	
	}
    return 0;
}
int init_array(int array[],int n,int m,int d)
{
	int i=0;
	int candidate = 0;
	array[0] = n;
	
	for (i=1; i<d; i++) {
		candidate = find_next_solution(array, i-1, 0, n, m, i+1,(m-n)+1);
		if (candidate > 0) {
			array[i] = candidate;
		}
		else {
			return 1;/* break or return ? we probably should and try all values of n for the first case ?*/
		}
		
	}
	return 1;
}
/***
 * This is the recursive backtracking algorithm and the iterative one too.
 *
 *******/
int find_anti_prime_sequence(int array[],int i,int candidate_to_not_use,int n,int m,int d,int length)
{
	int candidate = 0;
	if (i<0) {
		return 0;
	}
	if (i>=length-1 /* && candidate_to_not_use == 0 */) {
		return 1;
	}
	//dump_array("main array", array, i);
	//candidate = find_next_solution(array, i, candidate_to_not_use, n, m, d,length);
	while(1)
	{
		if(i<0)
			return 0;
		if(i>= length - 1)
			return 1;
		candidate = find_next_solution(array, i, candidate_to_not_use, n, m, d,length);
		if(candidate >0)
		{
			array[++i] = candidate;
			candidate_to_not_use=0;
		}
		else
		{
			
			if(i==0 && array[i] < m)
			{
				array[i]++;
				candidate_to_not_use =0;				
			}
			else
			{
				candidate_to_not_use = array[i];
				i--;
			}
		}		
	}
	/*if (candidate > 0) {
	 array[i+1]=candidate;
	 return find_anti_prime_sequence(array,i+1,0,n,m,d,length);
	 }
	 else
	 {
	 if (i ==0 ) {
	 if (array[i] < m) {
	 array[i]++;
	 return find_anti_prime_sequence(array,i,0,n,m,d,length);
	 }
	 }
	 return find_anti_prime_sequence(array,i-1,array[i],n,m,d,length);
	 }*/
}
int find_next_solution(int array[],int index,int used,int n,int m,int d,int length)
{
	int candidate = 0;
	int i =0;	
	int from = 0;
	int to = index + 1;
	if (index + 1 < d) {
		from = 0;
	}
	else {
		from = index - d + 2;
	}
	if (to >= length) {
		to = length -1;
	}
	if(used>0){
		n=used+1;
	}
	while (n<=m) {
		for (i=0; i<=index; i++) {
			if (array[i]==n /* || n <= used */) {
				break;
			}
		}
		if (i>index)
		{
			array[to]=n;
			//	dump_array("array: ", array, length);
			//	fprintf(stderr, " -- %d %d\n",from  , to); 
			if (is_anti_prime(array, from, to,d)) {
				candidate = n;
				break;
			}
		}
		n++;
	}
	return candidate;
}
int is_anti_prime(int array[],int from,int to,int d)
{
	int is_seq_prime = 0;
	int current_d= 2;
	int i = 0;
	int j = 0;
	int sum = 0;
	while (current_d <= d) {
		for (i=from; i<to; i++) {
			int limit = i+current_d-1;
			if (limit>to) {
				limit = to;
			}
			for (j=i; j<=limit; j++) {
				sum += array[j];
			}
			is_seq_prime =  is_prime(sum);
			if(is_seq_prime)
			{
				break;
			}
			sum = 0;
			
		}
		if (is_seq_prime) {
			break;
		}
		current_d++;
	}
	return !is_seq_prime;
}

int is_prime(int n)
{
	int i = 3;
	int square_root = 0;
	if (n%2 == 0) {
		return 0;
	}
	square_root = sqrt(n);
	i = 3;
	for (i; i<=square_root; i++) {
		if (n%i == 0) {
			return 0;
		}
	}
	return 1;
}
void dump_array(const char * arrayname , int * array,int length)
{
	int i =0;
	fprintf(stderr, "dum_array %s: ",arrayname);
	for (i=0; i< length; i++) {
		fprintf(stderr, "%d " , array[i]);
	}
	//fprintf(stderr, "\n");
}
