#include"headers.h"

void DisplayHeader(){
	printf("\n+");
	for(int i = 0;i < 122; i++){
		printf("=");
	}
	printf("+");
	printf("\n|| %5s |%-50s%-51s|| %5s ||\n","Book_ID","|                     Title ","||                      Author","Pages");
	printf("+");
	for(int i = 0;i < 122; i++){
		printf("=");
	}
	printf("+\n");
}

void error(MYSQL* con) {
	printf("Error : %s\n",mysql_error(con));
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
		return;
	}

   
    MYSQL_ROW row;
   	DisplayHeader();	
    while ((row = mysql_fetch_row(result))){
        for(int i = 0; i < num_fields; i++){
        	if(i == 0){
        		printf("[ %8s ]", row[i] ? row[i] : "NULL");
        		continue;
        	}
        	if(i == 3){
           		printf("[ %6s ]", row[i] ? row[i] : "NULL");
        		continue;
        	}
            printf("[ %47s ]", row[i] ? row[i] : "NULL");
            
        }
            printf("\n");
    }

    mysql_free_result(result);
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
	DisplayHeader();
	for(int i = 0; i < numOfFields; i++){
       	if(i == 0){
       		printf("[ %8s ]", row[i] ? row[i] : "NULL");
       		continue;
       	}
       	if(i == 3){
    		printf("[ %6s ]", row[i] ? row[i] : "NULL");
   	  		continue;
    	}
	    printf("[ %47s ]", row[i] ? row[i] : "NULL");         
	}

	mysql_free_result(result);
	
	return searchID;
}

void deleteRecord(MYSQL* con,int bookID){
	int id = displayRecordByID(con,bookID);
	if(id == -1)
		return;
	
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
}

void displayMenu(){
	printf("\n+");
	for(int i = 0;i < 39; i++){
		printf("=");
	}
	printf("+");
	printf("\n[\t1: Show all books\t\t]\n[\t2: Insert Book\t\t\t]\n[\t3: Delete Book by Book_ID\t]\n[\t4: Search Book By Book_ID\t]\n[\t5: Update Book By Book_ID\t]\n[\t9: Shutdown Server\t\t]\n[\t0: Exit\t\t\t\t]\n");
	printf("+");
	for(int i = 0;i < 39; i++){
		printf("=");
	}
	printf("+\n");	
}

void updateRecord(MYSQL* con, int bookID){
	if(displayRecordByID(con,bookID) == -1)
		return;
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
}

void shutdownServer(MYSQL* con){
	if(mysql_shutdown(con,0)){
		error(con);
		return;
	}
	printf("\nMySQL server is shutdown.\n");
}

int connectToMysql(MYSQL* con){

	char username[30] = {};
	char password[30] = {};

	printf("\n\nUsername: \t");
	scanf("%[^\n]s*c",username);
	printf("\n\nPassword: \t");
	flush();
	scanf("%[^\n]s*c",password);
		
	if(mysql_real_connect(con,"localhost",username,password,"db1",0,NULL,0) == NULL){
		NewLine();
		error(con);
		return 1;
	}
	return 0;
}

int main(){

	MYSQL* con = mysql_init(NULL);
	if(con == NULL){
		error(con);
		exit(1);
	}
	char response = '\0';
	while(1){
		system("clear");
		
		if(connectToMysql(con) == 0)
			break;
			
		printf("\n\nTry again? Enter 'Y' or 'y';\t");
		flush();
		response = getchar();
		if(response != 'y' &&  response != 'Y')
			exit(1);
		flush();
	}
	/*	if(mysql_query(con,"use db1")){
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
		printf("\nYour choice(1,2,3,4,5,9, or 0): ");
		scanf("%d",&choice);
		int bookID = 0;
		char response = '\0';
		switch(choice){
			case 0: system("clear");
					exit(0);
			case 1: display(con);
					keyPrompt();
					break;
			case 2: do{	
						system("clear");
						insert(con);
						display(con);
						printf("\n\nIf you want to add another record, enter 'Y' or 'y',\n(any other key will take you back to menu): ");
						flush();
						response = getchar();						
					}while(response == 'Y' || response == 'y');
					break;
			case 3: do{	
						system("clear");
						getBookID(&bookID);
						deleteRecord(con,bookID);
						printf("\n\nIf you want to remove another record, enter 'Y' or 'y',\n(any other key will take you back to menu): ");
						flush();
						response = getchar();						
					}while(response == 'Y' || response == 'y');
					break;
			case 4: do{	
						system("clear");
						getBookID(&bookID);
						displayRecordByID(con,bookID);
						printf("\n\nIf you want to look up another record, enter 'Y' or 'y',\n(any other key will take you back to menu): ");
						flush();
						response = getchar();						
					}while(response == 'Y' || response == 'y');
					break;
			case 5: do{	
						system("clear");
						getBookID(&bookID);
						updateRecord(con,bookID);
						printf("\n\nIf you want to edit another record, enter 'Y' or 'y',\n(any other key will take you back to menu): ");
						flush();
						response = getchar();						
					}while(response == 'Y' || response == 'y');
					break;
			case 9: shutdownServer(con);
					flush();
					getchar();
					system("clear");
					exit(0);					
					break;
			default: system("clear");
					 printf("Invalid option!\n\nTry again.\n");
					 keyPrompt();
		}
	}
	
	return 0;
}
