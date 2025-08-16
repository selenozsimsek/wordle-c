#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define RESET "\x1b[0m"
#define BG_GREEN "\x1b[42m"
#define BG_YELLOW "\x1b[43m"
#define BG_GRAY "\x1b[100m"
#define UNDERLINE_ON  "\x1b[4m"
#define UNDERLINE_OFF "\x1b[24m"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, "Usage example: %s wordlist.txt\n", argv[0]);
        return -1;
    }

    printf("Welcome to Wordle!\n");
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) 
    {
        fprintf(stderr, "Error: Could not open %s\n", argv[1]);
        return -1;
    }

    int capacity = 25; 
    char **words = malloc(capacity * sizeof(char *));
    if (words == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        fclose(file);
        return -1;
    }
    char word[6];
    int i = 0; // size
    srand(time(NULL));
    while(fscanf(file, "%5s", word) != EOF)
    {
        words[i] = malloc(6 * sizeof(char)); 
        if (words[i] == NULL) 
        {
            fprintf(stderr, "Error: Memory allocation failed for word.\n");
            for (int j = 0; j < i; j++) 
            {
                free(words[j]);
            }
            free(words);
            fclose(file);
            return -1;
        }
        strcpy(words[i], word);
        i++;
        if(strlen(word) != 5)
        {
            fprintf(stderr, "Error: Word '%s' is not 5 letters long. Please check your word list.\n", word);
            for (int j = 0; j < i; j++) 
            {
                free(words[j]);
            }
            free(words);
            fclose(file);
            return -1;
        }
        if(i >= capacity)
        {
            capacity *= 2;
            char **tmp = realloc(words, capacity * sizeof(char *));
            if (tmp == NULL)
            {
                fprintf(stderr, "Error: Memory reallocation failed.\n");
                for (int j = 0; j < i; j++) 
                {
                    free(words[j]);
                }
                free(words);
                fclose(file);
                return -1; 
            }
            words = tmp;  
        }
        
    }
    char todays_word[6];
    strcpy(todays_word, words[rand() % i]);
    fclose(file);
    printf("You have six chances to guess a 5 lettered word.\n");
    char guess[6];
    printf("Word: _ _ _ _ _\n");
    for(int attempt = 0; attempt < 6; attempt++)
    {
        char result[6] = {' ', ' ', ' ', ' ', ' ', '\0'};
        char temp[6];
        strcpy(temp, todays_word);
        
        int found = 0;
        scanf("%5s", guess);
        if(strlen(guess) != 5)
        {
            printf("Warning : Guess must be 5 letters long. Please try again.\n");
            attempt--;
            continue;
        }
        for(int j = 0; j < i; j++)
        {
            if(strcmp(guess, words[j]) == 0)
            {
                found = 1;
                break;
            }
        }
        if(!found)
        {
            printf("Warning : Word not on word list. Please try again.\n");
            attempt--;
            continue;
        }
        if(strcmp(todays_word, guess) == 0)
        {
            printf("Congratulations! You have guessed today's word \"%s\"\n", todays_word);
            break;
        }
        else if(attempt == 5 && strcmp(todays_word, guess) != 0)
        {
            printf("Better luck next time! Today's word was \"%s\"\n", todays_word);
            break;
        }
        for(int j = 0; j < 5; j++)
        {
            if(temp[j] == guess[j])
            { 
                result[j] = 'G'; // doğru, yeşil printlenecek
                temp[j] = '*'; // bir daha sayılmaması adına
            }
            else if(strchr(temp, guess[j]))
            {
                char *pos = strchr(temp, guess[j]);
                result[j] = 'Y'; // mevcut, yeri yanlış
                *pos = '*';
            }
            else
                result[j] = 'X';
        }        
        for (int k = 0; k < 5; k++) 
        {
          switch (result[k]) 
            {
            case 'G':
            printf(BG_GREEN UNDERLINE_ON "%c " RESET, guess[k]);
            break;
            case 'Y':
            printf(BG_YELLOW UNDERLINE_ON "%c " RESET, guess[k]);
            break;
            default:
            printf(BG_GRAY UNDERLINE_ON "%c " RESET, guess[k]);
            }
        }
        printf("\n");

    }
    for(int j = 0; j < i; j++)
    {
        free(words[j]);
    }
    free(words);
    printf("Thank you for playing Wordle!\n");
    return 0;
}