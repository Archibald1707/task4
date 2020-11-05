#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Error
{
	FILE_AMOUNT = 1,
	FILE_NAME,
	MAX_LENGTH
};

char* getFileString(FILE* file)
{
    int memoryUsage = 10, argument = 0;
    size_t stringLength;
    char *stringPointer = malloc(10);
    while (fgets(stringPointer + argument,10, file))
    {
        stringLength = strlen(stringPointer);
        if (stringPointer[stringLength - 1] != '\n')
        {
            argument = stringLength;
            memoryUsage += 10;
            stringPointer = realloc(stringPointer, memoryUsage);
            if (stringPointer == NULL)
                break;
        }
        else
        {
            stringPointer[stringLength - 1] = '\0';
            return stringPointer;
        }
    }
    free(stringPointer);
    return NULL;
}

int counterOfString(FILE* file)
{
    int amountOfString = 0;
    char *temporaryString;
    while((temporaryString = getFileString(file)))
	{
        amountOfString++;
        free(temporaryString);
	}
    fseek(file, 0L, SEEK_SET);
    return amountOfString;
}

char* shortestSearch(FILE* file, size_t maxStringLength)
{
    char *shortString;
    char *temporaryString;
    size_t minLength;
    int used = 0;
    minLength = maxStringLength;
    while((temporaryString = getFileString(file)))
    {
        if((strlen(temporaryString) - 1) < minLength)
        {
            minLength = strlen(temporaryString) - 1;
            shortString = temporaryString;
            used++;
        }
        if(!used)
            free(temporaryString);
        used = 0;
    }
    fseek(file, 0L, SEEK_SET);
    return shortString;
}

FILE* deleteString(char *deletableString, FILE* file)
{
    char *temporaryString;
    FILE *temporaryFile;
    FILE *newFile;
    temporaryFile = fopen("temporary.txt", "w+");
    while((temporaryString = getFileString(file)))
    {
        if((strcmp(temporaryString, deletableString)) != 0)
        {
            fputs(temporaryString, temporaryFile);
            fputc('\n', temporaryFile);
        }
        free(temporaryString);
    }
    fclose(file);
    fseek(temporaryFile, 0L, SEEK_SET);
    newFile = fopen("new.txt", "w+");
    while((temporaryString = getFileString(temporaryFile)))
    {
        fputs(temporaryString, newFile);
        fputc('\n', newFile);
        free(temporaryString);
    }
    fseek(newFile, 0L, SEEK_SET);
    fclose(temporaryFile);
    remove("temporary.txt");
    return newFile;
}

int main(int argc, char** argv)
{
    int i, counter;
    size_t maxLength;
    FILE *inputFile;
    FILE *outputFile;
    FILE *neededFile;
    char *shortestString;

    if (argc != 3)
        exit(FILE_AMOUNT);

    maxLength = atoi(argv[2]);
    if(maxLength == 0)
        exit(MAX_LENGTH);

    inputFile = fopen(argv[1], "r+");
    if (inputFile == NULL)
        exit(FILE_NAME);
	
    outputFile = fopen("output_task4_2.txt", "w");
    if(outputFile == NULL)
        exit(FILE_NAME);
    neededFile = inputFile;

    counter = counterOfString(neededFile);
    for (i=0;i<counter;++i)
    {
        shortestString = shortestSearch(neededFile, maxLength);
        fputs(shortestString, outputFile);
        fputc('\n', outputFile);
        neededFile = deleteString(shortestString, neededFile);
        free(shortestString);
    }
    fclose(outputFile);
    fclose(neededFile);
    remove("new.txt");
    return 0;
}
