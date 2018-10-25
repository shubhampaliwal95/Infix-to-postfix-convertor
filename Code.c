/*
 * Logic in Computer Science Coding Assignment 1
 * Prateek Gupta                2015B4A70482H
 * Shubham Paliwal              2015B2A70xxxH
 * Vishal Chokkla               2015B5A70645H
 */

/*
 * Task 1: Convert the given Infix propositional logic expression to Postfix propositional logic expression.
 * Task 2: Convert the Postfix expression into rooted binary parse tree.
 * Task 3: Traverse the parse tree to output the Infix expression back by in-order traversal of the parse tree.
 */

#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

int iterator=0;
char finalPostfix[101];

// Task 1 implementation starts
struct stack
{
    int operators[101];
    int top;
};

struct stack s;

void push(char p)                                       // Pushes the element in stack
{
    if(s.top==100)
    {
        printf("stack is full\n");
        return;
    }
    else
    {
        s.top=s.top+1;
        s.operators[s.top]=p;
    }

}

char pop()                                              // Returns top most character from stack
{
    if(s.top==-1)
    {
        printf("%s","sorry, stack is empty.\n");
        return -1;
    }
    else
    {
        int temp = s.top;
        s.top = s.top-1;
        return s.operators[temp];
    }
}

int comparePrecedence(char op1, char op2)               // precedence order is ~ * + > )
{
    int prec1, prec2;
    switch(op1)
    {
        case '~': prec1 = 4;break;
        case '*': prec1 = 3;break;
        case '+': prec1 = 2;break;
        case '>': prec1 = 1;break;
        default: prec1 = 0;//for op1 = )
    }
    switch(op2)
    {
        case '~': prec2 = 4;break;
        case '*': prec2 = 3;break;
        case '+': prec2 = 2;break;
        case '>': prec2 = 1;break;
        default: prec2 = 0;//for op2 = )
    }
    return ((prec1>=prec2)?1:0);
}

char peek()                                             // Provides operator for Precedence comparison
{
    if(s.top==-1)
    {
        printf("Stack is empty!");
        return 0;
    }
    return s.operators[s.top];
}

void infixToPostfix(char *infix)                        // Infix to Postfix
{
    char postfix[101];
    int i;
    for (i=0;infix[i]!='\0';i++)
    {
        if(infix[i]=='(')
        {
           push(infix[i]);
        }
        if(isalpha(infix[i]))
        {
            postfix[iterator]=infix[i];
            iterator++;
        }
        if( infix[i] == ')')
        {
            while( s.operators[s.top] != '(')
            {
                postfix[iterator++]=pop();
            }
            char waste=pop();
        }

        else if(infix[i]=='~'||infix[i]=='+'||infix[i]=='*'||infix[i]=='>')
        {
            if(s.top==-1)
            {
                push(infix[i]);
            }
            else
            {
                while(comparePrecedence(peek(),infix[i]))
                {
                    postfix[iterator++] = pop();
                }
                push(infix[i]);
            }

        }
    }
    while(s.top>=0)                                     //non-empty stack
    {
        postfix[iterator++] = pop();
    }
    for( i=0;i<iterator-1;i++)
    {
        finalPostfix[i]=postfix[i];
    }
    finalPostfix[i]='\0';
}
// Task 1 implementation ends.


// Task 2 implementation starts.
typedef struct node
{
    char data;
    struct node* parent;
    struct node* left;
    struct node* right;
    int type;
}node;

typedef struct node* link;

typedef struct root
{
    link l;
}root;

link createNode(char c)                                 // Creates empty node
{
    link l = (link)malloc(sizeof(node));
    l->parent = NULL;
    l->left = NULL;
    l->right = NULL;
    l->data = c;
    if((c>=65&&c<=90)||(c>=97&&c<=122))
    {
        l->type = 0;
    }
    else if(c=='~')
    {
        l->type = 2;
    }
    else
    {
        l->type = 1;
    }
    return l;
}

void addNode(link current, char *postfix, int* length)  // Adds node and links it to parent
{
    if (current->type == 2)
    {
        link r = createNode(postfix[--(*length)]);
        r->parent = current;
        current->right = r;
        addNode(r,postfix,length);
    }
    else if (current->type == 1)
    {
        link r = createNode(postfix[--(*length)]);
        r->parent = current;
        current->right = r;
        addNode(r,postfix,length);
        link l = createNode(postfix[--(*length)]);
        l->parent = current;
        current->left = l;
        addNode(l,postfix,length);
    }
    return;
}

root makeParseTree(int* length, char* postfix)          // Makes the required parse tree
{
    root r;
    link top=createNode(postfix[--(*length)]);
    r.l = top;
    addNode(r.l,postfix,length);
    return r;
}
// Task 2 implementation ends.


// Task 3 implementation starts.
void inOrderTraversal(link l, int* index, char* infix)
{
    if(l!=NULL)
    {
        if(l->type != 0)
        {
            infix[*index] = '(';
            (*index)++;
        }
        inOrderTraversal(l->left,index,infix);
        infix[*index] = l->data;
        (*index)++;
        inOrderTraversal(l->right,index,infix);
        if(l->type != 0)
        {
            infix[*index] = ')';
            (*index)++;
        }
    }
    return;
}
// Task 3 implementation ends.


int main()
{
    char infix[101];
    printf("Enter the expression in infix form\n");
    scanf("%s",infix);
    infixToPostfix(infix);                                  // Task 1
    printf("Postfix form is :%s",finalPostfix);
    int length = strlen(finalPostfix);
    root tree = makeParseTree(&length, finalPostfix);       // Task 2
    char* infixOut = (char*)malloc(100*sizeof(char));
    int infixIndex=0;
    inOrderTraversal(tree.l,&infixIndex,infixOut);                 // Task 3
    infixOut[infixIndex] = '\0';
    printf("\nThe notation is -> %s",infixOut);
    return 0;
}