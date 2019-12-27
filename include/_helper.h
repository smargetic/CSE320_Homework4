#define TARGET "bin/jobber"

void assert_normal_exit(int status);

void assert_error_exit(int status);

void assert_no_signal(int status);

void _show_jobs(EVENT *ep, int *env, void *args);

void assert_num_jobs(EVENT *ep, int *env, void *args);

void assert_job_return_value(EVENT *ep, int *env, void *args);

void assert_job_exited(EVENT *ep, int *env, void *args);

void assert_job_aborted(EVENT *ep, int *env, void *args);
