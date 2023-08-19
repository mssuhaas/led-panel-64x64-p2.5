#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ELEMENTS 100 // You can adjust this based on your needs

char **parseStringToArray(const char *inputString, int *numElements)
{
    char **array = NULL;
    *numElements = 0;

    // Find the position of the opening and closing brackets
    char *openBracket = strchr(inputString, '[');
    char *closeBracket = strchr(inputString, ']');

    if (openBracket == NULL || closeBracket == NULL || openBracket >= closeBracket)
    {
        return NULL; // Invalid format
    }

    // Copy the content between brackets into a new string
    int contentLength = closeBracket - openBracket - 1;
    char *content = (char *)malloc(contentLength + 1);
    if (content == NULL)
    {
        return NULL; // Memory allocation error
    }
    strncpy(content, openBracket + 1, contentLength);
    content[contentLength] = '\0';

    // Tokenize the content by commas
    char *token = strtok(content, ",");
    while (token != NULL)
    {
        // Remove single quotes from the token
        char *cleanedToken = strdup(token + 1);
        if (cleanedToken[strlen(cleanedToken) - 1] == '\'')
        {
            cleanedToken[strlen(cleanedToken) - 1] = '\0';
        }

        // Allocate memory for the new element
        if (*numElements == 0)
        {
            array = (char **)malloc(sizeof(char *));
        }
        else
        {
            array = (char **)realloc(array, (*numElements + 1) * sizeof(char *));
        }

        if (array == NULL)
        {
            free(content);
            return NULL; // Memory allocation error
        }

        // Allocate memory for the cleaned element's content and copy the cleaned token
        array[*numElements] = cleanedToken;
        if (array[*numElements] == NULL)
        {
            // Handle memory allocation error
            free(content);
            for (int i = 0; i < *numElements; i++)
            {
                free(array[i]);
            }
            free(array);
            return NULL;
        }

        (*numElements)++;
        token = strtok(NULL, ",");
    }

    free(content);
    return array;
}

int main()
{
    const char *inputString = "['Timestamp','Node_Status','Light_Status','Rsl_Out','Latency','Data_Rate','Mac_Address','Packet_Size','Rsl_In','Etx','Rpl_Rank','Mac_tx_failed_count','Mac_tx_count','Neighbour']";

    int numElements;
    char **parsedArray = parseStringToArray(inputString, &numElements);

    if (parsedArray != NULL)
    {
        printf("Parsed array:\n");
        for (int i = 0; i < numElements; i++)
        {
            printf("%s\n", parsedArray[i]);
            free(parsedArray[i]); // Free each element's memory
        }
        free(parsedArray); // Free the array itself
    }
    else
    {
        printf("Parsing failed\n");
    }

    return 0;
}
