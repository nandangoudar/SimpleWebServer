/* TCPdaytimed.c - main */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "passiveTCP.h"
#include "errexit.h"
#include <time.h>

int		errexit(const char *format, ...);
void		TCPdaytimed(int fd);
int		passiveTCP(const char *service, int qlen);

#define QLEN	32

/*------------------------------------------------------------------------
 * main - Iterative TCP server for DAYTIME service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	struct	sockaddr_in fsin;	/* the from address of a client	*/
	char	*service = "daytime";	/* service name or port number	*/
	int	msock, ssock;		/* master & slave sockets	*/
	unsigned int	alen;		/* from-address length		*/

	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		errexit("usage: TCPdaytimed [port]\n");
	}

	msock = passiveTCP(service, QLEN);
	int fid;
	while (1) {
		alen = sizeof(fsin);
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
		if (ssock < 0)
			errexit("accept failed: %s\n", strerror(errno));
		else {
		if (fid = fork() == 0 ){

			TCPdaytimed(ssock);
			(void) close(ssock);
			}
		}
	}
}

/*------------------------------------------------------------------------
 * TCPdaytimed - do TCP DAYTIME protocol
 *------------------------------------------------------------------------
 */
void	TCPdaytimed(int fd)
{

//	char	*pts;			/* pointer to time string	*/
//	time_t now;			/* current time			*/
//	char	*ctime();
      	
//	(void) time(&now);
//	pts = ctime(&now);

	time_t rawtime;
   struct tm *info;
   char buffer[80];

   time( &rawtime );

   info = localtime( &rawtime );
	char *format = "%m/%d/%Y %X %a";
	
    strftime( buffer, 80 , format, info);
   printf("Formatted date & time : %s \n", buffer );
  
	//int wday = tm_wday;

//	printf ("%d \n ", ); 
	 send(fd, buffer, 80, 0);
}
