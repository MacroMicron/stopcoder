// This program give me 70/200 score on Samsung Top Coder Contest.
//
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


inline double timedelta(struct timeval start_time, struct timeval end_time)
{
	return (end_time.tv_sec - start_time.tv_sec) + (double)(end_time.tv_usec - start_time.tv_usec) / (double)BILLION;
}

typedef struct Pair_
{
	long fst; // length in this task
	long snd; // first index in this task
} Pair;

struct Molecule_
{
	long length, first;
	int RL; //RL: if from right to left -1 and left to right = 1
	char *data;
};
typedef struct Molecule_* Molecule;

typedef struct TestCase_
{
        Molecule molecule;
        Molecule rolecule;
        char *data;
        Pair answer; // Answer.fst is a length and Answer.snd is a first letter index
	int number;
} TestCase;

// 0 <= i <= length-1
inline char get_dnk(Molecule molecule, long i)
{
	return ((molecule->first+molecule->RL*i >= 0)&&(molecule->first+molecule->RL*i<molecule->length)) ? 
			molecule->data[molecule->first+molecule->RL*i] : 'X';
}

void shift_right(Molecule molecule)
{
	molecule->first++;
}

void shift_left(Molecule molecule)
{
	molecule->first--;
}

void reverse(Molecule molecule)
{
	molecule->first+=molecule->RL*(molecule->length-1);
	molecule->RL*=-1;
}

// must be molecule1->length == molecule2->length
Pair compare(Molecule molecule1, Molecule molecule2)
{
	long i;
	Pair current = {0, 0}, total = {0, 0};
	char ch;
	for (i=0; i < molecule1->length; i++)
	{
		char ch = get_dnk(molecule2, i);
		if ((ch != 'X') && (get_dnk(molecule1, i) == ch))
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
	for (i=0; i<molecule->length; i++)
	{
		printf("%c", get_dnk(molecule, i));
	}
	printf("\n");
}

void swap_tests(TestCase *test1, TestCase *test2)
{
		TestCase temp = {test1->molecule, test1->rolecule, test1->data, test1->answer, test1->number};
		test1->molecule = test2->molecule;
		test1->rolecule = test2->rolecule;
		test1->data = test2->data;
		test1->answer = test2->answer;
		test1->number = test2->number;
		
		test2->molecule = temp.molecule;
		test2->rolecule = temp.rolecule;
		test2->data = temp.data;
		test2->answer = temp.answer;
		test2->number = temp.number;
}

// return index of longest test
int longest_test(TestCase *tests, int T)
{
	if (T==1) return 0;
	int index = 1+longest_test(&tests[1], T-1);
	return tests[0].molecule->length > tests[index].molecule->length ? 0 : index;
}

void prints(TestCase *tests, int T)
{
        int test_case;
        for (test_case = 0; test_case < T; test_case++)
        {
                printf("testcase[%d]: length %ld\n", test_case, tests[test_case].molecule->length);
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
char **all_data = NULL;
TestCase *tests = NULL;
int T = 0; //number of tests
struct timeval start_time, current_time;
struct timezone start_zone, current_zone;
double timeout=2.99;
int timeoff = 0; // 1 - if time expired and 0 if didn't

int main(void)
{
	gettimeofday(&start_time, &start_zone);
	int test_case, index;
	TestCase *test = NULL;
	char *data = NULL;
	long number_of_dnk;
	Pair temp;
	
	setbuf(stdout, NULL);
	scanf("%d", &T);
	tests = (TestCase*) malloc(T*sizeof(TestCase));
	all_data = (char **) malloc(T*sizeof(char*));
	// read all tasks
        for(test_case = 0; test_case < T; test_case++)
        {
                test = &tests[test_case];
                test->molecule = (Molecule) malloc(sizeof(struct Molecule_));
                test->rolecule = (Molecule) malloc(sizeof(struct Molecule_));
                test->answer.fst = 1;
                test->answer.snd = 1;
		test->number = test_case + 1;
		scanf("%ld", &test->molecule->length);
                test->rolecule->length = test->molecule->length;
		all_data[test_case] = (char *) malloc(test->molecule->length * sizeof(char));
		data = test->molecule->data = test->rolecule->data = all_data[test_case];
		scanf("%s", data);
                test->rolecule->first = test->molecule->first = 0;
                test->rolecule->RL = test->molecule->RL = 1;
                reverse(test->rolecule);
		test->rolecule->first -= test->rolecule->length;
	}
	for(test_case = 0; test_case < T; test_case++)
	{
		test = &tests[test_case];
		if (!timeoff) for (number_of_dnk = 0; number_of_dnk < 2 * test->molecule->length-1 ; number_of_dnk++)
		{	
			shift_right(test->rolecule);
			temp = compare(test->molecule, test->rolecule);
			if (temp.fst > test->answer.fst) test->answer = temp;
			gettimeofday(&current_time, &current_zone);
			if ( timeout <= timedelta(start_time, current_time) ) {timeoff = 1; break;} 
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
		free(test->molecule->data);
                free(test->molecule);
                free(test->rolecule);
	}
	free(all_data);
	free(tests);
	return 0; //Your program should return 0 on normal termination.
}

