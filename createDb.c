#include"headers.h"

void error(MYSQL* con) {
	printf("Error : %s\n",mysql_error(con));
}

void runQuery(MYSQL* con,const char* query){
	if(mysql_query(con,query)){
		error(con);
		return;
	}
}

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



void display(MYSQL* con){
	system("clear");
	char* query = {"SELECT * FROM library"};
   	runQuery(con,query);
   
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

int searchByID(MYSQL* con, int searchID){
	
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
	int id = searchByID(con,bookID);
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
	
	runQuery(con,query);
	
	printf("\n\nAbove record is deleted!\n");
}

void insertRecord(MYSQL* con){
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
	
	runQuery(con,query);
	
	printf("\n\nRecord added successfully!\n");
}

void displayMenu(){
	printf("\n+");
	for(int i = 0;i < 39; i++){
		printf("=");
	}
	printf("+");
	printf("\n[\t1: Show books\t\t\t]\n[\t2: Insert Book\t\t\t]\n[\t3: Delete Book by Book_ID\t]\n[\t4: Edit Book Info By Book_ID\t]\n[\t9: Shutdown Server\t\t]\n[\t0: Exit\t\t\t\t]\n");
	printf("+");
	for(int i = 0;i < 39; i++){
		printf("=");
	}
	printf("+\n");	
}

void shutdownServer(MYSQL* con){
	if(mysql_shutdown(con,0)){
		error(con);
		return;
	}
	printf("\nMySQL server is shutdown.\n");
}

void searchByTitle(MYSQL* con){
	printf("\n\nEnter search string:\t");
	char searchTitle[52] = {};
	flush();
	scanf("%[^\n]s*c",searchTitle);
	char query[250] = {"SELECT * FROM library WHERE title LIKE '%"};
	strcat(query,searchTitle);
	strcat(query,"%'");
	runQuery(con,query);
	MYSQL_RES* result = mysql_store_result(con);
	if(result == NULL){
		error(con);
		return;
	}
	
	int numOfFields = mysql_num_fields(result);
	int numOfRows = mysql_num_rows(result);
	if(numOfRows == 0){
		printf("\nNo match found for %s!\n",searchTitle);
		return;
	}
	else
		printf("\n%d match(s) found for \"%s\"!\n\n",numOfRows,searchTitle);
	
	MYSQL_ROW row;
	   	DisplayHeader();	
	    while ((row = mysql_fetch_row(result))){
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
	        printf("\n");
	    }
	
	    mysql_free_result(result);
}

void searchByAuthor(MYSQL* con){
	printf("\n\nEnter search string:\t");
	char searchAuthor[52] = {};
	flush();
	scanf("%[^\n]s*c",searchAuthor);
	char query[250] = {"SELECT * FROM library WHERE author LIKE '%"};
	strcat(query,searchAuthor);
	strcat(query,"%'");
	runQuery(con,query);
	MYSQL_RES* result = mysql_store_result(con);
	if(result == NULL){
		error(con);
		return;
	}
	
	int numOfFields = mysql_num_fields(result);
	int numOfRows = mysql_num_rows(result);
	if(numOfRows == 0){
		printf("\nNo match found for %s!\n",searchAuthor);
		return;
	}
	else
		printf("\n%d match(s) found for \"%s\"!\n\n",numOfRows,searchAuthor);
	
	MYSQL_ROW row;
	DisplayHeader();	
	while ((row = mysql_fetch_row(result))){
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
        printf("\n");
    }	
    mysql_free_result(result);
}

int connectToMysql(MYSQL* con){

	char username[30] = {};
	char password[30] = {};

	printf("\n\nUsername: \t");
	scanf("%[^\n]s*c",username);
	printf("\n\nPassword: \t");
	flush();
	scanf("%[^\n]s*c",password);
		
	if(mysql_real_connect(con,"localhost",username,password,NULL,0,NULL,CLIENT_MULTI_STATEMENTS) == NULL){
		NewLine();
		error(con);
		return 1;
	}
	return 0;
}

void search(MYSQL* con){

	int choice = 0;
	int bookID = 0;
	char response = '\0';
	flush();
	
	while(1){
		system("clear");
		printf("\n+");
		for(int i = 0;i < 39; i++){
			printf("=");
		}
		printf("+");
		printf("\n[\t1: Show all books\t\t]\n[\t2: Search Book By Book_ID\t]\n[\t3: Search Book By Title\t\t]\n[\t4: Search Book By Author\t]\n[\t0: Exit\t\t\t\t]\n");
		printf("+");
		for(int i = 0;i < 39; i++){
			printf("=");
		}
		printf("+\n");
	
		printf("\nYour choice(1,2,3,4, or 0): ");
		scanf("%d",&choice);
		
	switch(choice){
		case 0: system("clear");
				return;
		case 1: display(con);
				keyPrompt();
				break;
		case 2:do{	
					system("clear");
					getBookID(&bookID);
					searchByID(con,bookID);
					printf("\n\nLook up another record? Enter 'Y' or 'y',\n(any other key will take you back to menu): ");
					flush();
					response = getchar();						
				}while(response == 'Y' || response == 'y');
				break;
		case 3:do{	
					system("clear");
					searchByTitle(con);
					printf("\n\nLook up another record? Enter 'Y' or 'y',\n(any other key will take you back to menu): ");
					flush();
					response = getchar();						
				}while(response == 'Y' || response == 'y');
				break;
		case 4:do{	
					system("clear");
					searchByAuthor(con);
					printf("\n\nLook up another record? Enter 'Y' or 'y',\n(any other key will take you back to menu): ");
					flush();
					response = getchar();						
				}while(response == 'Y' || response == 'y');
				break;
		default:system("clear");
				printf("Invalid option!\n\nTry again.\n");
				keyPrompt();
	}
	}
}

void editRecord(MYSQL* con, int bookID){
	char bookIDstring[10] = {};	
	itoa(bookID,bookIDstring);
	while(1){
			system("clear");
			if(searchByID(con,bookID) == -1)
				return;
			printf("\n\n\n\tWhat do you want to update?\n");
			printf("\t\t* (t)itle,\n");
			printf("\t\t* (a)uthor,\n");
			printf("\t\t* (p)ages,\n");
			printf("\t\t* all of the above (f)ields,\n");
			printf("\t\t* (r)eturn\n\t(f,t,a,p or r): ");
			char response = '\0';
			flush();
			response = getchar();
			
			char query[250] = {"UPDATE library SET "};
			char newTitle[50] = {};
			char newAuthor[50] = {};
			int newPages = 0;
			char stringNewPages[10];
			switch(response){
				case 't': strcat(query,"title='");
						  printf("\n\nNew title: ");
						  flush();
						  scanf("%[^\n]s*c",newTitle);
						  strcat(query,newTitle);
						  strcat(query,"'");
						  strcat(query," WHERE book_id=");
						  strcat(query,bookIDstring);
						  runQuery(con,query);
						  break;
				case 'a': strcat(query,"author='");
						  printf("\n\nNew Author: ");
						  flush();
						  scanf("%[^\n]s*c",newAuthor);
						  strcat(query,newAuthor);
						  strcat(query,"'");
						  strcat(query," WHERE book_id=");
						  strcat(query,bookIDstring);
						  runQuery(con,query);
						  break;
				case 'p': strcat(query,"pages=");
						  printf("\n\nNew No. of Pages: ");
						  flush();
						  scanf("%d",&newPages);
						  itoa(newPages,stringNewPages);
						  strcat(query,stringNewPages);
						  strcat(query," WHERE book_id=");
						  strcat(query,bookIDstring);
						  runQuery(con,query);		
						  break;	
				case 'f':strcat(query,"title='");
				 
						printf("\n\nNew title: ");
						flush();
						scanf("%[^\n]s*c",newTitle);
						
						printf("New author: ");
						flush();
						scanf("%[^\n]s*c",newAuthor);
					
						printf("New number of pages: ");
						scanf("%d",&newPages);
						itoa(newPages,stringNewPages);
							
						strcat(query,newTitle);
						strcat(query,"',author='");
						strcat(query,newAuthor);
						strcat(query,"',pages=");
						strcat(query,stringNewPages);
						strcat(query," WHERE book_id=");
						strcat(query,bookIDstring);
					
						runQuery(con,query);
						break;
				case 'r': system("clear");
						  return;
				default: printf("Invalid option!\n\nTry again.\n");
						 keyPrompt();	  				 
			}
	}

	printf("\n\nUpdate successful!\n");
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
	if(mysql_query(con,"USE db1")){
		if(mysql_query(con,"CREATE DATABASE db1")){
			error(con);
			mysql_close(con);
			exit(1);
		}

		if(mysql_query(con,"USE db1")){
			error(con);
			mysql_close(con);
			exit(1);
		}	

		if(mysql_query(con,"CREATE TABLE library(book_id INT PRIMARY KEY AUTO_INCREMENT,title VARCHAR(50),author VARCHAR(50),pages INT)")){
			error(con);
			mysql_close(con);
			exit(1);
		}
	}
	
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
			case 1: search(con);
					break;
			case 2: do{	
						system("clear");
						insertRecord(con);
						display(con);
						printf("\n\nAdd another record? Enter 'Y' or 'y',\n(any other key will take you back to menu): ");
						flush();
						response = getchar();						
					}while(response == 'Y' || response == 'y');
					break;
			case 3: do{	
						system("clear");
						getBookID(&bookID);
						deleteRecord(con,bookID);
						printf("\n\nRemove another record? Enter 'Y' or 'y',\n(any other key will take you back to menu): ");
						flush();
						response = getchar();						
					}while(response == 'Y' || response == 'y');
					break;
			case 4: do{	
						system("clear");
						getBookID(&bookID);
						editRecord(con,bookID);
						printf("\n\nEdit another record? Enter 'Y' or 'y',\n(any other key will take you back to menu): ");
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
