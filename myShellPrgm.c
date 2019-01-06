/*************************************************************************************
 *** Author : Anugesh Balasubramaniam
 *** Date: 01/30/2017
 *** Description: My shell algorithm is an attempt to imitate the bash terminal
 on linux and unix. It waits for the user to enter any bash commands
 and also some additional commands such as add 1 2 3 will calculate
 and display the total, muls 1 2 3 will calculate and display the product,
 and args “argument,1” 2 3 four will calculate the number of arguments
 and print the arguments that were given. It also checks to see if the
 user enters either < or > to indicate the program to redirect the output
 or input from a file. I assumed that redirecting from an input file will only be
 performed for certain bash commands like “sort” and “cat” because of the thought
 that typing ls < filename will be more work for the user compared to typing ls -al.
 The steps on how compile and execute my program are all listed below.
**************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **getln();

//function prototypes
void distributor(int countArgs, char **argv);
void commandArgs(int countArgs, char **argv);
void redirectToFile(char *fileName, char **argv, int fileOut, int fileIn);
void addArgs(int countArgs, char **argv);
void printArgCount(int countArgs, char **argv);
void process(char *fileName, int countArgs, char **argv);
void multiplyArgs(int countArgs, char **argv);

//INPUT: Arguments from the parser provided
//OUTPUT: Necessary output for each command performed by the user
//FUNCTION: Takes arguments from the parser and displays necessary output to the screen
int main()
{
    int i, countArgs, shell = 1;
    char **argv;
    
    
    while(shell == 1)
    {
        printf(">>");
        
        countArgs = 0;
        argv = NULL;
        
        argv = getln();
        
        for(i = 0; argv[i] != NULL; i++)
        {
            countArgs++;
        }
        
        if(strcmp(argv[0], "exit") == 0)
        {
            printf("\n\n[Shell program terminated]\n");
            exit(0);
        }
        else
        {
                distributor(countArgs, argv);
        }
    }
}

//INPUT: Parameters from main(). Total number of arguments and the arguments provided by the user
//OUTPUT: Necessary feedback of what the shell is doing and if any error's occur
//FUNCTION: Distributes to the necessary function for proper operation of the arguments provided
void distributor(int countArgs, char **argv)
{
    int fileOut = 0;
    char *fileName;
    char usrPath[30] = "/usr/bin/";
    char binPath[30] = "/bin/";
    
    if(strcmp(argv[0], "add") == 0)
    {
        for(int i = 0; argv[i] != NULL; i++)
        {
            if ((strcmp(argv[i], ">")) == 0)
            {
                fileName = argv[i+1];
                argv[i] = NULL;
                countArgs = countArgs-2;
                fileOut = 1;
            }
        }
        if (countArgs >= 3)
        {
            if (fileOut == 1)
            {
                printf("Redirected output to: %s\n", fileName);
                process(fileName, countArgs, argv);
            }
            else
            {
                addArgs(countArgs, argv);
            }
            
        }
        else
        {
            printf("Not enough arguments to perform command. Expecting at least 3 arguments. Please try again.\n");
        }
    }
    else if (strcmp(argv[0], "args") == 0)
    {
        for(int i = 0; argv[i] != NULL; i++)
        {
            if ((strcmp(argv[i], ">")) == 0)
            {
                fileName = argv[i+1];
                argv[i] = NULL;
                countArgs = countArgs-2;
                fileOut = 1;
            }
        }

        if (countArgs >= 2)
        {
            if (fileOut == 1)
            {
                printf("Redirected output to: %s\n", fileName);
                process(fileName, countArgs, argv);
            }
            else
            {
                printArgCount(countArgs, argv);
            }
        }
        else
        {
            printf("Not enough arguments to perform command. Expecting at least 2 arguments. Please try again.\n");
        }
    }
    else if(strcmp(argv[0], "muls") == 0)
    {
        for(int i = 0; argv[i] != NULL; i++)
        {
            if ((strcmp(argv[i], ">")) == 0)
            {
                fileName = argv[i+1];
                argv[i] = NULL;
                countArgs = countArgs-2;
                fileOut = 1;
            }
        }
        
        if (countArgs >= 2)
        {
            if (fileOut == 1)
            {
                printf("Redirected output to: %s\n", fileName);
                process(fileName, countArgs, argv);
            }
            else
            {
                multiplyArgs(countArgs, argv);
            }
        }
        else
        {
            printf("Not enough arguments to perform command. Expecting at least 2 arguments. Please try again.\n");
        }
    }
    else
    {

        strcat(usrPath,argv[0]);
        strcat(binPath,argv[0]);
        if (access(usrPath, F_OK) != 0){
            
            if (access(binPath, F_OK) != 0)
            {
                printf("-shell: %s: command not found\n", argv[0]);
            }
            else
            {
                commandArgs(countArgs, argv);
            }
        }
        else
        {
            commandArgs(countArgs, argv);
        }
    }
}

//INPUT: Total number of arguments and arguments provided by the user
//OUTPUT: Prints out the arguments that the user entered to multiply and outputs the calculated product
//FUNCTION: Converts user arguments to decimal numbers and multiplies them together to calculate the product
void multiplyArgs(int countArgs, char **argv)
{
    int i, j, ascii;
    double num;
    double total = 1;
    
    for (i = 1; argv[i] != NULL; i++)
    {
        num = atof(argv[i]);
        total = total * num;
        
        if ((argv[i][0] == '0') && (argv[i][1] == 'x'))
        {
            for (j = 2; argv[i][j] != '\0'; j++)
            {
                ascii = (int)argv[i][j];
            }
        }
        
        if (i == (countArgs-1))
        {
            printf("%s ", argv[i]);
        }
        else
        {
            printf("%s x ", argv[i]);
        }
        
    }
    printf("= %0.2f\n", total);

}

//INPUT: name of file, total number of arguments, arguments provided by the user
//OUTPUT: redirects to command output to file
//FUNCTION: redirects the standard output to a file for non bash commands
void process(char *fileName, int countArgs, char **argv)
{
    FILE *fp;
    
    int pid, status;
    
    pid = fork();
    
    if(pid == -1)
    {
        printf("Error: the fork cannot make a copy process");
        exit(1);
    }
    else if (pid == 0)
    {
        if (strcmp(argv[0], "add") == 0)
        {
            fp = freopen(fileName, "w+", stdout);
            addArgs(countArgs, argv);
            exit(0);
            fclose(fp);
        }
        else if(strcmp(argv[0], "args") == 0)
        {
            fp = freopen(fileName, "w+", stdout);
            printArgCount(countArgs, argv);
            exit(0);
            fclose(fp);
        }
        else if(strcmp(argv[0], "muls") == 0)
        {
            fp = freopen(fileName, "w+", stdout);
            multiplyArgs(countArgs, argv);
            exit(0);
            fclose(fp);
        }
    }
    else
    {
        wait(&status);
    }
    
}

//INPUT: total number of arguments, arguments provided by the user
//OUTPUT: displays content to either terminal or redirects file if needed
//FUNCTION: redirects the standard output to a file for bash commands
void commandArgs(int countArgs, char **argv)
{
    int pid, status, i, fileOut = 0, fileIn = 0;
    char *fileName;
    
    pid = fork();
    
    if (pid == -1)
    {
        printf("Error: the fork cannot make a copy process");
        exit(1);
    }
    else if (pid == 0)
    {
        for (i = 0; argv[i] != NULL; i++)
        {
            if (strcmp(argv[i], ">") == 0)
            {
                fileName = argv[i+1];
                argv[i] = NULL;
                fileOut = 1;
            }
            else if(strcmp(argv[i], "<") == 0)
            {
                fileName = argv[i+1];
                argv[i] = argv[i+1];
                fileIn = 1;
            }
        }
        
        if (fileOut == 1)
        {
            redirectToFile(fileName, argv, fileOut, fileIn);
        }
        else if(fileIn == 1)
        {
            redirectToFile(fileName, argv, fileOut, fileIn);
        }
        else
        {
            if (countArgs > 1)
            {
                execvp(argv[0], argv);
            }
            else
            {
                execlp(argv[0], argv[0], NULL);
            }
        }
        
    }
    else
    {
        wait(&status);
    }
}

//INPUT: name of file, total number of arguments, output files occured, input file occured
//OUTPUT: redirects to command output to file
//FUNCTION: redirects the standard output to a file for bash commands
void redirectToFile(char *fileName, char **argv, int fileOut, int fileIn)
{
    FILE *fp = NULL;
    
    if ((fileOut == 1) && (fileIn == 0))
    {
        printf("Redirected output to: %s\n", fileName);
        fp = freopen(fileName, "w+", stdout);
        execlp(argv[0], argv[0], argv[1], NULL);
        fclose(fp);
    }
    else if((fileOut == 0) && (fileIn == 1))
    {
        printf("Redirected input from: %s\n", fileName);
        execvp(argv[0], argv);
        
    }
}

//INPUT: Total number of arguments and arguments provided by the user
//OUTPUT: Prints out the arguments that the user entered to add and outputs the calculated sum
//FUNCTION: Converts user arguments to decimal numbers and add them together to calculate the sum
void addArgs(int countArgs, char **argv)
{
    int i, j, ascii;
    double num;
    double total = 0;

    for (i = 1; argv[i] != NULL; i++)
    {
        num = atof(argv[i]);
        total = total + num;
        
        if ((argv[i][0] == '0') && (argv[i][1] == 'x'))
        {
            for (j = 2; argv[i][j] != '\0'; j++)
            {
                ascii = (int)argv[i][j];
            }
        }
        
        if (i == (countArgs-1))
        {
            printf("%s ", argv[i]);
        }
        else
        {
            printf("%s + ", argv[i]);
        }
        
    }
    printf("= %0.2f\n", total);
    
}

//INPUT: total number of arguments and arguments provided by the user
//OUTPUT: outputs the total number of arguments and each argument seperated by a comma
//FUNCTION: prints as many arguments the user entered
void printArgCount(int countArgs, char **argv)
{
    int i;
    
    printf("argc = %d, args = ", (countArgs-1));
    
    for (i = 1; argv[i] != NULL; i++)
    {
        if (i == (countArgs-1))
        {
            printf("%s\n", argv[i]);
        }
        else
        {
            printf("%s, ", argv[i]);
        }
        
    }
    
}


