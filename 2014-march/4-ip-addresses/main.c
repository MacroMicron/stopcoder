// just compare string in order
//
//
// timeout of test/myhard.txt:
// Case #1
// 999901 99902
//
// real    0m34.550s
// user    0m34.502s
// sys     0m0.000s


/*
In S-Topcoder Contest,
You should use the statndard input/output
in order to receive a score properly.
Do not use file nput and output
Please be very careful. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Answer;

int main(void)
{
	char pairs[100000][36];
	int T, test_case;
	long N, history_connect;
	// buffer variables
	long index;
	char reverse[36], str[18];

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
		// Print to standard output(screen).
		printf("Case #%d\n", test_case+1);
		scanf("%ld", &N);
		for (history_connect = 0; history_connect < N; history_connect++)
		{
			scanf("%ld %s %s", &index, pairs[history_connect], str);
			strcat(pairs[history_connect], " ");
			strcat(pairs[history_connect], str);
		}
		for (history_connect = 0; history_connect < N; history_connect++)
		{
			sscanf(pairs[history_connect], "%s %s", str, reverse);
			strcat(reverse, " ");
			strcat(reverse, str);
			for (index = history_connect+1; index < N; index++)
			{
				// if equal
				if (!strcmp(reverse, pairs[index])) 
				{
					printf("%ld %ld\n", history_connect+1, index+1);
					goto END;
				}
			}
		}

		// Print the answer to standard output(screen).
		printf("%d\n", Answer);
END:	continue;
	}

	return 0;//Your program should return 0 on normal termination.
}
