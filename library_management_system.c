#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE_USERNAME 20
#define MAX_SIZE_PASSWORD 20
#define FILE_NAME "lms.bin"
#define FILE_HEADER_SIZE sizeof(sFileHeader)
#define MAX_BOOK_NAME 50
#define MAX_AUTHOR_NAME 50
#define MAX_STU_NAME 50

typedef struct
{
	char username[MAX_SIZE_USERNAME];
	char password[MAX_SIZE_PASSWORD];
}sFileHeader;

int isFileExists(const char *path)
{
	//Try to open file
	FILE *fp=fopen(path,"rb");
	int status = 0;
	// If File does not exist
	if (fp!=NULL)
	{
		status= 1;
		// File exist hence we close the file
		fclose(fp);
	}
	return status;
}

void start()
{
	FILE *fp = NULL;
	
	const char defaultUsername[]="Peeyush\n";
	const char defaultPassword[]="Peeyush\n";
	sFileHeader fileHeaderInfo={0};
	
	int status = 0;
	status= isFileExists(FILE_NAME);
	if (!status)
	{
		// Create the binary file
		fp = fopen(FILE_NAME,"wb");
		if(fp != NULL)
		{
			strncpy(fileHeaderInfo.username,defaultUsername,sizeof(defaultUsername));
			strncpy(fileHeaderInfo.password,defaultPassword,sizeof(defaultPassword));
			fwrite(&fileHeaderInfo,FILE_HEADER_SIZE,1,fp);
			fclose(fp);
		}
	}
}

void welcomeMessage()
{
	printf("\n\n\n\n\n");
	printf("\n\t\t**************************************************\n");
	printf("\n\t\t==================================================");
	printf("\n\t\t=                 WELCOME                        =");
	printf("\n\t\t=                   TO                           =");
	printf("\n\t\t=                 LIBRARY                        =");
	printf("\n\t\t=                MANAGEMENT                      =");
	printf("\n\t\t=                 SYSTEM                         =");
	printf("\n\t\t==================================================");
	printf("\n\n\t\t**************************************************\n");
	printf("\n\n\n\n Enter any key to continue...");
	getch();
}

void printMessageCenter(const char *message)
{
	int len = 0;
	int pos = 0;
	
	// Calculate How Many space we need to print
	len = (68- strlen(message)/2);
	printf("\t\t\t");
	for(pos=0; pos<len ; pos++)
	{
		// Print Space
		printf(" ");
	}
	// Print the Message
	printf("%s",message);	
}

void headMessage(const char *message)
{
	system("cls");
	printf("\t\t\t**********************************************************");
	printf("\n\t\t\t**************** LIBRARY MANAGEMENT SYSTEM ***************");
	printf("\n\t\t\t**********************************************************");
	printMessageCenter(message);
}

typedef struct
{
	unsigned int books_id;
	char bookName[MAX_BOOK_NAME];
	char authorName[MAX_AUTHOR_NAME];
	char studentName[MAX_STU_NAME];
		
}s_BooksInfo;
int isNameValid(const char *name)
{
	int validName=1;
	int len = 0;
	int index= 0;
	len = strlen(name);
	for(index = 0; index<len; ++index)
	{
		if(!(isalpha(name[index])) && (name[index]!='\n') && (name[index]!=' '))
		{
			validName = 0;
			break;	
		}	
	}
	return validName;
	
}
void addBook()
{
	s_BooksInfo addBookInfoInDataBase={0};
	FILE *fp = NULL;
	
	fp=fopen(FILE_NAME,"ab+");
	if(fp==NULL)
	{
		printf("Could NOT Open File");
		exit(1);
	}
	headMessage("ADD NEW BOOKS");
	printf("\n Enter Your Details Below");
	printf("\n ************************");
	printf("\n BOOK ID NO :");
	fflush(stdin);
	scanf("%u",&addBookInfoInDataBase.books_id);
	int status = 0;
	do
	{
		printf("\n\t\t Book Name:");
		fflush(stdin);
		fgets(addBookInfoInDataBase.bookName,MAX_BOOK_NAME,stdin);
		status = isNameValid(addBookInfoInDataBase.bookName);
		if(!status)
		{
			printf("\n\t\t Name Contains Invalid Character. Please Enter Again");
		}
	}while(!status);
	
	do
	{
		printf("\n\t\t Author Name:");
		fflush(stdin);
		fgets(addBookInfoInDataBase.authorName,MAX_AUTHOR_NAME,stdin);
		status = isNameValid(addBookInfoInDataBase.authorName);
		if(!status)
		{
			printf("\n\t\t Author Name Contains Invalid Character. Please Enter Again");
		}
	}while(!status);
	
	do
	{
		printf("\n\t\t Student Name:");
		fflush(stdin);
		fgets(addBookInfoInDataBase.studentName,MAX_STU_NAME,stdin);
		status = isNameValid(addBookInfoInDataBase.studentName);
		if(!status)
		{
			printf("\n\t\t Student Name Contains Invalid Character. Please Enter Again");
		}
	}while(!status);
	
	fwrite(&addBookInfoInDataBase,sizeof(addBookInfoInDataBase),1,fp);
	fclose(fp);
	
}

void searchBook()
{
	int found = 0;
	char bookName[MAX_BOOK_NAME] = {0};
	
	s_BooksInfo addBookInfoInDatabase = {0};
	
	FILE *fp = NULL;
	
	fp=fopen(FILE_NAME,"rb");
	if(fp==NULL)
	{
		printf("Could NOT open File");
		exit(1);
	}
	
	headMessage("SEARCH BOOKS");
	if(fseek(fp,FILE_HEADER_SIZE,SEEK_SET)!=0)
	{
		fclose(fp);
		printf("\n Facing Issue While Reading");
		exit(1);
	}
	printf("\n\n\n \t Enter Book Name to Search: \t\t ");
	fflush(stdin);
	fgets(bookName,MAX_BOOK_NAME,stdin);
	
	while (fread(&addBookInfoInDatabase,sizeof(addBookInfoInDatabase), 1,fp))
	{
		if(!strcmp(addBookInfoInDatabase.bookName,bookName))
		{
			found = 1;
			break; 
		}
	}
	if(found)
	{
		printf("\n\t\t\tBook ID : %u",addBookInfoInDatabase.books_id);
		printf("\n\t\t\tBook Name : %s",addBookInfoInDatabase.bookName);
		printf("\t\t\tAuthor's Name : %s",addBookInfoInDatabase.authorName);
	}
	else
	{
		printf("\n No Records Found");
	}
	
	fclose(fp);
	printf("\n\n\n\t Enter any key to go to Main Menu");
	getchar();
	
}

void viewBook()
{
	int found =0;
	char bookName[MAX_BOOK_NAME]={0};
	s_BooksInfo addBookInfoInDatabase = {0};
	
	FILE *fp = NULL;
	
	fp=fopen(FILE_NAME,"rb");
	if(fp==NULL)
	{
		printf("Could NOT open File");
		exit(1);
	}
	headMessage("VIEW BOOKS");
	unsigned int countBook = 1;
	if(fseek(fp,FILE_HEADER_SIZE,SEEK_SET)!=0)
	{
		fclose(fp);
		printf("\n Facing Issue While Reading");
		exit(1);
	}
	while (fread(&addBookInfoInDatabase,sizeof(addBookInfoInDatabase), 1,fp))
	{
		printf("\n\n\t\t Book Count = %d", countBook);
		printf("\n\t\t\tBook ID : %u",addBookInfoInDatabase.books_id);
		printf("\n\t\t\tBook Name : %s",addBookInfoInDatabase.bookName);
		printf("\t\t\tAuthor's Name : %s",addBookInfoInDatabase.authorName);
		
		found = 1;
		++countBook;
	}
	fclose(fp);
	if(!found)
	{
		printf("\n\t\t No RECORDS");
	}
	printf("\n\n\n\t Enter any key to go to Main Menu");
	fflush(stdin);
	getchar();
}

void deleteBook()
{
	int found = 0;
	int bookDelete = 0;
	sFileHeader fileHeaderInfo={0};
	
	char bookName[MAX_BOOK_NAME]={0};
	s_BooksInfo addBookInfoInDatabase = {0};
	FILE *fp = NULL;
	FILE *tmpFp = NULL;
	int status = 0;
	headMessage("DELETE BOOKS");
	fp=fopen(FILE_NAME,"rb");
	if(fp==NULL)
	{
		printf("Could NOT open File");
		exit(1);
	}
	tmpFp = fopen("tmp.bin","wb");
	if(tmpFp==NULL)
	{
		fclose(fp);
		printf("File is not Opened\n");
		exit(1);
	}
	
	fread(&fileHeaderInfo,FILE_HEADER_SIZE,1,fp);
	fwrite(&fileHeaderInfo,FILE_HEADER_SIZE,1,tmpFp);
	printf("\n Enter Book ID to Delete:");
	scanf("%d", &bookDelete);
	
	while (fread(&addBookInfoInDatabase.books_id,sizeof(addBookInfoInDatabase),1,fp))
	{
		if (addBookInfoInDatabase.books_id!= bookDelete)
		{
			fwrite(&addBookInfoInDatabase,sizeof(addBookInfoInDatabase),1,tmpFp);
		}
		else
		{
			found = 1;
		}
	}
	(found)?printf("\n\t\t Record Deleted Successfully.."):printf("\n\t\t Record Not Found");
	fclose(fp);
	fclose(tmpFp);
	remove(FILE_NAME);
	rename("tmp.bin",FILE_NAME);
}

void menu()
{
	int choice = 0;
	do
	{
		headMessage("MAIN MENU");
		printf("\n\n\n\t\t 1. Add Books");
		printf("\n\n\n\t\t 2. Search Books");
		printf("\n\n\n\t\t 3. View Books");
		printf("\n\n\n\t\t 4. Delete Books");
		printf("\n\n\n\t\t 0. Exit");
		
		printf("\n\n\t\t Enter Choice:");
		scanf("%d",&choice);
		
		switch(choice)
		{
			case 1:
				addBook();
				break;
			case 2:
				searchBook();
				break;
			case 3:
				viewBook();
				break;
			case 4:
				deleteBook();
				break;
			case 0:
				printf("\n\n\n Thank You!! \n\n\n");
				exit(1);
				break;
			default:
				printf("\n\n\n\n\n INVALID INPUT!! Please Try Again..");
		}
		
	}while(choice!=0);
}
void login()
{
	unsigned char userName[MAX_SIZE_USERNAME]={0};
	unsigned char password[MAX_SIZE_PASSWORD]={0};
	
	sFileHeader fileHeaderInfo={0};
	FILE *fp = NULL;
	headMessage("Login");
	fp = fopen(FILE_NAME,"rb");
	if (fp == NULL)
	{
		printf("Could Not Open File");
		exit(1);
	}
	fread(&fileHeaderInfo,FILE_HEADER_SIZE,1,fp);
	fclose(fp);
	int L = 0;
	do
	{
		printf("\n Enter Username:");
		fgets(userName,MAX_SIZE_USERNAME,stdin);
		printf("\n Enter Password:");
		fgets(password,MAX_SIZE_PASSWORD,stdin);
		
		if ( (!strcmp(userName,fileHeaderInfo.username) ) && (!strcmp(password,fileHeaderInfo.password)) )
		{
			menu();
		}
		else
		{
			printf("Login Failed. Enter UserName and Password Again\n\n");
			L++;
		}
	}while(L<=2);
	if (L>2)
	{
		headMessage("LOGIN FAILED");
		printf("UNKNOWN USER!!");
		getch();
		system("cls");
	}
}

int main()
{
	start();
	welcomeMessage();
	login();
	return 0;
}
