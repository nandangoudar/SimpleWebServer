#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include <string.h>
#define max 5

char    q[max][1024];
char data[1024];                                                // queue to hold produced numbers
int     first  = 0;                                     // index into q of next free space
int last   = 0;                                 // index into q of next available number
int     count = 0;                                      // number of items in the queue
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;      // mutual exclusion lock
pthread_mutex_t empty = PTHREAD_MUTEX_INITIALIZER;      // synchronization lock

/* insertQ is called by the producer threads to put a number on the queue */
void insertQ(char x[], int *first) {
        //char getdata[1024];

        pthread_mutex_lock(&mutex);             // lock access to the queue
        strcpy(q[*first], x); 
	*first++;                                     // put item on the queue
       // first = (first+1) %5;
        count++;            
//	printf("count in insert = %d \n",count);                                     // increment queue size
        pthread_mutex_unlock(&mutex);   // unlock queue
        pthread_mutex_unlock(&empty);   // start a waiting consumer
}

/* dequeue is called by consumer threads to retrieve a number
   from the queue.  The thread will be suspended if there is
   nothing on the queue. */
void dequeue( int *last) {
        char getdata[1024];                                             // item removed from the queue
        /* wait if the queue is empty. */
        while (count == 0) pthread_mutex_lock(&empty);
    pthread_mutex_lock(&mutex);         // lock access to the queue
    strcpy(getdata,q[*last]);                            // get item from the queue
    *last++;
	//last = (last+1) % 5;
  	printf("count in dqueue = %d \n",count) ;
	  count--;
//	printf("count = %d",count) ;                                           // decrement queue size
    pthread_mutex_unlock(&mutex);       // unlock queue
	 printf ("string dequeued from queue is : %s \n" , getdata);
	conv(strlen(getdata));
}

/*  Producer thread. */
void * prod(void *arg) {
        int input;
        while(1){
/*        printf("enter the option 1 or 2 \n");
        printf ("1. to enter the string in queue \n");
        printf ("2. exit\n");
        scanf ("%d", &input);
        
        
        if (input == 1)
	{*/
                printf("********Enter the string to insert in queue******* \n");
               scanf ("%s", data);
		// fgets(data,sizeof(data),stdin);
                insertQ(data, &first);
                sleep(1);
      /* }
	 else
                exit(1);

*/
        }
 }


/*  Consumer thread  */
void * con(void *arg) {
     //  printf("in comsumer thread \n");
        while(1)
        {
               // if (first !=0)
               // {
               dequeue(&last);    
                       // printf(" in consumer %s \n", read);
        //        }
		
        }
 }
//function to count length and print string
void printstr(char x[])
{
	printf ("string dequeued from queue is %s \n" , x);
        printf("number of words in string %zd \n", strlen(x));

}

void main()
{
 pthread_t threadp1;				// Thread objects
 pthread_t threadc1;
 int zero = 0;  int one = 1;

 pthread_create(&threadp1, NULL, prod, &zero);	// start first producer thread
 pthread_create(&threadc1, NULL, con,  NULL);	// start consumer thread
 
pthread_exit(NULL);							// main thread quits
}

void conv(int number)
{

        printf("string length in digits: %d \n", number);
	printf("string length in digits: ");
        int x = number;

        int len =0;
        do
        {
                x=x/10;
                len++;
        }while (x!=0);
        char *num ;
        int unit=0;
        int tens=0;
        int hun=0;
        unit = number%10;
        tens = ((number/10)%10);
        hun = (number/100)%10;
    /* Base cases */
    if (len == 0) {
        fprintf(stderr, "empty string\n");
        return;
    }
    if (len > 3) {
        fprintf(stderr, "Length more than 4 is not supported\n");
        return;
    }



 /* The first string is not used, it is to make array indexing simple */
    char *single_digits[] = { "zero", "one", "two", "three", "four",
                              "five", "six", "seven", "eight", "nine"};

    /* The first string is not used, it is to make array indexing simple */
    char *two_digits[] = {"", "ten", "eleven", "twelve", "thirteen", "fourteen",
                     "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};

    /* The first two string are not used, they are to make array indexing simple*/
    char *tens_multiple[] = {"", "", "twenty", "thirty", "forty", "fifty",
                             "sixty", "seventy", "eighty", "ninety"};

        char *tens_power[] = {"hundred"};

    /* Used for debugging purpose only */
   // printf("\n%s: ", num);

    /* For single digit number */
    if (len == 1) {
        printf("%s\n", single_digits[unit-0]);
        return;
    }
                if (tens ==1){
        printf("%s \n", two_digits[unit+1]);
        return;
        }
	 if (hun==0)
        {
                if (unit !=0){
                printf("%s ", tens_multiple[tens]);
                printf("%s\n", single_digits[unit-0]);
                }
                else
                        printf("%s \n", tens_multiple[tens]);
        }
        else
        {       if (unit != 0 && tens !=0 ) 
		{
                printf("%s ", single_digits[hun]);
                printf("%s " , tens_power[0]);
                printf("%s ", tens_multiple[tens]);
                printf("%s\n", single_digits[unit-0]);	
		}
                else if (unit == 0 && tens !=0)
		{

                         printf("%s ", single_digits[hun]);
                printf("%s " , tens_power[0]);
                printf("%s \n", tens_multiple[tens]);
                }
                else {   
                         printf("%s ", single_digits[hun]);
                printf("%s " , tens_power[0]);
                printf("%s\n", single_digits[unit-0]);
                }
        }

	printf("\n\n");

}

