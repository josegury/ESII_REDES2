/* TCPechod.c - main, TCPechod */

#define	_USE_BSD
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define	BUFSIZE		4096

#include <errno.h>
/*extern int	errno;*/

#include "error.h"
#include "socket_utils.h"


/*------------------------------------------------------------------------
 * main - Iterative UDP server for ECHO service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*service = "echo";	/* service name or port number	*/
	struct sockaddr_in fsin;	/* the from address of a client	*/
	int	sock;			/* server socket		*/
	unsigned int	alen;		/* from-address length		*/
	char	buf[BUFSIZE];		/* input buffer         	*/
	int 	nrec; 			/* number of bytes received 	*/


	switch (argc) {
	case	1:
		break;
	case	2:
		service = argv[1];
		break;
	default:
		errexit("usage: UDPechod [port]\n");
	}

	sock = passiveUDP(service);

	while (1) {
		alen = sizeof(fsin);
		if (nrec = recvfrom(sock, buf, sizeof(buf), 0,
				(struct sockaddr *)&fsin, &alen) < 0)
			errexit("recvfrom: %s\n", strerror(errno));
		(void) sendto(sock, buf, nrec, 0,
			(struct sockaddr *)&fsin, sizeof(fsin));
	}
}


