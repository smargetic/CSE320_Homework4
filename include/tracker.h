typedef struct job {
    int jobid;                      /* Job ID of job. */
    JOB_STATUS status;              /* Status of job. */
    int started;                    /* Whether job has been started. */
    int ended;                      /* Whether job has ended. */
    int canceled;                   /* Whether job was canceled by user. */
    int stopped;                    /* Whether the job is currently stopped, according to SIGCHLD . */
    int result;			    /* Exit status of job. */
    int pgid;                       /* Process group ID of job pipeline. */
    struct timeval creation_time;   /* Time job was queued. */
    struct timeval change_time;     /* Time of last status change. */
} JOB;

JOB *job_table[MAX_JOBS];

void tracker_show_jobs(void);
