#include <sys/types.h>
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <netinet/in.h> 
#include<pthread.h> //for threading , link with lpthread
 
void *connection_handler(void *);
 
int main(int argc , char *argv[])
{
    int socket_desc , new_socket , c , *new_sock;
    struct sockaddr_in server , client;
    char *message;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8207 );

	int enable = 1;
        if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
                puts("Failed to set sock option SO_REUSEADDR");
		return 1;
	}
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }
//    puts("bind done");
//Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
//        puts("Connection accepted");
         
        //Reply to the client
//        message = "Hello Client , I have received your connection. And now I will assign a handler for you\n";
//        write(new_socket , message , strlen(message));
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
//        puts("Handler assigned");
    }
     
    if (new_socket<0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}

char *home_page = "/home/htdoc/index.html";
//char *sjsu_image = "/home/htdoc/sjsu.jpg";
int num_hits =0;
 int num_fail =0;
pthread_mutex_t thread_lock;
int byte_sent =0;

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
	pthread_t thread_id = pthread_self();
    printf("Thread %u \n", thread_id);
	pthread_mutex_lock(&thread_lock);
	num_hits++;
	pthread_mutex_unlock(&thread_lock);
	
	  //Get the socket descriptor
    int sock = *(int*)socket_desc;
     char str[1024];
    char *msg;
	char req [1024];
 
	 if( recv(sock, req , sizeof(req) , 0) < 0)
    {
        puts("recv failed");
    }
    puts("request received\n");
    puts(req);
    char rec_req[1024], rec_type[20], path[100];
	
	strcpy(rec_req, req);
	sscanf(rec_req, "%s %s ", rec_type, path);
	puts(rec_type);
	puts(path);
	char type[20];
	char im_Path[100];
	char *status = "HTTP/1.1 200 OK\r\n";
	char *server = "Server: 207httpd/0.0.1\r\n";
	char *conn = "Connection: close\r\n";
	char *txt = "Content-Type: text/html\r\n";
	char *jpg = "Content-Type: image/jpeg\r\n";
	char *len = "Content-Length: 8000\r\n";
	char *len1 = "Content-Length: 686743\r\n";
	char *gif = "Content-Type: image/gif\r\n";
	char *blank = "\r\n";
	char html_Path[50];
	char image_path[50];
	char httpd_path[50];
	char home_image[50];
	char index_path[50];
	strcpy (index_path , "/index.html");
	strcpy (httpd_path, "/httpd.html");	
	strcpy(type, "GET");
	strcpy (html_Path, "/");
	strcpy (image_path, "/sjsu.jpg");
	strcpy (home_image, "/home.gif");
//	printf("expected typr:  %s \n", type);
//	printf("expected filename /path : %s \n", im_Path); 	
	if (strcmp(rec_type, type)!= 0)
	{	
	msg = "Request Type is Invalid \n";
	pthread_mutex_lock(&thread_lock);
	byte_sent = send(sock , msg , strlen(msg), 0) + byte_sent;
	pthread_mutex_unlock(&thread_lock);
	//printf("%p \n ",msg);
	//puts(msg);
	pthread_mutex_lock(&thread_lock);
        num_fail++;
        pthread_mutex_unlock(&thread_lock);
	printf("Number of invalid requests : %d \n" , num_fail);
	printf("Number of Bytes sent : %d \n" , byte_sent);

	return 0;	
	} 
//	if((strcmp (path, html_Path) || strcmp (path,index_path)) == 0)
	if (strcmp (path,html_Path) == 0)
	{
		char buf[1024];
	//	char *str;
	//	msg = "send file... \n";
		 pthread_mutex_lock(&thread_lock);
        //      byte_sent=send(sock , str , strlen(str), 0)+byte_sent;
                printf("inside response \n");
                byte_sent=write (sock, status, strlen(status))+byte_sent;
                byte_sent=write (sock,server , strlen(server) )+byte_sent;
                byte_sent= write (sock,conn , strlen(conn) )+byte_sent;
                byte_sent=send (sock, txt , strlen(txt),0)+byte_sent;
               // byte_sent=send (sock, jpg , strlen(txt),0)+byte_sent;
		byte_sent=send (sock, len , strlen(len),0 )+byte_sent;
                byte_sent=send (sock, blank, strlen(blank),0)+byte_sent;
		pthread_mutex_unlock(&thread_lock);
        /*        FILE *rf1;
                rf1 = fopen("sjsu.jpg", "r");
                if (rf1 != NULL) {
                 fread(buf, 1, 200, rf1);
                 buf[(sizeof buf)-1] = 0;
                // printf("%s", buf);
//              str ="HTTP/1.1 200 OK\r\n" "Server: 207httpd/0.0.1\r\n" "Connection: close\r\n" "Content-Type: image/jpeg\r\n"  "Content-Length: 5713\r\n""\r\n";       
                pthread_mutex_lock(&thread_lock);
		byte_sent=send(sock , buf , strlen(buf),0)+byte_sent;
                pthread_mutex_unlock(&thread_lock);
		}
		fclose(rf1);*/
		FILE *rf;
		rf = fopen("/home/htdocs/index.html", "r");
		if (rf != NULL) {
       		 fread(buf, 2, 500, rf);
     	  	 buf[(sizeof buf)-1] = 0;
       		// printf("%s", buf);
//		str ="HTTP/1.1 200 OK\r\n" "Server: 207httpd/0.0.1\r\n"	"Connection: close\r\n"	"Content-Type: image/jpeg\r\n"	"Content-Length: 5713\r\n""\r\n";	
		pthread_mutex_lock(&thread_lock);
		byte_sent=send(sock , buf , strlen(buf),0)+byte_sent; 
		pthread_mutex_unlock(&thread_lock);
		fclose(rf);
    		}
	}
	else  if (strcmp (path,index_path) == 0)
        {
                char buf[2048];
        //      char *str;
        //      msg = "send file... \n";
                 pthread_mutex_lock(&thread_lock);
        //      byte_sent=send(sock , str , strlen(str), 0)+byte_sent;
                printf("inside response \n");
                byte_sent=write (sock, status, strlen(status))+byte_sent;
                byte_sent=write (sock,server , strlen(server) )+byte_sent;
                byte_sent= write (sock,conn , strlen(conn) )+byte_sent;
                byte_sent=send (sock, txt , strlen(txt),0)+byte_sent;
               // byte_sent=send (sock, jpg , strlen(txt),0)+byte_sent;
                byte_sent=send (sock, len , strlen(len),0 )+byte_sent;
                byte_sent=send (sock, blank, strlen(blank),0)+byte_sent;
                pthread_mutex_unlock(&thread_lock);
                FILE *rf;
                rf = fopen("/home/htdocs/index.html", "r");
                if (rf != NULL) {
                 fread(buf, 2, 500, rf);
                 buf[(sizeof buf)-1] = 0;
                // printf("%s", buf);
//              str ="HTTP/1.1 200 OK\r\n" "Server: 207httpd/0.0.1\r\n" "Connection: close\r\n" "Content-Type: image/jpeg\r\n"  "Content-Length: 5713\r\n""\r\n";       
                pthread_mutex_lock(&thread_lock);
                byte_sent=send(sock , buf , strlen(buf),0)+byte_sent;
                pthread_mutex_unlock(&thread_lock);
                fclose(rf);
                }
        }

	else if (strcmp (path, image_path) == 0)
        {
                char buf[10240];
		printf("inside image condition \n");
        //      char *str;
        //      msg = "send file... \n";
                 pthread_mutex_lock(&thread_lock);
        //      byte_sent=send(sock , str , strlen(str), 0)+byte_sent;
                printf("inside response \n");
                byte_sent=write (sock, status, strlen(status))+byte_sent;
                byte_sent=write (sock,server , strlen(server) )+byte_sent;
                byte_sent= write (sock,conn , strlen(conn) )+byte_sent;
                byte_sent=write (sock, jpg , strlen(jpg))+byte_sent;
                byte_sent=send (sock, len1 , strlen(len1),0 )+byte_sent;
                byte_sent=send (sock, blank, strlen(blank),0)+byte_sent;

                pthread_mutex_unlock(&thread_lock);
                FILE *rf;
		
		if ( (rf = fopen("/home/htdocs/sjsu.jpg", "rb")) == NULL)
                        printf("error in opening image \n");

                fseek (rf, 0, SEEK_END);
       long fileLength ;
                fileLength = ftell(rf);
        rewind(rf);
         char *sendbuf;
        sendbuf = (char*) malloc (sizeof(char)*fileLength);
        size_t result = fread(sendbuf, 1, fileLength, rf);

                if(result>0){

                // buf[0] = '\0';

        //      uint32_t x = htonl(buf);
                // printf("%s", buf);
//              str ="HTTP/1.1 200 OK\r\n" "Server: 207httpd/0.0.1\r\n" "Connection: close\r\n" "Content-Type: image/jpeg\r\n"  "Content-Length: 5713\r\n""\r\n";       
                pthread_mutex_lock(&thread_lock);
                byte_sent=send(sock ,sendbuf , fileLength,0)+byte_sent;
                pthread_mutex_unlock(&thread_lock);
                buf[0] = '\0';
                }  
        fclose(rf);
	
	}

         
	else if (strcmp (path, httpd_path) == 0)
        {
                char buf[2048];
		puts(path);
        //      char *str;
        //      msg = "send file... \n";
                 pthread_mutex_lock(&thread_lock);
        //      byte_sent=send(sock , str , strlen(str), 0)+byte_sent;
                printf("inside response \n");
                byte_sent=write (sock, status, strlen(status))+byte_sent;
                byte_sent=write (sock,server , strlen(server) )+byte_sent;
                byte_sent= write (sock,conn , strlen(conn) )+byte_sent;
                byte_sent=send (sock, txt , strlen(txt),0)+byte_sent;
                byte_sent=send (sock, len , strlen(len),0 )+byte_sent;
                byte_sent=send (sock, blank, strlen(blank),0)+byte_sent;

                pthread_mutex_unlock(&thread_lock);
                FILE *rf;
                rf = fopen("/home/htdocs/httpd.html", "r");
                if (rf != NULL) {
                 fread(buf, 5, 1000, rf);
                 buf[(sizeof buf)-1] = 0;
                // printf("%s", buf);
//              str ="HTTP/1.1 200 OK\r\n" "Server: 207httpd/0.0.1\r\n" "Connection: close\r\n" "Content-Type: image/jpeg\r\n"  "Content-Length: 5713\r\n""\r\n";       
                pthread_mutex_lock(&thread_lock);
                byte_sent=send(sock , buf , strlen(buf),0)+byte_sent;
                pthread_mutex_unlock(&thread_lock);
                fclose(rf);
                }

        }
	else if (strcmp (path, home_image) == 0)
        {
                char buf[10240];
                printf("inside image condition \n");
        //      char *str;
        //      msg = "send file... \n";
                 pthread_mutex_lock(&thread_lock);
        //      byte_sent=send(sock , str , strlen(str), 0)+byte_sent;
                printf("inside response \n");
                byte_sent=write (sock, status, strlen(status))+byte_sent;
                byte_sent=write (sock,server , strlen(server) )+byte_sent;
                byte_sent= write (sock,conn , strlen(conn) )+byte_sent;
                byte_sent=write (sock, gif , strlen(gif))+byte_sent;
                byte_sent=send (sock, len1 , strlen(len1),0 )+byte_sent;
                byte_sent=send (sock, blank, strlen(blank),0)+byte_sent;

                pthread_mutex_unlock(&thread_lock);
                FILE *rf;
                
                if ( (rf = fopen("/home/htdocs/home.gif", "rb")) == NULL)
			printf("error in opening image \n");
               
		fseek (rf, 0, SEEK_END);
       long fileLength ;
		fileLength = ftell(rf);
        rewind(rf);
         char *sendbuf;
        sendbuf = (char*) malloc (sizeof(char)*fileLength);
        size_t result = fread(sendbuf, 1, fileLength, rf);
 
                if(result>0){

                // buf[0] = '\0';

        //      uint32_t x = htonl(buf);
                // printf("%s", buf);
//              str ="HTTP/1.1 200 OK\r\n" "Server: 207httpd/0.0.1\r\n" "Connection: close\r\n" "Content-Type: image/jpeg\r\n"  "Content-Length: 5713\r\n""\r\n";       
                pthread_mutex_lock(&thread_lock);
                byte_sent=send(sock ,sendbuf , fileLength,0)+byte_sent;
                pthread_mutex_unlock(&thread_lock);
		buf[0] = '\0';
                }
	fclose(rf);
        }



	else {
		msg = "Invalid path or Filename- \n";
	pthread_mutex_lock(&thread_lock);
	 byte_sent=send(sock , msg , strlen(msg),0)+byte_sent;
	 pthread_mutex_unlock(&thread_lock);
	printf("%p \n ", msg);
	pthread_mutex_lock(&thread_lock);
        num_fail++;
        pthread_mutex_unlock(&thread_lock);
	printf("Number of invalid requests : %d \n" , num_fail);
	printf("Number of Bytes sent : %d \n" , byte_sent);
	return 0;
	}
	
	printf("Number of requests : %d \n" , num_hits);
	printf("Number of invalid requests : %d \n" , num_fail);
	printf("Number of Bytes sent : %d \n" , byte_sent);	
    free(socket_desc);
    sleep(1);
    pthread_exit(NULL);
}
