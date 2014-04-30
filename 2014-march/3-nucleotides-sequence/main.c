// Simple algo with molecule and rolecule.
// Molecule string fixed and rolecule string we shift right 2*N-1 times.
// O(N^2) char comparsions.
// 
// This algo takes many time.
// Smart time feature use:
// 1. we read all tasks and perform first tasks with short length
// 2. if we spend all time then break and print "zaglushki"

/*
In S-Topcoder Contest,

You should use the statndard input/output

in order to receive a score properly.

Do not use file input and output

Please be very careful. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define BILLION  1000000L

typedef unsigned long long data_type; // it's a type used for low level storing data
const int bits_for_dnk = 2; // bites for encoding {A, C, G, T}
int dnk_per_data_type = 0; // = sizeof(data_type)*8/bits_for_dnk; how many dnk we can store in one long long int type, for example
enum dnk_type {dnk_A = 0, dnk_C, dnk_G, dnk_T};
char dnk_type_to_char[4] = {'A', 'C', 'G', 'T'};
enum dnk_type char_to_dnk_type(char ch)
{
	switch(ch) {
		case 'A': return dnk_A;
		case 'C': return dnk_C;
		case 'G': return dnk_G;
		case 'T': return dnk_T;
	}
	return dnk_A;
}

inline double timedelta(struct timeval start_time, struct timeval end_time)
{
	return (end_time.tv_sec - start_time.tv_sec) + (double)(end_time.tv_usec - start_time.tv_usec) / (double)BILLION;
}

typedef struct Pair_
{
	long fst; // length in this task
	long snd; // first index in this task
} Pair;

// 2DimNNumber is a number YZ where 0<=Z<N, 0<=Y<N
typedef Pair NNumber2D;

inline long ToDec(NNumber2D number, long Dimension)
{
	return number.fst * Dimension + number.snd;
}

inline long MinusToDec(NNumber2D number1, NNumber2D number2, long Dimension)
{
	return (number1.fst - number2.fst) * Dimension + (number1.snd - number2.snd);
}

NNumber2D Plus(NNumber2D number, long n, long Dimension)
{
	NNumber2D result = {number.fst + n / Dimension, number.snd + n % Dimension};
	if (result.snd >= Dimension) {result.snd-=Dimension; result.fst++;}
	else if (result.snd < 0) {result.snd+=Dimension; result.fst--;}
	return result;
}

// if we have matrix data unsigned long long int[99]:
// where every line - is an unsigned long long int on low level
//    01234567
//00  XXXXXXXX
//01  XXXXXXXX
//02  XXXXXXXX
//03  XXXXXXXX
//04  XXXXXXXX
//05  XXXXXXXX
//06  XXXXXXXX
//07  XXXXXXXX
//08  XXXXXXXX
//..  ........
//99  XXXX----
//
// begin.fst = 00 - is line number where string begins
// begin.snd = 0 - is a "bit" number where string begins
// end.fst = 99 - is a line number where string ends
// end.snd = 3 - is a "bit" number where string ends
struct Molecule_
{
	NNumber2D begin, end; 
	long length, first;
	// RL - rudiment if begin < end then RL == 1; if begin > end then RL == -1
	int RL; //RL: if from right to left -1 and left to right = 1
	data_type *data;
};
typedef struct Molecule_* Molecule;

inline long get_length(Molecule molecule)
{
	return molecule->RL*MinusToDec(molecule->end, molecule->begin, dnk_per_data_type) + 1;
}

typedef struct TestCase_
{
        Molecule moleculeL; // compare molecule and rolecule and move 
        Molecule roleculeL; // 	     rolecule to left (remove_first roleculeL)
	Molecule moleculeR; // compare molecule and rolecule and move
	Molecule roleculeR; //       rolecule to right (remove_first moleculeR)
	data_type *data;
	long length;
        Pair answer; // Answer.fst is a length and Answer.snd is a first letter index
	int number; // TestCase #number
} TestCase;

inline enum dnk_type get_dnk_from_data(data_type *data, NNumber2D i)
{
	return (enum dnk_type) (0b11&(data[i.fst] >> 2*(dnk_per_data_type - 1 - i.snd)));
}

void set_dnk_in_data(data_type *data, NNumber2D i, enum dnk_type dnk)
{
	data[i.fst] |= (data_type)dnk << 2*(dnk_per_data_type - 1 - i.snd);
}

// 0 <= i <= length-1
/*inline enum dnk_type get_dnk(Molecule molecule, long i)
{
	return (enum dnk_type) (0b11&((molecule->RL == 1) ? 
			molecule->data[molecule->first + i/dnk_per_data_type] >> 2*(dnk_per_data_type - 1 - (i%dnk_per_data_type)) :
 			molecule->data[molecule->first - i/dnk_per_data_type] >> 2*(i%dnk_per_data_type)));
}*/

inline enum dnk_type get_dnk(Molecule molecule, long i)
{
	return get_dnk_from_data(molecule->data, Plus(molecule->begin, molecule->RL * i, dnk_per_data_type));
}

void set_dnk(Molecule molecule, long i, enum dnk_type dnk)
{
	set_dnk_in_data(molecule->data, Plus(molecule->begin, molecule->RL * i, dnk_per_data_type), dnk);
}

void remove_first(Molecule molecule)
{
	if (get_length(molecule)  > 0)
	{
		molecule->begin = Plus(molecule->begin, molecule->RL, dnk_per_data_type); 
	}	
}

void remove_last(Molecule molecule)
{
	if (get_length(molecule) > 0)
	{
		molecule->end = Plus(molecule->end, (-1)*molecule->RL , dnk_per_data_type);
	}
}

void reverse(Molecule molecule)
{
	NNumber2D temp = molecule->begin;
	molecule->begin = molecule->end;
	molecule->end = temp;
	molecule->RL*=-1;
}

// must be molecule1->length == molecule2->length
Pair compare(Molecule molecule1, Molecule molecule2)
{
	long length1 = get_length(molecule1), length2 = get_length(molecule2);
	long i, length = length1 < length2 ? length1 : length2; // minimal length between molecule1 and molecule2
	Pair current = {0, 0}, total = {0, 0};
	for (i=0; i<length; i++)
	{
		if (get_dnk(molecule1, i) == get_dnk(molecule2, i))
		{
			current.fst++;
			if (current.snd==0) current.snd=i+1; //indexes begin from 1
		}
		else
		{
			if (current.fst > total.fst) total = current;
			current.fst = 0;
			current.snd = 0;
		}
	}
	if (current.fst > total.fst) total = current;
	return total;
}

void printm(Molecule molecule)
{
	long i;
	for (i=0; i < get_length(molecule); i++)
	{
		printf("%c", dnk_type_to_char[get_dnk(molecule, i)]);
	}
	printf("\n");
}

// we should to have malloced memory before this operation
// error! po ssulke
void readm(Molecule molecule, long length)
{
	long i;
	char ch;
	NNumber2D zero = {0, 0};
	molecule->begin = zero;
	molecule->end = Plus(zero, length-1, dnk_per_data_type);
	molecule->RL = 1;
	for (i=molecule->begin.fst; i <= molecule->end.fst; i++)
	{
		molecule->data[i] = (data_type) 0;
	}
	for (i=0; i<length; i++)
	{
		scanf("%c", &ch);
		set_dnk(molecule, i, char_to_dnk_type(ch));
	}
}

// all memory should be allocated before!
void copym(Molecule molecule1, Molecule molecule2)
{
	molecule1->begin = molecule2->begin;
	molecule1->end   = molecule2->end;
	molecule1->RL    = molecule2->RL;
	molecule1->data  = molecule2->data;
}

void swap_tests(TestCase *test1, TestCase *test2)
{
		TestCase temp = {test1->moleculeL, test1->roleculeL, test1->moleculeR, test1->roleculeR, test1->data, test1->length, test1->answer, test1->number};
		test1->moleculeL = test2->moleculeL;
		test1->roleculeL = test2->roleculeL;
		test1->moleculeR = test2->moleculeR;
		test1->roleculeR = test2->roleculeR;
		test1->data = test2->data;
		test1->length = test2->length;
		test1->answer = test2->answer;
		test1->number = test2->number;
		
		test2->moleculeL = temp.moleculeL;
		test2->roleculeL = temp.roleculeL;
		test2->moleculeR = temp.moleculeR;
		test2->roleculeR = temp.roleculeR;
		test2->data = temp.data;
		test2->length = temp.length;
		test2->answer = temp.answer;
		test2->number = temp.number;
}

// return index of longest test
int longest_test(TestCase *tests, int T)
{
	if (T==1) return 0;
	int index = 1+longest_test(&tests[1], T-1);
	return tests[0].length > tests[index].length ? 0 : index;
}

void prints(TestCase *tests, int T)
{
        int test_case;
        for (test_case = 0; test_case < T; test_case++)
        {
                printf("testcase[%d]: length %ld\n", test_case, tests[test_case].length);
        }
}

// intellectual sorting tests from simple to hard
void sort_tests(TestCase *tests, int T)
{
	if (T==1) return;
	int index = longest_test(tests, T);
	if (index != T-1) swap_tests(&tests[index], &tests[T-1]);
	sort_tests(tests, T-1);
}

//========================================================================

// global variables
data_type **all_data = NULL;
TestCase *tests = NULL;
int T = 0; //number of tests
struct timeval start_time, current_time;
struct timezone start_zone, current_zone;
double timeout=2.99;
int timeoff = 0; // 1 - if time expired and 0 if didn't

int main(void)
{
	gettimeofday(&start_time, &start_zone);
	dnk_per_data_type = sizeof(data_type)*8/bits_for_dnk; // should be in init 
	int test_case, index;
	TestCase *test = NULL;
	data_type *data = NULL;
	long number_of_dnk;
	Pair temp;
	
	setbuf(stdout, NULL);
	scanf("%d", &T);
	tests = (TestCase*) malloc(T*sizeof(TestCase));
	all_data = (data_type **) malloc(T*sizeof(data_type*));
	// read all tasks
	for(test_case = 0; test_case < T; test_case++)
	{
	        test = &tests[test_case];
	        test->moleculeL = (Molecule) malloc(sizeof(struct Molecule_));
	        test->roleculeL = (Molecule) malloc(sizeof(struct Molecule_));
	        test->moleculeR = (Molecule) malloc(sizeof(struct Molecule_));
        	test->roleculeR = (Molecule) malloc(sizeof(struct Molecule_));
	        test->answer.fst = test->answer.snd = 1;
		test->number = test_case + 1;
		scanf("%ld", &test->length);
		all_data[test_case] = (data_type *) malloc(sizeof(data_type) * (test->length / dnk_per_data_type + 1));
		data = test->moleculeL->data = test->roleculeL->data = test->roleculeR->data = test->moleculeR->data = all_data[test_case];
		char ch;
		scanf("%c", &ch); // will read '\n' char
		readm(test->moleculeL, test->length); // automatically other mollecules and rolecules will be initalized
		copym(test->roleculeL, test->moleculeL);
		copym(test->moleculeR, test->moleculeL);
		copym(test->roleculeR, test->moleculeL);
		reverse(test->roleculeL);
		reverse(test->roleculeR);
	}
	for(test_case = 0; test_case < T; test_case++)
	{
		test = &tests[test_case];
		if (!timeoff) for (number_of_dnk = 0; number_of_dnk < test->length; number_of_dnk++)
		{
			if (test->answer.fst > test->length - number_of_dnk) break;
			if (test->answer.fst == test->length - number_of_dnk) if (test->answer.snd == 1) break;
			if (number_of_dnk == 0)
			{
				temp = compare(test->moleculeL, test->roleculeL);
				if (temp.fst > test->answer.fst) test->answer = temp;
				if (temp.fst == test->answer.fst) if (temp.snd < test->answer.snd) test->answer = temp;
			}
			else
			{
				remove_first(test->roleculeL);
				temp = compare(test->moleculeL, test->roleculeL);
				if (temp.fst > test->answer.fst) test->answer = temp;
				if (temp.fst == test->answer.fst) if (temp.snd < test->answer.snd) test->answer = temp;

				remove_first(test->moleculeR);
				temp = compare(test->moleculeR, test->roleculeR);
				if (temp.fst > test->answer.fst) test->answer = temp;
				if (temp.fst == test->answer.fst) if (temp.snd < test->answer.snd) test->answer = temp;
			}
			gettimeofday(&current_time, &current_zone);
			//if ( timeout <= timedelta(start_time, current_time) ) {timeoff = 1; break;} 
		}
	}

	for(test_case = 1; test_case <= T; test_case++)
	{
		index = -1;
		do {
			index++;
			test = &tests[index];
		} while (test->number != test_case);
		// Print the answer to standard output(screen).
                printf("Case #%d\n", test->number);
                printf("%ld %ld\n", test->answer.snd, test->answer.fst);
	}
	
        for(test_case = 0; test_case < T; test_case++)
	{
                test = &tests[test_case];
//		free(test->data);
                free(test->moleculeL);
                free(test->roleculeL);
		free(test->moleculeR);
                free(test->roleculeR);
	}
	free(all_data);
	free(tests);
	return 0; //Your program should return 0 on normal termination.
}

