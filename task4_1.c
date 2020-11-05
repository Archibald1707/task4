#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Error
{
    FILE_NAME = 1,
    FILE_OPEN,
    TMP_FILE_REMOVE
};

char* getFileString(FILE* file)
{
    int memoryUsage = 20, argument = 0;
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

void fileCopy(FILE* original, FILE* copy)
{
    char temporary;
    while ((temporary = fgetc(original)) != EOF)
        fputc(temporary, copy);
    fseek(original, 0L, SEEK_SET);
    fseek(copy, 0L, SEEK_SET);
}

int stringAmount(char* compared, FILE* file)
{
    char *temporaryString;
    int amount = 0;
    while ((temporaryString = getFileString(file)))
    {
        if ((strcmp(compared, temporaryString)) == 0)
            ++amount;
    }
    fseek(file, 0L, SEEK_SET);
    return amount;
}

int main(int argc, char** argv)
{
    int amountOfString, maximumOfString = 0;
    char *temporaryInputString;
    char *frequentString;
    FILE *inputFile;
    FILE *temporaryFile;

    if (argc > 2)
        exit(FILE_NAME);

    inputFile = fopen(argv[1], "r");
    if (inputFile == NULL)
        exit(FILE_OPEN);

    temporaryFile = fopen("temporaryFile.txt", "w+");
    if (temporaryFile == NULL)
        exit(FILE_OPEN);

    fileCopy(inputFile, temporaryFile);

    while ((temporaryInputString = getFileString(inputFile)))
    {
        amountOfString = stringAmount(temporaryInputString, temporaryFile);
        if (amountOfString >= maximumOfString)
        {
            maximumOfString = amountOfString;
            frequentString = temporaryInputString;
        }
    }
    fclose(temporaryFile);
    fclose(inputFile);

    if ((remove("temporaryFile.txt")) == -1)
        exit(TMP_FILE_REMOVE);

    printf("The most frequent string is: %s\n", frequentString);
    free(frequentString);
    remove(argv[1]);
    return 0;
}
