#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ext/json.c"
#include "ext/ansi.c"
#define ARBITRARY_LIMIT 500

// stack functions
char *s_create(int);
int s_is_empty(int);
int s_is_full(int, int);
void s_push(char *, int, int *, char);
char s_pop(char *, int *);

// array item remove and shift algo
void a_remove_item(int *, struct QuizQuestion *, int);

// misc
int randint(int, int);
int abcd_to_1234(char);
char _1234_to_abcd(int a);

int main()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    print_banner();
    printf("%s", FORMAT_STRING_TO_ANSI(ANSI_GREEN, ANSI_BOLD, "Parsing Quiz data from the json file...\n"));
    struct JSONParseResult res = parse("data.json");
    char *buffer = malloc(ARBITRARY_LIMIT * sizeof(char));
    sprintf(buffer, "%sParsed %s%d%s%s questions from data.json%s\n", ANSI_GREEN, ANSI_BOLD, res.count, ANSI_RESET, ANSI_GREEN, ANSI_RESET);
    printf("%s\n", buffer);
    int score = 0, q_size, t_top = -1;
    double total_time;
    printf("%s", FORMAT_STRING_TO_ANSI(ANSI_BLUE, ANSI_BOLD, "Enter the number of questions to ask: "));
    scanf("%i", &q_size);

    struct QuizQuestion *questions = malloc(q_size * sizeof(struct QuizQuestion));
    double *res_time = malloc(q_size * sizeof(double));
    char *responses = s_create(q_size);

    printf("%s", FORMAT_STRING_TO_ANSI(ANSI_CYAN, ANSI_BOLD, "________________________________________|\n\n"));
    for (int ques = 0; ques < q_size; ques++)
    {
	int choice = randint(0, res.count - 1);
	char answer = 'z';
	struct QuizQuestion curr = res.questions[choice];

	printf("%s%sQ%d: %s%s\n", ANSI_GREEN, ANSI_BOLD, ques + 1, curr.question, ANSI_RESET);
	printf(" %s%sa. %s%s\n", ANSI_YELLOW, ANSI_BOLD, curr.option1, ANSI_RESET);
	printf(" %s%sb. %s%s\n", ANSI_YELLOW, ANSI_BOLD, curr.option2, ANSI_RESET);
	printf(" %s%sc. %s%s\n", ANSI_YELLOW, ANSI_BOLD, curr.option3, ANSI_RESET);
	printf(" %s%sd. %s%s\n", ANSI_YELLOW, ANSI_BOLD, curr.option4, ANSI_RESET);
	printf("%s", FORMAT_STRING_TO_ANSI(ANSI_MAGENTA, ANSI_BOLD, "Your choice (a, b, c, d): "));
	time_t start = time(NULL);
	while (answer != 'a' && answer != 'b' && answer != 'c' && answer != 'd' && answer != 'A' && answer != 'B' && answer != 'C' && answer != 'D')
	{
	    scanf(" %c", &answer);
	    if (answer == '\n')
	    {
		continue;
	    }
	}
	time_t end = time(NULL);
	double time_taken = difftime(end, start);
	responses[++t_top] = answer;
	// s_push(responses,q_size, &t_top, answer);
	res_time[t_top] = time_taken;
	questions[t_top] = curr;
	printf("%sSelected answer: %s%c (%.2f seconds)%s\n", ANSI_BLUE, ANSI_BOLD, answer, time_taken, ANSI_RESET);

    }
    printf("%s", FORMAT_STRING_TO_ANSI(ANSI_CYAN, ANSI_BOLD, "________________________________________|\n\n"));
    printf("%s", FORMAT_STRING_TO_ANSI(ANSI_GREEN, ANSI_BOLD, "=================ANALYSIS=================\n\n"));
    for (int i = 0; i < q_size; i++)
    {
	printf("%s%sQ%i: %s%s\n", ANSI_BLUE, ANSI_BOLD, i + 1, questions[i].question, ANSI_RESET);
	char correct[100], response[100];
	struct QuizQuestion q = questions[i];
	switch (q.answer)
	{
	    case 1:
		strcpy(correct, q.option1);
		break;
	    case 2:
		strcpy(correct, q.option2);
		break;
	    case 3:
		strcpy(correct, q.option3);
		break;
	    case 4:
		strcpy(correct, q.option4);
		break;
	}
	switch (abcd_to_1234(responses[i]))
	{
	    case 1:
		strcpy(response, q.option1);
		break;
	    case 2:
		strcpy(response, q.option2);
		break;
	    case 3:
		strcpy(response, q.option3);
		break;
	    case 4:
		strcpy(response, q.option4);
		break;
	}
	printf("%s%sCorrect: (%c) %s%s\n", ANSI_GREEN, ANSI_BOLD, _1234_to_abcd(q.answer), correct, ANSI_RESET);
	printf("%s%s[%.2f secs] Your Answer: (%c) %s%s\n", abcd_to_1234(responses[i]) == q.answer ? ANSI_GREEN : ANSI_RED, ANSI_BOLD, res_time[i], responses[i], response, ANSI_RESET); 
	if (abcd_to_1234(responses[i])==q.answer) score+=1;
	total_time += res_time[i];

    }
    printf("%s%sScore: %i/%i\nTotal Time: %.2f\nAverage Time: %.2f%s\n", ANSI_CYAN, ANSI_BOLD, score, q_size, total_time, total_time/q_size, ANSI_RESET);

}

int randint(int min, int max)
{
    srand(time(NULL));
    return (rand() % (max - min + 1)) + min;
}

int abcd_to_1234(char ch)
{
    switch (ch)
    {
	case 'A':
	case 'a':
	    return 1;
	case 'B':
	case 'b':
	    return 2;
	case 'C':
	case 'c':
	    return 3;
	case 'D':
	case 'd':
	    return 4;
    }
    return -1;
}

char _1234_to_abcd(int a)
{
    if (a >= 1 && a <= 4)
    {                               // Ensure a is within valid range
	return (char)(a + 'a' - 1); // Convert to lowercase letters
    }
    return '?'; // Return '?' for invalid input
}

char *s_create(int size)
{
    return malloc(size * sizeof(char));
}

int s_is_empty(int top)
{
    return top == -1;
}

int s_is_full(int size, int top)
{
    return size == top;
}

void s_push(char *stack, int size, int *top, char data)
{
    if (s_is_full(size, *top))
    {
	printf("stack overflow");
	return;
    }
    stack[++*top] = data;
}

char s_pop(char *stack, int *top)
{
    if (s_is_empty(*top))
    {
	printf("stack underflow");
	return -1;
    }
    --*top;
    return stack[*top + 1];
}

void a_remove_item(int *size, struct QuizQuestion *arr, int index)
{
    for (int i = index; i < *size - 1; i++)
    {
	arr[i] = arr[i + 1];
    }
    (*size)--;
}
