#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 256

// Function prototypes
void injectBytesIntoBinary();
void displayMenu();
void exitTool();

// Function to inject bytes into the binary at a specified offset
void injectBytesIntoBinary() {
    FILE *inputFile, *outputFile;
    char inputFileName[MAX_INPUT_SIZE];
    char outputFileName[MAX_INPUT_SIZE];
    unsigned char *fileData;
    size_t fileSize;
    size_t injectOffset;
    size_t injectSize;
    unsigned char injectData[MAX_INPUT_SIZE];
    
    // Get user input for file and injection details
    printf("Enter the name of the binary file to modify: ");
    fgets(inputFileName, sizeof(inputFileName), stdin);
    inputFileName[strcspn(inputFileName, "\n")] = 0;  // Remove newline

    // Open the binary file for reading
    inputFile = fopen(inputFileName, "rb");
    if (inputFile == NULL) {
        printf("Error: Could not open file %s\n", inputFileName);
        return;
    }

    // Get file size
    fseek(inputFile, 0, SEEK_END);
    fileSize = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);
    
    // Allocate memory to read the file data
    fileData = (unsigned char *)malloc(fileSize);
    if (fileData == NULL) {
        printf("Error: Memory allocation failed.\n");
        fclose(inputFile);
        return;
    }
    
    // Read the file into memory
    fread(fileData, 1, fileSize, inputFile);
    fclose(inputFile);

    // Prompt user for injection details
    printf("Enter the byte sequence to inject (max %d bytes, in hex, e.g., 90 90 90): ", MAX_INPUT_SIZE);
    fgets((char *)injectData, sizeof(injectData), stdin);
    char *token = strtok((char *)injectData, " ");
    injectSize = 0;
    while (token != NULL) {
        sscanf(token, "%2hhx", &injectData[injectSize]);
        injectSize++;
        token = strtok(NULL, " ");
    }

    // Ask for the injection offset
    printf("Enter the offset (position in the file to inject the bytes, in decimal):");
    scanf("%zu", &injectOffset);
    
    // Check if the offset is valid
    if (injectOffset > fileSize) {
        printf("Error: Offset is beyond the file size.\n");
        free(fileData);
        return;
    }

    // Create the output file name
    snprintf(outputFileName, sizeof(outputFileName), "modified_%s", inputFileName);

    // Open the output file for writing
    outputFile = fopen(outputFileName, "wb");
    if (outputFile == NULL) {
        printf("Error: Could not create output file %s\n", outputFileName);
        free(fileData);
        return;
    }

    // Write the data before the injection point
    fwrite(fileData, 1, injectOffset, outputFile);

    // Write the injected bytes
    fwrite(injectData, 1, injectSize, outputFile);

    // Write the remaining data after the injection point
    fwrite(fileData + injectOffset, 1, fileSize - injectOffset, outputFile);

    // Clean up
    fclose(outputFile);
    free(fileData);

    printf("Injection complete! The modified binary is saved as %s\n", outputFileName);
}

// Display the main menu
void displayMenu() {
    int userChoice;
    
    printf("\nBinary Injection Tool\n");
    printf("1. Inject Bytes into Binary\n");
    printf("2. Exit\n");
    
    printf("Enter your choice: ");
    scanf("%d", &userChoice);
    getchar();  // Consume newline character

    switch (userChoice) {
        case 1:
            injectBytesIntoBinary();
            break;
        case 2:
            exitTool();
            break;
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

// Exit the program
void exitTool() {
    printf("Exiting the Binary Injection Tool.\n");
    exit(0);
}

// Main program loop
int main() {
    while (1) {
        displayMenu();
    }
    return 0;
}
