/* UDPdaytime.c - UDPdaytime, main */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "error.h"
#include "socket_utils.h"

int	UDPdaytime(const char *host, const char *service); 


#define	LINELEN		128
#define	MSG		"what time is it?\n"

/*------------------------------------------------------------------------
 * main - UDP client for DAYTIME service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*host = "localhost";	/* host to use if none supplied	*/
	char	*service = "daytime";	/* default service port		*/

	switch (argc) {
	case 1:
		host = "localhost";
		break;
	case 3:
		service = argv[2];
		/* FALL THROUGH */
	case 2:
		host = argv[1];
		break;
	default:
		fprintf(stderr, "usage: UDPdaytime [host [port]]\n");
		exit(1);
	}
	UDPdaytime(host, service);
	exit(0);
}

/*------------------------------------------------------------------------
 * UDPdaytime - invoke Daytime on specified host and print results
 *------------------------------------------------------------------------
 */
UDPdaytime(const char *host, const char *service)
{
	char	buf[LINELEN+1];		/* buffer for one line of text	*/
	int	s, n;			/* socket, read count		*/

	s = connectUDP(host, service);

	(void) write(s, MSG, strlen(MSG)); /* send something to the server, 
					      since nothing has been sent up to now */

	n = read(s, buf, LINELEN);
	buf[n] = '\0';		/* ensure null-terminated	*/
	(void) fputs( buf, stdout );
}

