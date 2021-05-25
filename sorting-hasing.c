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
	if (a == NULL) {
		printf("nothing to print.\n");
		return;
	}
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("a[%02d] ", i);
	printf("\n");
	for(int i = 0; i < MAX_ARRAY_SIZE; i++)
		printf("%5d ", a[i]);
	printf("\n");
}


int selectionSort(int *a)
{
	int min;
	int minindex;
	int i, j;

	printf("Selection Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for (i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		minindex = i;
		min = a[i];
		for(j = i+1; j < MAX_ARRAY_SIZE; j++)
		{
			if (min > a[j])
			{
				min = a[j];
				minindex = j;
			}
		}
		a[minindex] = a[i];
		a[i] = min;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);
	return 0;
}

int insertionSort(int *a)
{
	int i, j, t;

	printf("Insertion Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for(i = 1; i < MAX_ARRAY_SIZE; i++)
	{
		t = a[i];
		j = i;
		while (a[j-1] > t && j > 0)
		{
			a[j] = a[j-1];
			j--;
		}
		a[j] = t;
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int bubbleSort(int *a)
{
	int i, j, t;

	printf("Bubble Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for(i = 0; i < MAX_ARRAY_SIZE; i++)
	{
		for (j = 0; j < MAX_ARRAY_SIZE; j++)
		{
			if (a[j-1] > a[j])
			{
				t = a[j-1];
				a[j-1] = a[j];
				a[j] = t;
			}
		}
	}

	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int shellSort(int *a)
{
	int i, j, k, h, v;

	printf("Shell Sort: \n");
	printf("----------------------------------------------------------------\n");

	printArray(a);

	for (h = MAX_ARRAY_SIZE/2; h > 0; h /= 2)
	{
		for (i = 0; i < h; i++)
		{
			for(j = i + h; j < MAX_ARRAY_SIZE; j += h)
			{
				v = a[j];
				k = j;
				while (k > h-1 && a[k-h] > v)
				{
					a[k] = a[k-h];
					k -= h;
				}
				a[k] = v;
			}
		}
	}
	printf("----------------------------------------------------------------\n");
	printArray(a);

	return 0;
}

int quickSort(int *a, int n)
{
	int v, t;
	int i, j;

	if (n > 1)
	{
		v = a[n-1];
		i = -1;
		j = n - 1;

		while(1)
		{
			while(a[++i] < v);
			while(a[--j] > v);

			if (i >= j) break;
			t = a[i];
			a[i] = a[j];
			a[j] = t;
		}
		t = a[i];
		a[i] = a[n-1];
		a[n-1] = t;

		quickSort(a, i);
		quickSort(a+i+1, n-i-1);
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


