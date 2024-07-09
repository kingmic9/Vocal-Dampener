#include <stdio.h>
#include <stdlib.h>


// Declare a function that will be used in the main routine
void remvocals(FILE* inf, FILE* outf);


/* 
Main routine

Take in a .wav file name as the first parameter, and take in where to write the resultant .wav file name
with the vocals removed as the second parameter.
*/
int main(int argc, char *argv[]) {
    // Check if two command-line arguments are provided, if not issue an error message. 
    if (argc != 3) {
        printf("%d arguments provided.\nPlease input two filenames, the sourcefile and the destination file.\n", argc);
        exit(EXIT_FAILURE);
    }


    // Prepare the input file
    FILE* input_file = fopen(argv[1], "rb");
    // check that the File pointer is not null, i.e. no errors have occured. If an error occurs, issue an error message.
    if  (input_file == NULL) {
	printf("Input file %s is invalid or does not exist\n", argv[1]);}


    // Prepare the output file
    FILE* output_file = fopen(argv[2], "wb");
    // This file arleady exists / or it will be overwritten -> all set to use already


    // Remove the vocals from one file and write its result to the output file
    remvocals(input_file, output_file);
    

    // Close the files
    fclose(input_file);
    fclose(output_file);


    // Terminate the program
    return EXIT_SUCCESS;
    }


void remvocals(FILE* inf, FILE* outf) {
	// Copy the header of the .wav file (first 44 bytes) into the output file
	void* buf = malloc(1);
    // Allocate one byte space, then read from header and write to output 44 times. 
	for (int i = 0; i < 44; i++) {
		fread(buf, 1, 1, inf);
		fwrite(buf, 1, 1, outf);}
	free(buf); 
    // free the byte buffer


	/* 
    Treat the rest of the input file as a sequence of shorts. 
    Take each pair of shorts left and right, and compute combined = (left - right) / 2. 
    Write two copies of combined to the output file.
    This process will effectively remove what is common within the sound channels, which will dampen the vocals in a well-formatted file. 
    */


    // Allocate space for two shorts
	short* left_n_right = (short*) malloc(2 * sizeof(short));
	int j = 2;

    // ensure that a short pair is always processed
	while (j == 2) {
		j = fread(left_n_right, 2, 2, inf); // Read 2 items of size 2 bytes each from inf
		// Now perform the computation(s)
		short left = left_n_right[0];
		short right = left_n_right[1];
		left = (left - right) / 2;
		left_n_right[0] = left;
		left_n_right[1] = left;
		// The values in left_n_right should now be those of (left - right) / 2
		// Now we write the dampened result.
		fwrite(left_n_right, 2, j, outf);
	}
	// Deallocate used memory
	free(left_n_right);
	// This should now yield the removed vocals as needed.
}

