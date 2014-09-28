/* UDPecho.c - main, UDPecho */

#include "error.h"
#include "socket_utils.h"

int	UDPecho(const char *host, const char *service);

#define	LINELEN		128

/*------------------------------------------------------------------------
 * main - UDP client for ECHO service
 *------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
	char	*host = "localhost";
	char	*service = "echo";

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
		fprintf(stderr, "usage: UDPecho [host [port]]\n");
		exit(1);
	}
	

	UDPecho(host, service);
	exit(0);
}

/*------------------------------------------------------------------------
 * UDPecho - send input to ECHO service on specified host and print reply
 *------------------------------------------------------------------------
 */
int
UDPecho(const char *host, const char *service)
{
	struct sockaddr_in sockaddr;	/* an Internet endpoint address		*/
	struct hostent	*phe;		/* pointer to host information entry	*/
	struct servent	*pse;		/* pointer to service information entry	*/
	char	buf[LINELEN+1];		/* buffer for one line of text		*/
	int	s, nchars;		/* socket descriptor, read count	*/


	s = createUDPsocket();

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;

   /* Map service name to port number */
	if ( pse = getservbyname(service, "udp") )
		sockaddr.sin_port = pse->s_port;
	else if ((sockaddr.sin_port=htons((unsigned short)atoi(service))) == 0)
		errexit("can't get \"%s\" service entry\n", service);

    /* Map host name to IP address, allowing for dotted decimal */
	if ( phe = gethostbyname(host) )
		memcpy(&sockaddr.sin_addr, phe->h_addr, phe->h_length);
	else if ( (sockaddr.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
		errexit("can't get \"%s\" host entry\n", host);

	while (fgets(buf, sizeof(buf), stdin)) {
		buf[LINELEN] = '\0';	/* insure null-terminated */
		nchars = strlen(buf);
		(void) sendto(s, buf, nchars, 0, (struct sockaddr_in *) & sockaddr, sizeof(sockaddr));

		if (recvfrom(s, buf, nchars, 0, NULL, NULL) < 0)
			errexit("socket read failed: %s\n",
					strerror(errno));
		fputs(buf, stdout);
	}
}

