
# include <time.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <assert.h>

/*
 * given a vector it returns its magnitude
 */
double magnitude(double* vec, int n){
	double res;
	int i;
	res=0.0;
	for(i=0;i<n;i++){
		res += (*(vec))*(*(vec));
		vec++;
	}
	vec = vec - n;
	return sqrt(res);
}
/*
 * normalize the given vector in-place
 */
void normalize(double* vec,int n, double magnitude){
	int i;
	for(i=0;i<n;i++){
		*vec = (*vec)/magnitude;
		vec++;
	}
	vec = vec - n;
}

/*
 * given an epsilon and two vectors , returns 1 iff |vec1[i] - vec2[i]|<epsilon for each i
 */

int isCloseEnough(double* vec1, double* vec2,int n,double epsilon){
	int i;
	for(i=0;i<n;i++){
		if(fabs(*vec1-*vec2)>epsilon){
			return 0;
		}
		vec1++;
		vec2++;
	}
	vec1 = vec1- n;
	vec2 = vec2 - n;
	return 1;
}
/*
 * creates a vector of random positive values
 */
void createRandVec(double* randVec , int vecLength){
	int i ;
	for (i = 0; i<vecLength; ++i){
		*randVec = rand();
		randVec++;
	}
	randVec = randVec- vecLength;

}
/*
 * returns the dotProduct of two given vectors
 */

double dotProduct(double* firstRow, double* secondRow, int m){
	int i ; double resultProduct = 0.0;
	for(i = 0 ;i<m ; ++i){
		resultProduct += (*(firstRow)) * (*(secondRow ));
		firstRow++;
		secondRow++;
	}
	secondRow = secondRow - m;
	firstRow = firstRow - m;

return resultProduct;
}

/*
 * changes the next vector to be the product between the cov matrix and the current vector
 */
void matProduct(FILE* covMat, double* currVec, double* nextVec, double* tempVec, int vecLength){
	int size[2];int i; int numOfItems;
	rewind(covMat);
	numOfItems = fread(size,sizeof(int),2,covMat);
	assert(numOfItems==2);
	for(i = 0; i< vecLength;++i){
		numOfItems = fread(tempVec,sizeof(double),vecLength,covMat);
		assert(numOfItems==vecLength);
		*nextVec = dotProduct(tempVec,currVec,vecLength);
		nextVec++;
	}
	nextVec = nextVec - vecLength;
}
/*
 * replace between the two vector pointers
 */
void swap(double** bk,double** bk1){
	double* temp = *bk;
	*bk = *bk1;
	*bk1 = temp;
}


int main(int argc, char* argv[]){
	FILE* inputFile;FILE* outputFile; int sizes[2]; int n;int numOfItems;

	double* bk; double*bk1; double* temp;
	assert(argc == 3);
	inputFile= fopen(argv[1],"r");
	outputFile =fopen(argv[2],"w");
	numOfItems =  fread(sizes,sizeof(int),2,inputFile);
	assert(numOfItems==2);
	n = sizes[0];
	bk = (double*)malloc(n*sizeof(double));
	bk1 = (double*)malloc(n*sizeof(double));
	temp = (double*)malloc(n*sizeof(double));
	createRandVec(bk,n);
	do{
		matProduct(inputFile,bk,bk1,temp,n);
		normalize(bk1,n,magnitude(bk1,n));
		swap(&bk,&bk1);

	}while(isCloseEnough(bk,bk1,n,0.00001) == 0);
	sizes[0] = 1;
	sizes[1] = n;
	numOfItems  = fwrite(sizes,sizeof(int),2,outputFile);
	assert(numOfItems == 2);
	numOfItems  = fwrite(bk,sizeof(double),n,outputFile);
	assert(numOfItems==n);
	fclose(inputFile);
	fclose(outputFile);
	free(bk);
	free(bk1);
	free(temp);

	return 0;


}

int noneZeroVal(double* vec, int n){ /* This function returns the number of none zero value*/
	int j ; int cnt;
	cnt = 0;
	j = 0;
	while(j != n){
		if (*(vec) != 0.0){
			cnt++;
		}
		j++;
		vec++;
	}
	vec = vec- n;
	return cnt;

}


