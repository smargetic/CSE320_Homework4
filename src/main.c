#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "jobber.h"
#include "task.h"
//#include "jobs.h"
//CANNOT ADD OTHER FUNCTIONS TO MAIN --> WILL ONLY TEST MAIN


int main(int argc, char *argv[])
{
    // TO BE IMPLEMENTED

    int true =0;
    //outer while loop processes multiple commands
    while(true==0){

        char* answer = sf_readline("jobber> ");
        if(*answer!='\0'){
        //printf("ANSWER: %s\n", answer);
        int N = strlen(answer);
        char* tempAnswer = (char*)malloc(N*sizeof(char));
        // = answer;
        //int length =0;
        strcpy(tempAnswer, answer);

        answer = strtok(answer, " ");
        int lengthFirstPart = strlen(answer);
        char* ptr2First = (char*)malloc(lengthFirstPart*sizeof(char));

        strcpy(ptr2First, answer);

        strcpy(answer, tempAnswer);

        //int taskNumber =0;

        if(strcmp(answer, "help")==0){
            //printf("\nHELPPPP");
            printf("\nAvailable commands:");
            printf("\nhelp (0 args) Print this help message");
            printf("\nquit (0 args) Quit the program");
            printf("\nenable (0 args) Allow jobs to start");
            printf("\ndisable (0 args) Prevent jobs from starting");
            printf("\nspool (1 args) Spool a new job");
            printf("\npause (1 args) Pause a running job");
            printf("\nresume (1 args) Resume a paused job");
            printf("\ncancel (1 args) Cancel an unfinished job");
            printf("\nexpunge (1 args) Expunge a finished job");
            printf("\nstatus (1 args) Print the status of a job");
            printf("\njobs (0 args) Print the status of all jobs\n");
        //return 1;
        } else if (strcmp(answer, "quit")==0){
            //printf("\nQUITTT\n");
            //NEED TO CHANGE STATUS TO ABORTED
            //NEED TO EXPUNGE
            jobs_fini();
            true =-1;

        } else if (strcmp(ptr2First, "status")==0){
            for(int t=0; t<7;t++){
                answer++;
            }
            int value = *answer - '0';
            int lengthAfter = strlen(answer);
            if(!(lengthAfter>1)){
                if(value<8){
                    if(job_get_status(value)!=-1){
                        //thing does exist

                    }
                    //int value = String.valueOf(*answer);
                    //printf("\nVALUE AFTER %s", answer);
                    //printf("\nVALUE OF INT %d\n", value);
                }
            }
        } else if (strcmp(answer, "jobs")==0){
            //if nothing is available
            //int somethingFound =0;
            int enabledStat = jobs_get_enabled();
            if(enabledStat ==0){
                printf("Starting jobs is enabled\n");
            } else {
                printf("Starting jobs is disabled\n");
            }

            for(int m=0;m<8;m++){
                int stat = job_get_status(m);
                if(stat!=-1){
                    //somethingFound =1;
                    char* tempString = job_get_taskspec(m);
                    if(stat==0){
                        printf("job %d [new]: %s\n", m, tempString);
                    } else if(stat==1){
                        printf("job %d [waiting]: %s\n", m, tempString);
                    } else if(stat==2){
                        printf("job %d [running]: %s\n", m, tempString);
                    } else if(stat==3){
                        printf("job %d [paused]: %s\n", m, tempString);
                    } else if(stat ==4){
                        printf("job %d [canceled]: %s\n", m, tempString);
                    } else if(stat ==5){
                        printf("job %d [completed]: %s\n", m, tempString);
                    } else if(stat ==6){
                        printf("job %d [aborted]: %s\n", m, tempString);
                    }
                }
            }

        } else if (strcmp(answer, "enable")==0){
            //printf("IN HERE\n");
            if(jobs_get_enabled()==-1){
                //printf("ENABLE");
                //for(int i=0; i<8; i++){
                jobs_set_enabled(1);
                //    jobs_set_enabled(i);
                //}
            } else {

            }
            jobs_set_enabled(0);
        } else if (strcmp(answer, "disable")==0){
            jobs_set_enabled(0);

        } else if (strcmp(ptr2First, "spool")==0){
            //strcpy(answer, tempAnswer);
            //printf("SPoOOLL\n");

            answer = (answer+6);

            int count=1;
            int goInSpaceCount =0;
            char* tempPointer = answer;
            int length = strlen(answer);
            int beginPresent =0;
            int endPresent =0;
            int countCorrect =-1;
            int wrongness =0;

            //If only the spool command was given
            if(*tempPointer=='\0'){
                printf("\nWrong number of args (given %d, required: 1) for command 'spool'\n", (count-1));
                wrongness++;
                //break;
            }else if((*tempPointer =='\'')&&(wrongness==0)){
                //if there are quotes present, it only gets taken as 1 argument
                for(int i =0; i<length; i++){
                    if(*tempPointer=='\''){
                        if(i==0){
                            beginPresent++;
                        }
                        if(i==(length-1)){
                            endPresent++;
                        }
                    }
                }
            }

            if(((beginPresent==0)||(endPresent==0))&&(wrongness==0)){ //only possibly gets set if there are quotes
                //either the end or beginning quote is missing, therefore must be counted as seperate arguments
                goInSpaceCount++;
            } else{
                if(wrongness==0){
                    countCorrect =0;
                }
            }

            if((goInSpaceCount!=0)&&(wrongness==0)){
                //I count the number of arguments seperately
                tempPointer = answer;
                count++;

                for(int i=0;i< length; i++){
                    if(*tempPointer == ' '){
                        count ++;
                    }
                    tempPointer++;
                }

            }

            //I check that the right number of arguments were given in order to execution
            if((count>2)&&(wrongness==0)){
                printf("\nWrong number of args (given %d, required: 1) for command 'spool'\n", (count-1));
                //break;
                wrongness++;
            } else{
                if(wrongness==0){
                    countCorrect =0;
                }
            }

            //If the right number of arguments are given, I exectue
            if((countCorrect==0)&&(wrongness==0)){
                //SHOULD REMOVE QUOTES
                if((beginPresent==1)&&(endPresent==1)){
                    //I need to remove the quotes
                    int length = strlen(answer);
                    answer++;
                    answer[length-2] = '\0';
                }

                /*
                char **doublePtr = &answer;
                TASK* taskGiven = parse_task(doublePtr);
                char* first = taskGiven->pipelines->first->commands->first->words->first;
                jobs_init();
                printf("LAST PART: %s\n", first);
                */
                //printf("\nGOING TO CREATE\n");
                int returnJobCreate = job_create(answer);
                if(returnJobCreate == -1){
                    printf("Error: spool");
                }

            }

        } else if (strcmp(ptr2First, "pause")==0){
            int value = *answer - '0';
            int lengthAfter = strlen(answer);
            if(!(lengthAfter>1)){
                if(value<8){
                    if(job_get_status(value)!=-1){
                        //thing does exist
                        job_pause(value);
                    }
                }
            }
            //I get the int that follows the command
        } else if (strcmp(ptr2First, "resume")==0){
            int value = *answer - '0';
            int lengthAfter = strlen(answer);
            if(!(lengthAfter>1)){
                if(value<8){
                    if(job_get_status(value)!=-1){
                        //thing does exist
                        job_resume(value);
                    }
                }
            }
            //job_resume()
            //I get the int that follows the command
        } else if (strcmp(ptr2First, "cancel")==0){
            int value = *answer - '0';
            int lengthAfter = strlen(answer);
            if(!(lengthAfter>1)){
                if(value<8){
                    if(job_get_status(value)!=-1){
                        //thing does exist
                        //job_resume(value);
                    }
                }
            }
            //I get the int that follows the command
        } else if(strcmp(ptr2First, "expunge")==0){
            int value = *answer - '0';
            int lengthAfter = strlen(answer);
            if(!(lengthAfter>1)){
                if(value<8){
                    if(job_get_status(value)!=-1){
                        int complete =job_expunge(value);
                        if(complete==-1){
                            printf("\nError: expunge");
                        }
                    }
                }
            }
        } else {
            printf("Unrecognized command: %s\n",answer);

        }
    }
    }
    //return(-1);
    exit(EXIT_FAILURE);
    //return -1;
}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */
