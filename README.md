# P232
A C  preprocessor to perform given arithmetic operations on matrices.

## Running the project
- Use a linux environment
- `./p232 myCprog.c`

## Given Arithmetic Operations
- `@int B(10)` Declares 1D integer array B with 10 elements
- `@int A(3,5)` Declares 2D integer array A with 3x5 elements 
- `@read B < f1` Reads all elements of an array from a file
- `@copy C = A` Copies all elements of an array to another array.
- `@init A = 2` Initializes all elements of an array to a value
- `@print A` Prints the array elements on the screen
- `@dotp C = A . B` Calculates the dot product of two 1D arrays and assigns the result to the preprocessor variable P_dot
- `@add C = A + B` Adds two arrays and assigns the result to an array
- `@mult C = A * B` Multiplies two 2D arrays and assigns the result to a 2D array
- `@sum A` Sums all elements of an array and assigns the result to the preprocessor variable P_sum
- `@aver A` Calculates the average of all elements in an array and assigns the result to the preprocessor variable P_aver

## Example
- myCprog
````````
#include “P_header.h”
int main()
{
  @int X(10)
  @init X = 0
  @print X
  return 0;
}
````````
- expanded.c
````````
#include “P_header.h”
int main()
{
  int X[10];
  for (int i=0; i<10; ++i)
  X[i]=0;
  for (int i=0; i<10; ++i)
  printf(“%d “, X[i]);
  printf(“\n”);
  return 0;
}
````````
