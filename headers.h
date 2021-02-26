//headers
#include<stdio.h>
#include<mariadb/mysql.h>
#include<stdlib.h>
#include<string.h>


//function to flush input buffers
void flush(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF)  {}
}

//newline function
void NewLine(void){
	printf("\n");
}

//input prompt
void keyPrompt(void){
	printf("\n\nPress any key to go back to menu.\n");
	flush();
	getchar();
}

/*Beware! negative  numbers may cause problems*/

void reverse(char s[]){
     int i, j;
     char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}  

void itoa(int n, char s[]){
     int i, sign;

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}
