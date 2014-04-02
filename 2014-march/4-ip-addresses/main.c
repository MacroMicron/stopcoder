// this decision takes 150/150 on topcoder
//
// upgrade simple order to binary tree with strings
// for speed up string comparsion
//
// timeout for test/myhard.txt
// Case #1
// 99901 99902
//
// real    0m0.099s
// user    0m0.092s
// sys     0m0.004s


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

int Answer = 0;

struct Node_ {
	long number;
	char name[36];
	struct Node_ *left;
	struct Node_ *right;
};
typedef struct Node_ Node;
typedef Node* Tree;

Node heap[100000];
long heap_pointer = 0;


Tree init_tree_head(const char* string, long index)
{
        Tree tree = &heap[heap_pointer++];
        strcpy(tree->name, string);
        tree->number = index;
        tree->right = tree->left = NULL;
	return tree;
}

// tree != NULL; length of string < 36
void add_to_tree(Tree tree, const char* string, long index)
{
/*	if (tree !=NULL)
	{*/
		int result = strcmp(tree->name, string);
		if (result < 0)
		{
			if (tree->left != NULL) 
			{
				add_to_tree(tree->left, string, index);
			}
			else 
			{
				tree->left = &heap[heap_pointer++];
				strcpy(tree->left->name, string);
				tree->left->number = index;
				tree->left->left = tree->left->right = NULL;
			}			
		}
		else if (result > 0)
		{
			if (tree->right != NULL)
			{
				add_to_tree(tree->right, string, index);
			}
			else
			{
				tree->right = &heap[heap_pointer++];
				strcpy(tree->right->name, string);
				tree->right->number = index;
				tree->right->right = tree->right->left = NULL;
			}
		}
/*	}
	else
	{
		tree = &heap[heap_pointer++];
		strcpy(tree->name, string);
		tree->number = index;
		tree->right = tree->left = NULL;
	}*/
}

// length of string <= 36; index returned if found and -1 if didn't
long search_in_tree(Tree tree, const char *string)
{
	if (tree != NULL)
	{
		int result = strcmp(tree->name, string);
                if (result < 0)
                {
			return search_in_tree(tree->left, string);
		}
		else if (result > 0)
		{
			return search_in_tree(tree->right, string);
		}
		else return tree->number;
	}
	return -1;
}

void printf_tree(Tree tree)
{
	long i;
	for (i=0; i<heap_pointer; i++)
	{
		printf("heap[%ld] = {(%ld) ,%ld, %s, (%ld), (%ld)}\n", 
			i,
			(long)&heap[i], 
			heap[i].number, 
			heap[i].name, 
			(long)heap[i].left, 
			(long)heap[i].right);
	}
}

int main(void)
{
	int T, test_case;
	long N, history_connect;
	Tree tree = NULL;
	long index;
	char reverse[36], str[18], string[36];


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
		// Print to standard output(screen).
		printf("Case #%d\n", test_case+1);
		scanf("%ld", &N);
		for (history_connect = 0; history_connect < N; history_connect++)
		{
			scanf("%ld %s %s", &index, string, str);
			strcat(string, " ");
			strcat(string, str);
			if (tree == NULL) tree = init_tree_head(string, index);
			else add_to_tree(tree, string, index);
		}
		for (history_connect = 0; history_connect < heap_pointer; history_connect++)
		{
			sscanf(heap[history_connect].name, "%s %s", str, reverse);
			strcat(reverse, " ");
			strcat(reverse, str);
			index = search_in_tree(tree, reverse);
			if (index != -1)
			{
				printf("%ld %ld\n", history_connect+1, index);
				goto END;
			}
		}
		printf("%d\n", Answer);
END:		tree = NULL;
		heap_pointer = 0;
	}

	return 0;//Your program should return 0 on normal termination.
}

