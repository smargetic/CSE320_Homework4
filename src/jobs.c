/*
 * Job manager for "jobber".
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>


#include "jobber.h"
#include "task.h"

#include "anotherHeader.h"

//MUST CREATE THE JOB TABLE
//static char* table[MAX_JOBS];
//static
 struct attributes{
    int jobNumber;
    JOB_STATUS status;
    //char* task;
    TASK task;
    pid_t pid;
    char* charTask; //NEED TO FREE
    pid_t pidJob;
    //int enabled;
};


//job table
static struct attributes jobs[MAX_JOBS];
static int availibility[MAX_JOBS];
static int enabled;
static int numberJobsRunning;

/*
static void signalHandler(int signo){
    if(signo==SIGINT){

    } else if(signo==SIGQUIT){

    }

}
*/

int setJobToRun(int jobID){
    if(jobs[jobID].status!=WAITING){
        return -1;
    }
    jobs[jobID].status = RUNNING;
    numberJobsRunning++;

    PIPELINE_LIST tempPipeline;
    tempPipeline.first = jobs[jobID].task.pipelines->first;
    tempPipeline.rest = jobs[jobID].task.pipelines->rest;
    int truth1 = 0;
    //int pipelineVal =0;
    while(truth1==0){

        //printf("\nGOT IN OUTSIDE LOOP");

            //I go through the commands and run them concurrently

            //I found out how many commands there are
            int status;
            int truth2 =0;
            int countForCL =0;
            COMMAND_LIST tempCL = *tempPipeline.first->commands;
            while(truth2==0){
                countForCL++;
                if(tempCL.rest!=NULL){
                    tempCL = *tempCL.rest;
                } else{
                    truth2=-1;
                }
            }
            tempCL = *tempPipeline.first->commands;


            int new_pipe[2];
            int old_pipe[2];
            int truth = 0;
            int i =0;
            COMMAND cmd = *tempCL.first;

            while(truth==0){


                    //create a new pipe for each command
                if(tempCL.rest!=NULL){
                    pipe(new_pipe);
                }
                pid_t pid = fork();

                if(pid<0){
                        //major major error
                    abort();
                }else if(pid==0){

                        //child
                    if(i>0){
                            //previous command
                        dup2(old_pipe[0], 0);
                        close(old_pipe[0]);
                        close(old_pipe[1]);
                    }
                        //next command
                    if(tempCL.rest!=NULL){
                        close(new_pipe[0]);
                        dup2(new_pipe[1],1);
                        close(new_pipe[1]);
                    }
                    else {
                        close(new_pipe[0]);
                        close(new_pipe[1]);
                    }

                        int count =0;
                        int length = strlen(cmd.words->first);
                        char* firstWord = malloc(length*sizeof(char));
                        strcpy(firstWord, cmd.words->first);
                        // = cmd.words->first;
                        COMMAND tempCommand = cmd;
                        while(cmd.words->rest!=NULL){
                            count++;
                            cmd.words = cmd.words->rest;
                        }

                        char* args[count+2];
                        for(int t=0; t<=count; t++){
                            args[t] = tempCommand.words->first;
                            tempCommand.words = tempCommand.words->rest;
                        }
                        args[count+1]=NULL;
                        //printf("\nTHIS IS FIRST WORD: %s", firstWord);
                        //printf("\nTHIS IS THE NEXT WORD: %s", args[0]);
                        //printf("\nTHIS IS THE NEXT WORD: %s", args[1]);
                        /*
                        if(tempCL.rest==NULL){
                            printf("\nNEXT IS BLANK");
                            int loc = open("blankFile", O_CREAT | O_RDWR);
                            dup2(loc, 1);
                            execvp(firstWord, args);
                            close(loc);
                        }
                        */
                        if(execvp(firstWord, args)<0){
                            jobs[jobID].status = ABORTED;
                            sf_job_status_change(jobID, RUNNING,ABORTED);
                            abort();
                        }
                        if(tempCL.rest==NULL){
                            //exit(0);
                        }
                } else if(pid>0){
                    if(i==0){
                        sf_job_start(jobID, getppid());
                        sf_job_status_change(jobID, WAITING,RUNNING);
                        jobs[jobID].pid = getppid();
                    }
                    if(i>0){
                        //previous command
                        close(old_pipe[0]);
                        close(old_pipe[1]);
                    }

                    //printf("\nNEXT COMMAND %s", tempCL.rest->first->words->first);

                    if(tempCL.rest!=NULL){
                        for(int i=0; i<2; i++){
                            old_pipe[i] = new_pipe[i];
                        }
                    }

                }
                i++;
                if((i>0)&&(tempCL.rest==NULL)){
                    truth =-1;
                    //exit(0);
                } else if((i>0)&&(tempCL.rest!= NULL)){
                    tempCL = *tempCL.rest;
                    cmd = *tempCL.first;
                }
                pid = waitpid(-1, &status, 0);
                //while((pid = waitpid(-1, &status, 0))>0){
                //    if(WIFEXITED(status)){


                  //  }
                //}

            }
        if(tempPipeline.rest==NULL){
            //printf("\nNEXT IS NULL\n");
            jobs[jobID].status = COMPLETED;
            sf_job_status_change(jobID, RUNNING,COMPLETED);
            numberJobsRunning--;
            truth1 =-1;
        } else{
            tempPipeline = *tempPipeline.rest;
        }

        //truth1 =-1;
    }

    return 0;
}




int jobs_init(void) { //Initialize spooler
    //Initialize the jobs facility at the very beginning
    for(int i=0; i<MAX_JOBS;i++){
        availibility[i] =0;
        jobs[i].pidJob =-1;
    }
    enabled =0;
    // TO BE IMPLEMENTED
    abort();
}

void jobs_fini(void) { //Finalize job spooler
    for(int i=0; i<MAX_JOBS;i++){
        availibility[i]=0;
    }

    // TO BE IMPLEMENTED
    //abort();
}

int jobs_set_enabled(int val) { //Set whether jobs can be started
    //enabled =1;
    //int locations[8];
    if(val ==1){
        enabled = 1;
        for(int i=0; i< 8; i++){
            if(numberJobsRunning<=4){
                setJobToRun(i);
            }
        }

        //if(numberJobsRunning<=4){

            //setJobToRun
            // the enabled flag is set, I must set them to run
        //}
    }
    //printf("GET HERe\n");
    //setJobToRun(0);
    if(val ==0){
        enabled =0;
    }
    return 0;
}

int jobs_get_enabled() { //Get whether jobs can be started
    if(enabled ==1){
        return 0;
    }
    return -1;
    // TO BE IMPLEMENTED

    //abort();
}

int job_create(char *command) { //create a new job

    int location =-1;

    for(int i=0;i<MAX_JOBS;i++){
        if(availibility[i]==0){
            availibility[i]=1;
            location = i;
            break;
        }
    }
    if(location==-1){
        //printf("\nGOT HERE\n");
        //printf("\nError: spool");
        return -1;
    }
    sf_job_create(location);
    //sf_job_create
    char* charTaskVal = command;
    char **doublePtr = &command;
    TASK* taskGiven = parse_task(doublePtr);
    //char* first = taskGiven->pipelines->first->commands->first->words->first;
    jobs[location].jobNumber = (location);
    jobs[location].task = *taskGiven;
    sf_job_status_change(location, WAITING, NEW);
    jobs[location].status = WAITING;
    jobs[location].charTask = charTaskVal;
    jobs[location].pidJob = -1;
    if((enabled==1)&&(numberJobsRunning<=4)){
        setJobToRun(location);
    }
    return(0);
    //abort();
}

int job_expunge(int jobid) { //expunge a job
    if((jobs[jobid].status==1)||(jobs[jobid].status==6)){
        //I remove the job from the job table
        availibility[jobid]=0;
        //I set data equal to 0
        jobs[jobid].task = *(TASK *)NULL;
        jobs[jobid].charTask = NULL;
        jobs[jobid].status =-1;

        sf_job_expunge(jobid);

        return 0;
    }
    return -1;
}

int job_cancel(int jobid) {  //Atempt to cancel a job
    //USE SIGINT --> terminates process
    //
    if(jobs[jobid].pid!=-1){

    } //else {
    return -1;
    //}
    // TO BE IMPLEMENTED
    //abort();
}

int job_pause(int jobid) { //Pause a running job
    //USE SIGSTP -->pauses process
    if(jobs[jobid].pid!=-1){
        sf_job_pause(jobid, jobs[jobid].pidJob);
    } //else {
    return -1;
    // TO BE IMPLEMENTED
    //abort();
    return -1;
}

int job_resume(int jobid) { //Resume a puased job
    //USE SIGCONT
    if(jobs[jobid].pid!=-1){
        sf_job_resume(jobid, jobs[jobid].pidJob);
    } //else {
    return -1;
    // TO BE IMPLEMENTED
    abort();
}

int job_get_pgid(int jobid) { //Get the process group ID of a job's runner process
    if(availibility[jobid]!=1){
        return -1;
    }
    //NEED TO FIX
    return jobs[jobid].pid;
    // TO BE IMPLEMENTED
    //abort();
}

JOB_STATUS job_get_status(int jobid) {//Get the current status of a job
    //I check to make sure the job exists
    if(availibility[jobid]!=1){
        return -1;
    }
    //I return the job status
    JOB_STATUS jobStatus = jobs[jobid].status;
    return jobStatus;

    //if job has been completed, then exit status will be shown as well
    // TO BE IMPLEMENTED
    //abort();
}

int job_get_result(int jobid) { //Get the result (exit status) of a job
    return(jobs[jobid].status);
    // TO BE IMPLEMENTED
    //abort();
}

int job_was_canceled(int jobid) { //Determine if a job was successfully canceled
    // TO BE IMPLEMENTED
    abort();
}

char *job_get_taskspec(int jobid) { //Get the task specification of a job
    if(availibility[jobid]!=1){
        return NULL;
    }

    return jobs[jobid].charTask;
    // TO BE IMPLEMENTED
    //abort();
}
