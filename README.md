# Equation-Evaluation-System-

This C program is designed to read equations from a file, process them, and provide various functionalities such as printing equations, evaluating them using expression trees, printing postfix expressions, and saving results to an output file.

## Functionality

### 1. Read Equations from File
- The program prompts the user to enter the filename from which equations are to be read.
- It reads equations from the file, sanitizes them, and stores them in a linked list.

### 2. Print Equations
- Displays the equations that have been read from the file, along with their corresponding equation numbers.

### 3. Evaluate Equations Using Expression Tree
- Converts the infix expressions to postfix notation.
- Builds expression trees from postfix expressions.
- Evaluates the expression trees and prints the results.

### 4. Print Postfix Expressions
- Converts the infix expressions to postfix notation and prints both the infix and postfix forms of the equations.

### 5. Save to Output File (Postfix and Results)
- Prompts the user to enter the output file name.
- Saves the original infix expressions, corresponding postfix expressions, and their evaluated results to the output file.

### 6. Exit
- Terminates the program.

## Usage

1. Compile the program using a C compiler.
2. Run the executable.
3. Choose an option from the menu based on the desired functionality.
4. Follow the prompts or instructions provided by the program.

## Additional Notes

- Equations should be stored in the input file with each equation on a separate line.
- Only basic arithmetic operations (`+`, `-`, `*`, `/`, `%`) are supported.
- The program handles positive/negative signs at the beginning of numbers.
- Division by zero and modulo by zero errors are handled appropriately.
- The program ensures proper handling of parentheses in expressions.

## Requirements
- C compiler 
