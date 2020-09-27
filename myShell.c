#include "myShellFunc.h"
#include "myShellFunc.c"

int main()
{
  char *args[MAXLINE/2 + 1] = {'\0', }, *newArgs[MAXLINE/2+1]={'\0',};
  char *cmds[MAXCMD]={'\0', };
  char *nth=(char*)malloc(sizeof(MAXLINE));
  int should_run = 1;
  int pid,n,i,flag,and=0;

  while(1) {
    printf("myShell>");
    fflush(stdout);

    getCmd(args,cmds);

    pid_t pid;
    pid=fork();

    if(pid==0){

      if(strcmp(args[0],"exit")==0){should_run=0;return 0;} //exit명령어 입력시 탈출
      if(cmds[ccnt-1][strlen(cmds[ccnt-1])-1]=='&')and=1;  //백그라운드로 실행

      if(strcmp(args[0],"history")==0)showHistory(cmds);  //history 명령어 입력
      if(strcmp(args[0],"!!")==0)recentHistory(cmds,args);  //!!입력
      if(args[0][0]=='!'&&args[0][1]!='!'){ //!n입력시
        strncpy(nth,args[0]+1, strlen(args[0])-1);
        n=atoi(nth);
        nthHistory(cmds,args,n);
      }

      i=0;flag=0;
      while(i<cnt){
        if(strcmp(args[i], ">")==0){ // redirect in인 경우
          flag= 1;
          redirectOut(args);
          execvp(args[0],args);
          break;
        }
        else if(strcmp(args[i], "<")==0){ //redirect out인 경우
          flag=1;
          redirectIn(args);
          execvp(args[0],args);
          break;
        }
        else if(strcmp(args[i], "|")==0){ //파이프인 경우
          flag= 1;
          pipeFunc(args);
          break;
        }
        i++;
      }

      if(flag==0) execvp(args[0],args);  //일반 명령어 처리

    }

    else if(pid>0){ //부모 프로세스일때
      if(and==0) wait(NULL);  //&가 포함되어 있으면 기다린다
      if(strcmp(args[0],"exit")==0){should_run=0;return 0;}//exit가 입력되면 부모 프로세스도 끝나야 하기에 죽인다.
    }
    else{ //fork가 정상적으로 실행 되지 않았을때 에러메세지 출력후 종료
      fprintf(stderr, "프로세스를 생성할 수 없습니다.\n");
      should_run=0;
    }
    freeArgs(args);
  }
  return 0;
}
