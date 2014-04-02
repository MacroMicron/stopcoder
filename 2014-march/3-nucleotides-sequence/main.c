// Simple algo with molecule and rolecule.
// Molecule string fixed and rolecule string we shift right 2*N-1 times.
// O(N^2) char comparsions.
// 
// This algo takes many time.
// On test/myhard.txt we have
// Case #1
// 1 999999
//
// real    299m54.518s
// user    299m32.455s
// sys     0m5.724s

/*
In S-Topcoder Contest,

You should use the statndard input/output

in order to receive a score properly.

Do not use file input and output

Please be very careful. 
*/

#include <stdio.h>

char data[1000000];

typedef struct Pair_
{
	long fst;
	long snd;
} Pair;

struct Molecule_
{
	long length, first;
	int RL; //RL: if from right to left -1 and left to right = 1
};
typedef struct Molecule_* Molecule;

// 0 <= i <= length-1
inline char get_dnk(Molecule molecule, long i)
{
	return ((molecule->first+molecule->RL*i >= 0)&&(molecule->first+molecule->RL*i<molecule->length)) ? 
			data[molecule->first+molecule->RL*i] : 'X';
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
			if (current.snd==0) current.snd=i+1;
		}
		else
		{
			if (current.fst > total.fst) {total.fst = current.fst; total.snd = current.snd;}
			current.fst = 0;
			current.snd = 0;
		}
	}
	if (current.fst > total.fst) {total.fst = current.fst; total.snd = current.snd;}
	return total;
}

void print(Molecule molecule)
{
	long i;
	for (i=0; i<molecule->length; i++)
	{
		printf("%c", get_dnk(molecule, i));
	}
	printf("\n");
}

int main(void)
{
	int T, test_case;
	long number_of_dnk;
	long Answer, First;
	Pair temp;
	struct Molecule_ molecule_, rolecule_;
	Molecule molecule = &molecule_, rolecule = &rolecule_;
	/*
	   The freopen function below opens input.txt file in read only mode, and afterward,
	   the program will read from input.txt file instead of standard(keyboard) input.
	   To test your program, you may save input data in input.txt file,
	   and use freopen function to read from the file when using scanf function.
	   You may remove the comment symbols(//) in the below statement and use it.
	   But before submission, you must remove the freopen function or rewrite comment symbols(//).
	 */
	// freopen("input.txt", "r", stdin);

	/*
	   If you remove the statement below, your program's output may not be rocorded
	   when your program is terminated after the time limit.
	   For safety, please use setbuf(stdout, NULL); statement.
	 */
	setbuf(stdout, NULL);

	scanf("%d", &T);
	for(test_case = 0; test_case < T; test_case++)
	{
		
		/////////////////////////////////////////////////////////////////////////////////////////////
		/*
		   Implement your algorithm here.
		   The answer to the case will be stored in variable Answer.
		 */
		/////////////////////////////////////////////////////////////////////////////////////////////
		Answer = 0;
		First = 0;
		scanf("%ld", &molecule->length);
		rolecule->length = molecule->length;
		scanf("%s", data);
		rolecule->first = molecule->first = 0;
		rolecule->RL = molecule->RL = 1;
		reverse(rolecule);
		rolecule->first -= rolecule->length;
		for (number_of_dnk = 0; number_of_dnk < 2*molecule->length-1; number_of_dnk++)
		{	
			shift_right(rolecule);
			temp = compare(molecule, rolecule);
			if (temp.fst>Answer) {Answer=temp.fst;	First=temp.snd;}
			//print(molecule);
			//print(rolecule);
			//printf("%ld %ld\n", temp.fst, temp.snd);
		}
		
		
		
		
		

		// Print the answer to standard output(screen).
		printf("Case #%d\n", test_case+1);
		printf("%ld %ld\n", First, Answer);
	}

	return 0;//Your program should return 0 on normal termination.
}

