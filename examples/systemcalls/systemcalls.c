#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "systemcalls.h"

bool do_system(const char *cmd) {
    // Call the system() function with the command set in cmd
    int result = system(cmd);
    
    // Check if system() call was successful and if the command executed successfully
    return (result != -1 && WIFEXITED(result) && WEXITSTATUS(result) == 0);
}

bool do_exec(int count, ...) {
    va_list args;
    va_start(args, count);
    
    char *command[count + 1];
    for (int i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL; // Null-terminate the array of arguments

    pid_t pid = fork();
    if (pid == -1) {
        // Fork failed
        return false;
    } else if (pid == 0) {
        // Child process
        execv(command[0], command);
        // If execv returns, there was an error
        perror("execv failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for child to finish
        
        // Return true if the command executed successfully
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
    
    va_end(args);
}

bool do_exec_redirect(const char *outputfile, int count, ...) {
    va_list args;
    va_start(args, count);

    char *command[count + 1];
    for (int i = 0; i < count; i++) {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL; // Null-terminate the array of arguments

    pid_t pid = fork();
    if (pid == -1) {
        // Fork failed
        return false;
    } else if (pid == 0) {
        // Child process
        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd == -1) {
            perror("Failed to open output file");
            exit(EXIT_FAILURE);
        }
        
        dup2(fd, STDOUT_FILENO); // Redirect stdout to the file
        close(fd); // Close the file descriptor
        
        execv(command[0], command);
        
        // If execv returns, there was an error
        perror("execv failed");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for child to finish
        
        // Return true if the command executed successfully
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }

    va_end(args);
}