/********************

 FILENAME : func.h

DESCRIPTION : This is a header file having structures and
	      function declaration

*********************/

#ifndef FUNC_H
#define FUNC_H

typedef struct customer //structure used to handle customer report
{
    int id;
    char name[20];
    char phone_number[10];
    char password[8];
    char address[20];
    int points;
    struct customer* next;
}cst;


typedef struct stock //structure used to handle stock report
{
    int product_id;
    char name_of_product[20];
    int units_available;
    int unit_price;
    char desc[20];
    struct stock* next;
}stok;


struct cart  //structure used for shop here
{
    int pid,unit,up;
}buy[3];

//declaration of functions
char password[15];
int customer_registration();
void view_customer();
void view_stock();
int file_maintainance();
int edit_customer();
int delete_customer();
int add_stock();
int edit_stock();
int shop_here();
int report();
int modify_stock_after_purchase(int pid,int preq);

//pointer of struct cst type
cst *cust,*new,*prev,*ptr;
//pointer of struct stok type
stok *stk,*new1,*prev1,*ptr1;

#endif
