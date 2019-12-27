#include <criterion/criterion.h>
#include <criterion/logging.h>

#include "jobber.h"
#include "event.h"
#include "driver.h"
#include "tracker.h"
#include "_helper.h"


///
/// help_test
/// @brief check program's response to `help` command
///        no event should happen
///
static COMMAND help_test_script[] = {
    { "help", NO_EVENT, 0, NULL, NULL },
    { "quit", NO_EVENT, 0, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, help_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/help_test";
    err = run_test(name, TARGET, help_test_script, &status);
    cr_assert_eq(err, 0);
    assert_normal_exit(status);
}

///
/// enable_disable_test
/// @brief check program's response to `enable` & `disable` command
///        no event should happen
///
static COMMAND enable_disable_test_script[] = {
    { "enable", NO_EVENT, 0, NULL, NULL },
    { "disable", NO_EVENT, 0, NULL, NULL },
    { "quit", NO_EVENT, 0, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, enable_disable_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/enable_disable_test";
    err = run_test(name, TARGET, enable_disable_test_script, &status);
    cr_assert_eq(err, 0);
    assert_normal_exit(status);
}

///
/// job_completion_test
/// @brief spool a simple job and see whether it can complete
///        first spool, then enable
///        check CREATE, START, END & EXPUNGE
///
static COMMAND job_completion_test_script[] = {
    { "spool 'echo hello1'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", NO_EVENT, 0, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, NULL  },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_completion_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_completion_test";
    err = run_test(name, TARGET, job_completion_test_script, &status);
    cr_assert_eq(err, 0);
    assert_normal_exit(status);
}

///
/// job_completion_test_2
/// @brief spool a simple (e.g. no pipeline & pipe) job and see whether it can complete
///        first enable, then spool (diff. from job_completion_test_2)
///        check CREATE, START, END & EXPUNGE
///
static COMMAND job_completion_test_2_script[] = {
    { "enable", NO_EVENT, 0, NULL, NULL },
    { "spool 'echo hello1'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, NULL  },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_completion_test_2, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_completion_test_2";
    err = run_test(name, TARGET, job_completion_test_2_script, &status);
    cr_assert_eq(err, 0);
    assert_normal_exit(status);
}

///
/// job_completion_with_expunge_test
/// @brief spool a simple job and see whether it can complete
///        first spool, then enable, then expunge
///        check CREATE, START, END & EXPUNGE
///
static COMMAND job_completion_with_expunge_test_script[] = {
    { "spool 'echo hello1'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", NO_EVENT, 0, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, assert_num_jobs, (void *)1 },
    { "expunge $0", EXPUNGE_EVENT, 1, NULL, NULL },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)0 },
    { NULL, EOF_EVENT, 0, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_completion_with_expunge_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_completion_with_expunge_test";
    err = run_test(name, TARGET, job_completion_with_expunge_test_script, &status);
    cr_assert_eq(err, 0);
    assert_normal_exit(status);
}

///
/// multi_job_completion_with_expunge_test
/// @brief spool multiple simple jobs and see whether it can complete
///        expunge job manually and check EXPUNGE event
///        check CREATE, START, END & EXPUNGE (with job number check)
///
static COMMAND multi_job_completion_with_expunge_test_script[] = {
    { "spool 'echo hello1'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo hello2'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo hello3'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo hello4'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", NO_EVENT, 0, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, NULL  },
    { NULL, END_EVENT, 1, NULL, NULL  },
    { NULL, END_EVENT, 1, NULL, NULL  },
    { NULL, END_EVENT, 1, NULL, assert_num_jobs, (void *)4 },
    { "expunge $0", EXPUNGE_EVENT, 1, NULL, NULL },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)3 },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, multi_job_completion_with_expunge_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/multi_job_completion_with_expunge_test";
    err = run_test(name, TARGET, multi_job_completion_with_expunge_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_cancel_test
/// @brief spool a simple sleep job, cancel it, then expunge it manually
/// and check EXPUNGE event
///
static COMMAND job_cancel_test_script[] = {
    { "spool 'sleep 3'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", NO_EVENT, 0, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "cancel $0", END_EVENT, 5, NULL, NULL },
    { "quit", EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)0 },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_cancel_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_cancel_test";
    err = run_test(name, TARGET, job_cancel_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_pause_test
/// @brief spool a simple sleep job and pause it manually (without resume)
///
static COMMAND job_pause_test_script[] = {
    { "spool 'sleep 3'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", START_EVENT, 1, NULL, NULL },
    { "pause $0", PAUSE_EVENT, 1, NULL, NULL }, 
    { "quit", END_EVENT, 5, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)0 },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(pause_resume_suite, job_pause_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_pause_test";
    err = run_test(name, TARGET, job_pause_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_resume_test
/// @brief spool a simple sleep job, pause it manually, then resume
///
static COMMAND job_resume_test_script[] = {
    { "spool 'sleep 3'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", START_EVENT, 1, NULL, NULL },
    { "pause $0", PAUSE_EVENT, 1, NULL, _show_jobs }, 
    { "resume $0", RESUME_EVENT, 1, NULL, NULL }, 
    { NULL, END_EVENT, 5, NULL, NULL  },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)0 },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(pause_resume_suite, job_resume_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_resume_test";
    err = run_test(name, TARGET, job_resume_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_doc_sample_pipeline_test
/// @brief test whether program can complete the sample command in document
///        spool 'echo start ; cat /etc/passwd | grep bash > out ; echo done'
///
static COMMAND job_doc_sample_pipeline_test_script[] = {
    { "spool 'echo start ; cat /etc/passwd | grep bash > out ; echo done'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", START_EVENT, 1, NULL, NULL },
    { "quit", END_EVENT, 1, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)0 },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_doc_sample_pipeline_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_doc_sample_pipeline_test";
    err = run_test(name, TARGET, job_doc_sample_pipeline_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_pipe_test_1
/// @brief spool a job with pipe
///        see whether it can complete successfully
///
static COMMAND job_pipe_test_1_script[] = {
    { "spool 'cat /etc/passwd | grep bash > out'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", START_EVENT, 1, NULL, NULL },
    { "quit", END_EVENT, 1, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)0 },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_pipe_test_1, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_pipe_test_1";
    err = run_test(name, TARGET, job_pipe_test_1_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_pipe_test_2
/// @brief spool a job with 4 pipes and output redirection
///        see whether it can complete successfully
///
static COMMAND job_pipe_test_2_script[] = {
    { "spool 'cat /etc/passwd | grep bash | grep bash | grep bash | grep bash > out'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", START_EVENT, 1, NULL, NULL },
    { "quit", END_EVENT, 1, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)0 },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_pipe_test_2, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_pipe_test_2";
    err = run_test(name, TARGET, job_pipe_test_2_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_pipeline_test_1
/// @brief spool complicated jobs with pipeline and pipe
///        see whether it can complete successfully
///
static COMMAND job_pipeline_test_1_script[] = {
    { "spool 'echo start; cat /etc/passwd | grep bash > out; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'find bin -name jobber | grep jobber > out; echo job2; echo job2 | grep j'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo job3'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, NULL  },
    { NULL, END_EVENT, 1, NULL, NULL  },
    { NULL, END_EVENT, 1, NULL, NULL  },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)3 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)2 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)0 },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_pipeline_test_1, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_pipeline_test_1";
    err = run_test(name, TARGET, job_pipeline_test_1_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_pipeline_test_2
/// @brief spool complicated jobs with pipeline and pipe
///        also check pause and resume under this condition
///        see whether it can complete successfully
///
static COMMAND job_pipeline_test_2_script[] = {
    { "spool 'sleep 1; echo start; cat /etc/passwd | grep bash > out; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 1; find bin -name jobber | grep jobber > out; echo job2; echo job2 | grep j'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 1; echo job3'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo job4; sleep 5; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "pause $3", PAUSE_EVENT, 1, NULL, NULL }, 
    { "resume $3", RESUME_EVENT, 1, NULL, NULL }, 
    { NULL, END_EVENT, 2, NULL, NULL },
    { NULL, END_EVENT, 2, NULL, NULL },
    { NULL, END_EVENT, 2, NULL, NULL },
    { NULL, END_EVENT, 6, NULL, NULL },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)4 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)3 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)2 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)0 },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(pause_resume_suite, job_pipeline_test_2, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_pipeline_test_2";
    err = run_test(name, TARGET, job_pipeline_test_2_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_pipeline_test_3
/// @brief spool complicated jobs with pipeline and pipe
///        also check pause, resume AND CANCEL (diff. from job_pipeline_test_2)
///        see whether it can complete successfully
///
static COMMAND job_pipeline_test_3_script[] = {
    { "spool 'sleep 1; echo start; cat /etc/passwd | grep bash > out; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 1; find bin -name jobber | grep jobber > out; echo job2; echo job2 | grep j'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 1; echo job3'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo job3; sleep 3; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "pause $3", PAUSE_EVENT, 1, NULL, NULL },   
    { "resume $3", RESUME_EVENT, 1, NULL, NULL }, 
    { NULL, END_EVENT, 2, NULL, NULL  },
    { NULL, END_EVENT, 2, NULL, NULL  },
    { NULL, END_EVENT, 2, NULL, NULL  },
    { "cancel $3", END_EVENT, 1, NULL, NULL },   
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)4 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)3 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)2 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)0 },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(pause_resume_suite, job_pipeline_test_3, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_pipeline_test_3";
    err = run_test(name, TARGET, job_pipeline_test_3_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_pipeline_test_4
/// @brief spool complicated jobs with pipeline and pipe
///        also check pause, resume, cancel AND EXPUNGE (diff. from job_pipeline_test_3)
///        see whether it can complete successfully
///
static COMMAND job_pipeline_test_4_script[] = {
    { "spool 'sleep 1; echo start; cat /etc/passwd | grep bash > out; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 1; find bin -name jobber | grep jobber > out; echo job2; echo job2 | grep j'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 1; echo job3'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo job3; sleep 3; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "pause $3", PAUSE_EVENT, 1, NULL, NULL }, 
    { "resume $3", RESUME_EVENT, 1, NULL, NULL }, 
    { NULL, END_EVENT, 2, NULL, NULL  },
    { NULL, END_EVENT, 2, NULL, NULL  },
    { NULL, END_EVENT, 2, NULL, NULL  },
    { "cancel $3", END_EVENT, 1, NULL, NULL }, 
    { "expunge $3", EXPUNGE_EVENT, 1, NULL, NULL },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)3 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)2 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, assert_num_jobs, (void *)0 },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(pause_resume_suite, job_pipeline_test_4, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_pipeline_test_4";
    err = run_test(name, TARGET, job_pipeline_test_4_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_pipeline_test_5
/// @brief spool complicated jobs with pipeline and pipe
///        also check pause, resume, cancel and MULTIPLE expunge (diff. from job_pipeline_test_4)
///        see whether it can complete successfully
///
static COMMAND job_pipeline_test_5_script[] = {
    { "spool 'sleep 1; echo start; cat /etc/passwd | grep bash > out; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 1; find bin -name jobber | grep jobber > out; echo job2; echo job2 | grep j'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 1; echo job3'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo job3; sleep 3; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "pause $3", PAUSE_EVENT, 1, NULL, NULL }, 
    { "resume $3", RESUME_EVENT, 1, NULL, NULL }, 
    { NULL, END_EVENT, 2, NULL, NULL  },
    { NULL, END_EVENT, 2, NULL, NULL  },
    { NULL, END_EVENT, 2, NULL, NULL  },
    { "cancel $3", END_EVENT, 1, NULL, NULL }, 
    { "expunge $3", EXPUNGE_EVENT, 1, NULL, NULL },
    { "expunge $2", EXPUNGE_EVENT, 1, NULL, NULL },
    { "expunge $1", EXPUNGE_EVENT, 1, NULL, NULL },
    { "expunge $0", EXPUNGE_EVENT, 1, NULL, NULL },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)0 },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(pause_resume_suite, job_pipeline_test_5, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_pipeline_test_5";
    err = run_test(name, TARGET, job_pipeline_test_5_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_pipeline_enable_disable_test
/// @brief spool complicated jobs with pipeline and pipe
///        under enabled and disabled condition
///        should not create jobs under disabled condition
///        see whether it can complete successfully
///
static COMMAND job_pipeline_enable_disable_test_script[] = {
    { "enable", NO_EVENT, 0, NULL, NULL },

    { "spool 'echo start; cat /etc/passwd | grep bash > out; echo end'", CREATE_EVENT, 1,NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL  },
    { NULL, END_EVENT, 1, NULL, NULL  },
    { "spool 'echo job3'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL  },
    { NULL, END_EVENT, 1, NULL, NULL  },

    { "disable", NO_EVENT, 0, NULL, NULL },
    { "spool 'echo start; cat /etc/passwd | grep bash > out; echo end'", NO_EVENT, 0, NULL, _show_jobs },

    { "enable", CREATE_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL  },
    { NULL, END_EVENT, 1, NULL, NULL  },

    { "quit", NO_EVENT, 0, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_pipeline_enable_disable_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_pipeline_enable_disable_test";
    err = run_test(name, TARGET, job_pipeline_enable_disable_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_pipeline_enable_disable_expunge_test
/// @brief spool complicated jobs with pipeline and pipe
///        under enabled and disabled condition
///        should not create jobs under disabled condition
///        also check expunge command under disabled condition
///        see whether it can complete successfully
///
static COMMAND job_pipeline_enable_disable_expunge_test_script[] = {
    { "enable", NO_EVENT, 0, NULL, NULL },

    { "spool 'echo job 0 start; cat /etc/passwd | grep bash > out; echo job 0 end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL  },
    { NULL, END_EVENT, 1, NULL, NULL  },
    { "spool 'echo job 1'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL  },
    { NULL, END_EVENT, 1, NULL, NULL  },

    { "disable", NO_EVENT, 0, NULL, NULL },
    { "spool 'echo job 2 start; cat /etc/passwd | grep bash > out; echo job 2 end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "expunge $0", EXPUNGE_EVENT, 1, NULL, NULL },

    { "enable", START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, NULL  },

    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)2 },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_pipeline_enable_disable_expunge_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_pipeline_enable_disable_expunge_test";
    err = run_test(name, TARGET, job_pipeline_enable_disable_expunge_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_overload_test_1
/// @brief spool 9 jobs.
///        the 9th should not be created
///
static COMMAND job_overload_test_1_script[] = {
    { "spool 'echo hello1'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo hello2'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo hello3'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo hello4'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo hello5'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo hello6'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo hello7'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo hello8'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'echo hello9'", NO_EVENT, 0, NULL, _show_jobs },
    { "quit", NO_EVENT, 0, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_overload_test_1, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_overload_test_1";
    err = run_test(name, TARGET, job_overload_test_1_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_overload_test_2
/// @brief spool 5 sleep jobs.
///        the 4th job should not start until 0-3th are canceled.
///
static COMMAND job_overload_test_2_script[] = {
    { "enable", NO_EVENT, 0, NULL, NULL },
    { "spool 'sleep 100; echo 0'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "spool 'sleep 100; echo 1'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "spool 'sleep 100; echo 2'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "spool 'sleep 100; echo 3'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL },

    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },

    { "cancel $0", END_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "cancel $1", END_EVENT, 1, NULL, NULL },
    { "cancel $2", END_EVENT, 1, NULL, NULL },
    { "cancel $3", END_EVENT, 1, NULL, NULL },
    { "cancel $4", END_EVENT, 1, NULL, NULL },

    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)5 },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_overload_test_2, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_overload_test_2";
    err = run_test(name, TARGET, job_overload_test_2_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_overload_test_3
/// @brief spool 10 sleep jobs.
///        the 9th and 10th should not be created,
///        and the 4-7th jobs should not start until 0-3th are canceled.
///
static COMMAND job_overload_test_3_script[] = {
    { "disable", NO_EVENT, 0, NULL, NULL },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", NO_EVENT, 0, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", NO_EVENT, 0, NULL, _show_jobs },

    { "enable", NO_EVENT, 0, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },

    { "cancel $0", END_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "cancel $1", END_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "cancel $2", END_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "cancel $3", END_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },

    { "cancel $4", END_EVENT, 1, NULL, NULL },
    { "cancel $5", END_EVENT, 1, NULL, NULL },
    { "cancel $6", END_EVENT, 1, NULL, NULL },
    { "cancel $7", END_EVENT, 1, NULL, NULL },

    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)8 },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_overload_test_3, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_overload_test_3";
    err = run_test(name, TARGET, job_overload_test_3_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_overload_expunge_refill_test
/// @brief spool 10 jobs. the 9th and 10th should not be created
///        then expunge some jobs
///        we should be able to spool jobs again until we reach upper limit of 8 again
///
static COMMAND job_overload_expunge_refill_test_script[] = {
    { "disable", NO_EVENT, 0, NULL, NULL },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", NO_EVENT, 0, NULL, _show_jobs },
    { "spool 'sleep 100; echo end'", NO_EVENT, 0, NULL, _show_jobs },

    { "enable", NO_EVENT, 0, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },

    { "cancel $0", END_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "cancel $1", END_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "cancel $2", END_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { "cancel $3", END_EVENT, 1, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },

    { "cancel $4", END_EVENT, 1, NULL, NULL },
    { "cancel $5", END_EVENT, 1, NULL, NULL },
    { "cancel $6", END_EVENT, 1, NULL, NULL },
    { "cancel $7", END_EVENT, 1, NULL, NULL },

    { "expunge $6", EXPUNGE_EVENT, 1, NULL, NULL },
    { "expunge $7", EXPUNGE_EVENT, 1, NULL, NULL },

    { "spool 'echo job 11'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, NULL },
    { "spool 'echo job 12'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, NULL },
    { "spool 'echo job 13'", NO_EVENT, 0, NULL, _show_jobs },
    { "spool 'echo job 14'", NO_EVENT, 0, NULL, _show_jobs },

    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)8 },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_overload_expunge_refill_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_overload_expunge_refill_test";
    err = run_test(name, TARGET, job_overload_expunge_refill_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_abort_test
/// @brief spool job that should abort
///        (rsrc/abort is a C program (rsrc/abort.c) that calls abort() in main)
///        check whether job really aborts (return value should be 134)
///
static COMMAND job_abort_test_script[] = {
    { "disable", NO_EVENT, 0, NULL, NULL },
    { "spool 'tests/bin/abort'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", NO_EVENT, 0, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, assert_job_aborted, NULL },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_abort_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_abort_test";
    err = run_test(name, TARGET, job_abort_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}


///
/// job_complete_test
/// @brief spool job that should complete successfully
///        check whether job really completes with return value 0
///
static COMMAND job_complete_test_script[] = {
    { "disable", NO_EVENT, 0, NULL, NULL },
    { "spool 'echo test'", CREATE_EVENT, 1, NULL, _show_jobs },
    { "enable", NO_EVENT, 0, NULL, NULL },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, assert_job_exited, (void *)0 },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_complete_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_complete_test";
    err = run_test(name, TARGET, job_complete_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_input_redirection_test
/// @brief spool job with input redirection
///        check whether job really completes with return value 0
///
static COMMAND job_input_redirection_test_script[] = {
    { "enable", NO_EVENT, 0, NULL, NULL },
    { "spool 'echo test < /etc/passwd'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, assert_job_exited, (void *)0 },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_input_redirection_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_input_redirection_test";
    err = run_test(name, TARGET, job_input_redirection_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_input_redirection_test
/// @brief spool job with output redirection
///        check whether job really exits with status 0
///
static COMMAND job_output_redirection_test_script[] = {
    { "enable", NO_EVENT, 0, NULL, NULL },
    { "spool 'echo test > out'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, assert_job_exited, (void *)0 },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_output_redirection_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_output_redirection_test";
    err = run_test(name, TARGET, job_output_redirection_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}

///
/// job_io_redirection_test
/// @brief spool job with input & output redirection
///        check whether job really exits with status 0
///
static COMMAND job_io_redirection_test_script[] = {
    { "enable", NO_EVENT, 0, NULL, NULL },
    { "spool 'grep root < /etc/passwd > out'", CREATE_EVENT, 1, NULL, _show_jobs },
    { NULL, START_EVENT, 1, NULL, NULL },
    { NULL, END_EVENT, 1, NULL, assert_job_exited, (void *)0 },
    { "quit", NO_EVENT, 0, NULL, assert_num_jobs, (void *)1 },
    { NULL, EXPUNGE_EVENT, 1, NULL, NULL },
    { NULL, EOF_EVENT, 1, NULL, NULL },
    { 0 }
};

Test(basic_tests_suite, job_io_redirection_test, .timeout=10)
{
    int err, status;
    char *name = "basic_tests_suite/job_io_redirection_test";
    err = run_test(name, TARGET, job_io_redirection_test_script, &status);
    cr_assert_eq(err, 0);
    assert_no_signal(status);
}
