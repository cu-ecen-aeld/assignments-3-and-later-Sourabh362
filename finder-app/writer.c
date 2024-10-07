#include <stdio.h>    // For FILE, fopen, fprintf, fclose
#include <syslog.h>   // For syslog functions
#include <unistd.h>   // For close (if needed for file descriptors)
#include <string.h>   // For string functions (if needed)
#include <stdlib.h>   // For exit() (optional, depending on your error handling)
  
int main(int argc, char *argv[])
{
    // Open syslog with the program name, options, and facility
    openlog(argv[0], LOG_PID | LOG_CONS, LOG_USER);

    // Check for correct number of arguments
    if (argc != 3) {
        syslog(LOG_ERR, "Usage: %s <filename> <content>", argv[0]);
        printf( "\nUsage: %s <filename> <content>\n", argv[0]);
        closelog(); // Close syslog before exiting
        return 1;
    }

    const char *file = argv[1];
    const char *content = argv[2];
    syslog(LOG_DEBUG, "Writing '%s' to '%s'", content, file);
    printf("\nWriting '%s' to '%s'\n", content, file);

    // Open the file for writing
    FILE *f = fopen(file, "w");
    if (f == NULL) {
        syslog(LOG_ERR, "fopen() failed for file: %s", file);
        printf("fopen() failed for file: %s", file);
        closelog(); // Close syslog before exiting
        return 1;
    }

    // Write content to the file
    if (fprintf(f, "%s\n", content) < 0) {
        syslog(LOG_ERR, "fprintf() failed for file: %s", file);
        printf("fprintf() failed for file: %s", file);

        fclose(f); // Close the file before exiting
        closelog(); // Close syslog before exiting
        return 1; 
    }

    fclose(f); // Correctly close the file
    closelog(); // Close syslog

    return 0;
}