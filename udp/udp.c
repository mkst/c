#define _POSIX_C_SOURCE 200112L

// lifted from http://beej.us/guide/bgnet/examples/talker.c
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <errno.h>

#include "k.h"

void *get_in_addr(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET)
  {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


K k_send(K x, K y, K z)
{
  if(x->t != KC || y->t != -KI || z->t != KG)
  {
    R krr((S)"type");
  }

 if(0 > y->i || y->i > 99999)
 {
    R krr((S)"port");
 }

  if(0 > z->n || z->n > 65507)
  {
    R krr((S)"payload");
  }

  // Q does not nul-terminate the string, so do that here
  char *host = (char*) calloc((1 + x->n) * sizeof(char), '\0');
  if(host == NULL)
  {
    R krr((S)"malloc");
  }
  memcpy(host, x->G0, x->n);

  char port[6];
  snprintf(port, sizeof port, "%d", y->i);

	struct addrinfo hints, *ai, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if(getaddrinfo(host, port, &hints, &ai) != 0)
  {
		R krr((S)"getaddrinfo");
	}

  free(host);

  int fd;
	for(p = ai; p != NULL; p = p->ai_next) {
		if((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
    {
			continue;
		}
		break;
	}

  freeaddrinfo(ai);

	if(p == NULL)
  {
		R krr((S)"socket");
	}

  int sent;
	if((sent = sendto(fd, z->G0, z->n, 0, p->ai_addr, p->ai_addrlen)) == -1)
  {
    R krr((S)"sendto");
	}

	close(fd);

  R ki(sent);
}

K k_recv(I fd)
{
  C buf[65536];
  J cnt;
  K res = ktn(KG, 0);

  struct sockaddr_storage remote_addr;
  socklen_t addr_len;
	addr_len = sizeof remote_addr;

  while((cnt = recvfrom(
    fd,
    buf,
    sizeof buf,
    MSG_DONTWAIT,
		(struct sockaddr *)&remote_addr,
    &addr_len)) > 0)
  {
    jv(&res, kpn(buf, cnt));
	}

  char sender[INET6_ADDRSTRLEN];

  inet_ntop(
    remote_addr.ss_family,
    get_in_addr((struct sockaddr *)&remote_addr),
    sender,
    sizeof sender);

  if(cnt < 0 && errno != EAGAIN && errno != EWOULDBLOCK)
  {
		sd0(fd);
    close(fd);
  }
  if(cnt == 0)
  {
		sd0(fd);
    close(fd);
  }

  if(res->n > 0)
  {
    K ksender = kpn(sender, strlen(sender));
    K values = ktn(0, 0);
    jk(&values, ksender);
    ja(&values, &res);

    k(0, (S)".udp.recv", values, (K)0);
  }

	R (K)0;
}

K1(k_listen)
{
  if(xt != -KI)
  {
    R krr((S)"type");
  }
	if(0 > x->i || x->i > 99999)
	{
		R krr((S)"port");
	}

  int fd;
  struct addrinfo hints, *servinfo, *p;

	char port[6];
  snprintf(port, sizeof port, "%d", x->i);

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  if(getaddrinfo(NULL, port, &hints, &servinfo) != 0) {
    krr((S)"getaddrinfo");
    R ki(0);
  }

  int res;
  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next)
  {
    if((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
    {
      continue;
    }
    if((res = bind(fd, p->ai_addr, p->ai_addrlen)) == -1)
    {
      close(fd);
      continue;
    }
    break;
  }

  if(p == NULL)
  {
    krr((S)"bind");
    R ki(0);
  }

  freeaddrinfo(servinfo);

  if(res == 0)
  {
    sd1(fd, k_recv); // setup callback
  }

  R kb(res == 0);
}
