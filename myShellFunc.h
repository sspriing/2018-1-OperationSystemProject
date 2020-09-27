#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXLINE 80
#define MAXCMD 100

void getCmd(char* args[],char* cmds[]); //명령문 받음
void freeArgs(char* args[]); //메모리 해제

void showHistory(char* cmds[]); //100개까지의 명령어 입력내역 출력
void recentHistory(char*cmds[], char* args[]);  //바로 직전의 명령어 실행
void nthHistory(char*cmds[],char* args[], int n); //n번 전의 명령어 실행
void exeCmd(char* cmd ,char* args[]); //history기능에서 명령어 실행시 사용하는 함수

void redirectIn(char*args[]);//redirect in 함수
void redirectOut(char* args[]);//redirect out 함수
void pipeFunc(char* args[]);//파이프 함수
