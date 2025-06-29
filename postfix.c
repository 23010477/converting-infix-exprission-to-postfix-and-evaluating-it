#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

#define RED "\033[0;91m"
#define ORANGE "\033[38;5;214m"
#define YELLOW "\033[0;93m"
#define GREEN "\033[0;92m"
#define BLUE "\033[0;94m"
#define PURPLE "\033[35m"
#define PINK "\033[38;5;200m"
#define RESET "\033[0m"

// Node definition for stack
typedef struct Node {
    double data;  // Changed to double for floating-point support
    struct Node *next;
} Node;

// Stack definition
typedef struct {
    Node *top;
} Stack;

// Stack operations
Stack* initialize() {
    Stack *s = (Stack*)malloc(sizeof(Stack));
    if (!s) {
        printf(RED "Error: Memory allocation failed for stack.\n" RESET);
        exit(EXIT_FAILURE);
    }
    s->top = NULL;
    return s;
}

void push(Stack *s, double value) {
    Node *n = (Node*)malloc(sizeof(Node));
    if (!n) {
        printf(RED "Error: Memory allocation failed for node.\n" RESET);
        exit(EXIT_FAILURE);
    }
    n->data = value;
    n->next = s->top;
    s->top = n;
}

double pop(Stack *s) {
    if (s->top == NULL) {
        printf(RED "Error: Stack underflow\n" RESET);
        exit(EXIT_FAILURE);
    }
    Node *p = s->top;
    double data = p->data;
    s->top = p->next;
    free(p);
    return data;
}

double peek(Stack *s) {
    if (s->top == NULL) {
        printf(RED "Error: Stack is empty\n" RESET);
        exit(EXIT_FAILURE);
    }
    return s->top->data;
}

int isEmpty(Stack *s) {
    return s->top == NULL;
}
// Function to determine operator precedence
int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}
// Function to check if a character is an operator
bool is_operator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}
// Function to convert infix expression to postfix
char* infixToPostfix(char* exp) {
    Stack* s = initialize();
    int i, j = 0;
    char* post = (char*)malloc((strlen(exp) * 2) + 1);
    if (!post) {
        printf(RED "Error: Memory allocation failed.\n" RESET);
        return NULL;
    }
    for (i = 0; exp[i] != '\0'; i++) {
        if (exp[i] == ' ') continue;
        // Handle numbers (including negative and floating-point)
        if (isdigit(exp[i]) || (exp[i] == '-' && (i == 0 || exp[i-1] == '(' || is_operator(exp[i-1])) && isdigit(exp[i+1]))) {
            // Copy the entire number to output
            while (isdigit(exp[i]) || exp[i] == '.' || 
                (exp[i] == '-' && (i == 0 || exp[i-1] == '(' || is_operator(exp[i-1])))) {
                post[j++] = exp[i++];
            }
            post[j++] = ' ';
            i--;
            continue;
        }
        if (exp[i] == '(') {
            push(s, exp[i]);
        }
        else if (exp[i] == ')') {
            while (!isEmpty(s) && peek(s) != '(') {
                post[j++] = pop(s);
                post[j++] = ' ';
            }
            if (isEmpty(s)) {
                printf(RED "Error: Mismatched parentheses.\n" RESET);
                free(post);
                return NULL;
            }
            pop(s); // Remove '('
        }
        else if (is_operator(exp[i])) {
            while (!isEmpty(s) && peek(s) != '(' && 
                priority(peek(s)) >= priority(exp[i])) {
                post[j++] = pop(s);
                post[j++] = ' ';
            }
            push(s, exp[i]);
        }
    }
    // Pop remaining operators
    while (!isEmpty(s)) {
        if (peek(s) == '(') {
            printf(RED "Error: Mismatched parentheses.\n" RESET);
            free(post);
            return NULL;
        }
        post[j++] = pop(s);
        post[j++] = ' ';
    }

    post[j] = '\0';
    return post;
}

// Evaluate postfix expression with floating-point support
double evaluatePostfix(char* exp) {
    Stack *s = initialize();
    int i = 0;
    while (exp[i] != '\0') {
        if (exp[i] == ' ') {
            i++;
            continue;
        }
        // Handle numbers (including negative and floating-point)
        if (isdigit(exp[i]) || (exp[i] == '-' && isdigit(exp[i+1]))) {
            char numStr[32];
            int j = 0;
            // Capture the entire number including sign and decimal point
            while (isdigit(exp[i]) || exp[i] == '.' || 
            (exp[i] == '-' && (j == 0 && (i == 0 || exp[i-1] == '(' || is_operator(exp[i-1]))))) {
                numStr[j++] = exp[i++];
            }
            numStr[j] = '\0';
            // Convert to double and push onto stack
            push(s, atof(numStr));
            continue;
        }
        if (is_operator(exp[i])) {
            if (isEmpty(s)) {
                printf(RED "Error: Not enough operands for operator %c\n" RESET, exp[i]);
                exit(EXIT_FAILURE);
            }
            double b = pop(s);
            
            if (isEmpty(s)) {
                printf(RED "Error: Not enough operands for operator %c\n" RESET, exp[i]);
                exit(EXIT_FAILURE);
            }
            double a = pop(s);
            switch (exp[i]) {
                case '+': push(s, a + b); break;
                case '-': push(s, a - b); break;
                case '*': push(s, a * b); break;
                case '/': 
                if (fabs(b) < 1e-12) {
                    printf(RED "Error: Division by zero.\n" RESET);
                    exit(EXIT_FAILURE);
                }
                push(s, a / b); 
                break;
                case '^': push(s, pow(a, b)); break;
                default:
                printf(RED "Error: Invalid operator %c\n" RESET, exp[i]);
                exit(EXIT_FAILURE);
            }
            i++;
        } else {
            printf(RED "Error: Invalid character '%c' in expression\n" RESET, exp[i]);
            exit(EXIT_FAILURE);
        }
        }
    
    if (isEmpty(s)) {
        printf(RED "Error: No result found\n" RESET);
        exit(EXIT_FAILURE);
    }
    
    double result = pop(s);
    
    if (!isEmpty(s)) {
        printf(RED "Error: Malformed expression - operands remaining\n" RESET);
        exit(EXIT_FAILURE);
    }
    return result;
}

int main() {
    while (1) {
        printf(PURPLE "Welcome to our calculator!\n" RESET); 
        printf(PURPLE "Enter an infix expression (or 'q' to quit): " RESET); 
        char in[256];
        fgets(in, sizeof(in), stdin);
        
        // Remove newline and check for quit command
        in[strcspn(in, "\n")] = '\0';
        if (strcmp(in, "q") == 0 || strcmp(in, "Q") == 0) {
            break;
        }
        
        char *post = infixToPostfix(in);
        if (!post) {
            continue;
        }
        
        printf(GREEN "Postfix expression: %s\n" RESET, post);
        double result = evaluatePostfix(post);
        printf(GREEN "Result: %g\n" RESET, result);
        free(post);
        
        printf(YELLOW "\n**********************************************************\n" RESET);
    }
    return 0;
}