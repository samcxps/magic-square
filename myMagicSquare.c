#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
    int size;           // dimension of the square
    int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* 
 * Prints magic square for debugging
 */
void printMagicSquare(MagicSquare *magic_square) {
    printf("\nMagic Square:\n");
    for (int i = 0; i < magic_square->size; i++) {
        for (int j = 0; j < magic_square->size; j++) {
            printf("%i", magic_square->magic_square[i][j]);
            printf(",");
        }
        printf("\n");
    } 
}

/* 
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {
    int size;

    printf("Enter a magic square's size (odd integer >=3)\n");
    while(1) {
        scanf("%d", &size);

        // Make sure size is off
        if (size % 2 == 0) {
            printf("Size must be odd.\n");
            exit(1);
        } else if (!(size >= 3)) {  // Make sure size >= 3
            printf("Size must be >= 3.\n");
            exit(1);
        }

        break;
    }

    return size;
} 
   
/*
 * Makes a magic square of size n using the alternate 
 * Siamese magic square algorithm from assignment and 
 * returns a pointer to the completed MagicSquare struct.
 *
 * n the number of rows and columns
 */
MagicSquare *generateMagicSquare(int n) {

    // Allocate memory for MagicSquare struct
    MagicSquare *squarePtr = malloc(sizeof(MagicSquare));
    if (squarePtr == NULL) {
        printf("Can't allocate memory for squarePtr.\n");
        exit(1);
    }

    // Assign squarePtr->size to n 
    squarePtr->size = n;

    //Dynamically allocate a 2D array of size to MagicSquare struct
    squarePtr->magic_square = malloc(sizeof(int*) * squarePtr->size);
    if (squarePtr->magic_square == NULL) {
        printf("Can't allocate memory for magic_square.\n");
        exit(1);
    }
    for (int i = 0; i < squarePtr->size; i++) {
        *(squarePtr->magic_square + i) = malloc(sizeof(int) * squarePtr->size);
        if (*(squarePtr->magic_square + i) == NULL) {
            printf("Can't allocate memory for magic_square[%i].\n", i);
            exit(1);
        }
    }
    
    // Since we are using malloc(), init all spaces in 2d array to 0
    for (int i = 0; i < squarePtr->size; i++) {
        for (int j = 0; j < squarePtr->size; j++) {
	    *(*(squarePtr->magic_square + i) + j) = 0;	
	} 
    
    }

    // Start at top mid row for siamese method    
    int row = 0;
    int col = n / 2;

    // Always start in the same spot so we do this outside loop
    *(*(squarePtr->magic_square + row) + col) = 1;

    // Need to keep track of old row and col in case new row and col are already full
    int oldRow;
    int oldCol;
    for (int i = 2; i <= n*n; i++) {
        // Keeping track of row and col before we change them
        oldRow = row;
        oldCol = col;

        // If we are at first row, we want to goto last row
        if (row == 0) {
            row = n - 1;
        } else {  // Else we decrement to goto row above
            row--;
        }

        // If we are at last column, we want to goto first column
        if (col == n - 1) {
            col = 0;
        } else {  // Else we increment col for next column
            col++;
        }

        // Continously check if new row,col is full until we find empty one
        // If the new row,col is already full, we want to go down vertically 
        //    one box but stay in same column
        while (*(*(squarePtr->magic_square + row) + col) > 0) {

            // If already in last row, goto first row
            if (row == n - 1) {
                row = 0;
            } else {  // Else go "down" 1 row by incrementing
                row = oldRow + 1;
            }

            // col does not need to change because we want to stay in same column
            col = oldCol;
        }

        // After correct row and col have been found, we store next number
        *(*(squarePtr->magic_square + row) + col) = i;
    }

    return squarePtr;    
} 

/*
 * Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {

    //Create the file and check if it was created successfully.
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Can't create file for writing.\n");
        exit(1);
    }

    // Write size to file
    fprintf(fp, "%i\n", magic_square->size);

    // Write square to file
    for (int row = 0; row < magic_square->size; row++) {
        for (int col = 0; col < magic_square->size; col++) {
            fprintf(fp, "%i", *(*(magic_square->magic_square + row) + col));
            if (col < magic_square->size - 1) {
                fprintf(fp, ",");
            }
        }
        fprintf(fp, "\n");
    }

    //Close the file.
    if (fclose(fp) != 0) {
        printf("Error while closing the file.\n");
        exit(1);
    } 
}

/*
 * Generates a magic square of the user specified size and
 * output the quare to the output filename
 */
int main(int argc, char *argv[]) {

    //Check if number of command-line arguments is correct.
    if (argc != 2 ) {
        printf("Usage: ./myMagicSquare <output_filename>\n");
        exit(1);
    }

    // Get magic square's size from user
    int size = getSize();
    
    // Generate the magic square
    MagicSquare *square = generateMagicSquare(size);

    // Output the magic square
    fileOutputMagicSquare(square, *(argv + 1));

    // Free all dynamically allocated memory (square and square->magic_square)
    for (int i = 0; i < size; i++) {
        free(*(square->magic_square + i));
        *(square->magic_square + i) = NULL;
    }
    free(square->magic_square);
    square->magic_square = NULL;
    free(square);
    square = NULL;

    return 0;
}
