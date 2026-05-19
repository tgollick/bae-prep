#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define STACK_SIZE 20

int contents[STACK_SIZE];
int top = 0;

void make_empty(void);
bool is_empty(void);
bool is_full(void);
void push(char c);
int pop(void);
void stack_underflow(void);
void stack_overflow(void);

int main(void)
{
    // Test a valid set of parentheses
    printf("Please enter a string of parenteses: ");
    int ch;

    while ((ch = getchar()) != '\n')
    {
        if (ch == '(' || ch == '{')
        {
            push(ch);
        }
        else if (ch == ')')
        {
            if (pop() != '(')
            {
                printf("Invalid Parenteses!\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (ch == '}')
        {
            if (pop() != '{')
            {
                printf("Invalid Parenteses!\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (is_empty())
    {
        printf("Valid Parentheses!\n");
    }
    else
    {
        printf("Invalid Parentheses!\n");
    }
}

void make_empty(void)
{
    top = 0;
}

bool is_empty(void)
{
    return top == 0;
}

bool is_full(void)
{
    return top == STACK_SIZE;
}

void push(char c)
{
    if (is_full())
    {
        stack_overflow();
    }
    else
    {
        contents[top++] = c;
    }
}

int pop(void)
{
    if (is_empty())
    {
        stack_underflow();
        return 0;
    }
    else
    {
        return contents[--top];
    }
}

void stack_overflow(void)
{
    printf("Your stack is full! Stack overflow error! ABORTING");
    exit(EXIT_FAILURE);
}

void stack_underflow(void)
{
    printf("Your stack is empty! Stack underflow error! ABORTING");
    exit(EXIT_FAILURE);
}
