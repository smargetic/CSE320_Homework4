#include <criterion/criterion.h>
#include <criterion/logging.h>

#include "jobber.h"
#include "event.h"
#include "driver.h"
#include "tracker.h"
#include "_helper.h"

/*
 * start_quit_test
 */
static COMMAND start_quit_test_script[] = {
    { "quit", NO_EVENT, 0, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(black_box_suite, start_quit_test, .timeout=10)
{
    int err, status;
    char *name = "black_box_suite/start_quit_test";
    err = run_test(name, TARGET, start_quit_test_script, &status);
    cr_assert_eq(err, 0);
    assert_normal_exit(status);
}

COMMAND demo_test_script[] = {
    { "spool 'echo hello1'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo hello2'", CREATE_EVENT, 1, NULL, NULL },
    { "cancel $1", STATUS_EVENT, 1, NULL, NULL },
    { "cancel $0", STATUS_EVENT, 1, NULL, assert_num_jobs, (void *)2 },
    { "quit", NO_EVENT, 0, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(black_box_suite, demo_test, .timeout=10)
{
    int err, status;
    char *name = "black_box_suite/demo_test";
    err = run_test(name, TARGET, demo_test_script, &status);
    cr_assert_eq(err, 0);
    assert_normal_exit(status);
}
