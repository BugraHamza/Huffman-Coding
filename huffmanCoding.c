#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Huffman
{
	char letter;
	int frequency;
	struct Huffman *right, *left, *next;
};
typedef struct Huffman HUFFMAN;

HUFFMAN * getFrequencyList(HUFFMAN*, char*);
HUFFMAN * insertTop(HUFFMAN*, char);
HUFFMAN * insetTill(HUFFMAN*, HUFFMAN*);
HUFFMAN * insertionSort(HUFFMAN*);
HUFFMAN * binaryTree(HUFFMAN*);
int countLevels(HUFFMAN*);
void printLevel(HUFFMAN*, int);
void printTree(HUFFMAN*);

int main()
{
	char *str, method[4], temp;
	int N;
	HUFFMAN *Tree = NULL;
	FILE *fp;

	printf("Choose the input method [User/file] : ");
	scanf("%s", method);

	if( strcmp(method, "user") == 0)
	{

		printf("Enter the length of the string : ");	//The length of the string
		scanf("%d", &N);

		str = (char*) malloc( (N + 1) * sizeof(char));	//Allocating
	
		printf("Enter the string : ");		//String is taken
		scanf("%c", &temp);			//Clearing the buffer
		fgets(str, N + 1, stdin);
		
	}

	else
	{
		fp = fopen("input.txt","r");
		fseek(fp, 0, SEEK_END);
		N = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		str = (char*) malloc( (N + 1) * sizeof(char));
		fgets(str, N, fp);
		printf("%s\n", str);
	}

	Tree = getFrequencyList(Tree, str);		//A linked list is created by using frequency list

	Tree = insertionSort(Tree);			//The linked list is sorted by frequency

	Tree = binaryTree(Tree);			//The list is converted to binary tree 

	printTree(Tree);				//The tree is printing 
}

HUFFMAN * getFrequencyList(HUFFMAN *head, char *s)
{
	/*
	 * Adds each character in the string into the linked list
	 * 
	 * Returns head
	 */

	int i = 0;

	while( *(s + i) )
		head = insertTop(head, *(s + i++));
	return head;	
}

HUFFMAN * insertTop(HUFFMAN *head, char inLetter)
{
	/*
	 * If there is no inLetter in the linked list before,
	 * 	adds a new node at the top and sets the frequency as 1
	 *  
	 * Returns head
	 */

	HUFFMAN *newElement, *curr = head;
	if( !head )
	{
		head = (HUFFMAN*)malloc(sizeof(HUFFMAN));
		head -> letter = inLetter;
		head -> frequency = 1;
		head -> next = NULL;
		head -> right = NULL;
		head -> left = NULL;
		return head;
	}

	while( curr -> next && curr -> letter != inLetter)
		curr = curr -> next;

	if(curr -> letter == inLetter)
	{
		curr -> frequency++;
		return head;
	}

        newElement = (HUFFMAN*) malloc(sizeof(HUFFMAN));
	newElement -> letter = inLetter;
        newElement -> frequency = 1;
	newElement -> next = head;
	newElement -> right = NULL;
	newElement -> left = NULL;
	return newElement;
}

HUFFMAN * insertTill(HUFFMAN *head, HUFFMAN *data)
{
	/*
	 * Checks the nodes before node DATA
	 *
	 * After this function has run, 
	 * 	the linked list has been sorted up to node DATA
	 * 
	 * Returns head
	 */

	HUFFMAN *curr = head, *beforeCurr = NULL, *beforeData = head;
	
	while(beforeData -> next && beforeData -> next != data)
		beforeData = beforeData -> next;


	while (curr != data && curr -> frequency <= data -> frequency )
	{
		beforeCurr = curr;
		curr = curr -> next;
	}

	if(curr -> frequency > data -> frequency)
	{
		beforeData -> next = data -> next;

		if(beforeCurr)
		{
			beforeCurr -> next = data;
			data -> next = curr;
		}
		else
		{
			data -> next = curr;
			head = data;
		}
	}

	return head;
}

HUFFMAN * insertionSort(HUFFMAN *head)
{
	/*
	 * An insertion sort implementation
	 * 
	 * Returns head
	 */

	HUFFMAN *currH = head, *currN;

	if(currH -> next)
		currN = currH -> next;
	else return head;
	
	while(currN)
	{
		currH = insertTill(currH, currN);
	
		currN = currN -> next;
	}

	head = currH;
	
	return head;
}

HUFFMAN * binaryTree(HUFFMAN *head)
{
	/*
	 * Converts a linked list to a binary tree
	 * 
	 * Returns the head node of the binary tree
	 */

	HUFFMAN *curr = head, *tail = head, *newNode;
	
	if(!head -> next)
		return head;

	while( curr -> next )
	{
		newNode = (HUFFMAN*) malloc(sizeof(HUFFMAN));
		newNode -> frequency = curr -> frequency + curr -> next -> frequency;
		newNode -> letter = '\0';
		newNode -> right = curr -> next;	
		newNode -> left = curr;
		tail = curr;
		while(tail -> next)
			tail = tail -> next;
		tail -> next = newNode;	
		newNode -> next = NULL;
		curr = insertTill(curr, newNode);
		curr = curr -> next -> next;
	}

	return curr;
}

/*
The following functions are found in the webpage below :
https://stackoverflow.com/questions/37062169/how-to-print-elements-from-binary-tree-by-level-in-c
*/

int countLevels(HUFFMAN *head)
{	
	/*
	 * Counts the levels of a binary tree recursively
	 * 
	 * Returns the height of the tree 
	 */

	int leftLevel, rightLevel;

	if( !head )
		return 0;

	leftLevel = 1 + countLevels(head -> left);
	rightLevel = 1 + countLevels(head -> right);

	return leftLevel > rightLevel ? leftLevel : rightLevel;
}

void printLevel(HUFFMAN *head, int level)
{
	/*
	 * Prints a level in the binary tree
	 * 
	 * If the character is a whitespace '\b' or newline '\n' character,
	 * 		prints the specifiers instead
	 * If any other character,
	 * 	prints the frequency and the letter.
	 * 
	 * NO RETURN
	 */

	if( head && level == 0 )
	{
		if( head -> letter == ' ' )
			printf("%d\\b ", head -> frequency);
		else if( head -> letter == '\n' )
			printf("%d\\n ", head -> frequency);
		else
			printf("%d%c  ", head -> frequency, head -> letter);
		
	}
	else if (head)
	{
		printLevel(head -> left, level - 1);
		printLevel(head -> right, level - 1);
	}
	else
	{
		printf("  ");
	}
}

void printTree(HUFFMAN *head)
{
	/*
	 * Prints the tree level order
	 * 
	 * NO RETURN
	 */

	int i = 0, level = countLevels(head);
	
	for(; i < level; i++)
	{
		printLevel(head, i);
		printf("\n");
	}
}
