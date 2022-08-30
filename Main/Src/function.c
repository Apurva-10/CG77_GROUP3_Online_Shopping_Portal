/***************************************************************************

FILENAME : function.c

DESCRIPTION : includes all the functions used in the online shopping portal

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <termios.h>
#include "func.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/***************************************************************************
 
FUNCTION NAME : customer_to_list

DESCRIPTION : customer file data is added to linkedlist

***************************************************************************/

int customer_to_list()
{
	struct customer *last,*new_cust,*ptr;
	FILE *fp;
	if((fp=fopen("customer","rb"))==NULL) //opening and checking whether file customer is NULL
	{
		return(1);
	}
	new_cust=(struct customer *)malloc(sizeof(struct customer)); //allocating memory to new_cust node
	fread(new_cust,sizeof(struct customer),1,fp);
	while(!feof(fp))
	{
		if(cust==NULL) 
		{
			cust=last=new_cust;
			new_cust->next=NULL;
		}
		else
		{
			last->next=new_cust;
			last=new_cust;
			new_cust->next=NULL;
		}
		new_cust=(struct customer *)malloc(sizeof(struct customer));
		fread(new_cust,sizeof(struct customer),1,fp);
	}
}


/***************************************************************************
 
FUNCTION NAME : list_to_customer

DESCRIPTION : linkedlist data is added back to customer file

***************************************************************************/

int list_to_customer()
{
	struct customer *ptr;
	if(cust==NULL)
	{
		return(1);
	}
	FILE *fp;
	fp=fopen("customer","wb");
	for(ptr=cust;(ptr);ptr=ptr->next)
	{
		fwrite(ptr,sizeof(struct customer),1,fp);
	}
	fclose(fp);
}


/***************************************************************************
 
FUNCTION NAME : stock_to_list

DESCRIPTION : stock file data is added to linkedlist

***************************************************************************/

int stock_to_list()
{
	struct stock *last1,*new_stk,*ptr1;
	FILE *fp1;
	if((fp1=fopen("stock","rb"))==NULL)
	{
		return(1);
	}
	new_stk=(struct stock *)malloc(sizeof(struct stock));
	fread(new_stk,sizeof(struct stock),1,fp1);
	while(!feof(fp1))
	{
		if(stk==NULL)
		{
			stk=last1=new_stk;
			new_stk->next=NULL;
		}
		else
		{
			last1->next=new_stk;
			last1=new_stk;
			new_stk->next=NULL;
		}
		new_stk=(struct stock *)malloc(sizeof(struct stock));
		fread(new_stk,sizeof(struct stock),1,fp1);
	}
}


/***************************************************************************
 
FUNCTION NAME : list_to_stock

DESCRIPTION : linkedlist  data is added back to stock file

***************************************************************************/

int list_to_stock()
{
	struct stock *ptr1;
	if(stk==NULL)
	{
		return(1);
	}
	FILE *fp1;
	fp1=fopen("stock","wb");
	for(ptr1=stk;(ptr1);ptr1=ptr1->next)
	{
		fwrite(ptr1,sizeof(struct stock),1,fp1);
	}
	fclose(fp1);
}


/***************************************************************************
 
FUNCTION NAME : report

DESCRIPTION : It is a password validated fuction to show stock and customer 
	      report to the employee

***************************************************************************/

int report()
{
    pthread_mutex_lock(&lock);
    printf("\nEnter admin Password\n");
    getchar();
    pass_mask();
    if(strcmp(password ,"gs3osp")) //validating admin password
    {

        printf("\nInvalid Password\n");
        return 1;
    }
    //shows the submenu
    int ch;
    while(ch != 3){
        printf("\n1. Stock Report\n2. Customer Report\n3. Main menu\n");
        scanf("%d",&ch); //accept choice from user
        switch(ch) 
        {
        case 1 :
            view_stock();
            break;
        case 2 :
            view_customer();
            break;
        case 3 :
            return 1;
        default :
            printf("\nInvalid choice\n");
            break;
        }
    }
    pthread_mutex_unlock(&lock);
}


/***************************************************************************
 
FUNCTION NAME : modify_stock_after_purchase

DESCRIPTION : This function is used to modify the product units available in 
 	      the  stock after every purchase

***************************************************************************/

int modify_stock_after_purchase(int pid,int preq)
{
    
    pthread_mutex_lock(&lock);
    stok *new,*prev,*ptr; //pointer of struct stok type
	if(!stk) //stk==NULL
	{
		printf("\nEmpty Stock list\n");
		return(1);
	}
	for(ptr=stk;(ptr) && ptr->product_id != pid;ptr=ptr->next);
	if(ptr==NULL)
	{
		printf("\n%d Product ID  not found\n",pid);
		return(1);
	}
	ptr->units_available=ptr->units_available-preq; //deducting units purchased from stock

    pthread_mutex_unlock(&lock);
}


/***************************************************************************
 
FUNCTION NAME : shop_here

DESCRIPTION : It allows the user to buy maximum of 3 grocery item at a time 
	      and generate its sales bill

***************************************************************************/

int shop_here()
{
    
    pthread_mutex_lock(&lock);
    printf("\nEnter customer ID\n");
    int id;
    scanf("%d",&id);
    cst *new,*prev,*ptr; //pointer of type struct cst
	if(!cust) //cust==NULL
	{
		printf("\nNo customer\n");
		return(1);
	}
	for(ptr=cust;(ptr) && ptr->id!=id;ptr=ptr->next);
	if(ptr==NULL) //id not found
	{
		printf("\n%d Customer not found\n",id);
		return(1);
	}
    getchar();
    printf("\nEnter customer Password\n");
    pass_mask();
    // checking customer password
	if(!strcmp(ptr->password,password)){
        printf("\nWrong Password \n");
		return(1);
	}
	printf("\n        Login Successful            \n");
	if(ptr->points >= 100) //cashback points greater than equal to 100 eligible for cashback
        printf("\n                    Congratulations ! You are eligible for 5% cashback on Purchase above Rs.200.\n                    Points will be deducted as cash from the Total bill\n\n\n");
	int ch=0,items_count=0;
	for(int i=0;i<3;i++)
    {
        buy[i].pid=-1;
    }
	for(items_count=0;items_count<3 && ch!=2;)
    {
        int pid,preq;
	// sub menu for shop here
        printf("\n1.Continue Shopping\n2.Checkout \n3.Main menu \n");
        scanf("%d",&ch);
        switch(ch)
        {
        case 1 :
            {

                printf("\nEnter Product ID to buy\n");
                scanf("%d",&pid);
                stok *new1,*prev1,*ptr1;
                if(!stk) //stk == NULL 
                {
                    printf("\nNo Product\n");
                    continue;
                }
                for(ptr1=stk;(ptr1) && ptr1->product_id!=pid;ptr1=ptr1->next);
                if(ptr1==NULL) //Product id not found
                {
                    printf("\n%d Product not found\n\n",id);
                    continue;
                }
                printf("\nEnter Units required\n");
                scanf("%d",&preq);
                if(ptr1->units_available < preq){
                printf("\nProduct out of Stock \n");
                    continue;
                }
                printf("\nProduct added to Bill\n");
                buy[items_count].pid=pid;
                buy[items_count].unit=preq;
                buy[items_count].up=ptr1->unit_price;
                items_count++;
                if(items_count==3)
                {
                    int ch;
                    while(ch!=1){
                        printf("\nYou have reached item limit of 3\n1. Checkout\n2. Main menu\n");
                        scanf("%d", &ch);
                        switch(ch)
                        {
                        case 1 ://check out
                            {
                                float total=0,net=0,cashback=0;
                                for(int i=0;i<3;i++){
                                if(buy[i].pid != -1) total+=(buy[i].up * buy[i].unit); //total price 
                                }
                                if(total > 200) cashback = (0.05 * total); //calculate cashback point
                                int tmp=cashback;
                                if(ptr->points <= 100) cashback=0;
                                ptr->points=ptr->points-cashback;
                                for(int i=0;i<3;i++)
                                {
                                    if(buy[i].pid != -1)
                                    modify_stock_after_purchase(buy[i].pid,buy[i].unit);
                                }
                                time_t t = time(NULL);
                                struct tm tm = *localtime(&t);
                                printf("\n                        SALES BILL                      \n");
                                printf("\nCustomer ID : %d         Customer Name : %s           Date : %d-%02d-%02d\n",id,ptr->name,tm.tm_mday, tm.tm_mon + 1,tm.tm_year + 1900);
                                for(int i=0;i<3;i++)
                                if(buy[i].pid != -1)
                                printf("\nProduct ID %d : %d            Units Ordered : %d         Unit Price : %d          Total Price : %f",i+1,buy[i].pid,buy[i].unit,buy[i].up,total);
                                printf("\n                                                                   Total Price : %f",total);
                                printf("\n                                                                   cashback : %f",cashback);
                                printf("\n                                                                 Net Payble : %f\n",total-cashback);
                                ptr->points+=tmp;

                                break;
                            }
                        case 2:
                            return 1;
                        default :
                            printf("\nInvalid choice\n");
                        }
                    }
                }
                if(items_count<=3)
                break;

            }
        case 2 : //check out if item limit is < 3
            {
                float total=0,net=0,cashback=0;
                for(int i=0;i<3;i++){
                    if(buy[i].pid != -1) total+=(buy[i].up * buy[i].unit);
                }
                if(total > 200) cashback = (0.05 * total);
                int tmp=cashback;
                if(ptr->points <= 100) cashback=0;
                ptr->points=ptr->points-cashback;
                for(int i=0;i<3;i++)
                {
                    if(buy[i].pid != -1)
                    modify_stock_after_purchase(buy[i].pid,buy[i].unit);
                }
                time_t t = time(NULL);
                struct tm tm = *localtime(&t);
                printf("\n                        SALES BILL                      \n");
                printf("\nCustomer ID : %d         Customer Name : %s           Date : %d-%02d-%02d\n",id,ptr->name,tm.tm_mday, tm.tm_mon + 1,tm.tm_year + 1900);
                for(int i=0;i<3;i++)
                if(buy[i].pid != -1)
                printf("\nProduct ID %d : %d            Units Ordered : %d         Unit Price : %d          Total Price : %f",i+1,buy[i].pid,buy[i].unit,buy[i].up,total);
                printf("\n                                                                   Total Price : %f",total);
                printf("\n                                                                   cashback : %f",cashback);
                printf("\n                                                                 Net Payble : %f\n",total-cashback);
                ptr->points+=tmp;

                break;

            }

        case 3 : return(1);

        default :
            printf("\nInvalid Choice\n");
            break;
        }

    }

    pthread_mutex_unlock(&lock);

}


/***************************************************************************
 
FUNCTION NAME : edit_stock

DESCRIPTION : This function allows the employee to edit product description, 
	      units available and unit price 

***************************************************************************/

int edit_stock()
{

    pthread_mutex_lock(&lock);
    stok *new,*prev,*ptr;
	if(!stk) //stk == NULL
	{
		printf("\nEmpty Stock list\n");
		return(1);
	}
	int meno;
	printf("\nEnter Product id to modify\n");
	scanf("%d",&meno);
	for(ptr=stk;(ptr) && ptr->product_id != meno;ptr=ptr->next);
	if(ptr==NULL) 
	{
		printf("\n%d Product ID  not found\n",meno);
		return(1);
	}
	int ch=0;
	while(ch != 4)
    {
        printf("\n1. Modify item description\n2. Modify units available\n3. Modify unit price\n4. Back to File Maintainance\n");
        scanf("%d",&ch);
        switch(ch)
        {
        case 1 :
            {
                char desc[30];
                while(1){
                    printf("\nEnter Product desc (Max  20 characters)\n");
                    getchar();
                    gets(desc);
                    if(strlen(desc) < 3 || strlen(desc) > 20){
                        printf("\nInvalid desc\n");
                    continue;
                    }
                else break;
                }
                strcpy(ptr->desc,desc);
                break;

            }
        case 2 :
            {
                int unit;
                while(1){
                    printf("\nEnter number of unit\n");
                    scanf("%d",&unit);
                    if(unit < 1){
                    printf("\nInvalid input\n");
                    continue;
                        }
                    else break;
                }
                ptr->units_available=unit;
                break;
            }
        case 3 :
            {
                int up;
                while(1){
                    printf("\nEnter unit price\n");
                    scanf("%d",&up);
                    if(up < 1){
                    printf("\nInvalid input\n");
                    continue;
                    }
                    else break;
                }
                ptr->unit_price=up;
            }
        case 4 :
            break;

        }
    }

    pthread_mutex_unlock(&lock);
}


/***************************************************************************
 
FUNCTION NAME : add_stocks

DESCRIPTION : It allows the employee to add new product and its details to
	      the stock file

***************************************************************************/

int add_stock()
{

    pthread_mutex_lock(&lock);
    int unit,up;
    char name[30],desc[30];
    getchar();
    while(1){
        printf("\nEnter Product name (Max  20 characters)\n");
        gets(name);
	//validations for name
        if(strlen(name) < 3 || strlen(name) > 20){
            printf("\nInvalid name\n");
            continue;
        }
        else break;
    }
    while(1){
        printf("\nEnter Product desc (Max  20 characters)\n");
        gets(desc);
	// validation for description
        if(strlen(desc) < 3 || strlen(desc) > 20){
            printf("\nInvalid desc\n");
            continue;
        }
        else break;
    }
    while(1){
        printf("\nEnter number of unit\n");
        scanf("%d",&unit);
        if(unit < 1){
            printf("\nInvalid input\n");
            continue;
        }
        else break;
    }
    while(1){
        printf("\nEnter unit price\n");
        scanf("%d",&up);
	//validation for unit price
        if(up < 1){
            printf("\nInvalid input\n");
            continue;
        }
        else break;
    }

    struct stock *newstock = malloc(4004*sizeof(newstock));
    newstock->product_id=1001;
    newstock->units_available=unit;
    newstock->unit_price=up;
    strcpy(newstock->name_of_product,name);
    strcpy(newstock->desc,desc);
    newstock->next=NULL;
    if(stk == NULL){
         stk = newstock;

    }
    //Otherwise, find the last customer and add the newcustomer
    else
    {
        struct stock *laststock = stk;

        //last customer's next address will be NULL.
        while(laststock->next != NULL)
        {
            laststock = laststock->next;
        }

        //add the newcustomer at the end of the linked list
        newstock->product_id=laststock->product_id+1;
        laststock->next = newstock;
    }
    //free(newstock);

    pthread_mutex_unlock(&lock);
}


/***************************************************************************
 
FUNCTION NAME : file_maintainance

DESCRIPTION : It is a password validated fuction having a sub menu to edit 
	      customer details, delete customer, add stock and edit stock

***************************************************************************/

int file_maintainance()
{
    printf("\nEnter Password\n");
    getchar();
    pass_mask();
    //checking password
    if(strcmp(password,"gs3osp")){
        printf("\nInvalid Password\n");
        return;
    }
    printf("\n                          Authenticated Successfully                      \n\n");
    int ch=0;
    while(ch != 5 )
    {   //sub menu
        printf("\n1.Edit Customer\n2.Delete Customer\n3.Add Item/Stock\n4.Edit Stock\n5.Back to main menu\n");
        printf("Enter Choice- ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:edit_customer();
                break;
            case 2:delete_customer();
                break;
            case 3:add_stock();
                break;
            case 4:edit_stock();
                break;
            case 5:break;
            default:printf("\n Invalid choice\n");
        }
    }
}


/***************************************************************************
 
FUNCTION NAME : delete_customer

DESCRIPTION : It allows the  employee to delete the customer record from
	      customer file

***************************************************************************/

int delete_customer()
{
    cst *new,*prev,*ptr;
	if(!cust) //cust == NULL
	{
		printf("\nEmpty Customer List\n");
		return(1);
	}
	int meno;
	printf("Enter Customer ID to delete - ");
	scanf("%d",&meno);
	if(meno==cust->id)
	{
		ptr=cust;
		cust=cust->next;
		free(ptr); // releasing memory
	}
	else
	{
		for(ptr=cust;(ptr) && ptr->id!=meno;prev=ptr,ptr=ptr->next);
		if(ptr==NULL)
		{
			printf("\n%d Customer ID  not found",meno);
			return(1);
		}
		prev->next=ptr->next;
		free(ptr);
	}
}


/***************************************************************************
 
FUNCTION NAME : edit_customer

DESCRIPTION : This function allows the employee to edit customer name,
 	      address and phone number

***************************************************************************/

int edit_customer()
{
    cst *new,*prev,*ptr;
	if(!cust) //cust==NULL
	{
		printf("\nEmpty Customer list\n");
		return(1);
	}
	int meno;
	printf("\nEnter Customer id to modify\n");
	scanf("%d",&meno);
	for(ptr=cust;(ptr) && ptr->id != meno;ptr=ptr->next);
	if(ptr==NULL)
	{
		printf("\n%d Customer ID  not found\n",meno);
		return(1);
	}
	int ch=0;
	while(ch != 4)
    {   //sub menu
        printf("\n1. Modify Name of customer\n2. Modify Address of customer\n3. modify phone number of customer\n4. Back to File Maintainance\n");
        scanf("%d",&ch);
        	getchar();

        switch(ch)
        {
        case 1 :
            {
                char name[30];
                while(1)
                {
                    printf("\nEnter name (Maximum 20 characters)\n");
                    gets(name);
		    //validation
                    if(strlen(name) > 20 || strlen(name) == 0){
                            printf("\nInvalid Input\n");
                            continue;
                        }
                    else break;
                }
                strcpy(ptr->name,name);
                break;

            }
        case 2 :
            {
                char address[30];
                while(1)
                {
                    printf("\nEnter address (Maximum 20 characters)\n");
                    gets(address);
		    //validation
                    if(strlen(address) > 20 || strlen(address) == 0){
                            printf("\nInvalid Input\n");
                            continue;
                        }
                    else break;
                }
                strcpy(ptr->address,address);
                break;
            }
        case 3 :
            {
                char phone_number[20];
                getchar();
                while(1){
                printf("\nEnter a valid mobile number\n");
                gets(phone_number);
		//validation
                int flag=0;
		for(int z=0;z<strlen(phone_number);z++) if(isdigit(phone_number[z])) flag++;
		if(flag > 10 || flag < 10){
                printf("\nInvalid Input\n");
                    continue;
                }
                else break;
                }
                strcpy(ptr->phone_number,phone_number);
            }
        case 4 :
            break;

        }
    }
}


/***************************************************************************
 
FUNCTION NAME : customer_registration

DESCRIPTION : This function add new customer to the customer file and take 
	      customer details as input from the customer
	      
***************************************************************************/

int customer_registration()
{
    char name[30],address[30],phone_number[20];
    int points;
    while(1){
        printf("\nEnter name (Maximum 20 characters)\n");
        getchar();
        gets(name);
        if(strlen(name) > 20 || strlen(name) < 3){
            printf("\nInvalid Input\n");
            continue;
        }
        else break;
    }
    while(1){
        printf("\nEnter password (Minimum 3 and Maximum 8 characters)\n");
        pass_mask();
        if(strlen(password) > 8 || strlen(password) < 3){
            printf("\nInvalid Input\n");
            continue;
        }
        else break;
    }
    while(1){
        printf("\nEnter address (Maximum 20 characters)\n");
        gets(address);
        if(strlen(address) > 20 || strlen(address) == 0){
            printf("\nInvalid Input\n");
            continue;
        }
        else break;
    }
    while(1){
        printf("\nEnter a valid mobile number\n");
        gets(phone_number);
	int flag=0;
	for(int z=0;z<strlen(phone_number);z++)
	{
		if(isdigit(phone_number[z])) flag++;
	}
        if(flag > 10 || flag < 10){
            printf("\nInvalid Input\n");
            continue;
        }
        else break;
    }
    struct customer *newcustomer = malloc(sizeof(cst));
    newcustomer->id = 100;
    strcpy(newcustomer->name,name);
    strcpy(newcustomer->password,password);
    strcpy(newcustomer->address,address);
    strcpy(newcustomer->phone_number,phone_number);
    newcustomer->points=0;
    newcustomer->next = NULL;
    if(cust == NULL)
         cust = newcustomer;
    //Otherwise, find the last customer and add the newcustomer
    else
    {
        struct customer *lastcustomer = cust;

        //last customer's next address will be NULL.
        while(lastcustomer->next != NULL)
        {
            lastcustomer = lastcustomer->next;
        }

        //add the newcustomer at the end of the linked list
        newcustomer->id=lastcustomer->id+1;
        lastcustomer->next = newcustomer;
    }


}


/***************************************************************************
 
FUNCTION NAME : view_customer

DESCRIPTION : This function allows the employee to view the customer details 

***************************************************************************/

void view_customer()
{
    cst *new,*prev,*ptr;//pointer of type struct cst
	if(!cust) //cust==NULL
	{
		printf("\nEmpty Customer List\n");
		return(1);
	}
    printf("\n                      DETAILED CUSTOMER REPORT                       \n");
    for(ptr=cust;(ptr);ptr=ptr->next)
	{
		printf("\nCustomer ID : %d           Name : %s            Address : %s         Phone Number :  %s          Cashback Points : %d\n",ptr->id,ptr->name,ptr->address,ptr->phone_number,ptr->points);

	}
}


/***************************************************************************
 
FUNCTION NAME : view_stock

DESCRIPTION : This function allows the employee to view stock details 

***************************************************************************/

void view_stock()
{
    stok *new1,*prev1,*ptr1;//variables of type struct stok
	if(!stk) //stk == NULL
	{
		printf("\nEmpty Stock List\n");
		return(1);
	}
	printf("\n                      DETAILED STOCK REPORT                       \n");
    for(ptr1=stk;(ptr1);ptr1=ptr1->next)
	{
        printf("\nProduct ID : %d           Description : %s            Unit Price : %d         Total Stock Available :  %d\n",ptr1->product_id,ptr1->desc,ptr1->unit_price,ptr1->units_available);

	}

}


/***************************************************************************
 
FUNCTION NAME : pass_mask

DESCRIPTION : This function is used to mask the password entered by the
	      customer and employee both

***************************************************************************/

int pass_mask()
{
    struct termios oflags, nflags;
    /* disabling echo */
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
        perror("tcsetattr");
        return EXIT_FAILURE;
    }

    printf("password: ");
    gets(password);
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
        perror("tcsetattr");
        return EXIT_FAILURE;
    }

    return 0;
}

