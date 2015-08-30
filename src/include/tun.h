/*
 * tun.h
 *
 * Copyright (c) 2001 Dug Song <dugsong@monkey.org>
 *
 * $Id: tun.h,v 1.3 2002/01/22 20:59:02 dugsong Exp $
 */

#ifndef TUN_H
#define TUN_H

#include <dnet.h>
#include <event.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*tun_handler)(void *buf, size_t len, void *arg);

struct tun {
	struct addr		  src;
	struct addr		  dst;
	HANDLE			  handle;
	struct event	  ev;
	tun_handler		  callback;
	void			  *arg;
};

typedef struct tun tun_t;



tun_t	*tun_open(struct addr *src, struct addr *dst, int mtu);
int	 tun_register(tun_t *tun, tun_handler callback, void *arg);
tun_t	*tun_close(tun_t *tun);

#endif /* TUN_H */
