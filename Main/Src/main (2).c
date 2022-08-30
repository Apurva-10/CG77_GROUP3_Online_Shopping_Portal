/**************************************************************************

FILENAME : main.c

DESCRIPTION : It is a file having main function which includes
	      the main menu of online shopping portal

***************************************************************************/

#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "func.h"
#include "function.c"


/***************************************************************************
 
FUNCTION NAME : main

DESCRIPTION : This function includes the main menu of online shopping portal 

***************************************************************************/

int main()
{
	pthread_t thread_id;
	cust=NULL;
        stk=NULL;
	int ch=0;
	customer_to_list(); //calling function customer_to_list to get customer file data into list
	stock_to_list(); //calling function stock_to_list to get stock file data to list
	while(ch != 6){
        printf("ABC online - an online shopping portal for groceries\n**********************************************************\n");
        printf("1. Customer Registration\n2. Shop Here\n3. View Stock details\n4. File maintainance\n5. Report\n6. Exit\n");
        printf("Enter a valid choice\n");
        scanf("%d",&ch);
        switch(ch) //To display the main menu to the user
        {
            case 1 :
                customer_registration();
                break;
            case 2 :
                shop_here();
                break;
            case 3 :
                view_stock();
                break;
            case 4 :
                file_maintainance();
                break;
            case 5 :
                report();
                break;
            case 6 :
                break;
            default:
                printf("\nInvalid choice. Please enter a valid choice\n");
        }
	}
	if(cust!=NULL)
    list_to_customer(); //calling list_to_customer to add data from list to customer file
    if(stk!=NULL)
    list_to_stock(); //calling list_to_stock to add data from list to stock file
    pthread_mutex_destroy(&lock);
}

