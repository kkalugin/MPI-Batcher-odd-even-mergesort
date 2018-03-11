#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

#define MAX_NUM 1000.

void swap(double* a, double* b){
		*a = *b + *a;
		*b = *a - *b;
		*a = *a - *b;
}

void compexch(double* a, double* b){	
	if(*b < *a)
		// Swap a and b		
		swap(a, b); 
}

void oddEvenMerge(double* a, int left, int n, int r){
	int m = r*2;
	
	if(m < n){
		oddEvenMerge(a, left, n, m);      // even subsequence
		oddEvenMerge(a, left + r, n, m);    // odd subsequence
		for (int i = left + r; i + r < left + n; i += m)
			compexch(a + i, a + i + r);
	}
	else
		compexch(a + left, a + left + r);
}

void oddEvenMergeSort(double* a, int left, int n){
	if(n > 1){
		int m = n/2;

		oddEvenMergeSort(a, left, m);
		oddEvenMergeSort(a, left + m, m);
		oddEvenMerge(a, left, n, 1);
	}
		
    /** lo is the starting position and
     *  n is the length of the piece to be merged,
     *  r is the distance of the elements to be compared
     */	
}

void oddEvenMergeSortFinal(double* a, int Asize, int size){
	int n;
	for(int i = size / 2; i >= 1; i /= 2){
		n = Asize / i;
		for(int j = 0; j < i; j++)
			oddEvenMerge(a, j * n, n, 1);
	}
	
	
		
    /** lo is the starting position and
     *  n is the length of the piece to be merged,
     *  r is the distance of the elements to be compared
     */	
}


int main(int argc, char *argv[]){
    double* A = NULL;
	double* initA = NULL;
		double* ptrA = NULL;

    int rank, size;
	int Asize = 8192;
	int sizePerProc;
	
	
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	
	sizePerProc = Asize / size;
    if(rank==0){
		initA = new double [Asize];
        ptrA = initA;
		
		srand(time(NULL)); 
        for(int i = 0; i < Asize; i++)
			*ptrA++ = (double)rand() / (double)(RAND_MAX) * MAX_NUM;
		
		/*
		ptrA = initA;
		printf("Start\nMatrix A is:\n");
        for(int i = 0; i < Asize; i++)           
            printf("%.2f\t", *ptrA++);				


		
		/** sorts a piece of length n of the array
		 *  starting at position lo
		 */
		 
	}
	
	A = new double [sizePerProc];
	MPI_Scatter(initA, sizePerProc , MPI_DOUBLE, A, sizePerProc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	oddEvenMergeSort(A, 0, sizePerProc);
	
	MPI_Gather(A, sizePerProc, MPI_DOUBLE, initA, sizePerProc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
    if(rank==0){
		/*
		ptrA = initA;	
		for(int j = 0; j < size; j++){
			printf("\n%d segment is\n", j);
			for(int i = 0; i < sizePerProc; i++)           
				printf("%.1f\t", *ptrA++);	
			printf("\n");
		}
		*/
	
		oddEvenMergeSortFinal(initA, Asize, size);		
		
		ptrA = initA;
        printf("\nNow we merge this shit together:\n");
        for(int i = 0; i < Asize; i++)           
            printf("%.1f\t", *ptrA++);				
	    printf("\n");				

    }
	
    delete []initA;


    MPI_Finalize();
}
