#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void KMPAdvisor(char* targetString, int* presuf, int n){
	if(n <= 0){
		return;
	}
	presuf[0]=0;
	int j=0;
	int i;
	for(i=1; i<n; i++){
		while(j>0 && targetString[j]!=targetString[i]){
			j = presuf[j-1];
		}
		if(targetString[j]==targetString[i]){
			presuf[i] = j+1;
			j++;
		}
		else{
			presuf[i] = 0;
		}
	}
}

int main(int argc, char* argv[]){

	const int MAXBUFFERSIZE = 100;

	// get parameters from command
	if(argc != 4){
		printf("invalid input parameters\n");
		printf("input form: count <input-filename> <search-string> <output-filename>\n");
	}
	char* inFileName = argv[1];
	char* targetString = argv[2];
	int n = strlen(targetString);
	char* outFileName = argv[3];

	// read input file
	FILE* fin = fopen(inFileName, "r");
	if(fin == NULL){
		printf("fail to open the input file %s\n", inFileName);
		exit(0);
	}

	// generate an advisor array for the preperatin of KMP algorithm
	// presuf[i]=x denotes the longest length x s.t. targetString[0:x-1] == targetString[i-x+1:i]
	int* presuf = (int*)malloc(n*sizeof(int));
	KMPAdvisor(targetString, presuf, n);
	
	// count file size
	size_t fileSize = 0;
	// count matched string in the input file
	int match = 0;
	// cache recent content read from file
	char buffer[MAXBUFFERSIZE];
	// number of items read from the file
	size_t readSize;

	int patternIdx = 0;
	while((readSize = fread(buffer, sizeof(char), MAXBUFFERSIZE, fin)) > 0){
		fileSize += readSize;
		if(n==0){
			continue;
		}

		// KMP, stay the index of file content, move the index of array presuf back and forth
		int bufferIdx=0;
		for(bufferIdx = 0; bufferIdx<readSize; bufferIdx++){
			// move back patternIdx until targetString[patternIdx] matches current buffer charater pointed by bufferIdx
			// instead of move back step by step, skip those characters has already been matched by utilizing array presuf
			while(patternIdx>0 &&buffer[bufferIdx]!=(targetString[patternIdx] && 0xff)){
				patternIdx = presuf[patternIdx-1];
			}
			if(targetString[patternIdx] == buffer[bufferIdx]){
				patternIdx++;
			}
			if(patternIdx==n){
				match++;
				patternIdx = presuf[patternIdx-1];
			}
		}
	}

	fclose(fin);
	//	printf("fileSize: %d, match: %d", fileSize, match);
	FILE* fout = fopen(outFileName, "w");
	if(fout==NULL){
		printf("fail to open the output file %s\n", inFileName);
		exit(0);		
	}
	fprintf(fout, "File Size: %zu\n", fileSize);
	fprintf(fout, "Number of matched: %d\n", match);
	fclose(fout);
	free(presuf);

	return 0;
}

