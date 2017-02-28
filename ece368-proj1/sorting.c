#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h> 
#include <sys/resource.h> 
#include <time.h> 

long *Load_File(char *Filename, int *Size){
	FILE *fptr = fopen(Filename,"r");
	char line[100];
	long number;
	char *ptr_no_use;
	int maxlen;
	long *array = NULL;
	int count = 0;
	if (fptr == NULL){ printf("Cannot open %s\n", Filename);	}
	else{
		fgets(line,sizeof(line),fptr);
		maxlen = strtol(line,&ptr_no_use,10);
		array = malloc(sizeof(long) * maxlen);
		while (fgets(line,sizeof(line),fptr) != NULL){
			number = strtol(line,&ptr_no_use,10);
			array[count] = number; 
			count++;
			if(feof(fptr)){ break;}
		}
	}
	*Size = count;
	return array;
}

int Save_File(char *Filename, long *Array, int Size){
	FILE *fptr = fopen(Filename,"w");
	if(fptr == NULL) {return EXIT_FAILURE; }
	int i;
	char * str = malloc(sizeof(char) * 200);
	sprintf(str,"%d",Size);
	fputs(str,fptr);
	fputs("\n",fptr);
	free(str);
	for(i = 0; i < Size; i++){
		char * str = malloc(sizeof(char) * 200);
		sprintf(str,"%ld",Array[i]);
		fputs(str,fptr);
		fputs("\n",fptr);
		free(str);
	}
	fclose(fptr);
	return i;
}


int *gen_sq1(int N, int *sq_size_final){
    int sq_size = 10;
	int *res = malloc(sizeof(int) * sq_size);
	res[0] = 1;
	int p2 = 0;
	int p3 = 0;
	int u2 = 0;
	int u3 = 0;
	int i = 1;
	int first = 0;
	int last;
	int middle;
	while(res[i-1] < N){
		first = sq_size;
		sq_size *= 2;
		res = realloc(res,sizeof(int) * sq_size);
		for(; i < sq_size; i++){
			if(res[p2] * 2 == res[i-1]){
				p2 += 1;
			}
			if(res[p3] * 3 == res[i-1]){
				p3 += 1;
			}
			u2 = res[p2] * 2;
			u3 = res[p3] * 3;
			if(u2 < u3){
				p2 += 1;
				res[i] = u2;
			}
			else{
				p3 += 1;
				res[i] = u3;
			}
		}
	}
	if(first == 10){ first = 0;}  //special case;
	last = sq_size - 1;
	middle = (first+last) / 2;
	while(res[middle] > N){
		last = middle - 1;
		middle = (first + last) / 2;
	}
	while(res[last] > N){
		last--;                //find the smallest K.
	}
	*sq_size_final = last+1;
	int *sq1 = malloc(sizeof(int) * (last+1));
	while(last >=0 ){
		sq1[last] = res[last];
		last--;
	}
	free(res);
	return sq1;
}


void Save_Seq1(char * Filename, int N){
	int sq_size;
	int *sq = gen_sq1(N,&sq_size);
	int i;
	FILE *fptr = fopen(Filename,"w");
	if(fptr == NULL) {return; }
	for(i = 0; i < sq_size; i++){
		char * str = malloc(sizeof(char) * 200);
		sprintf(str,"%d",sq[i]);
		fputs(str,fptr);
		fputs("\n",fptr);
		free(str);
	}
	free(sq);
	fclose(fptr);
}

int *gen_sq2(int N, int *sq_size_final){
	int count = 0;
	int i;
	int size = N;
	while(size != 0){
		if(size == 9 ||size == 10){
			size = 11;
		}
		size = size / 1.3;
		count++;
	}
	*sq_size_final = --count;   //delete the final zero
	int *res = malloc(sizeof(int) * count);
	res[0] = N / 1.3;
	for(i = 1; i < count; i++){
		if(res[i-1] == 9 || res[i-1] == 10){
			res[i-1] = 11;
		}
		res[i] = res[i-1] / 1.3;
	}
	return res;
}


void Save_Seq2(char * Filename, int N){
	int sq_size;
	int *sq = gen_sq2(N,&sq_size);
	int i;
	FILE *fptr = fopen(Filename,"w");
	if(fptr == NULL) {return; }
	for(i = 0; i < sq_size; i++){
		char * str = malloc(sizeof(char) * 200);
		sprintf(str,"%d",sq[i]);
		fputs(str,fptr);
		fputs("\n",fptr);
		free(str);
	}
	free(sq);
	fclose(fptr);
}

void Shell_Insertion_Sort(long *Array, int Size, double *NComp, double *NMove){
	int sq_size;
	int i;
	int j;
	int k;
	long temp;
	double ncomp = 0;
	double nmove = 0;
	int *sq = gen_sq1(Size, &sq_size);
	sq_size -= 1;
	for(;sq_size >= 0; sq_size--){
		k = sq[sq_size];
		for(j = k; j < Size; j++){
			temp = Array[j];
			nmove++;
			i = j;
			while(i >= k){
				ncomp++;
				if(Array[i-k] > temp){
				Array[i] = Array[i-k];
				i = i - k;
				nmove++;
				}
				else{break;}
			}
			Array[i] = temp;
			nmove++;
		}
	} 
	*NComp = ncomp;
	*NMove = nmove;
}

void Improved_Bubble_Sort(long *Array, int Size, double *NComp, double *NMove){
	int sq_size;
	int i;
	int j;
	int k;
	int k_loop;
	long temp;
	double ncomp = 0;
	double nmove = 0;
	int *sq = gen_sq2(Size, &sq_size);
	for(k_loop = 0; k_loop < sq_size; k_loop++){
		k = sq[k_loop];
		for(i = k; i < Size; i++){
			ncomp++;
			if(Array[i] < Array[i-k]){
				temp = Array[i];
				Array[i] = Array[i-k];
				Array[i-k] = temp;
				nmove += 3;
				j = i-k;
				while(j-k >= 0){
					ncomp++;
					if(Array[j] < Array[j-k]){
						temp = Array[j];
						Array[j] = Array[j-k];
						Array[j-k] = temp;
						nmove += 3;
					}
					else{break;}
				}
			}
		}

	} 
	*NComp = ncomp;
	*NMove = nmove;
}

int main(){
	int size = 1000000;
	Save_Seq1("sq1.txt",size);
	Save_Seq2("sq2.txt",size);
	return 0;
}





