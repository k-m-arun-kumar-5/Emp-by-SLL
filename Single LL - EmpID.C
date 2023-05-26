/* ======================================================================
  File Name     : Single LL - EmpID.C
  Description   : Maintaining Employee's Record using EmpID based Single Linked List.
  Author        : K.M. Arun Kumar alias Arunkumar Murugeswaran
  Date          : 23rd April 2007.
  Remarks     1 : Single Link list is dedicated designed for employee record.
              2 : employee record is arranged based on Employee ID (key) in Single Linked List.
              3 : support operation only on specfic emp data or whole employee records.
              4 : by default, global execution flow flag is disable for debugging.
  Known Bugs
  Modification
       History  :
  Others        :
====================================================================== */
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "time.h"
#include "stdlib.h"
#include "conio.h"

#define NAME_SIZE      15
#define PHONE_SIZE     (10 + 1)
/* add, delete, print, quit, retrieve */
#define MAX_OPER       05
/* temp string size for input validation */
#define OPER_SIZE      20
/* insert node oper */
#define ADD_OPER          01
#define DELETE_OPER       02
#define PRINT_OPER        03
#define RETRIEVE_OPER     04
#define QUIT_OPER         05
/* EMPID linked(single/double/circular) List */
#define EMPID_TARGET    01
/* delete whole list irrespect of keys */
#define DEL_WHOLE_LIST  02
/* range of empid for oper */
#define EMPID_RANGE     04
/* search using emp name, which is unordered & non unique */
#define EMPNAME_TARGET  06
/* true state of  oper */
#define TRUE            01
/* false state of oper */
#define FALSE           00
/* allow access to global mega_head's curptr */
#define FREE_ACCESS     00
/* only empid list is allowed access to global mega_head's curptr */
#define EMPID_ACCESS    01
#define EMPPHONE_ACCESS 02
#define NO_ACCESS       03
/* Emp record to be modified */
#define MODIFY_RECORD    01
#define NO_MODIFY_RECORD 00
 /* unique record */
#define NO_EXIST_RECORD  02
#define EXIST_RECORD     03
#define TRACE_ON         1
#define TRACE_OFF        0
/* display mega_data info for debug */
#define MEGA_DATA_PRINT 02
#define INVALID_DATA    -1
/* access_curptr does not allow to access */
#define ACCESS_DENIED   -1

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;

typedef char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long int64_t;
typedef enum { SUCCESS, FAILURE} sys_status_t;

#define NULL_DATA_PTR                            ((void *)0)
#define NULL_CHAR                               ('\0')

#define STR_MAX_CHARS                            (7)
#define EMP_MIN_ID                      (1)
#define EMP_MAX_ID                      (1000)

struct employee
{
  long int emp_id;                        /* Emp Id as the one key for emp linked list  */
  char emp_name[NAME_SIZE];
  char emp_phone[PHONE_SIZE];             /* phone as the other key for emp linked list  */
};

struct employee_list                      /* data struct */
{
  struct employee emp_data;
  struct employee_list *idforeptr;        /* emp_id front ptr */
  struct employee_list *idbackptr;        /* emp_id back ptr */
};
typedef struct employee_list emp_record;

struct megadata                            /* link control */
{
  int count;                               /*  number of nodes */
  emp_record *idheadptr;                   /*  contains emp_id head's node */
  emp_record *idtailptr;                   /*  contains emp_id tail's node */
  emp_record *curptr;                      /*  temp ptr contains current node */
  emp_record *preptr;                      /*  temp ptr contains predecessor's node */
  char access_curptr;                      /*  acts as control flag to access curptr */
};

/* global variable defination */
struct megadata mega_head;                 /* contains head link for emp linked list */
int trace_flag = TRACE_OFF;                 /* global trace eecution for debugging */
char input_str[STR_MAX_CHARS];

/* function declaration */
extern int Initialize_MegaEmpList(void );
extern int Link_Menu(void);
extern int Get_EmpData(void);
extern int Add_EmpNode(int target_mode, struct employee empdata, int exist);
extern int Delete_EmpData(int target_mode, void *from_del, void *to_del, int char_size, struct employee *get_delnodedata);
extern int Delete_EmpNode(int target_mode);
extern int Get_DelEmpData(int del_mode);
extern int Print_EmpList( int target_mode);
extern int Search_EmpNode(int target_mode, void *dataptr, void *target, unsigned int *node_nos_ptr);
extern int Retrieve_EmpData(int retrieve_mode);
extern int Trace_flag(void);
extern int Print_EmpList(int target_mode);
extern int Access_Curptr(int access_mode);

uint16_t Get_Validate_Input_Number(void *const int32_input_num_ptr, char *const input_str_ptr, const unsigned int input_str_max_chars, const int32_t valid_min_value, const int32_t valid_max_value);
uint16_t Get_Input_Str(char *const input_str_ptr, const unsigned int input_str_max_chars);
uint16_t Str_to_Num_Conv(void *const num_conv_from_str, const char *const num_in_str);
uint32_t Power_Of(const uint8_t base, const uint8_t power);
uint16_t Get_Input_Num_Char_Str(char *const input_str_ptr, const unsigned int input_str_req_chars);
uint16_t Get_Input_Alpha_Char_Str(char *const input_str_ptr, const unsigned int input_str_max_chars);
/* =========================================================
   Function name : main
   Description   : operation on Single Linked list
   Remarks       : assume data input are valid
   Func ID       : 1
=========================================================== */
int main()
{
  unsigned int control;
  long int oper_data;
  struct employee temp_emp;        /* Temp Emp data before processing */


  /* initialise the head link for employees record */
  if(!Initialize_MegaEmpList())
  {
      printf("\n ERR[1.1]: Unable to create a list");
      exit(FALSE);
  }
   /* Some Basic operation on Emp Single Linked list */
  do
  {
    control = Link_Menu();
   /* Trace_flag(); */       /* for debugging */
    switch( control)
    {
       case ADD_OPER:
	     if(!Get_EmpData())
	       continue;
	     break;
       case DELETE_OPER:
         printf("\n 1 - Emp Id, 2 - whole records \n Enter oper: ");
        // scanf("%d",&oper_data);
		 if((Get_Validate_Input_Number(&oper_data, input_str, STR_MAX_CHARS, EMPID_TARGET, DEL_WHOLE_LIST)) != SUCCESS)
         {
	         printf("\n ERR: Invalid delete operation");
             continue;
         }
         if(!Get_DelEmpData(oper_data))
	        continue;
	 	 break;
	   case PRINT_OPER:
         printf("\n Print Emp records based on: 1 - ID, 2 - Mega Node \n Enter : ");
        // scanf("%d", &oper_data);
		 if((Get_Validate_Input_Number(&oper_data, input_str, STR_MAX_CHARS, EMPID_TARGET, MEGA_DATA_PRINT)) != SUCCESS)
         {
	         printf("\n ERR: Invalid print operation");
             continue;
         }
         if(!Print_EmpList(oper_data))
	       continue;
	     break;
	   case RETRIEVE_OPER:
	    /* retrieve given a phone, name or empid */
	    /*  printf("\n Retrieve Emp records based on: 1 - ID : Enter : ");
		 if((Get_Validate_Input_Number(&oper_data, input_str, STR_MAX_CHARS, EMPID_TARGET, EMPID_TARGET)) != SUCCESS)
         {
	         printf("\n ERR: Invalid retrieve operation");
             continue;
         } */
		//scanf("%d", &oper_data);
		oper_data = EMPID_TARGET;
		switch(oper_data)
		{
		   /* maps user input name search to search mode, to avoid conflict with DEL_WHOLE_LIST macro, used in search func */
			case EMPID_TARGET :
			   break;
                        default:
                          printf("\n ERR[1.2]: Invalid retrieve mode ");
                          continue;
	        }
		if(!Retrieve_EmpData(oper_data))
		   continue;
	    break;
       case QUIT_OPER:
         if(!Get_DelEmpData(DEL_WHOLE_LIST))
         ;
	 	 break;
      default:
	     printf("\n ERR[1.3]: Invalid Linked list operation: %d", control);
     }
  } while(control != QUIT_OPER );
 return TRUE;
}

/* ==================================================================
   Function Name : Initialize_MegaEmpList()
   Description   : creates a empty linked list
   Remarks       :
   Func_ID       : 2
=================================================================== */
int Initialize_MegaEmpList(void)
{
   /* Empty Linked list */
   mega_head.count = 0;
   mega_head.idheadptr = NULL;
   mega_head.idtailptr = NULL;
   Access_Curptr(FREE_ACCESS);
   if(trace_flag)
        printf("\n TRACE[2.1]: A SLL Initialised");
   return TRUE;
}

/* ==================================================================
   Function Name : Link_Menu()
   Description   : get & valid input code for single linked list operation
   Remarks       : assume that input data are valid
   fUNC_ID       : 3
=================================================================== */
int Link_Menu(void)
{
  char get_oper[OPER_SIZE];
  char check_oper[OPER_SIZE];
  int get_ctrl;

  printf("\n MENU for Single Linked List Operation in Emp record :- ");
  printf("\n =======================================================");
  printf("\n 1 - Add a Emp  2 - Delete a Emp  3 - Display Emp  4 - Retrieve Emp  5 - Quit \n Enter : ");
  //scanf("%d",&get_ctrl);
  if((Get_Validate_Input_Number(&get_ctrl, get_oper, MAX_OPER, ADD_OPER, QUIT_OPER)) != SUCCESS)
  {
	  return QUIT_OPER + 1;
  }
  return get_ctrl;
}

int Trace_flag(void)
{
   printf("\n 1: Use to Enable Global Trace : ");
   //scanf("%d", &trace_flag);
   if((Get_Validate_Input_Number(&trace_flag, input_str, 2, TRACE_OFF, TRACE_ON)) != SUCCESS)
   {
	    printf("\n ERR : Invalid Trace flag, default flag = OFF");
	    trace_flag = TRACE_OFF;
	    return FALSE;
   }
   return TRUE;
}

/* ==================================================================
   Function Name : Get_EmpData(void)
   Description   : get emp id & emp name
   Remarks       : assume that input data are valid & phone is not a key
   fUNC_ID       : 5
=================================================================== */
int Get_EmpData(void)
{
   struct employee get_empdata;
   struct employee retrieve_empdata;
   long int emp_id;
   char emp_phone[PHONE_SIZE];
   unsigned int nodepos = 0;
   int modify_flag = NO_EXIST_RECORD;
   char modify;

   printf("\n Enter Employee's ID : ");
  // scanf("%ld", &emp_id);
   if((Get_Validate_Input_Number(&emp_id, input_str, STR_MAX_CHARS, EMP_MIN_ID, EMP_MAX_ID)) != SUCCESS)
   {
	  printf("\n ERR : Invalid Emp ID");
	  return FALSE;
   }
   nodepos = Search_EmpNode(EMPID_TARGET, &retrieve_empdata, &emp_id, &nodepos);
   /* unable to access mega emp head, give up */
   if(nodepos == ACCESS_DENIED)
   {
	   return FALSE;
   }
   if( nodepos != FALSE )
   {   /* retrieve_empdata contains emp record for existing empid */
	   printf("\n WARN[5.1]: emp_id: %ld already exist at node pos %d",emp_id, nodepos );
	   Access_Curptr(FREE_ACCESS);
	   if(trace_flag)
	     printf("\n TRACE[5.2]: Exist Emp record: ID: %ld, name: %s, phone: %s",retrieve_empdata.emp_id, retrieve_empdata.emp_name, retrieve_empdata.emp_phone );
	  /* printf("\n Do you want to modify its record(y/n) :");
	   scanf(" %c", &modify);
	   if(modify != 'y')
	   {
		   Access_Curptr(FREE_ACCESS);
	   	   return FALSE;
       }
       // modify existing emp record
       modify_flag = MODIFY_RECORD; */
       return EXIST_RECORD;
   }
   get_empdata.emp_id = emp_id;
   printf("\n Enter Employee's name having emp_id: %ld: ", get_empdata.emp_id);
  // scanf(" %[^\n]", get_empdata.emp_name);
   if((Get_Input_Alpha_Char_Str(get_empdata.emp_name, NAME_SIZE)) != SUCCESS)
   {
	   printf("\n ERR: Invalid Emp Name");
	   Access_Curptr(FREE_ACCESS);
	   return FALSE;
   }
   printf("\n Enter Employee's phone having emp_id: %ld: ", get_empdata.emp_id);
   //scanf(" %s", emp_phone);
   if((Get_Input_Num_Char_Str(get_empdata.emp_phone, PHONE_SIZE )) != SUCCESS)
   {
	   printf("\n ERR: Invalid Emp Phone ");
	   Access_Curptr(FREE_ACCESS);
	   return FALSE;
   }
   if((Add_EmpNode(EMPID_TARGET, get_empdata,modify_flag )) == TRUE)
   {
	   printf("\n Added Emp Record: ID - %ld, name: %s, phone: %s", get_empdata.emp_id,get_empdata.emp_name, get_empdata.emp_phone );
	   return TRUE;
   }
    Access_Curptr(FREE_ACCESS);
   return FALSE;
}

/* ==================================================================
   Function Name : int Search_EmpNode( int target_mode, void *dataptr, void *target, int phone_size)
   Description   : search for given key
   Remarks       : assumes unique & ordered emp id & phone. doesnt support phone as key.
                   headptr, phone input data are reserved for futhur use
   fUNC_ID       : 6
=================================================================== */
int Search_EmpNode(int target_mode, void *dataptr, void *target, unsigned int *node_count_ptr)
{
    int nodepos = FALSE;            /* curptr node position from its head */
    long int target_empid;
    char target_phone[PHONE_SIZE];
    int retrieve_flag = FALSE;
    char target_name[NAME_SIZE];
    struct employee *retrieve_empdata;
    emp_record *temp_emp_record;

    if(mega_head.access_curptr != FREE_ACCESS)
    {
        printf("\n ERR[6.3]: Unable to access head control data: %d",mega_head.access_curptr);
		return ACCESS_DENIED;
    }
    mega_head.preptr = NULL;
    switch(target_mode)
    {
	   case EMPID_TARGET:
	      target_empid = *(long int *) target;
	      Access_Curptr(EMPID_ACCESS);
	      mega_head.curptr = mega_head.idheadptr;
	      /* check for emp id, which is ordered and unique */
		  while( mega_head.curptr != NULL && mega_head.curptr->emp_data.emp_id < target_empid)
          {
		  	  /* move to next node of emp id list to check again */
		   	  mega_head.preptr = mega_head.curptr;
		   	  mega_head.curptr = mega_head.curptr->idforeptr;
              ++nodepos;
	      }
	      /* reached either end of emp id list or empty list */
	      if(mega_head.curptr == NULL)
	      {
	     	 if(trace_flag)
	    	   printf("\n TRACE[6.1]: Id: %ld not found",target_empid);
	    	 nodepos = FALSE;
	    	 break;
	      }
		  /* found that emp id is existing in list*/
		  if(target_empid == mega_head.curptr->emp_data.emp_id)
		  {
             /* data retrieve for target which is unique & ordered */
	          memcpy(dataptr, &mega_head.curptr->emp_data, sizeof(struct employee));
			  nodepos = TRUE;
	         if(trace_flag)
	           printf("\n TRACE[6.3]: Found Emp record: ID: %ld, name: %s, phone: %s ", ((struct employee *)dataptr)->emp_id, ((struct employee *)dataptr)->emp_name, ((struct employee *)dataptr)->emp_phone);
              break;
		  }
		  if(trace_flag)
	         printf("\n TRACE[6.2]: Emp Id: %ld not found",target_empid);
		  nodepos = FALSE;
		  break;
	   case DEL_WHOLE_LIST:
	      Access_Curptr(EMPID_ACCESS);
	      if(mega_head.idheadptr)
	      {
			 if(node_count_ptr == NULL)
			 {
				 printf("\n ERR[6.3]: node count ptr = NULL ");
				 return FALSE;
			 }
	         mega_head.curptr = mega_head.idheadptr;
	         /* retrieves whole emp records from emp list before destroying it */
             while(mega_head.curptr)
             {
				 temp_emp_record = mega_head.curptr;
				 if(*node_count_ptr < OPER_SIZE)
                    *((struct employee *)dataptr + (*node_count_ptr)++) = mega_head.curptr->emp_data;
				  else
					printf("\n ERR[6.4] : nos of delete whole emp records > %u", OPER_SIZE);
				free(mega_head.curptr);
                mega_head.curptr = temp_emp_record->idforeptr;
	         }
             Initialize_MegaEmpList();
             nodepos = TRUE;
	      }
		  else
		  {
			  printf("\n ERR: Emp records is already empty");
			  nodepos = FALSE;
		  }
       break;
		  #ifdef EMP_NAME
       case EMPNAME_TARGET:
          /* initialise local target before copying */
          memset(target_name,0,sizeof(target_name));
          /* assumes that target string terminates with '\0' */
	      memcpy(target_name, (char *)target, max_size);
          if(trace_flag)
	         printf("\n Emp name to be searched: %s",target_name );
		  Access_Curptr(EMPID_ACCESS);
		  mega_head.curptr = mega_head.idheadptr;
		  /* check for emp name, which is unordered and not unique */
		  while( mega_head.curptr != NULL )
		  {   /* dont know why comparing using memcmp is not working in Turbo C */
			  /*int n;
			  n = memcmp(mega_head.curptr->emp_data.emp_name,target_name,max_size);
			  printf("\n emp_id: %ld, name; %s, n %d, size: %d", mega_head.curptr->emp_data.emp_id, mega_head.curptr->emp_data.emp_name, n, max_size); */
			  if(!strcmp(mega_head.curptr->emp_data.emp_name,target_name))
		  	 {/* emp name matches target name  */
                printf("\n Found Emp record: ID: %ld, name: %s, phone: %s ",mega_head.curptr->emp_data.emp_id, mega_head.curptr->emp_data.emp_name, mega_head.curptr->emp_data.emp_phone);
                /* address of where retrive data should not changed, which cause loss of address */
                ((struct employee *)dataptr)[node_count] = mega_head.curptr->emp_data;
                ++node_count;
		     }
		  	 /* move to next node of emp id list to check again */
		   	  mega_head.preptr = mega_head.curptr;
		   	  mega_head.curptr = mega_head.curptr->idforeptr;
		      ++nodepos;
		  }
		  /* reached either end of emp id list or empty list */
		  if(node_count <= 0)
		  {
			  printf("\n ERR[6.3]: Emp name: %s does not exist",target_name );
			  nodepos = FALSE;
	      }
	      else    /* returns number of emp records matching target emp name */
	          nodepos = node_count;
	      break;
		 #endif
       default:
          printf("\n ERR[6.4]: doesn't support target search: %d",target_mode );
	      nodepos = FALSE;
    }
  return nodepos;
}

/* ==================================================================
   Function Name : Access_Curptr(int access_mode)
   Description   : control access to head's curptr
   Remarks       :
   fUNC_ID       : 7
=================================================================== */
int Access_Curptr(int access_mode)
{
	int istrue;
	switch(access_mode)
	{
		case FREE_ACCESS:
		case EMPID_ACCESS:
		case EMPPHONE_ACCESS:
		case NO_ACCESS:
		  mega_head.access_curptr = access_mode;
		  istrue = TRUE;
		  break;
		default:
		  printf("\n ERR[7.1]: Invalid access_mode : %d",access_mode );
		  istrue = FALSE;
     }
  return istrue;
 }

/* ==================================================================
   Function Name : Access_Curptr(int access_mode)
   Description   : control access to head's curptr
   Remarks       : consider only single link list of empid
   fUNC_ID       : 8
=================================================================== */
int Add_EmpNode(int target_mode, struct employee emp_data, int modify)
{
   emp_record *new_emprecord;
   int ret_state = FALSE;

   switch(target_mode)
   {
	   case EMPID_TARGET:
	      /* make sure u know where to insert node in empid list */
	      if(mega_head.access_curptr != EMPID_ACCESS)
	      {
	      	  printf("\n ERR[8.1]: Invalid head access: %d",mega_head.access_curptr );
	      	  break;
	      }
		  if(modify == MODIFY_RECORD)
	      {
              mega_head.curptr->emp_data = emp_data;
              Access_Curptr(FREE_ACCESS);
	      /* yet to consider changed phone to be ordered for multilink */
              ret_state = TRUE;
              break;
	      }
	      if(!(new_emprecord = (emp_record *)calloc(1, sizeof( emp_record))))
	      {
			 printf("\n ERR[8.2]: No new Memory to allocate %d elemnent of %d", 1, sizeof(emp_record) );
			 break;
		  }
		  new_emprecord->emp_data = emp_data;
		  /* to add node at end of empid list, update its tail ptr */
		  if(mega_head.curptr == NULL)
             mega_head.idtailptr = new_emprecord;
          /* add node at begin of empid list */
		  if(mega_head.preptr == NULL)
	      {
             new_emprecord->idforeptr = mega_head.idheadptr;
             mega_head.idheadptr = new_emprecord;
             /* new_emprecord->idbackptr = NULL; */
          }
	      else
	      {
			  /* add node at middle or end of empid list */
	         new_emprecord->idforeptr = mega_head.preptr->idforeptr;
	         mega_head.preptr->idforeptr  = new_emprecord;
	      }
	      ++mega_head.count;
	      ret_state = TRUE;
          break;
        default:
          printf("\n ERR[8.3]: doesn't support target search: %d",target_mode );
   }
   Access_Curptr(FREE_ACCESS);
   return ret_state;
}

/* ==================================================================
   Function Name : Print_EmpList
   Description   : displays emp datas
   Remarks       :
   fUNC_ID       : 9
=================================================================== */
int Print_EmpList(int target_mode)
{
   emp_record *curptr = NULL;       /* local temp ptr contains current node */
   int ret_state = TRUE;
   int i = 0;

   if(mega_head.access_curptr != FREE_ACCESS)
   {
	   printf("\n ERR[9.1]: Unable to access head control data: %d",mega_head.access_curptr);
	   return FALSE;
   }
    switch(target_mode)
    {
		case EMPID_TARGET:
		   curptr = mega_head.idheadptr;
		   if(curptr == NULL)
		   {
			   printf("\n Emp Record does not exist");
			   return TRUE;
		   }
           printf("\n Display Employee's record based on EMP_ID's list:");
		   printf("\n =================================================");
		   while(curptr)
		  {
			  ++i;
		      printf("\n %02d Node Data:: ID: %10ld, name: %10s, phone: %10s",\
		         i,curptr->emp_data.emp_id,curptr->emp_data.emp_name, curptr->emp_data.emp_phone);
			  if(trace_flag)
		         printf("\n %02d: Emp ID: %10ld, Addr Node: %#X, ID Front: %#X, ID Back: %#X",\
		            i, curptr->emp_data.emp_id, curptr, curptr->idforeptr,curptr->idbackptr );
		      curptr = curptr->idforeptr;
	      }
	      printf("\n =================================================");
          break;
    	/*	case EMPPHONE_TARGET:
		    printf("\n Display Employee's record based on EMP_PHONE's list:");
		   printf("\n ====================================================");
		   curptr = mega_head.phoneheadptr;
		   while(curptr)
		   {
			  ++i;
		      printf("\n %02d Node Data:: Phone: %10s, name: %10s, ID: %10ld ",\
			       i,curptr->emp_data.emp_phone,curptr->emp_data.emp_name, curptr->emp_data.emp_id,curptr);
			  printf("\n %02d: Addr Node: %#X, Ph Front: %#X, Ph Back: %#X, ID Front: %#X, ID Back: %#X ",\
			    i,curptr, curptr->phoneforeptr,curptr->phonebackptr,curptr->idforeptr,curptr->idbackptr );
		      curptr = curptr->phoneforeptr;
	       }
	       printf("\n =================================================");
	       break; */
        case MEGA_DATA_PRINT:
            printf("\n TRACE[9.1]: Display Emp's Mega Node's Control Info:");
           printf("\n ===================================================");
           printf("\n No of nodes: %d, access_curptr: %d, curptr: %#X, preptr: %#X", \
            mega_head.count, mega_head.access_curptr, mega_head.curptr, mega_head.preptr);
           printf("\n idheadptr: %#X, idtailptr: %#X ", mega_head.idheadptr, mega_head.idtailptr );
           printf("\n =================================================");
          break;
        default:
           printf("\n ERR[9.1]: Invalid display mode: %d",target_mode );
           ret_state = FALSE;
    }
    return ret_state;
}

/* ==================================================================
   Function Name : Delete_EmpData(int target_mode, void *from_del, void *to_iddel, int ph_size,struct employee *emp_deldata)
   Description   : searches specific node based on target key, and deletes it from list
   Remarks       : data retrived from deleted node is retrived in its pointer.
   fUNC_ID       : 10
=================================================================== */
int Delete_EmpData(int target_mode, void *from_del, void *to_del, struct employee *get_delnodedata, unsigned int *emp_nodes_ptr )
{
	long int from_iddel, to_iddel;
    char from_phdel[PHONE_SIZE], to_phdel[PHONE_SIZE];
    struct employee get_delempdata[OPER_SIZE];
    int ret_state = FALSE;
    unsigned int node_nos = 0, node_count = 0;

	switch(target_mode)
	{
       case EMPID_TARGET:
	     if((Search_EmpNode(target_mode, get_delnodedata, from_del, &node_nos)) == TRUE)
		 {
			 if(trace_flag)
                printf("\n Deleted Emp ID: %ld, name: %s, phone: %s, Addr: 0X%X",
                  get_delnodedata->emp_id, get_delnodedata->emp_name, get_delnodedata->emp_phone, mega_head.curptr);
             ret_state = Delete_EmpNode(target_mode);
		  }
		break;
		case DEL_WHOLE_LIST:
	       ret_state = Search_EmpNode(target_mode, get_delnodedata , NULL, emp_nodes_ptr );
           switch(ret_state)
           {
			case FALSE:
			 	get_delnodedata = NULL;
		     break;
			 case ACCESS_DENIED:
			    get_delnodedata = NULL;
			 break;
             case TRUE:
             break;
             default:
                get_delnodedata = NULL;
	     }
       break;
       default:
         printf("\n ERR[10.2]: doesn't support target search: %d",target_mode );
    }
    Access_Curptr(FREE_ACCESS);
    return ret_state;
}

/* ==================================================================
   Function Name : Delete_EmpNode(int target_mode)
   Description   : deletes specific data a node based on target key, also has deleted info in pointer
   Remarks       : for deleting whole list, nodepos is number of nodes in the list
   fUNC_ID       : 11
=================================================================== */
int Delete_EmpNode(int target_mode)
{
	int ret_state = FALSE;

	switch(target_mode)
	{
       case EMPID_TARGET:
         /* make sure u know where to insert node in empid list */
		 if(mega_head.access_curptr != EMPID_ACCESS)
		 {
		    printf("\n ERR[11.1]: Invalid head access: %d",mega_head.access_curptr );
		    break;
	     }
         /* to delete node at end of empid list, update its tail ptr */
         if(mega_head.curptr->idforeptr == NULL)
                 mega_head.idtailptr = mega_head.preptr;
         /* delete node at begin of emp id's list */
         if(mega_head.preptr == NULL)
         	 mega_head.idheadptr = mega_head.curptr->idforeptr;
		 else
	        /* delete node at middle or at end of emp id's list */
            mega_head.preptr->idforeptr = mega_head.curptr->idforeptr;
         free(mega_head.curptr);
         --mega_head.count;
         ret_state = TRUE;
         break;
       case DEL_WHOLE_LIST:
          /* uses empid list based destroying whole emp records */
          mega_head.preptr = mega_head.idheadptr;
		  if(mega_head.preptr)
		  {
			  printf("\n ERR: Emp ID is already empty");
			  return TRUE;
		  }
          while(mega_head.preptr)
          {
			 mega_head.curptr = mega_head.preptr->idforeptr;
			 free(mega_head.preptr);
             mega_head.preptr = mega_head.curptr;
	      }
	      Initialize_MegaEmpList();
	      ret_state = TRUE;
	      break;
	   case EMPID_RANGE:
	     break;
       default:
         printf("\n ERR[11.2]: doesn't support target search: %d",target_mode );
    }
    return ret_state;
}

/* ==================================================================
   Function Name : Get_DelEmpData(int del_mode)
   Description   : get either empid or ph specific or range to be deleted
   Remarks       : assume that correcponding data is valid & in order.
   fUNC_ID       : 12
=================================================================== */
int Get_DelEmpData(int del_mode)
{
	long int from_iddel, to_iddel;
    struct employee get_delempdata[OPER_SIZE];
    int ret_state = FALSE;
    unsigned int node_nos = 0, node_count = 0;

    switch(del_mode)
    {
		case EMPID_TARGET:
          printf("\n Enter Emp ID to be deleted: ");
         // scanf("%ld", &from_iddel);
          if((Get_Validate_Input_Number(&from_iddel, input_str, STR_MAX_CHARS, EMP_MIN_ID, EMP_MAX_ID)) != SUCCESS)
		  {
			 printf("\n ERR: Invalid Emp ID ");
			 return FALSE;
		  }
          /* to delete specfic empid  */
		   to_iddel = from_iddel;
           if(from_iddel == to_iddel)
           {
		      if((Delete_EmpData(del_mode, &from_iddel, NULL, get_delempdata, &node_nos)) == TRUE)
			  {
			      printf("\n Deleted Data: Emp ID: %ld, name: %s, phone: %s ", \
                    get_delempdata[0].emp_id, get_delempdata[0].emp_name, get_delempdata[0].emp_phone);
			      ret_state = TRUE;
		      }
			  else
			  {
				  printf("\n Emp ID : %ld does not exist", from_iddel);
			  }
			  break;
	       }
		   printf("\n ERR[12.2]: Support only single empid based deletion");
		   break;
		 case DEL_WHOLE_LIST:

           if((Delete_EmpData(del_mode, NULL, NULL, get_delempdata, &node_nos )) == TRUE)
		   {
			   printf("\n Whole list been Deleted");
          	  while(node_count < node_nos)
			  {
				   printf("\n Data: ID: %ld, name: %s, phone: %s", \
				   get_delempdata[node_count].emp_id, get_delempdata[node_count].emp_name, get_delempdata[node_count].emp_phone);
				  ++node_count;
		      }
			  ret_state = TRUE;
	       }
		 break;
		 default:
		   printf("\n ERR[12.1]: doesn't support target delete: %d",del_mode );
     }
     return ret_state;
 }

/* ==================================================================
   Function Name : Retrieve_EmpData(int retrieve_mode)
   Description   : get retrieve emp record based on empid, phone and name
   Remarks       : assume that correcponding input data is valid.
   fUNC_ID       : 13
=================================================================== */
int Retrieve_EmpData(int retrieve_mode)
{
	int ret_state = FALSE;
    int nodepos;
    struct employee retrieve_empdata[1];
    long int retrieve_empid;
    char retrieve_empname[NAME_SIZE];
    char retrieve_empphone[PHONE_SIZE];
    unsigned int count_node =0;

    /* initilize array using memory oper with 0 */
    memset(retrieve_empdata,0,sizeof(retrieve_empdata));
    switch(retrieve_mode)
    {
		case EMPID_TARGET:
		  printf("\n Enter Emp ID to retrieve its record: ");
		 // scanf("%ld", &retrieve_empid);
		 if((Get_Validate_Input_Number(&retrieve_empid, input_str, STR_MAX_CHARS, EMP_MIN_ID, EMP_MAX_ID)) != SUCCESS)
		 {
			 printf("\n ERR: Invalid Emp ID ");
			 Access_Curptr(FREE_ACCESS);
			 return FALSE;
		 }
	      if((Search_EmpNode(retrieve_mode, retrieve_empdata, &retrieve_empid, &count_node)) == TRUE)
          {
            /* retrieve_empdata contains emp record for existing empid */
		     printf("\n Emp Record: ID: %ld, name: %s, phone: %s", retrieve_empdata[0].emp_id, retrieve_empdata[0].emp_name, retrieve_empdata[0].emp_phone );
	         ret_state = TRUE;
	      }
	      else
	        printf("\n ERR[13.1]: Emp ID: %ld does not exist",retrieve_empid);
	     break;
		#ifdef EMP_NAME
		case EMPNAME_TARGET:
		  printf("\n Enter Emp Name to retrieve its record: ");
		  //scanf(" %[^\n]",retrieve_empname);
		  if((Get_Input_Alpha_Char_Str(retrieve_empname, NAME_SIZE)) != SUCCESS)
          {
	           printf("\n ERR: Invalid Emp Name");
	           Access_Curptr(FREE_ACCESS);
	           return FALSE;
           }
	      nodepos = Search_EmpNode(retrieve_mode, retrieve_empdata, retrieve_empname, &count_node);
          if( nodepos != FALSE )
          {
    		  while(count_node < nodepos)
			  {
				  printf("\n Matched Emp name: %s record, empid: %ld, empphone: %s",
				     retrieve_empdata[count_node].emp_name,retrieve_empdata[count_node].emp_id,retrieve_empdata[count_node].emp_phone);
				  ++count_node;
		      }
              ret_state = TRUE;
	      }
	      break;
		#endif
		default:
		  printf("\n ERR[13.2]: doesn't support retrievemode: %d",retrieve_mode );
    }
    Access_Curptr(FREE_ACCESS);
  return ret_state;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Str_to_Num_Conv

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           : digits are extracted from left to right format from digit in num_in_str

Func ID        : 02.04

BUGS           :
-*------------------------------------------------------------*/
uint16_t Str_to_Num_Conv( void *const num_conv_from_str_ptr, const char *const num_in_str)
{
	 int32_t num = 0, *num_conv_from_str;
	 uint32_t place;
	 int16_t cur_unit;
	 uint8_t num_chars = 0, base = 10, pos = 0, start_num_pos = 0 ;

	 if(num_conv_from_str_ptr == NULL_DATA_PTR || num_in_str == NULL_DATA_PTR )
	 {
		 #ifdef TRACE_ERROR
		   printf("ERR: data are null ptr \n");
		#endif
		return FAILURE;
	 }
	 num_conv_from_str = (int32_t *)num_conv_from_str_ptr;
	 *num_conv_from_str = 0;
	 if(num_in_str[0] >= '0' && num_in_str[0] <= '9')
	 {
		  start_num_pos = 0;
	 }
	 else if(num_in_str[0] == '-')
	 {
		 start_num_pos = 1;
	 }
	 else
	 {
    	 #ifdef TRACE_ERROR
		    printf("ERR: invalid char: %c \n", num_in_str[0]);
		 #endif
         return FAILURE;
	 }
	 num_chars = strlen(num_in_str + start_num_pos);
	 if(num_chars == 0)
	 {
		 #ifdef TRACE_ERROR
		    printf("ERR: data empty \n");
		 #endif
         return FAILURE;
	 }
	 pos = start_num_pos;
     for( place = Power_Of(base, num_chars - 1); place >= 1; place /= base, ++pos )
     {
     	 cur_unit = num_in_str[pos] - '0';
    	 if(cur_unit < 0 ||  cur_unit > 9 )
    	 {
	    	 #ifdef TRACE_ERROR
		       printf("ERR: invalid char at data[%d] = %c \n", pos, num_in_str[pos]);
		     #endif
             return FAILURE;
	     }
         num += (cur_unit * place);
     }
	 if(num_in_str[0] == '-')
	 {
		 *num_conv_from_str = -num;
	 }
	 else
	 {
	     *num_conv_from_str = num;
	 }
	 return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Power_Of

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 02.10

Bugs           :
-*------------------------------------------------------------*/
uint32_t Power_Of(const uint8_t base, const uint8_t power )
{
    uint32_t power_val = 1;
    uint8_t i = 0;

    if(power == 0)
    {
       return power_val;
    }
    for(i = 1; i <= power; ++i)
    {
      power_val *= base;
    }
    return power_val;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Validate_Number_Input

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 01.02

BUGS           :
-*------------------------------------------------------------*/
uint16_t Get_Validate_Input_Number(void *const input_num_ptr, char *const input_str_ptr, const unsigned int input_str_max_chars, const int32_t valid_min_value, const int32_t valid_max_value)
{
	int32_t temp_int, *int32_input_num_ptr;

	if(int32_input_num_ptr == NULL_DATA_PTR)
	{
		return FAILURE;
	}
	if(valid_min_value > valid_max_value)
	{
	   return FAILURE;
	}
	int32_input_num_ptr = (int32_t *)input_num_ptr;
	*int32_input_num_ptr = 0;
	if((Get_Input_Str(input_str_ptr, input_str_max_chars)) != SUCCESS)
		return FAILURE;
	if((Str_to_Num_Conv(&temp_int, input_str_ptr)) != SUCCESS)
	{
		memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
		return FAILURE;
	}
	memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
	if(temp_int < valid_min_value || temp_int > valid_max_value)
	{
		 #ifdef TRACE_ERROR
		      printf("ERR: input data - %d, out of range [%d,%d] \n", temp_int, valid_min_value, valid_max_value);
		 #endif
		 return FAILURE;
	}
	*int32_input_num_ptr = temp_int;
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Get_Input_Str

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 01.02

BUGS           :
-*------------------------------------------------------------*/
uint16_t Get_Input_Str(char *const input_str_ptr, const unsigned int input_str_max_chars)
{
    unsigned int input_str_num_chars = 0;
	char rcvd_char;

    if(input_str_ptr == NULL_DATA_PTR || input_str_max_chars <= 1)
	{
		return FAILURE;
	}
    memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
	while (1)
	{
		rcvd_char = (char) getchar();
		//scanf("%c", &rcvd_char);
		switch(rcvd_char)
        {
			case '\b':
              if(input_str_num_chars > 0)
			  {
			      input_str_ptr[input_str_num_chars] = NULL_CHAR;
				  --input_str_num_chars;
			  }
			break;
            case '\n':
			   if(input_str_num_chars != 0)
			   {
			      input_str_ptr[input_str_num_chars] = NULL_CHAR;
			      return SUCCESS;
			   }
            break;
            default:
     			if(input_str_num_chars + 1 < input_str_max_chars )
    		    {
		    	   input_str_ptr[input_str_num_chars] = rcvd_char;
                   ++input_str_num_chars;
	    		}
		    	else
			    {
			       printf("ERR: Input data num chars exceeds max chars : %u \n", input_str_max_chars - 1);
			       memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
				   fflush(stdin);
				   return FAILURE;
			    }
		}
	}
	return SUCCESS;
}
/*------------------------------------------------------------*
FUNCTION NAME  : Get_Input_Num_Char_Str

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 01.02

BUGS           :
-*------------------------------------------------------------*/
uint16_t Get_Input_Num_Char_Str(char *const input_str_ptr, const unsigned int input_str_req_chars)
{
    unsigned int input_str_num_chars = 0, i =0;
	char rcvd_char;

    if(input_str_ptr == NULL_DATA_PTR || input_str_req_chars <= 1)
	{
		return FAILURE;
	}
    memset(input_str_ptr, NULL_CHAR, input_str_req_chars);
	while (1)
	{
		rcvd_char = (char) getchar();
		//scanf("%c", &rcvd_char);
		switch(rcvd_char)
        {
			case '\b':
              if(input_str_num_chars > 0)
			  {
			      input_str_ptr[input_str_num_chars] = NULL_CHAR;
				  --input_str_num_chars;
			  }
			break;
            case '\n':
			   if(input_str_num_chars != 0)
			   {
			      input_str_ptr[input_str_num_chars] = NULL_CHAR;
				  if(input_str_ptr[0] == '0')
				  {
					  printf("\n ERR: starts with 0 char(s)");
					  memset(input_str_ptr, NULL_CHAR, input_str_req_chars);
					  return FAILURE;
				  }
				  if(input_str_num_chars  != input_str_req_chars - 1)
				  {
					 printf("\n ERR: Input num chars : %u != req chars : %u ", input_str_num_chars, input_str_req_chars - 1 );
					 memset(input_str_ptr, NULL_CHAR, input_str_req_chars);
					 return FAILURE;
				  }
			      return SUCCESS;
			   }
            break;
            default:
     			if(input_str_num_chars + 1 < input_str_req_chars )
    		    {
					if(rcvd_char >= '0' && rcvd_char <= '9')
					{
                       input_str_ptr[input_str_num_chars] = rcvd_char;
                       ++input_str_num_chars;
					}
					else
					{
						  printf("ERR: Input non numeric char \n");
			              memset(input_str_ptr, NULL_CHAR, input_str_req_chars);
				          fflush(stdin);
				          return FAILURE;
					}
	    		}
		    	else
			    {
			       printf("ERR: Input data num chars exceeds req chars : %u\n", input_str_req_chars - 1);
			       memset(input_str_ptr, NULL_CHAR,input_str_req_chars );
				   fflush(stdin);
				   return FAILURE;
			    }
		}
	}
	return SUCCESS;
}

/*------------------------------------------------------------*
FUNCTION NAME  : Get_Input_Num_Char_Str

DESCRIPTION    :

INPUT          :

OUTPUT         :

NOTE           :

Func ID        : 01.02

BUGS           :
-*------------------------------------------------------------*/
uint16_t Get_Input_Alpha_Char_Str(char *const input_str_ptr, const unsigned int input_str_max_chars)
{
    unsigned int input_str_num_chars = 0, i =0;
	char rcvd_char;

    if(input_str_ptr == NULL_DATA_PTR || input_str_max_chars <= 1)
	{
		return FAILURE;
	}
    memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
	while (1)
	{
		rcvd_char = (char) getchar();
		//scanf("%c", &rcvd_char);
		switch(rcvd_char)
        {
			case '\b':
              if(input_str_num_chars > 0)
			  {
			      input_str_ptr[input_str_num_chars] = NULL_CHAR;
				  --input_str_num_chars;
			  }
			break;
            case '\n':
			   if(input_str_num_chars != 0)
			   {
			      input_str_ptr[input_str_num_chars] = NULL_CHAR;
			      return SUCCESS;
			   }
            break;
            default:
     			if(input_str_num_chars + 1 < input_str_max_chars )
    		    {
					if((rcvd_char >= 'A' && rcvd_char <= 'Z') || (rcvd_char >= 'a' && rcvd_char <= 'z') || rcvd_char == ' ')
					{
                       input_str_ptr[input_str_num_chars] = rcvd_char;
                       ++input_str_num_chars;
					}
					else
					{
						  printf("ERR: Input non alphabet char \n");
			              memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
				          fflush(stdin);
				          return FAILURE;
					}
		    	}
		    	else
			    {
			       printf("ERR: Input data num chars exceeds max chars : %u \n", input_str_max_chars - 1);
			       memset(input_str_ptr, NULL_CHAR, input_str_max_chars);
				   fflush(stdin);
				   return FAILURE;
			    }
		}
	}
	return SUCCESS;
}


