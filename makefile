all:

gcc hw2-1.c -o hw2-1
gcc tcp-con-daytimeClient.c -o tcp-con-daytimeClient
gcc -pthread queue_work.c -o queue_work
gcc -pthread webserver-hw2.c -o webserver-hw2


clean:

rm -r hw2-1
rm -r tcp-con-daytimeClient
rm -r queue_work
rm -r webserver-hw2
 
