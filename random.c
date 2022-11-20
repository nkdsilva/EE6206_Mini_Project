#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<math.h>
#include<time.h>

#define LIST 100

typedef struct
{
  char student_index[15];//EG/2018/xxxx
  float assgnmt01_marks;//15%
  float assgnmt02_marks;//15%
  float project_marks;//20%
  float finalExam_marks;//50%
}student_marks;

student_marks markList[LIST];
int regList[LIST];

float generateNum(int lower, int upper)
{
  int num1=(rand()%(upper-lower+1)+lower);
  int num2=(rand()%(99-1));
  float num=num1+(float)num2/100;
  if(num>upper)
  {
    num-=1;
  }
  if(num<lower)
  {
    num+=1;
  }
  return num;
}

int regGenerate(int lower, int upper)
{
  int arrayLength=0;
  int num = (rand()%(upper-lower+1))+lower;
  for(int i=0;i<LIST;i++)
  {
    if(regList[i]==num)
    {
      num=regGenerate(lower,upper);
    }
    if(regList[i]==0)
    {
      arrayLength=i;
    }
  }
  regList[arrayLength]=num;
  return num;
}

student_marks randomStudent()
{
  int num= regGenerate(1000,2000);
  char snum[100];
  sprintf(snum,"EG/2018/%d",num);
  student_marks temp;
  strncpy(temp.student_index,snum,12);
  temp.assgnmt01_marks= generateNum(0,15);
  temp.assgnmt02_marks= generateNum(0,15);
  temp.project_marks= generateNum(0,20);
  temp.finalExam_marks= generateNum(0,50);
  return temp;
}

student_marks* studentList()
{
  for(int i=0;i<LIST;i++)
  {
    student_marks student = randomStudent();
    markList[i] = student;        
    printf("%d\t%s\t%f\t%f\t%f\t%f\n",i+1,
    student.student_index,
    student.assgnmt01_marks,
    student.assgnmt02_marks,
    student.project_marks,
    student.finalExam_marks);
  }
  return markList;
}

void main()
{
  srand(time(0));
  for(int a=0;a<LIST;a++)
  {
    regList[a]=0;
  }
  int fd = open("StudentMarks.txt",O_RDWR|O_CREAT|O_TRUNC,0644);
  student_marks* tempList= studentList();
  for(int j=0;j<LIST;j++)
  {
    student_marks temp= *(tempList+j);
    int write_ret = write(fd,&temp,sizeof(temp));
    if(write_ret == -1)
    {
      printf("Write file error:%d\n",errno);
      perror("File write error: ");
      exit(1);
    }
  }
  close(fd);
  printf("Generated 100 student records successfully\n");
}








