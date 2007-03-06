/*
 * This file is part of MVTH - the Machine Vision Test Harness.
 *
 * Main event loop, client side.
 *
 * Copyright (C) 2003,2004 Samuel P. Bromley <sam@sambromley.com>
 *
 * This code is licensed under the terms and conditions of
 * the GNU GPL. See the file COPYING in the top level directory of
 * the MVTH archive for details.
 */

#if HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <unistd.h>
#include <errno.h>

int main( int argc, char *argv[])
{
#define REPLY_LEN 1024
	struct sockaddr_un address;
	int sock,conn;
	size_t addrLength;

	if ((sock=socket(PF_UNIX,SOCK_STREAM,0))<0)
	{
		perror("open:");
		return EXIT_FAILURE;
	}

	/* remove any pre-existing socket or other file */
	unlink("/tmp/sample-socket");

	address.sun_family = AF_UNIX; /* Unix domain socket */
	strcpy(address.sun_path,"/tmp/sample-socket");

	/* total length of address is included in the sun_family element */
	addrLength=sizeof(address.sun_family) + strlen(address.sun_path);

	if (bind(sock,(struct sockaddr*)&address, addrLength))
	{
		perror("bind:");
		return EXIT_FAILURE;
	}

	if (listen(sock,5))
	{
		perror("listen:");
		return EXIT_FAILURE;
	}

	while (1)
	{
		struct timeval tv={0,0};
		fd_set rfds;
		int retval;
		/* watch the socket for incomming connections */
		FD_ZERO(&rfds);
		FD_SET(sock,&rfds);
		retval=select(sock+1,&rfds,NULL,NULL,&tv);
		if (retval==-1)
		{
			perror("select");
			return EXIT_FAILURE;
		}
		if (retval)
		{
			conn=accept(sock,(struct sockaddr *)&address,&addrLength);
			if (conn<0)
			{
				fprintf(stderr,"Failed to accept a client.\n");
				return EXIT_FAILURE;
			}
			fprintf(stderr,"...getting data\n");
			char buff[1024];
			int amount;
			while ((amount=read(conn,buff,sizeof(buff)))>0)
			{
				if (write(1,buff,amount)!=amount)
				{
					perror("write:");
					return EXIT_FAILURE;
				}
			}
			fprintf(stderr,"\n....done.\n");
			close(conn);
			getchar();
		}
		fprintf(stderr,"Waiting...\n");
		usleep(1000UL);
	}

	close(sock);

	return EXIT_SUCCESS;
}
