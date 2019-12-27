
#define EVENT_SOCKET "event_socket"

typedef enum event_type {
    NO_EVENT, ANY_EVENT, EOF_EVENT,
    CREATE_EVENT, EXPUNGE_EVENT, STATUS_EVENT,
    START_EVENT, END_EVENT, PAUSE_EVENT, RESUME_EVENT
} EVENT_TYPE;

typedef struct event {
    int type;
    int jobid;
    int pgid;
    JOB_STATUS oldstat;
    JOB_STATUS newstat;
    int result;
    struct timeval time;
} EVENT;

extern char *event_type_names[];
