#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*  NarmeenMousa_1191656_p2_sec2 */

// Define the structure for the linked list node
typedef struct ListNode {
    char equation[100];
    struct ListNode *next;
} ListNode;

// Define the structure for the linked list
typedef struct {
    ListNode *head;
} LinkedList;

/* Stack structure */
struct Stack {
    int top;
    unsigned capacity;
    int* array;
};

// Define the structure for the expression tree node
typedef struct TreeNode {
    char data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

///prototype function
void readEquationsFromFile(LinkedList *list);
void printEquations(LinkedList *list);
struct Stack* createStack(unsigned capacity) ;
void push(struct Stack* stack, int item);
int pop(struct Stack* stack);
int top(struct Stack* stack);
int isEmpty(struct Stack* stack);
int precedence(char x);
void infixToPostfix(char equation[], char result[]);
void saveToOutputFile(LinkedList *list, const char *fileName);
TreeNode* createTreeNode(char data);
TreeNode* buildExpressionTree(char infix[]);
int evaluateExpressionTree(TreeNode* root);
void evaluateAndPrint(LinkedList* list);

// Function to read equations from a file and store them in a linked list
void readEquationsFromFile(LinkedList *list) {
    char filename[100];

    // Prompt the user to enter the filename
    printf("Enter the filename to read equations from: ");
    scanf("%s", filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s.\n", filename);
        return;
    }

    char equation[100];
    while (fgets(equation, sizeof(equation), file) != NULL) {
        // Ignore empty lines
        if (strcmp(equation, "\n") == 0 || strcmp(equation, "\r\n") == 0) {
            continue;
        }

        // Remove unwanted characters from the equation
        char sanitizedEquation[100];
        int j = 0;
        for (int i = 0; equation[i] != '\0'; i++) {
            if (isalnum(equation[i]) || equation[i] == '+' || equation[i] == '-' || equation[i] == '*' || equation[i] == '/' || equation[i] == '%' || equation[i] == '(' || equation[i] == ')') {
                sanitizedEquation[j++] = equation[i];
            }
        }
        sanitizedEquation[j] = '\0';

        // Add the sanitized equation to the end of the linked list
        ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
        strcpy(newNode->equation, sanitizedEquation);
        newNode->next = NULL;

        if (list->head == NULL) {
            // If the list is empty, set the new node as the head
            list->head = newNode;
        } else {
            // Find the last node and append the new node
            ListNode *current = list->head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    printf("Equations have been read successfully from %s\n", filename);

    fclose(file);
}

// Function to print equations from the linked list with equation numbers
void printEquations(LinkedList *list) {
    ListNode *current = list->head;
    int equationNumber = 1; // Initialize equation number

    while (current != NULL) {
        printf("Eq.%d: %s\n", equationNumber, current->equation);
        current = current->next;
        equationNumber++;
    }
}

/* Create a new stack */
struct Stack* createStack(unsigned capacity) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

/* Add an element to the stack */
void push(struct Stack* stack, int item) {
    stack->array[++stack->top] = item;
}

/* Extract an element from the stack and return its value */
int pop(struct Stack* stack) {
    if (isEmpty(stack))
        return 0;
    return stack->array[stack->top--];
}

/* Peek at the top of the stack without extracting the element */
int top(struct Stack* stack) {
    if (isEmpty(stack))
        return 0;
    return stack->array[stack->top];
}

/* Check if the stack is empty */
int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

/* Get the priority of an operator */
int precedence(char x) {
    if (x == '-' || x == '+') {
        return 0;
    }

    if (x == '*' || x == '/' || x == '%') {
        return 1;
    }

    if (x == '^') {
        return 2;
    }
    return -1;
}

/* Convert infix expression to postfix */
void infixToPostfix(char equnation[], char result[]) {
    int index = 0;
    struct Stack* stack = createStack(strlen(equation));

    for (int i = 0; i < strlen(equation); i++) {
        if ((equation[i] == '+' || equation[i] == '-') && (i == 0 || equation[i - 1] == '(' || equation[i - 1] == '+' || equation[i - 1] == '-' || equation[i - 1] == '*' || equation[i - 1] == '/' || equation[i - 1] == '%' || equation[i - 1] == '^')) {
            // Handle positive/negative sign at the beginning of a number
            result[index] = equation[i];
            index++;

            // Check if the next character is a digit
            if (i < strlen(equation) - 1 && isdigit(equation[i + 1])) {
                // Append the digit to the result
                result[index] = equation[i + 1];
                index++;

                // Move to the next character
                i++;
            }

            result[index] = ' ';
            index++;
        } else if (equation[i] >= '0' && equation[i] <= '9') {
            // Process digits
            result[index] = equation[i];
            index++;

            if (i < strlen(equation) - 1 && (equation[i + 1] < '0' || equation[i + 1] > '9')) {
                result[index] = ' ';
                index++;
            }

            if (i == strlen(equation) - 1) {
                result[index] = ' ';
                index++;
            }
        } else if (equation[i] == '+' || equation[i] == '-' || equation[i] == '*' || equation[i] == '/' || equation[i] == '%' || equation[i] == '^') {
            // Process operators
            if (isEmpty(stack) || stack->array[stack->top] == '(' || stack->array[stack->top] == '[' || stack->array[stack->top] == '{') {
                push(stack, equation[i]);
            } else {
                while (!isEmpty(stack) && precedence(equation[i]) <= precedence(top(stack))) {
                    result[index] = pop(stack);
                    index++;
                    result[index] = ' ';
                    index++;
                }
                push(stack, equation[i]);
            }
        } else if (equation[i] == '(' || equation[i] == '[' || equation[i] == '{') {
            // Process opening parentheses
            push(stack, equation[i]);
        } else if (equation[i] == ')' || equation[i] == ']' || equation[i] == '}') {
            // Process closing parentheses
            while (!isEmpty(stack) && stack->array[stack->top] != '(' && stack->array[stack->top] != '[' && stack->array[stack->top] != '{') {
                result[index] = pop(stack);
                index++;
                result[index] = ' ';
                index++;
            }
            if (!isEmpty(stack) && stack->array[stack->top] != '(' && stack->array[stack->top] != '[' && stack->array[stack->top] != '{') {
                return; // Error: mismatched parentheses
            } else {
                pop(stack);
            }
        }
    }

    // Process any remaining operators in the stack
    while (!isEmpty(stack)) {
        result[index] = pop(stack);
        index++;
        result[index] = ' ';
        index++;
    }

    result[index] = '\0';
}

int evaluate(char postfix[]) {
    struct Stack* stack = createStack(strlen(postfix));

    for (int i = 0; postfix[i] != '\0'; i++) {
        if (isdigit(postfix[i]) || (postfix[i] == '-' && isdigit(postfix[i + 1]))) {
            // If it's an operand (including negative numbers), convert the substring to an integer and push it onto the stack
            int sign = 1;
            if (postfix[i] == '-') {
                // Handle negative sign
                sign = -1;
                i++;  // Move to the next character
            }

            int operand = 0;
            while (isdigit(postfix[i])) {
                operand = operand * 10 + (postfix[i] - '0');
                i++;
            }
            push(stack, sign * operand);
        } else if (postfix[i] == ' ') {
            continue;  // Skip spaces
        } else {
            // Operator encountered
            int operand2 = pop(stack);
            int operand1 = pop(stack);

            switch (postfix[i]) {
                case '+':
                    push(stack, operand1 + operand2);
                    break;
                case '-':
                    push(stack, operand1 - operand2);
                    break;
                case '*':
                    push(stack, operand1 * operand2);
                    break;
                case '/':
                    if (operand2 == 0) {
                        printf("Error: Division by zero is undefined, so I will print 0 to indicate undefined result on this case.\n");
                        return 0;
                    }
                    push(stack, operand1 / operand2);
                    break;
                case '%':
                    if (operand2 == 0) {
                        printf("Error: Modulo by zero is undefined, so I will print 0 to indicate undefined result on this case.\n");
                        return 0;
                    }
                    push(stack, operand1 % operand2);
                    break;
                default:
                    // Handle unknown operators (you can change this behavior based on your requirements)
                    break;
            }
        }
    }

    if (isEmpty(stack)) {
        printf("Error: The expression is incomplete or invalid.\n");
        return 0;
    }

    // The final result is at the top of the stack
    return pop(stack);
}

// Function to evaluate equations using expression tree
void evaluateAndPrintResults(LinkedList *list) {
    ListNode *current = list->head;
    int equationNumber = 1; // Initialize equation number

    while (current != NULL) {
        char infix[100], postfix[100];
        strcpy(infix, current->equation);
        infixToPostfix(infix, postfix);

        // Print the equation number and the original infix expression
        printf("\nEquation.%d: %s\n", equationNumber, current->equation);

        // Print the result of the postfix expression
        int result = evaluate(postfix);
        printf("Result: %d\n", result);

        // Increment the equation number for the next iteration
        equationNumber++;

        current = current->next;
    }
}

// Function to save postfix expressions and results to an output file
void saveToOutputFile(LinkedList *list, const char *fileName) {
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Error creating output file.\n");
        return;
    }

    ListNode *current = list->head;
    while (current != NULL) {
        char infix[100], postfix[100];
        strcpy(infix, current->equation);
        infixToPostfix(infix, postfix);

        // Save original infix expression to the file
        fprintf(file, "Equation: %s\n", current->equation);

        // Save infix and postfix expressions to the file
        fprintf(file, "Infix: %s, Postfix: %s\n", infix, postfix);

        // Save the result of the postfix expression to the file
        int result = evaluate(postfix);
        fprintf(file, "Result: %d\n\n", result);

        current = current->next;
    }

    printf("Results have been saved to %s\n", fileName);

    fclose(file);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////  build tree exprssione ////////////////////////////////////////////////////
// Function to create a tree node
TreeNode* createTreeNode(char data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {
        printf("Error: Failed to allocate memory for a new tree node.\n");
        return NULL;
    }

    // Check if the data is an operator or operand
    if (isdigit(data)) {
        newNode->data = data - '0';  // Convert character to integer
        newNode->left = newNode->right = NULL;
    } else {
        newNode->data = data;
        newNode->left = newNode->right = NULL;
    }

    return newNode;
}


// Function to build an expression tree from infix notation
TreeNode* buildExpressionTree(char infix[]) {
    struct Stack* stack = createStack(strlen(infix));
    TreeNode* root = NULL;

    for (int i = 0; infix[i] != '\0'; i++) {
        if (isdigit(infix[i])) {
            // Convert operand character to integer
            int operandValue = infix[i] - '0';
            TreeNode* operandNode = createTreeNode(operandValue);
            push(stack, operandNode);
        } else if (infix[i] == '(') {
            push(stack, infix[i]);
        } else if (infix[i] == ')') {
            while (stack->top != -1 && stack->array[stack->top] != '(') {
                TreeNode* operatorNode = createTreeNode(pop(stack));
                operatorNode->right = pop(stack);
                operatorNode->left = pop(stack);
                push(stack, operatorNode);
            }
            pop(stack); // Pop '('
        } else {
            while (stack->top != -1 && precedence(infix[i]) <= precedence(top(stack))) {
                TreeNode* operatorNode = createTreeNode(pop(stack));
                operatorNode->right = pop(stack);
                operatorNode->left = pop(stack);
                push(stack, operatorNode);
            }
            push(stack, infix[i]);
        }
    }

    while (stack->top != -1) {
        TreeNode* operatorNode = createTreeNode(pop(stack));
        operatorNode->right = pop(stack);
        operatorNode->left = pop(stack);
        push(stack, operatorNode);
    }

    if (!isEmpty(stack)) {
        root = pop(stack);
    }

    free(stack);
    return root;
}


// Function to evaluate an expression tree
int evaluateExpressionTree(TreeNode* root) {
    if (root == NULL) {
        printf("Error: Empty expression tree.\n");
        return 0;
    }

    if (root->left == NULL && root->right == NULL) {
        return root->data - '0'; // Convert character to integer
    }

    int leftValue = evaluateExpressionTree(root->left);
    int rightValue = evaluateExpressionTree(root->right);

    switch (root->data) {
        case '+': return leftValue + rightValue;
        case '-': return leftValue - rightValue;
        case '*': return leftValue * rightValue;
        case '/': return leftValue / rightValue;
        case '%': return leftValue % rightValue;
    }

    return 0;
}


// Function to evaluate equations using expression tree and print results
void evaluateAndPrint(LinkedList* list) {
    ListNode* current = list->head;

    while (current != NULL) {
        char infix[100], postfix[100];
        int equationNumbe = 1;
        strcpy(infix, current->equation);
                // Check if the equation is empty
        if (infix[0] == '\0') {
            printf("The equation is empty.\n");
            continue;
        }
        infixToPostfix(infix, postfix);

        // Build expression tree from postfix notation
        TreeNode* expressionTree = buildExpressionTree(postfix);
        // Evaluate expression tree
        int result = evaluateExpressionTree(expressionTree);
        printf("Result: %d\n\n", result);

        // Move to the next equation
        current = current->next;
    }
}


/* Main function */
int main() {
    int choice;
    LinkedList equationList;
    equationList.head = NULL;
    char fileName[100];

    do {
        printf("\n********************** Menu **********************:\n");
        printf("1. Read equations from file\n");
        printf("2. Print equations\n");
        printf("3. Evaluate using Expression tree\n");
        printf("4. Print postfix expressions\n");
        printf("5. Save to output file (postfix and results)\n");
        printf("6. Exit\n");
        printf("\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                readEquationsFromFile(&equationList);
                break;

            case 2:
                printf("\nEquations read from file:\n");
                printEquations(&equationList);
                break;

            case 3:
                // Add code to evaluate equations
                evaluateAndPrintResults(&equationList);
                break;

            case 4:
                // Add code to print postfix expressions
                {
                    ListNode *current = equationList.head;
                    while (current != NULL) {
                        char infix[100], postfix[100];
                        strcpy(infix, current->equation);
                        infixToPostfix(infix, postfix);
                        printf("\nEquation: %s", current->equation);
                        printf("\nInfix: %s, Postfix: %s\n", infix, postfix);
                        current = current->next;
                    }
                }
                break;

            case 5:
            // Add code to save to output file
            {
                printf("Enter the output file name: ");
                scanf("%s", fileName);
                saveToOutputFile(&equationList, fileName);
            }
                break;

            case 6:
                printf("Exiting the program.\n");
                break;

            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 6);

    // Free allocated memory for the linked list nodes
    ListNode *current = equationList.head;
    while (current != NULL) {
        ListNode *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
