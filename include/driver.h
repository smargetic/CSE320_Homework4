/*
 * Maximum number of jobs we can create in one test script.
 */
#define JOBIDS_MAX 100

/*
 * Maximum size of buffer for command to be sent.
 */
#define SEND_MAX 200

/*
 * Commands for test scripts.
 */
typedef void (*ACTION)(EVENT *ep, int *env, void *args);

typedef struct command {
    char *send;
    EVENT_TYPE expect;
    int timeout;
    ACTION before;
    ACTION after;
    void *args;
} COMMAND;

int run_test(char *name, char *target, COMMAND *script, int *statusp);
