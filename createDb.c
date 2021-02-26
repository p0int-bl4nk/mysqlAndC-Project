#include"headers.h"

void error(MYSQL* con) {
	printf("Error : %s\n",mysql_error(con));
}

void display(MYSQL* con){
	system("clear");
   	if (mysql_query(con, "SELECT * FROM library")){
         error(con);
     }
   
     MYSQL_RES *result = mysql_store_result(con);
   
     if (result == NULL){
         error(con);
     }
   
     int num_fields = mysql_num_fields(result);
   
     MYSQL_ROW row;
   
     while ((row = mysql_fetch_row(result))){
         for(int i = 0; i < num_fields; i++){
             printf("%s\t ", row[i] ? row[i] : "NULL");
         }
   
         printf("\n");
     }
     printf("\n\nPress any key to go back to menu.\n");
     flush();
     getchar();
}
void deleteTuple(MYSQL* con){
	system("clear");
	printf("Enter the Book_id: ");
	int id = 0;
	scanf("%d",&id);
	char stringId[11];
	itoa(id,stringId);
	char query[500]={"DELETE FROM library WHERE book_id="};
	strcat(query,stringId);
	if(mysql_query(con,query)){
		error(con);
	}
}

void insert(MYSQL* con){
	system("clear");
	printf("Book Name: ");
	char title[50];
	flush();
	scanf("%[^\n]s*c",title);
	printf("Author: ");
	char author[50];
	flush();
	scanf("%[^\n]s*c",author);
	printf("Number of pages: ");
	int pages = 0;
	scanf("%d",&pages);
	strcat(title,"','");
	strcat(title,author);
	strcat(title,"','");
	char stringPages[10];
	itoa(pages,stringPages);
	strcat(title,stringPages);
	strcat(title,"')");
	char query[500] = {"INSERT INTO library(title,author,pages) VALUES('"};
	strcat(query,title);
	if(mysql_real_query(con,query,strlen(query))){
		error(con);
	}
}

void displayMenu(){
	printf("\n1: Show all books\n2: Insert record\n3: Delete record by Book_ID\n0: Exit\n");
}
int main(){

	MYSQL* con = mysql_init(NULL);
	if(con == NULL){
		printf("DB Error!\n");
		exit(1);
	}
	if(mysql_real_connect(con,"localhost","root","root","db1",0,NULL,0) == NULL){
		printf("Error  : %s\n",mysql_error(con));
		mysql_close(con);
	}
	/*if(mysql_query(con,"use db1")){
		if(mysql_query(con,"create database db1")){
			error(con);
			mysql_close(con);
			exit(1);
		}
		if(mysql_query(con,"create table library(book_id INT PRIMARY KEY AUTO_INCREMENT,title VARCHAR(50),author VARCHAR(50),pages INT)")){
				error(con);
				mysql_close(con);
				exit(1);
		}
	}*/
	
	int choice = 0;
	while(1){
		system("clear");
		displayMenu();
		printf("\nYour choice: (1,2,3, or 0) ");
		scanf(" %d",&choice);
		switch(choice){
			case 0: exit(0);
			case 1: display(con);
					break;
			case 2: insert(con);
					break;
			case 3: deleteTuple(con);
					break;
			default: printf("Invalid option!\n\nTry again.\n");
		}
	}
	
	return 0;
}
