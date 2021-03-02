#include"headers.h"

void error(MYSQL* con) {
	printf("Error : %s\n",mysql_error(con));
	keyPrompt();
}

void display(MYSQL* con){
	system("clear");
	
   	if (mysql_query(con, "SELECT * FROM library")){
        error(con);
        return;
    }
   
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL){
        error(con);
        return;
    }
   
    int num_fields = mysql_num_fields(result);
	int numOfRows = mysql_num_rows(result);
	if(numOfRows == 0){
		printf("\n\nNo records found!\n");
		keyPrompt();	
		return;
	}

   
    MYSQL_ROW row;
   
    while ((row = mysql_fetch_row(result))){
        for(int i = 0; i < num_fields; i++){
            printf("[ %s ]\t ", row[i] ? row[i] : "NULL");
        }
            printf("\n");
    }

    mysql_free_result(result);
    keyPrompt();
}

int displayRecordByID(MYSQL* con, int searchID){
	char searchIDstring[10];
	itoa(searchID,searchIDstring);
	char query[250] = {"SELECT * FROM library WHERE book_id="};
	strcat(query,searchIDstring);
	if(mysql_query(con,query)){
		error(con);
		return -1;
	}
	MYSQL_RES* result = mysql_store_result(con);
	if(result == NULL){
		error(con);
		return -1;
	}

	int numOfFields = mysql_num_fields(result);
	int numOfRows = mysql_num_rows(result);
	if(numOfRows == 0){
		printf("\nBook_ID: %d not found!\n",searchID);
		return -1;
	}
	MYSQL_ROW row = mysql_fetch_row(result);

	NewLine();
	for(int i = 0;i < numOfFields; i++){
		printf("[ %s ]\t",row[i]?row[i]:"NULL");
	}

	mysql_free_result(result);
	
	return searchID;
}

void deleteRecord(MYSQL* con,int bookID){
	int id = displayRecordByID(con,bookID);
	if(id == -1){
		return;
	}
	
	char stringId[11];
	itoa(id,stringId);
	
	char query[500]={"DELETE FROM library WHERE book_id="};
	strcat(query,stringId);
	
	printf("\n\nEnter 'y' to confirm deletion: ");
	char c = '\0';
	flush();
	if((c = getchar()) != 'y'){
		printf("\nDeletion aborted!\n");
		return;
	}
	
	if(mysql_query(con,query)){
		error(con);
		return;
	}
	
	printf("\n\nAbove record is deleted!\n");
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
	flush();
	scanf("%d",&pages);
	char stringPages[10];
	itoa(pages,stringPages);
	
	char query[500] = {"INSERT INTO library(title,author,pages) VALUES('"};
	strcat(query,title);
	strcat(query,"','");
	strcat(query,author);
	strcat(query,"','");		
	strcat(query,stringPages);
	strcat(query,"')");
	
	if(mysql_real_query(con,query,strlen(query))){
		error(con);
		return;
	}
	
	printf("\n\nRecord added successfully!\n");
	keyPrompt();
}

void displayMenu(){
	printf("\n1: Show all books\n2: Insert record\n3: Delete record by Book_ID\n4: Search Record By Book_ID\n5: Update Record By Book_ID\n0: Exit\n");
}

void updateRecord(MYSQL* con, int bookID){
	displayRecordByID(con,bookID);

	char bookIDstring[10] = {};	
	itoa(bookID,bookIDstring);

	printf("\n\nNew title: ");
	char newTitle[50] = {};
	flush();
	scanf("%[^\n]s*c",newTitle);
	
	printf("New author: ");
	char newAuthor[50];
	flush();
	scanf("%[^\n]s*c",newAuthor);
	
	printf("New number of pages: ");
	int newPages = 0;
	scanf("%d",&newPages);
	char stringNewPages[10];
	itoa(newPages,stringNewPages);
	
	char query[250] = {"UPDATE library SET title='"};
	strcat(query,newTitle);
	strcat(query,"',author='");
	strcat(query,newAuthor);
	strcat(query,"',pages=");
	strcat(query,stringNewPages);
	strcat(query," WHERE book_id=");
	strcat(query,bookIDstring);

	if(mysql_query(con,query)){
		error(con);
		return;
	}

	printf("\n\nUpdate successful!\n");
	keyPrompt();
}

int main(){

	MYSQL* con = mysql_init(NULL);
	if(con == NULL){
		error(con);
		exit(1);
	}
	if(mysql_real_connect(con,"localhost","root","root","db1",0,NULL,0) == NULL){
		error(con);
		mysql_close(con);
		exit(1);
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
		printf("\nYour choice: (1,2,3,4,5, or 0) ");
//		flush();
		scanf("%d",&choice);
		int bookID = 0;
		switch(choice){
			case 0: system("clear");
					exit(0);
			case 1: display(con);
					break;
			case 2: insert(con);
					break;
			case 3: system("clear");
					getBookID(&bookID);
					deleteRecord(con,bookID);
					keyPrompt();
					break;
			case 4: system("clear");
					getBookID(&bookID);
					displayRecordByID(con,bookID);
					keyPrompt();
					break;
			case 5: system("clear");
					getBookID(&bookID);
					updateRecord(con,bookID);
					break;
			default: system("clear");
					 printf("Invalid option!\n\nTry again.\n");
					 keyPrompt();
		}
	}
	
	return 0;
}
