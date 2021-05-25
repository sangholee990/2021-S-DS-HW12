/*
 * hw12-sorting-hashing.c
 *
 *  Created on: May 22, 2021
 *
 *  Homework 12: Sorting & Hashing
 *  Department of Computer Science at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ARRAY_SIZE			13	/* prime number */
#define MAX_HASH_TABLE_SIZE 	MAX_ARRAY_SIZE

/* 필요에 따라 함수 추가 가능 */
int initialize(int **a); //배열을 초기화 해주는 함수 
int freeArray(int *a); // 동적할당 받은 배열 
void printArray(int *a); // 배열원소를 출력하는 함수 

int selectionSort(int *a); //selectionsort(선택정렬)를 실행하는 함수 
int insertionSort(int *a); //insertionsort(삽입정렬)을 실행하는 함수 
int bubbleSort(int *a); // 버블정렬을 실행하는 함수 
int shellSort(int *a); // 셀 정렬을 실행하는 함수 
/* recursive function으로 구현 */
int quickSort(int *a, int n); // 퀵 정렬을 실행하는 함수 


/* hash code generator, key % MAX_HASH_TABLE_SIZE */
int hashCode(int key); //키값을 hashcode로 바꿔준다 

/* array a에대한 hash table을 만든다. */
int hashing(int *a, int **ht); //해쉬 테이블을 만들어준다 (배열 활용) 

/* hash table에서 key를 찾아 hash table의 index return */
int search(int *ht, int key); // 해쉬 테이블에서 사용자가 입력한 키를 찾아 리턴해주는 함수 


int main()
{
	char command; //사용자로 부터 커맨드를 입력받는 변수 
	int *array = NULL; // 배열 포인터 초기화 
	int *hashtable = NULL; // 해쉬테이블 포인터 초기화 
	int key = -1; // 키값 초기화 
	int index = -1; // 인덱스 초기화  

	srand(time(NULL)); // seed값을 실행중에 변하게하는 srand 함수 

	do{
		printf("----------------------------------------------------------------\n");
		printf("                        Sorting & Hashing                       \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize       = z           Quit             = q\n");
		printf(" Selection Sort   = s           Insertion Sort   = i\n");
		printf(" Bubble Sort      = b           Shell Sort       = l\n");
		printf(" Quick Sort       = k           Print Array      = p\n");
		printf(" Hashing          = h           Search(for Hash) = e\n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); //사용자에게 커맨드를 입력받는다 

		switch(command) {
		case 'z': case 'Z':
			initialize(&array); //배열 초기화  
			break;
		case 'q': case 'Q':
			freeArray(array); //배열 동적할당 해제 
			break;
		case 's': case 'S':
			selectionSort(array); //배열을 선택정렬하는 함수 호출 
			break;
		case 'i': case 'I':
			insertionSort(array); //배열을 삽입정렬하는 함수 호출 
			break;
		case 'b': case 'B':
			bubbleSort(array); //배열을 버블정렬 하는 함수 호출 
			break;
		case 'l': case 'L':
			shellSort(array); //배열을 쉘정렬 해주는 함수 호출 
			break;
		case 'k': case 'K':
			printf("Quick Sort: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array); // 정렬 하기전 배열 출력 
			quickSort(array, MAX_ARRAY_SIZE); // 퀵 정렬 함수 호출 
			printf("----------------------------------------------------------------\n");
			printArray(array); //정렬후의 배열 출력 

			break;

		case 'h': case 'H':
			printf("Hashing: \n");
			printf("----------------------------------------------------------------\n");
			printArray(array); //배열을 출력하는 함수 
			hashing(array, &hashtable); // 배열 활용하여 해싱 
			printArray(hashtable); // 해싱하여 얻은 해쉬 테이블 출력 
			break;

		case 'e': case 'E':
			printf("Your Key = ");
			scanf("%d", &key); // 사용자에게 키값 입력받음 
			printArray(hashtable); // 현재 해쉬테이블 출력 
			index = search(hashtable, key); // 해쉬테이블에 입력받은 키값과 일치하는 키가 있다면 인덱스 저장 
			printf("key = %d, index = %d,  hashtable[%d] = %d\n", key, index, index, hashtable[index]);
			break;

		case 'p': case 'P':
			printArray(array); //배열 관한 정보 출력 
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initialize(int** a)
{
	int *temp = NULL;

	/* array가 NULL인 경우 메모리 할당 */
	if(*a == NULL) { //배열이 존재하지 않는다면 
		temp = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE); //temp에 배열 사이즈 만큼 동적 할당 
		*a = temp;   /* 할당된 메모리의 주소를 복사 --> main에서 배열을 control 할수 있도록 함*/
	} else //배열이 존재 할 경우 
		temp = *a; //템프가 배열에 접근되도록 

	/* 랜덤값을 배열의 값으로 저장 */
	for(int i = 0; i < MAX_ARRAY_SIZE; i++) //모든 배열의 인덱스에 접근 
		temp[i] = rand() % MAX_ARRAY_SIZE; // 모든 인덱스 값들을 0~12의 값을 랜덤으로 초기화 

	return 0;
}

int freeArray(int *a)
{
	if(a != NULL) //배열이 존재하면 
		free(a); //동적할당 해제 
	return 0;
}

void printArray(int *a)
{
	if (a == NULL) { //배열이 존재하지 않는 경우 
		printf("nothing to print.\n");  //오류메세지  
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++) //배열에 모든 인덱스 접근 
		printf("a[%02d] ", i); //0~12까지 출력 
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("%5d ", a[i]); //배열의 원소값 출력 
	printf("\n");
}


int selectionSort(int *a)
{
	int min; //최솟값 변수 
	int minindex; //최소값 담겨있는 인덱스 담는 변수 
	int i, j; //반복문용 변수 

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); // 현재 배열 정보 출력 

	for (i = 0; i < MAX_ARRAY_SIZE; i++) //배열의 사이즈 만큼 반복  
	{
		minindex = i; // 최소값이 담겨있는 인덱스를 i로 초기화  
		min = a[i]; //최소값이 담길 변수에 인덱스 i에 위치한 배열의 원소를 담는다 
		for(j = i+1; j < MAX_ARRAY_SIZE; j++) // 배열의 최종인덱스까지 탐색 
		{
			if (min > a[j]) //min에 담겨있는 값이 j번째 인덱스 보다 클경우 
			{
				min = a[j]; // 최소값을 j번째 인덱스 값으로 바꿔준다 
				minindex = j; //최솟값의 인덱스를 j로 바꿔준다 
			}
		}
		a[minindex] = a[i]; // a[i]보다 작은 값을 가지고 있던 인덱스 J에 a[i] 삽입 
		a[i] = min; //더작은 최솟값을 swap 
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //정렬 이후의 배열 정보 출력 
	return 0;
}

int insertionSort(int *a)
{
	int i, j, t; // 반복문 변수, 원소값담을 변수 

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); //정렬하기 전 배열 정보 출력 

	for(i = 1; i < MAX_ARRAY_SIZE; i++) // 최종 인덱스 까지 접근 
	{
		t = a[i]; //인덱스 i번째의 원소 값을 t에 저장 
		j = i;// j에 i번째 인덱스를 저장 
		while (a[j-1] > t && j > 0) //이전 인덱스의 값이 더큰 경우 
		{  // swap 
			a[j] = a[j-1]; 
			j--;
		}
		a[j] = t; //a[j] 에 t삽입 
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //정렬이후 배열 정보 출력 

	return 0;
}

int bubbleSort(int *a)
{
	int i, j, t; // 위함수와 동일  

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); //정렬되기 이전의 배열 정보 출력 

	for(i = 0; i < MAX_ARRAY_SIZE; i++) // 0~12 인덱스 반복문 돌기 
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++) // j가 인덱스 1부터 12까지 반복문 돌기 
		{
			if (a[j-1] > a[j]) //j와 직전인덱스를 비교해서 큰값이 나올 경우 
			{
				t = a[j-1]; //a[j-1]와 a[j]를 swap 과정 
				a[j-1] = a[j]; 
				a[j] = t;
			}
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a); //정렬이후 배열 출력 

	return 0;
}

int shellSort(int *a)
{
	int i, j, k, h, v; 

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a); // 정렬 되기전 정보 출력 

	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2) // h는 배열 크기 1/2 부터 시작 한단계 수행 될때마다 h값 1/2 감소 시킴 
	{
		for (i = 0; i < h; i++) // 첫번째 부분집합의 기준을 h로 한다 0부터 h-1까지 반복  
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h) // i번째에서 h만큼 떨어진 원소들과 비교 
			{
				v = a[j]; //i에서 h만큼 떨어진 j번째 원소의 값을 v에 저장 
				k = j; //인덱스 j의 값을 k에 저장 
				while (k > h-1 && a[k-h] > v)
				{ //k의 값이 두번째 부분집합의 첫번째 인덱스와 같거나 크고, i의 원소보다 h 간격만큼 떨어진 j의 원소가 작은 경우 
					a[k] = a[k-h]; // 둘중에 더큰값을 뒤의 위치 즉, j번째 위치에 j이전의 h만큼 떨어진 원소 저장 
					k -= h; //k가 i에서 h만큼 한 번 이상 떨어진 곳의 인덱스 이므로 h만큼 빼서 이전의 h만큼 떨어진 인덱스로 이동 
				}
				a[k] = v; // j번째 원소를 while문을 탈출 한 k번째의 저장 
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a); // 정렬 후의 배열 상태 출력  

	return 0;
}

int quickSort(int *a, int n)
{
	int v, t;
	int i, j;

	if (n > 1) // 서브 리스트의 원소가 2개 이상인경우 (원소가 하나인 경우 정렬이 필요없다) 
	{
		v = a[n-1]; // 배열의 마지막 원소를 v에 저장한다 
		i = -1; //배열의 첫번째 원소부터 접근 가능하도록 i의 -1로 설정 
		j = n - 1; // 배열의 마지막 인덱스 번호를 j에 저장 

		while(1) // 무한루프가 돈다 
		{
			while(a[++i] < v); //i번째 원소가 추축보다 커질때까지 i를 증가시킨다 
			while(a[--j] > v); //i번째 원소가 추축보다 작아질때까지 j를 감소 시킨다 

			if (i >= j) break; // i번째 원소가 추축보다 크고 j번째 원소가 추축보다 작을때 반복문을 탈출한다 
			t = a[i]; // t에 임시로 i번째 원소를 저장한다 
			a[i] = a[j]; // i번째에 j번째 원소를 저장한다 
			a[j] = t; // j번째에 i번째 원소를 저장한다 
		}
		t = a[i]; // t에 i번째 원소를 저장 
		a[i] = a[n-1]; //i 이전에 추축보다 작은 값이 있으므로 i번째 추축을 저장한다 
		a[n-1] = t; // 추축이있던 배열의 마지막 위치에 t에 임시 저장된 값 저장 

		quickSort(a, i); //추축보다 작은값이 모여있는 리스트를 퀵정렬로 정렬 
		quickSort(a+i+1, n-i-1); //추축보다 큰값이 모여있는 리스트를 퀵정렬로 정렬 
	}


	return 0;
}

int hashCode(int key) {
   return key % MAX_HASH_TABLE_SIZE;
}

int hashing(int *a, int **ht)
{
	int *hashtable = NULL;

	if(*ht == NULL) {
		hashtable = (int*)malloc(sizeof(int) * MAX_ARRAY_SIZE);
		*ht = hashtable;  
	} else {
		hashtable = *ht;	
	}

	for(int i = 0; i < MAX_HASH_TABLE_SIZE; i++)
		hashtable[i] = -1;

	

	int key = -1;
	int hashcode = -1;
	int index = -1;
	for (int i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		key = a[i];
		hashcode = hashCode(key);
		
		if (hashtable[hashcode] == -1)
		{
			hashtable[hashcode] = key;
		} else 	{

			index = hashcode;

			while(hashtable[index] != -1)
			{
				index = (++index) % MAX_HASH_TABLE_SIZE;
				
			}
			hashtable[index] = key;
		}
	}

	return 0;
}

int search(int *ht, int key)
{
	int index = hashCode(key);

	if(ht[index] == key)
		return index;

	while(ht[++index] != key)
	{
		index = index % MAX_HASH_TABLE_SIZE;
	}
	return index;
}


