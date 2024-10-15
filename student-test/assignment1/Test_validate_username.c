#include "unity.h"
#include <stdbool.h>
#include <stdlib.h>
#include "../../examples/autotest-validate/autotest-validate.h"
#include "../../assignment-autotest/test/assignment1/username-from-conf-file.h"

/**
* This function should:
*   1) Call the my_username() function in Test_assignment_validate.c to get your hard coded username.
*   2) Obtain the value returned from function malloc_username_from_conf_file() in username-from-conf-file.h within
*       the assignment autotest submodule at assignment-autotest/test/assignment1/
*   3) Use unity assertion TEST_ASSERT_EQUAL_STRING_MESSAGE the two strings are equal.  
*/
void test_validate_my_username()
{
    // Step 1: Call my_username() to get the hard-coded username
    const char *expected_username = my_username();

    // Step 2: Get the username from the configuration file
    char *actual_username = malloc_username_from_conf_file();

    // Step 3: Use Unity assertion to check if both usernames are equal
    TEST_ASSERT_EQUAL_STRING_MESSAGE(expected_username, actual_username, "Usernames do not match!");

    // Free allocated memory if necessary
    free(actual_username);
}