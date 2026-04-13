
#include <stdio.h>
#include <stdlib.h>   
#include <string.h>   
struct Node {
    char website[100];    
    char username[100];   
    char password[100];   
    struct Node *next;    
};


typedef struct Node Node;
void printLine() {
    printf("--------------------------------------------------\n");
}

void showMasked(char password[]) {
    int i;
    for (i = 0; password[i] != '\0'; i++) {
        printf("*");
    }
    printf("\n");
}
Node* createNode(char website[], char username[], char password[]) {
    Node *newNode = (Node*) malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("  Error: Memory allocation failed!\n");
        exit(1);
    }
    strcpy(newNode->website,  website);
    strcpy(newNode->username, username);
    strcpy(newNode->password, password);
    newNode->next = NULL;

    return newNode;
}

void addEntry(Node **head) {

    char website[100], username[100], password[100];

    printf("\n  --- ADD NEW ENTRY ---\n");
    printf("  Website  : ");
    scanf(" %[^\n]", website);  

    printf("  Username : ");
    scanf(" %[^\n]", username);

    printf("  Password : ");
    scanf(" %[^\n]", password);

    Node *newNode = createNode(website, username, password);
    if (*head == NULL) {
        *head = newNode;
    }
    
    else {
        Node *temp = *head;  
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    printLine();
    printf("  Entry added successfully!\n");
    printLine();
}
void displayAll(Node *head) {

    printf("\n  --- ALL SAVED ENTRIES ---\n");
    printLine();
    if (head == NULL) {
        printf("  No entries found!\n");
        printLine();
        return;
    }

    Node *temp = head;  
    int count = 1;
    while (temp != NULL) {

        printf("  Entry #%d\n", count);
        printf("  Website  : %s\n", temp->website);
        printf("  Username : %s\n", temp->username);
        printf("  Password : ");
        showMasked(temp->password);
        printLine();

        temp = temp->next;  
        count++;
    }
}

void searchEntry(Node *head) {

    char searchName[100];

    printf("\n  --- SEARCH ENTRY ---\n");
    printf("  Enter website to search: ");
    scanf(" %[^\n]", searchName);

    if (head == NULL) {
        printf("  List is empty!\n");
        return;
    }

    Node *temp = head;
    int found = 0; 

    while (temp != NULL) {

        if (strcmp(temp->website, searchName) == 0) {
            printf("\n  FOUND!\n");
            printLine();
            printf("  Website  : %s\n", temp->website);
            printf("  Username : %s\n", temp->username);
            printf("  Password : ");
            showMasked(temp->password);
            printLine();
            found = 1;
        }

        temp = temp->next;
    }

    if (found == 0) {
        printf("  Not found: %s\n", searchName);
    }
}
void deleteEntry(Node **head) {

    char targetName[100];

    printf("\n  --- DELETE ENTRY ---\n");
    printf("  Enter website to delete: ");
    scanf(" %[^\n]", targetName);

    if (*head == NULL) {
        printf("  List is empty! Nothing to delete.\n");
        return;
    }

    Node *temp = *head;   
    Node *prev = NULL;    

    
    while (temp != NULL) {

        if (strcmp(temp->website, targetName) == 0) {
            break;  
        }

        prev = temp;   
        temp = temp->next;  
    }
    if (temp == NULL) {
        printf("  Not found: %s\n", targetName);
        return;
    }
    if (prev == NULL) {
        *head = temp->next;  
    }
    
    else {
        prev->next = temp->next;  
    }
    free(temp);

    printLine();
    printf("  Deleted '%s' successfully!\n", targetName);
    printLine();
}

void saveToFile(Node *head) {

    FILE *file = fopen("passwords.txt", "w");

    if (file == NULL) {
        printf("  Could not open file!\n");
        return;
    }

    Node *temp = head;

    while (temp != NULL) {
        
        fprintf(file, "%s\n", temp->website);
        fprintf(file, "%s\n", temp->username);
        fprintf(file, "%s\n", temp->password);
        temp = temp->next;
    }

    fclose(file);
    printf("  Data saved to passwords.txt\n");
}

void loadFromFile(Node **head) {

    FILE *file = fopen("passwords.txt", "r");

    
    if (file == NULL) {
        return;
    }

    char website[100], username[100], password[100];

    
    while (fgets(website,  100, file) != NULL &&
           fgets(username, 100, file) != NULL &&
           fgets(password, 100, file) != NULL) {

        website[strcspn(website,   "\n")] = '\0';
        username[strcspn(username, "\n")] = '\0';
        password[strcspn(password, "\n")] = '\0';
        Node *newNode = createNode(website, username, password);

        if (*head == NULL) {
            *head = newNode;
        } else {
            Node *temp = *head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    fclose(file);
    printf("  Previous entries loaded!\n");
}

void freeAllNodes(Node **head) {

    Node *current = *head;
    Node *nextNode;

    while (current != NULL) {
        nextNode = current->next; 
        free(current);             
        current = nextNode;       
    }

    *head = NULL;  
}
int main() {

    Node *head = NULL;  
    loadFromFile(&head);

    int choice;
    while (1) {

        printf("\n");
        printLine();
        printf("       PASSWORD MANAGER\n");
        printLine();
        printf("  1. Add Entry\n");
        printf("  2. Display All\n");
        printf("  3. Search Entry\n");
         printf("  4. Delete Entry\n");
        printf("  5. Exit\n");
        printLine();
        printf("  Your choice: ");
        scanf("%d", &choice);

        if (scanf("%d", &choice) != 1) {
            printf("  Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // clear invalid input from buffer
            continue;
        }

        if (choice == 1) {
            addEntry(&head);
        }
        else if (choice == 2) {
            displayAll(head);
        }
        else if (choice == 3) {
            searchEntry(head);
        }
        else if (choice == 4) {
            deleteEntry(&head);
        }
        else if (choice == 5) {
            saveToFile(head);
            freeAllNodes(&head);
            printf("\n  Goodbye!\n\n");
            break;  
        }
        else {
            printf("  Invalid choice! Please enter 1 to 5.\n");
        }
    }

    return 0;
}