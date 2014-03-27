// this program give me only 20/200 score

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
        Molecule roleculeLR;
	Molecule roleculeRL;
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
// if RL == 1 then from Right to left compare
// if RL == 0 then from Left to Right compare
Pair compare(Molecule molecule1, Molecule molecule2, int RL)
{
	long i;
	Pair current = {0, 0}, total = {0, 0};
	char ch;
	if (RL)
	{
                for (i=molecule1->length-1; i>=0; i--)
                {
                        char ch = get_dnk(molecule2, i);
                        if ((ch != 'X') && (get_dnk(molecule1, i) == ch))
                        {
                                current.fst++;
                                if (current.snd==0) current.snd=molecule1->length-i; //indexes begin from 1
                        }
                        else
                        {
                                if (current.fst > total.fst) total = current;
                                current.fst = 0;
                                current.snd = 0;
        //                      if (ch == 'X') break;
                        }
                }
	}
	else
	{
		for (i=0; i<molecule1->length; i++)
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
	//			if (ch == 'X') break;
			}
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
		TestCase temp = {test1->molecule, test1->roleculeLR, test1->roleculeRL, test1->data, test1->answer, test1->number};
		test1->molecule = test2->molecule;
		test1->roleculeLR = test2->roleculeLR;
		test1->roleculeRL = test2->roleculeRL;
		test1->data = test2->data;
		test1->answer = test2->answer;
		test1->number = test2->number;
		
		test2->molecule = temp.molecule;
		test2->roleculeLR = temp.roleculeLR;
		test2->roleculeRL = temp.roleculeRL;
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
                test->roleculeLR = (Molecule) malloc(sizeof(struct Molecule_));
                test->roleculeRL = (Molecule) malloc(sizeof(struct Molecule_));
			if (temp.fst > test->answer.fst) test->answer = temp;
                test->answer.fst = 1;
                test->answer.snd = 1;
		test->number = test_case + 1;
		scanf("%ld", &test->molecule->length);
                test->roleculeRL->length = test->roleculeLR->length = test->molecule->length;
		all_data[test_case] = (char *) malloc(test->molecule->length * sizeof(char));
		data = test->molecule->data = test->roleculeLR->data = test->roleculeRL->data = all_data[test_case];
		scanf("%s", data);
                test->roleculeLR->first = test->roleculeRL->first = test->molecule->first = 0;
                test->roleculeLR->RL = test->roleculeRL->RL = test->molecule->RL = 1;
                reverse(test->roleculeLR);
		reverse(test->roleculeRL);
	}
	for(test_case = 0; test_case < T; test_case++)
	{
		test = &tests[test_case];
		if (!timeoff) for (number_of_dnk = 0; number_of_dnk < test->molecule->length; number_of_dnk++)
		{
			if (test->answer.fst > test->molecule->length - number_of_dnk) break;
			if (number_of_dnk == 0)
			{
				temp = compare(test->molecule, test->roleculeLR, 1);
				if (temp.fst > test->answer.fst) test->answer = temp;
			}
			else
			{
				shift_right(test->roleculeLR);
				temp = compare(test->molecule, test->roleculeLR, 1);
				if (temp.fst > test->answer.fst) test->answer = temp;
				shift_left(test->roleculeRL);
				temp = compare(test->molecule, test->roleculeRL, 0);
				if (temp.fst > test->answer.fst) test->answer = temp;
			}
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
                free(test->roleculeLR);
                free(test->roleculeRL);
	}
	free(all_data);
	free(tests);
	return 0; //Your program should return 0 on normal termination.
}

