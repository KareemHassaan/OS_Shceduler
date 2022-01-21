/*scheduling code
 -This code creidited to 
                         kareem hassaan mohamed
                         ID : 204
						 

 - code functionality is reading the processes information from the file entered by the user.
 - scheduling this processes with FCFS or RR algorithm according to the user choice.
 - calculating and printing the finishing time , turn around time and CPU utilization.
 -printing in each time the process which in running state and the processes which in the ready and block lists. */

/*----------------------------------------------------------------************************************************************************--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#define Null 0

FILE *Input ;                                                     	//pointer to a file.

typedef struct processes											//General structure for a single process
{
  int pro_ID;
  int pro_cpu_Time;
  int pro_IO_Time;
  int pro_Arival_Time;
  int pro_IO_delay;
  int pro_State;

  struct processes * Next;
}process;

//pointers to structure struct processes
process processing ;												
process * QR_FRONT ;												
process * QR_REAR  ;
process * QB_FRONT ;
process * QB_REAR  ;
process * Q_Temp   ;
process * PRO_Q_Temp;
process * B_Temp   ;

//Global variables initilization
int u8_sch_Alg , u8_Quantum ,File_Buffer ,Timer,Arrive,a,b;
char File_Name[30] ;
int  IN_pro_arr[5][4];
int  TERM_arr[5];
int Row = 5;
int i = 0;
int j = 0;
int CPU = 0;
int Cpu_OFF = 0;
int Run_Flag ;
int Block_Flag = 2;
int Pro_Num = 0;
int Run = 1;
float CPU_UTILIES =0;

//Functions prototype.
void Ready_Enqueue(int pro_ID_cpy , int pro_cpu_Time_cpy , int pro_IO_Time_cpy , int pro_Arival_Time_cpy , int pro_IO_delay_cpy , int pro_State_cpy );
/*Ready_Enqueue function prototype for enqueuing processes in the Ready queue takes arguments process(ID,CPU time,I/O time,Arrive time ) and return void*/
void Block_Enqueue(int pro_ID_cpy , int pro_cpu_Time_cpy , int pro_IO_Time_cpy , int pro_Arival_Time_cpy , int pro_IO_delay_cpy , int pro_State_cpy ); 
/*Block_Enqueue function prototype for enqueuing processes in the Block queue takes arguments process(ID,CPU time,I/O time,Arrive time ) and return void*/
struct processes * Ready_Dequeue();
/*Ready_Dequeue function prototype for _Dequeuing processes from the Ready queue takes (void) and return a pointer to structure to the _Dequeued process*/
int Ready_count();
/*Ready_count function prototype to count the number of processes in the Ready queue it takes (void) and return int (the number of processes) */
int Block_count();
/*Block_count function prototype to count the number of processes in the Block queue it takes (void) and return int (the number of processes) */
struct processes * Block_Dequeue(struct processes * DQ_process);
/*Block_Dequeue function prototype for _Dequeuing processes from the Block queue takes ( pointer to structure to the process needed to _Dequeue) and return a pointer to structure to the _Dequeued process*/
void Block_queue_maint();
/*Block_queue_maint function prototype that changed the process state from bloked to Ready when its IO blocking time end */
void print_Ready();
/*print_Ready  function prototype to print the processes that exist in the Ready queue*/
void print_Block();
/*print_Block  function prototype to print the processes that exist in the Block queue*/
void Turnaround ();
/*Turnaround function prototype to calculate the turnaround time for each process*/

int main(void)
{
	PRO_Q_Temp = 0;	
	  QR_FRONT = 0;
      QR_REAR  = 0;
	  QB_FRONT = 0;
	  QB_REAR  = 0;
	  Q_Temp   = 0;
	  B_Temp   = 0;
	 Run_Flag = 2;

	printf("Please enter the file name \n");      										//scan from the user the name of the file containg the processes information.
    scanf("%s",&File_Name);
    Input = fopen(File_Name ,"r");														//opening file in read mode.
		
		while(fscanf(Input , "%d" , &File_Buffer) != EOF )								//scaning processes information from the file
		{
				IN_pro_arr[i][j] = File_Buffer;      
				j++;
				
				if (4 == j)
				{		
					i++;
					j = 0;
				}
		}

									printf("processes Array we have:\n"); 												//printing the processes Array we have.
		
										for(a=0;a<5;a++)
										{
											for(b=0;b<4;b++)
											{
													printf(" %d \t",IN_pro_arr[a][b]);
											}
													printf("\n");
										}
    
	printf("Please enter the shceduling Algorithm you need \n (0) -- > FIFO \n (1)  -- > RR \n");    	//scaning the scheduling Algorithm needed from the user.
    scanf("%d",&u8_sch_Alg);																		
	
    switch (u8_sch_Alg)                                                                				 	//switch case to enter the selected scheduling Algorithm.
    {
	
	case 0 :																					 	    //Case 0 : First come first served scheduling Algorithm (FCFS).
					printf("Case 0 : (FCFS) \n");

	while(Run == 1)
	{ 
					printf("Time %d: ",Timer);
					
					for(Arrive =0 ;Arrive < i;Arrive++)													//for loop to check for upcoming processes										
					{ 
							if(IN_pro_arr[Arrive][3] == Timer)											//The processor busy with a process and upcoming process enqueued in the ready queue.
							{
								if(Run_Flag == 1)
								{ 
									Ready_Enqueue(IN_pro_arr[Arrive][0],IN_pro_arr[Arrive][1],IN_pro_arr[Arrive][2],IN_pro_arr[Arrive][3],IN_pro_arr[Arrive][1],0);  	//Ready_Enqueue function call 
				
								}
               
							else if(Run_Flag == 2)  													//The processor idle and upcoming process entered the running state
							{ 
                 
								processing . pro_ID          = IN_pro_arr[Arrive][0];
								processing . pro_cpu_Time    = IN_pro_arr[Arrive][1];
								processing . pro_IO_Time     = IN_pro_arr[Arrive][2];
								processing . pro_Arival_Time = IN_pro_arr[Arrive][3];
								processing . pro_IO_delay    = IN_pro_arr[Arrive][1];
								processing . Next            = 0;
								Q_Temp = &processing;
								Run_Flag = 1;
				 
               
				
							}
							}
					}
	
	
		if(Q_Temp != 0)																					//If there is a process in the running state.
		{  
				printf("%d:RUNNING  \t",Q_Temp -> pro_ID);
				PRO_Q_Temp = Q_Temp;
	
				if(CPU != (PRO_Q_Temp -> pro_cpu_Time))
				{
					CPU++;  
				}
   
				else if(CPU == (PRO_Q_Temp -> pro_cpu_Time))
				{ 
				
						if(PRO_Q_Temp -> pro_IO_delay != 0)
						{
			
								Block_Enqueue(PRO_Q_Temp -> pro_ID,PRO_Q_Temp -> pro_cpu_Time,PRO_Q_Temp -> pro_IO_Time,PRO_Q_Temp -> pro_Arival_Time,PRO_Q_Temp -> pro_IO_delay,PRO_Q_Temp -> pro_State);
								Q_Temp = 0;																																	//Block_Enqueue function call
						}
						else  if(PRO_Q_Temp -> pro_IO_delay == 0)
						{   
								TERM_arr[PRO_Q_Temp -> pro_ID] = Timer;																//Array to save the termination time of each process.
								Pro_Num++;		   
								PRO_Q_Temp -> pro_State = 1;
								Q_Temp = 0;
						}
			
								Q_Temp = Ready_Dequeue();																			//Ready_Dequeue function call.
								Block_queue_maint();																				//Block_queue_maint function call.
								CPU = 0;
			}
	}
		
		else if(Q_Temp == 0)																										//If the processor idle and there is no process in the running state 
		{
				Block_queue_maint();
				Run_Flag = 2;
				printf("cpu off");
				Cpu_OFF++;
		}
	
	
	print_Ready();                                                    															  //print_Ready function call
    print_Block();                                                     															  //print_Block function call
	printf("\n");

		if(Pro_Num == Row)																											//All processes are terminated
		{  
				Run = 0;																											//Run flag == 0 to get out from the while loop.
				printf("Finish time = %d \n",Timer);																				//printing the finishing time
				CPU_UTILIES = 1-(Cpu_OFF/Timer);																					//calculating CPU_UTILIES.
				printf("CPU_UTILIES = %f \n",CPU_UTILIES);		   																	//printing CPU_UTILIES.
		 }
	
		Timer++;																													//increment Timer (Global variable as a timer).
   	}
	
	Turnaround ();																													//calculating and printing the Turnaround of esch process.
						   																	

																		break;

/*----------------------------------------------------------------********************************RR*************************************------------------------------------------------------------------------------*/																		
    case 1 :
              printf("Please enter the value of the quantum you need in RR (must be postive intger number)\n");						//scan the value of the quantum of RR from user.
              scanf("%d",&u8_Quantum);

                if(u8_Quantum <= 0)																									//check that the quantum value is  postive intger number.
                    {
                       printf("sorry the value of the quantum you entered is not valid\n");
																		break;
                    }
                else
                    {				
	
while(Run == 1)
    {
		printf("Time %d: ",Timer);
        for(Arrive =0 ;Arrive < i;Arrive++)																							//for loop to check for upcoming processes
        { 
            if(IN_pro_arr[Arrive][3] == Timer)
            {
                if(Run_Flag == 1)																									//The processor busy with a process and upcoming process enqueued in the ready queue.
               { 
						Ready_Enqueue(IN_pro_arr[Arrive][0],IN_pro_arr[Arrive][1],IN_pro_arr[Arrive][2],IN_pro_arr[Arrive][3],IN_pro_arr[Arrive][1],0);		
																																	//Ready_Enqueue function call.
			   }
               
			   else if(Run_Flag == 2)																								//The processor idle and upcoming process entered the running state.
               { 
                 
					processing . pro_ID          = IN_pro_arr[Arrive][0];
					processing . pro_cpu_Time    = IN_pro_arr[Arrive][1];
					processing . pro_IO_Time     = IN_pro_arr[Arrive][2];
					processing . pro_Arival_Time = IN_pro_arr[Arrive][3];
					processing . pro_IO_delay    = IN_pro_arr[Arrive][1];
					processing . Next            = 0;
					Q_Temp = &processing;
					Run_Flag = 1;			
               }
            }
        }
	
	
	if(Q_Temp != 0)																													//If there is a process in the running state.
	{  
		printf("%d:RUNNING  \t",Q_Temp -> pro_ID);
	    PRO_Q_Temp = Q_Temp;
		
			if(PRO_Q_Temp -> pro_cpu_Time > u8_Quantum)																				//CPU time bigger than the quantum.
			{
					if(CPU != u8_Quantum)
					{
						CPU++;
					}
				
					else if	(CPU == u8_Quantum)
					{			
						Ready_Enqueue(PRO_Q_Temp -> pro_ID,PRO_Q_Temp -> pro_cpu_Time,PRO_Q_Temp -> pro_IO_Time,PRO_Q_Temp -> pro_Arival_Time,PRO_Q_Temp -> pro_IO_delay,PRO_Q_Temp -> pro_State);
							
						Q_Temp = Ready_Dequeue();																					//Ready_Enqueue function call.
						CPU = 0;
					}
		
	
			}
			else if(PRO_Q_Temp -> pro_cpu_Time <= u8_Quantum)																		//CPU time smaller than or equal the quantum.
			{
				
				if(CPU != u8_Quantum)
				{
					
					if(CPU == (PRO_Q_Temp -> pro_cpu_Time))
					{
								if(PRO_Q_Temp -> pro_IO_delay != 0)
								{	
			
									Block_Enqueue(PRO_Q_Temp -> pro_ID,PRO_Q_Temp -> pro_cpu_Time,PRO_Q_Temp -> pro_IO_Time,PRO_Q_Temp -> pro_Arival_Time,PRO_Q_Temp -> pro_IO_delay,PRO_Q_Temp -> pro_State);
									
								}
								else  if(PRO_Q_Temp -> pro_IO_delay == 0)
								{   
									TERM_arr[PRO_Q_Temp -> pro_ID] = Timer;															//Array to save the termination time of each process.
									Pro_Num++;		   
									PRO_Q_Temp -> pro_State = 1;
									
								}
					}
				}
									CPU++;
			}
				
				else if(CPU == u8_Quantum)
				{
					Q_Temp = Ready_Dequeue();																						//Ready_Dequeue function call.
					CPU = 0;
				} 
	     
		Block_queue_maint();
	}

		
	else if(Q_Temp == 0)																											//If the processor idle and there is no process in the running state 
	{
		Block_queue_maint();																										//Block_queue_maint function call.
	    Run_Flag = 2;
		printf("cpu off");
	    Cpu_OFF++;
	}
	
	
	print_Ready();																														//print_Ready function call
	print_Block();																														//print_Block function call
	printf("\n");																														

	if(Pro_Num == Row)
    {
		Run = 0;																														//Run flag == 0 to get out from the while loop.
		printf("Finish time = %d \n",Timer);																							//printing the finishing time
	    CPU_UTILIES = 1-(Cpu_OFF/Timer);																								//calculating CPU_UTILIES.
		printf("CPU_UTILIES = %f \n",CPU_UTILIES);		   																				//printing CPU_UTILIES.
	}
		Timer++;																														//increment Timer (Global variable as a timer).
   	}
	
		Turnaround ();																													//calculating and printing the Turnaround of esch process.
						break;

     default :
              printf("sorry the number you entered is not valid\n");
						break;
	}
}
}

void Ready_Enqueue( int pro_ID_cpy ,int pro_cpu_Time_cpy,int pro_IO_Time_cpy,int pro_Arival_Time_cpy,int pro_IO_delay_cpy,int pro_State_cpy)
/*Ready_Enqueue function implementation by linked list for enqueuing processes in the Ready queue takes arguments process(ID,CPU time,I/O time,Arrive time ) and return void*/
{
	process * temp_pro = (process*)malloc(sizeof(process));						//Dynamic memory allocation

	temp_pro -> pro_ID          = pro_ID_cpy;
	temp_pro -> pro_cpu_Time    = pro_cpu_Time_cpy;
	temp_pro -> pro_IO_Time     = pro_IO_Time_cpy;
	temp_pro -> pro_Arival_Time = pro_Arival_Time_cpy;
	temp_pro -> pro_IO_delay    = pro_IO_delay_cpy;
	temp_pro -> pro_State       = pro_State_cpy;
	temp_pro -> Next            = 0;
 
	if(QR_FRONT == 0 && QR_REAR == 0)											//condition of one process in the queue(FRONT == REAR) Pointers.
	{
		QR_FRONT=temp_pro;
		QR_REAR =temp_pro;
	 
		return;
	}

		QR_REAR -> Next = temp_pro;
		QR_REAR         = temp_pro;
	   
}
void Block_Enqueue( int pro_ID_cpy ,int pro_cpu_Time_cpy,int pro_IO_Time_cpy,int pro_Arival_Time_cpy,int pro_IO_delay_cpy,int pro_State_cpy)
/*Block_Enqueue function implementation by linked list for enqueuing processes in the Block queue takes arguments process(ID,CPU time,I/O time,Arrive time ) and return void*/
{
	process * temp_pro = (process*)malloc(sizeof(process));							//Dynamic memory allocation

	temp_pro -> pro_ID          = pro_ID_cpy;
	temp_pro -> pro_cpu_Time    = pro_cpu_Time_cpy;
	temp_pro -> pro_IO_Time     = pro_IO_Time_cpy;
	temp_pro -> pro_Arival_Time = pro_Arival_Time_cpy;
	temp_pro -> pro_IO_delay    = pro_IO_delay_cpy;
	temp_pro -> pro_State       = pro_State_cpy;
	temp_pro -> Next            = 0;

	if(QB_FRONT == 0 && QB_REAR == 0)												//condition of one process in the queue(FRONT == REAR) Pointers.
	{
		QB_FRONT=temp_pro;
		QB_REAR =temp_pro;
	  
		return;
	}

		QB_REAR -> Next = temp_pro;
		QB_REAR         = temp_pro;
	   
	   
}
struct processes * Ready_Dequeue()
/*Ready_Dequeue function implementation by linked list for _Dequeuing processes from the Ready queue takes (void) and return a pointer to structure to the _Dequeued process*/
{ 
	process * temp_pro = (process *) malloc(sizeof(process *));									//Dynamic memory allocation
  

		if(QR_FRONT==0)																			//There is no elements in the queue
		{
				return 0;
		}
		
		else if (QR_FRONT == QR_REAR)															//There is only one elements in the queue										
		{
				temp_pro = QR_FRONT;
				QR_FRONT = 0;
				QR_REAR  = 0;
				return temp_pro;
		}
		
		else
		{
				temp_pro = QR_FRONT;
				QR_FRONT =  QR_FRONT ->  Next;
				temp_pro ->   Next =   0;
				return temp_pro;
		}
	
	return temp_pro;
}
int Ready_count()
/*Ready_count function implementation to count the number of processes in the Ready queue it takes (void) and return int (the number of processes) */
{
	int counter = 0;
	process * pro_count = (process *) malloc(sizeof(process));												//Dynamic memory allocation
	pro_count -> Next = QR_FRONT;
   
		while( pro_count -> Next  != 0)																		//counting the element in the ready queue.
		{
				pro_count = pro_count -> Next;
				counter++;
		}
	return counter;
}
int Block_count()
/*Block_count function implementation to count the number of processes in the Block queue it takes (void) and return int (the number of processes) */
{
	int counter = 0;
	process * pro_count = (process *) malloc(sizeof(process));													//Dynamic memory allocation
	pro_count -> Next = QB_FRONT;
   
		while( pro_count -> Next  != 0)																			//counting the element in the Block queue.
		{
				pro_count = pro_count -> Next;
				counter++;
		}
	return counter;
}
struct processes * Block_Dequeue(struct processes * DQ_process)
/*Block_Dequeue function mplementation by linked list  for _Dequeuing processes from the Block queue takes ( pointer to structure to the process needed to _Dequeue) and return a pointer to structure to the _Dequeued process*/
{
	process * temp_pro = (process *) malloc(sizeof(process));											//Dynamic memory allocation

		if(QB_FRONT == 0 && QB_REAR == 0)																//There is no element in the queue.
		{
				return 0;
		}

		else if(QB_FRONT == DQ_process)																	//The element needed is the first element in the queue.
		{
				temp_pro = DQ_process;
				QB_FRONT  = DQ_process -> Next;
				return temp_pro;
		}
          
		 else if(QR_REAR == DQ_process)																	//The element needed is the Last element in the queue.
        {
				process * temp = (process *) malloc(sizeof(process));
				temp -> Next  = QB_FRONT;
      
					while(temp -> Next != DQ_process)
					{
							temp = temp -> Next ;
					}
            
				temp_pro = DQ_process;
				temp -> Next = 0;
				QR_REAR = temp;
             
            return temp_pro;
        }

		else																							//The element needed is a Random element in the queue.
		{
				process * temp = (process *) malloc(sizeof(process));									//Dynamic memory allocation
				temp -> Next  = QB_FRONT;
		
					while(temp -> Next != DQ_process)
					{
							temp = temp -> Next ;
					}
				
				temp_pro = DQ_process;
				temp -> Next = DQ_process -> Next;
		
			return temp_pro;
		}
	return temp_pro;
}
void Block_queue_maint()
/*Block_queue_maint function mplementation that changed the process state from bloked to Ready when its IO blocking time end */
{
		process * B_Temp = (process *) malloc(sizeof(process));
		B_Temp = QB_FRONT;

			if(B_Temp == 0)																			// Empty list
			{
					return;
			}

			else
			{
					while(B_Temp != 0)
					{
							if( B_Temp -> pro_IO_delay == 0)										//process finshed its Blocking time go to from blocked list to the ready queue.
							{
									Ready_Enqueue(B_Temp -> pro_ID,B_Temp -> pro_cpu_Time,B_Temp -> pro_IO_Time,B_Temp -> pro_Arival_Time,B_Temp -> pro_IO_delay,B_Temp -> pro_State);
									Block_Dequeue(B_Temp);
	 
	 
							}
							else
							{
									(B_Temp -> pro_IO_delay)--;
   
							}
									B_Temp = B_Temp -> Next;
					}
			}
}
void print_Ready()
/*print_Ready  function mplementation to print the processes that exist in the Ready queue*/
{
		process * QP_Temp = (process *) malloc(sizeof(process));
		QP_Temp  = QR_FRONT;

			if(QP_Temp == 0)
			{ 
					return;	 
            }   

					while(QP_Temp != 0)
					{
							printf("%d:READY \t",(QP_Temp -> pro_ID));
							QP_Temp = QP_Temp -> Next;
					}
	}
void print_Block()
/*print_Block  function mplementation to print the processes that exist in the Block queue*/
{
		process * BP_Temp = (process *) malloc(sizeof(process));
		BP_Temp = QB_FRONT;
	
			if(BP_Temp == 0)
			{ 
					return;	 
            }  
						while(BP_Temp != 0)
						{
								printf("%d:BLOCKED \t",(BP_Temp -> pro_ID));
								BP_Temp = BP_Temp -> Next;
						}
}
void Turnaround ()
/*Turnaround function mplementation to calculate the turnaround time for each process*/
{
	int r,Turn_around; 
	
		for(r=0;r<Row;r++)
		{
				Turn_around= TERM_arr[IN_pro_arr[r][0]] - (IN_pro_arr[r][3]) + 1 ;
				printf("Turn around time of process ID (%d) = %d \n",IN_pro_arr[r][0] ,Turn_around);
		}
}