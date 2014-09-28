/* daytimed.c - main */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h> 

#include "error.h"
#include "socket_utils.h"


#define	QLEN		 32

#define	LINELEN		128

/*------------------------------------------------------------------------
 * main - Iterative server for DAYTIME service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*service = "echo";	/* service name or port number	*/
	char	buf[LINELEN];		/* buffer for one line of text	*/
	struct sockaddr_in fsin;	/* the request from address	*/
	unsigned int	alen;		/* from-address length		*/
	int	tsock; 			/* TCP master socket		*/
	int 	auxsock,n; 
	int	usock;			/* UDP socket			*/
	int	nfds, nrec;
	fd_set	rfds;			/* readable file descriptors	*/
	fd_set  afds;			/* descriptores activos         */
	int	ssock;			/* TCP slave socket	*/

	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		errexit("usage: daytimed [port]\n");
	}

	tsock = passiveTCP(service, QLEN);
	usock = passiveUDP(service);

	nfds=getdtablesize();

	FD_ZERO(&afds);
	FD_SET(tsock, &afds);
	FD_SET(usock, &afds);
	while (1) {		
		memcpy(&rfds, &afds, sizeof(rfds));

		if (select(FD_SETSIZE, &rfds, (fd_set *)0, (fd_set *)0,
				(struct timeval *)0) == -1)
			errexit("select error: %s\n", strerror(errno));

		if (FD_ISSET(tsock, &rfds)) {
			alen = sizeof(fsin);
			ssock = accept(tsock, (struct sockaddr *)&fsin,
				&alen);
			/*if (ssock < 0)
				errexit("accept failed: %s\n",
						strerror(errno));*/
			if(ssock!=-1)	
				FD_SET(ssock, &afds);
		}

		if (FD_ISSET(usock, &rfds)) {
			alen = sizeof(fsin);
			if (nrec=recvfrom(usock, buf, sizeof(buf), 0,
				(struct sockaddr *)&fsin, &alen) < 0)
				errexit("recvfrom: %s\n",
					strerror(errno));
			(void) sendto(usock, buf, nrec, 0,
				(struct sockaddr *)&fsin, sizeof(fsin));
		}

		for(auxsock=0; auxsock < nfds; ++auxsock) {
			if ((auxsock!=tsock) && (FD_ISSET(auxsock, &rfds)) && (auxsock!=usock)) {
				n = recv (auxsock, buf, sizeof(buf), 0);
				if (n > 0)
					send (auxsock, buf, n, 0);
				if (n == 0) {
					close(auxsock);
					FD_CLR(auxsock, &afds);
				}
			}
		}
	}
	(void) close(tsock);
	(void) close(usock);
}

