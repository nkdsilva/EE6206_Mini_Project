#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

struct student_marks
{
  char student_index[15];
  float assgnmt01_marks;
  float assgnmt02_marks;
  float project_marks;
  float finalExam_marks;
};

void printStudent( struct student_marks *student);
void dataInsert(struct student_marks *student);
int dataSearch(struct student_marks *student, int *position);
void studentRecord();
int insertRecord();
int editRecord();
int searchRecord();
int updateRecord();

int main()
{
  printf("Module code:ZZ6309\n");
  printf("Module name:Linux Operating Systems\n");
  studentRecord();
  return 0;
}

void printStudent(struct student_marks *student)
{
  printf("\nStudent Index:%s\nAssignment 01:%.2f\nAssignment 02:%.2f\nProject:%.2f\nFinal exam:%.2f\n",student->student_index,
  student->assgnmt01_marks,
  student->assgnmt02_marks,
  student->project_marks,
  student->finalExam_marks);
  printf("Successfully inserted!\n");
}

void studentRecord()
{
  int option;
  printf("1.Insert\n");
  printf("2.Update\n");
  printf("3.Search\n");
  printf("Enter option: ");
  
  scanf("%d",&option);
  
  switch(option)
  {
    case 1:
      insertRecord();
      break;
    case 2:
      //editRecord();
      break;
    case 3:
      searchRecord();
      break;
    default:
      printf("Invalid option\n");
  }
}
void dataInsert(struct student_marks *student)
{
  FILE *fd;
  fd = fopen("Marks.txt","a+");
  if(fd==NULL)
  {
    printf("File open error:%d\n",errno);
    perror("File open\n");
    exit(0);
  }
  fprintf(fd,"%s,%.2f,%.2f,%.2f,%.2f\n",student->student_index,
  student->assgnmt01_marks,
  student->assgnmt02_marks,
  student->project_marks,
  student->finalExam_marks);
  fclose(fd);
}

int dataSearch(struct student_marks *student, int *position)
{
  FILE *fd;
  fd = fopen("StudentMarks.txt","a+");
  if(fd==NULL)
  {
    printf("File open error:%d\n",errno);
    perror("File open\n");
    exit(0);
  }
  char line[1024];
  char *token;
  
  while(fgets(line,1024,fd))
  {
    char *index=strtok(line,",");
    if(strcmp(index,student->student_index)==0)
    {
      printf("Found the record\n");
      printf("Student index:%s\n",student->student_index);
      
      token=strtok(NULL,",");
      student->assgnmt01_marks=atof(token);
      token=strtok(NULL,",");
      student->assgnmt02_marks=atof(token);
      token=strtok(NULL,",");
      student->project_marks=atof(token);
      token=strtok(NULL,",");
      student->finalExam_marks=atof(token);
      
      if(position!=NULL)
      {
        *position=ftell(fd);
      }
      if(fclose(fd)!=0)
      {
        printf("Data search failed\n");
        exit(0);
      }
      return 0;
    }
  }
  printf("Not found\n");
  fclose(fd);
  return -1;
}

int insertRecord()
{
  struct student_marks *student = (struct student_marks *)malloc(sizeof(struct student_marks));
  
  printf("Enter student index number: ");
  scanf("%s",student->student_index);
  printf("Assignment 01 marks: ");
  scanf("%f",&student->assgnmt01_marks);
  printf("Assignment 02 marks: ");
  scanf("%f",&student->assgnmt02_marks);
  printf("Project marks: ");
  scanf("%f",&student->project_marks);
  printf("Final exam marks: ");
  scanf("%f",&student->finalExam_marks);
  dataInsert(student);
  printStudent(student);
  return 0;
}

int searchRecord()
{
  struct student_marks *student=(struct student_marks *)malloc(sizeof(struct student_marks));
  
  printf("Enter student registration number: ");
  scanf("%s",student->student_index);
  
  if(dataSearch(student,NULL)!=0)
  {
    printf("%s can't be found\n",student->student_index);
    exit(0);
  }
  return 0;
}

void updateRecord()
{
  struct student_marks student;
  FILE *fd;
  char student_index[15];
  printf("Enter student index: ");
  scanf("%s",student.student_index);
  
  fd=fopen("Marks.txt","r+");
  if(fd==NULL)
  {
    printf("Open file error:%d\n",errno);
    perror("File read error: ");
    exit(1);
  }
  while(1)
  {
    fread(&student,sizeof(student_marks),1,fd);
    if(feof(fd))
    {
      printf("Read file error:%d\n",errno);
      perror("Read error: ");
      exit(1);
    }
    else
    {
      if(strcmp(student.student_index,student_index)==0)
      {
        fseek(fd,-sizeof(student_marks),SEEK_CUR);
        
        printf("Enter student index number: ");
        scanf("%s",student.student_index);
  
        printf("Assignment 01 marks: ");
        scanf("%f",&student.assgnmt01_marks);
  
        printf("Assignment 02 marks: ");
        scanf("%f",&student.assgnmt02_marks);
  
        printf("Project marks: ");
        scanf("%f",&student.project_marks);
  
        printf("Final exam marks: ");
        scanf("%f",&student.finalExam_marks);
        
        int write_ret = fwrite(&student,sizeof(student_marks),1,fd);
        if(write_ret==-1)
        {
          printf("Write file error:%d\n",errno);
          perror("File write error: ");
          exit(1);
        }
        printf("Successfully updated\n");
      }
    }
  }
  fclose(fd);
}





