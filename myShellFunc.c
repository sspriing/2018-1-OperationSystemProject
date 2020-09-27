int cnt=0,ccnt=0;

void getCmd(char* args[],char* cmds[]){
  int argIndex = 0;
  char command[MAXLINE] = {'\0', };

  fgets(command, sizeof(command), stdin);//명령어 입력

  cmds[ccnt]=(char*)malloc(strlen(command));
  strncpy(cmds[ccnt],command,strlen(command));//히스토리에 저장
  ccnt++;

    //strotk함수 사용시 에러가 많아 직접 나누어준다.
  command[strlen(command) - 1] = 0;
  for(int i = 0, j = 0; i < strlen(command) + 1; i++) {
    if(command[i] == ' ' || command[i] == '\0') {//space or null
      args[argIndex] = (char *)malloc(sizeof(char)*(j + 1));//memory allocate
      strncpy(args[argIndex], &command[i - j], j);//string copy
      args[argIndex++][j] = 0;
      cnt++;
      j = -1;
    }
    j++;
  }
}

void freeArgs(char *args[])
{
  int i = 0;
  while(i < cnt) {
    free(args[i]);
    i++;
  }
  cnt = 0;
}



void showHistory(char* cmds[]){
  //history 배열안의 값들 출력
  for(int i=0;i<ccnt-1;i++){
    printf("%d %s",ccnt-i-1, cmds[i]);
  }
}

void recentHistory(char* cmds[], char* args[]){
  freeArgs(args);  //기존에 있던 args배열을 해제해줌
  exeCmd(cmds[ccnt-2],args);  //이전의 커맨드 실행시킨다
}

void nthHistory(char* cmds[],char* args[], int n){
  freeArgs(args);  //기존에 있던 args배열을 해제해줌
    if(cmds[ccnt-n-2]==NULL){printf("NO HISTORY\n");return;}  //예외처리
  printf("%s", cmds[ccnt-n-1]);
  exeCmd(cmds[ccnt-n-1],args);  //n번째 커맨드 실행시킨다
}

void exeCmd(char* cmd,char* args[]){
  int argIndex = 0;
  char* command;

  command=(char*)malloc(strlen(cmd));
  strncpy(command,cmd,strlen(cmd));

  //strotk함수 사용시 에러가 많아 직접 나누어준다.
  command[strlen(command) - 1] = 0;
  for(int i = 0, j = 0; i < strlen(command) + 1; i++) {
    if(command[i] == ' ' || command[i] == '\0') { //space or NULL
      args[argIndex] = (char *)malloc(sizeof(char)*(j + 1)); //memory allocate
      strncpy(args[argIndex], &command[i - j], j); //string copy
      args[argIndex++][j] = 0;
      cnt++;
      j = -1; //문지열의 갯수를 다시 초기화 한다
    }
    j++;
  }
}

void redirectIn(char* args[]){
  int i = 1, fd;

  while(strcmp(args[i], "<") != 0)//'<' 와 같을 시 루프문 탈출
    i++;

  if(args) {
    if(!args[i + 1]) return;
    else {//파일을 연다
      if((fd = open(args[i + 1], O_RDONLY)) == -1) {
        fprintf(stderr, "%s\n", args[i + 1]);
        return;
      }
    }
   dup2(fd, 0);
   close(fd);

   while(args[i] != NULL) {
     args[i] = args[i + 2];
     i++;
   }
   args[i] = NULL;
  }
}

void redirectOut(char* args[]){
  int i = 0, fd;

  while(strcmp(args[i], ">") != 0)//'>' 와 같을 시 루프문 탈출
    i++;

  if(args[i]) {
    if(args[i + 1] == NULL) return;//> 가 마지막으로 입력된 문자일 때
    else {
      if((fd = open(args[i + 1], O_RDWR | O_CREAT | S_IROTH, 0644)) == -1) {
        fprintf(stderr, "%s\n", args[i + 1]);
	return;
      }
    }
   dup2(fd, 1);
   close(fd); //출력을 넣고 닫는다.
   args[i] = NULL;
   args[i + 1] = NULL;

   while(args[i] != NULL) {
     args[i] = args[i + 2];
   }
   args[i] = NULL;
  }
}

void pipeFunc(char* args[]){
  int i = 0, j = 0, fd[2];
  pid_t pid;
  char *argsPipe1[MAXLINE], *argsPipe2[MAXLINE];

  while(args[i] != NULL) {//| 문자 전의 문자열 주소 복사
    if(strcmp(args[i], "|") == 0) {
      argsPipe1[i] = NULL;
      break;
    }
    argsPipe1[i] = args[i];
    i++;
  }
  i++;
  while(args[i] != NULL) {//| 문자 이후의 문자열 주소 복사
    argsPipe2[j] = args[i];
    i++;j++;
  }

  //파이프 생성
  if(pipe(fd) == -1) {
    fprintf(stderr, "파이프 생성에 실패했습니다.");
    exit(1);
  }
  close(0);
  close(fd[1]);//입력 파이프 닫기
  dup(fd[0]);//식별자 복사
  close(fd[0]);
  execvp(argsPipe1[0], argsPipe1);//앞쪽 명령어 실행
  exit(1);

  pid = fork();

  if(pid == 0) {
    close(1);//STDOUT_FILENO 표준출력 닫기
    close(fd[0]);//출력 파이프 닫기
    dup(fd[1]);//식별자 복사
    close(fd[1]);
    execvp(argsPipe2[0], argsPipe2);//뒤쪽 명령어 실행
    exit(0);
  }
}
