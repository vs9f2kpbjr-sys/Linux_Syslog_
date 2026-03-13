#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* syslog kayıtlarını saklamak için kullanılan bağlı liste düğüm yapısı */

typedef struct LogNode {
    char logMessage [512]; //syslog satırı
    struct LogNode* next; //bir sonraki düğüm
    
} LogNode ;
/* yeni düğüm oluşturur */

LogNode* createNode (char*message) {
    
    LogNode* newNode = (LogNode*) malloc (sizeof(LogNode));
    if(newNode==NULL) {
        printf("Bellek ayrilamadi !\n");
        exit (1);
    }
    strcpy(newNode-> logMessage,message);
    newNode ->next = NULL;
    return newNode;
}
/* listenin sonunda yeni Log ekler */

void appendLog (LogNode** head, char* message) {
    LogNode* newNode= createNode(message);
    
    if (*head== NULL){
        *head = newNode;
        return;
    }
    LogNode* temp = *head;
    
    while (temp->next !=NULL){
        temp= temp->next;
    }
    temp->next =newNode;
}
/* tüm Logoları yazdırır*/

void printLogs (LogNode* head) {
    
    LogNode*temp =head ;
    printf ("\n--- SYSLOG KAYITLARI ---\n\n");
    
    while (temp !=NULL) {
        printf("%s", temp->logMessage);
        temp= temp->next;
    }
}
/* belirli kelimeye göre log arama*/

void searchLogs (LogNode* head,char* keyword) {
    
    LogNode* temp= head;
    int found =0;
    printf("\n '%s' kelimesini iceren loglar: \n\n",keyword);
    
    while (temp !=NULL) {
        if (strstr(temp->logMessage, keyword) != NULL) {
            printf ("%s", temp->logMessage);
            found =1;
        }
        temp = temp->next;
    }
    if (!found)
        printf("Kayit bulunamadi. \n");
}
/* bellegi temizler */

void freeList (LogNode* head) {
    
    LogNode* temp;
    while (head != NULL){
        temp = head;
        head =head->next;
        free (temp);
    }
}

/* syslog doyasini okuyup linked list içine ekler */
void readSyslogFile(LogNode** head) {
    
    FILE* file;
    char line [512] ;
    file = fopen ("/var/log/syslog", "r");
    
    if (file == NULL) {
        printf ("Syslog dosyasi acilamadi! \n");
        return ;
    }
    while (fgets (line, sizeof (line), file)) {
        appendLog (head, line);
    }
    fclose (file);
}
int main (void) {
    
    LogNode* logList = NULL;
    
    printf("Syslog okunuyor... \n");
    
    readSyslogFile (&logList);
    
    printLogs(logList);
    
    searchLogs (logList, "error");
    
    freeList(logList);
    
    return 0;
}
    

