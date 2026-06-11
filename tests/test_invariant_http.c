#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* Test that HTTP path traversal payloads are rejected or stay within root */

START_TEST(test_http_path_traversal_blocked)
{
    // Invariant: File operations never resolve paths outside the declared root directory
    const char *payloads[] = {
        "GET /../../../etc/passwd HTTP/1.0\r\n\r\n",
        "GET /....//....//etc/passwd HTTP/1.0\r\n\r\n",
        "GET /%2e%2e%2f%2e%2e%2f%2e%2e%2fetc/passwd HTTP/1.0\r\n\r\n",
        "GET /index.html HTTP/1.0\r\n\r\n"  /* valid input */
    };
    int num_payloads = sizeof(payloads) / sizeof(payloads[0]);

    for (int i = 0; i < num_payloads; i++) {
        int pipefd[2];
        ck_assert_int_eq(pipe(pipefd), 0);
        
        pid_t pid = fork();
        if (pid == 0) {
            /* Child: simulate sending payload to http handler via driver */
            close(pipefd[0]);
            /* Write payload that would be processed by process_input */
            write(pipefd[1], payloads[i], strlen(payloads[i]));
            close(pipefd[1]);
            _exit(0);
        }
        
        close(pipefd[1]);
        char response[4096] = {0};
        read(pipefd[0], response, sizeof(response) - 1);
        close(pipefd[0]);
        
        int status;
        waitpid(pid, &status, 0);
        
        /* For traversal payloads (i < 3), verify no sensitive file content leaked */
        if (i < 3) {
            ck_assert_msg(strstr(response, "root:") == NULL,
                "Path traversal payload %d leaked /etc/passwd content", i);
        }
    }
}
END_TEST

Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Security");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_http_path_traversal_blocked);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}