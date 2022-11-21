#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

typedef struct student_marks
{
  char student_index[15];
  float assgnmt01_marks;
  float assgnmt02_marks;
  float project_marks;
  float finalExam_marks;
}student_marks;

float markMax();
float markMin();
float markAvg();
float less5();

int main(void)
{
  int fd1_des[2],fd2_des[2],fd3_des[2];
  pid_t ppid = getpid();//Get parent id
  if(pipe(fd1_des)==-1 || pipe(fd2_des)==-1|| pipe(fd3_des)==-1)//Handling pipe error
  {
    perror("Pipe error: ");
    printf("Error no:%d\n",errno);
    exit(0);
  }
  else
  {
    printf("Pipes created successfully\n");
    pid_t PID_1 =fork();
    if(PID_1==-1)//Handling fork error
    {
      perror("Fork error: ");
      printf("Error no:%d\n",errno);
      exit(0);
    }
    else if(PID_1==0)
    {
      //Child process,C1
      pid_t PID_2=fork();
      if(PID_2==-1)
      {
        perror("PID_2 Fork error: ");
        printf("Error no:%d\n",errno);
        exit(0);
      }
      else if(PID_2==0)
      {
        //Child CC1
        //Get maximum of marks
        close(fd1_des[0]);
        float max  = markMax();
        int write_ret1= write(fd1_des[1],&max,sizeof(max));
        close(fd1_des[1]);
        if(write_ret1==-1)
        {
          perror("CC1:Write_ret1 error: ");
          printf("CC1:Error no:%d\n",errno);
          exit(0);
        } 
      }
      else
      {
        //Child C1
        //Get min of marks
        pid_t PID_3=fork();
        if(PID_3==-1)
        {
          perror("PID_3 Fork error: ");
          printf("Error no:%d\n",errno);
          exit(0);
        }
        else if(PID_3==0)
        {
          //Child CC2
          //Get maximum of marks
          close(fd2_des[0]);
          float min  = markMin();
          int write_ret2= write(fd2_des[1],&min,sizeof(min));
          close(fd2_des[1]);
          if(write_ret2==-1)
          {
            perror("CC2:Write_ret2 error: ");
            printf("CC2:Error no:%d\n",errno);
            exit(0);
          } 
        }
        else
        {
          close(fd3_des[0]);
          float avg  = markAvg();
          int write_ret3= write(fd2_des[1],&avg,sizeof(avg));
          close(fd3_des[1]);
          if(write_ret3==-1)
          {
            perror("CC2:Write_ret3 error: ");
            printf("CC2:Error no:%d\n",errno);
            exit(0);
          }   
        }
      }
    }
    else
    {
      //Parent process,P
      float min,max,avg;
      int less5;
      sleep(0.01);
      close(fd1_des[1]);
      int read_ret1= read(fd1_des[0],&max,sizeof(max));
      close(fd1_des[0]);
      if(read_ret1==-1)
      {
        perror("Parent:Read max error: ");
        printf("Parent:Error no:%d\n",errno);
        exit(0);
      }
      close(fd2_des[1]);
      int read_ret2= read(fd2_des[0],&min,sizeof(min));
      close(fd2_des[0]);
      if(read_ret2==-1)
      {
        perror("Parent:Read min error: ");
        printf("Parent:Error no:%d\n",errno);
        exit(0);
      }
      close(fd3_des[1]);
      int read_ret3= read(fd3_des[0],&avg,sizeof(avg));
      close(fd3_des[0]);
      if(read_ret3==-1)
      {
        perror("Parent:Read avg error: ");
        printf("Parent:Error no:%d\n",errno);
        exit(0);
      }
      printf("Analyzing the student marks:\n");
      printf("---------------------------------\n");
      printf("Maximum mark=%.2f\nMinimum mark=%.2f\nAverage mark=%.2f\n",max,min,avg);
    }
  }
}

float markMax()
{
  FILE *fd;
  student_marks student;
  float max=0;
  int errNo;
  
  fd = fopen("StudentMarks.txt","r");
  if(fd==NULL)
  {
    perror("File open error: ");
    printf("Error no:%d\n",errno);
    exit(1);
  }
  while(1)
  {
    fread(&student,sizeof(student_marks),1,fd);
    if(feof(fd))
    {
      break;
    }
    if((errNo==ferror(fd))>0)
    {
      perror("File read error: ");
      printf("Error no:%d\n",errno);
      exit(1);
    }
    else
    {
      if(student.assgnmt01_marks>max)
      {
        max=student.assgnmt01_marks;
      }
    }
  }
  fclose(fd);
  return max;
}

float markMin()
{
  FILE *fd;
  student_marks student;
  float min=0;
  int errNo;
  
  fd = fopen("StudentMarks.txt","r");
  if(fd==NULL)
  {
    perror("File open error: ");
    printf("Error no:%d\n",errno);
    exit(1);
  }
  while(1)
  {
    fread(&student,sizeof(student_marks),1,fd);
    if(feof(fd))
    {
      break;
    }
    if((errNo==ferror(fd))>0)
    {
      perror("File read error: ");
      printf("Error no:%d\n",errno);
      exit(1);
    }
    else
    {
      if(student.assgnmt01_marks<min)
      {
        min=student.assgnmt01_marks;
      }
    }
  }
  fclose(fd);
  return min;
}

float markAvg()
{
  FILE *fd;
  student_marks student;
  float avg=0;
  int errNo;
  float sum=0;
  int count=0;
  
  fd = fopen("StudentMarks.txt","r");
  if(fd==NULL)
  {
    perror("File open error: ");
    printf("Error no:%d\n",errno);
    exit(1);
  }
  while(1)
  {
    fread(&student,sizeof(student_marks),1,fd);
    if(feof(fd))
    {
      break;
    }
    if((errNo==ferror(fd))>0)
    {
      perror("File read error: ");
      printf("Error no:%d\n",errno);
      exit(1);
    }
    else
    {
      count++;
      sum+=student.assgnmt01_marks;
    }
  }
  avg=sum/count;
  fclose(fd);
  return avg;
}
