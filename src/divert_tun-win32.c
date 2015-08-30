/*
 * tun-win32.c
 *
 * Copyright (c) 2001 Dug Song <dugsong@monkey.org>
 *
 * $Id: tun-win32.c,v 1.1 2002/02/25 06:21:59 dugsong Exp $
 */

#include "config.h"

#include "tun.h"
#include "windivert.h"

#define WINDIVERT_PARAM_QUEUE_LEN_DEFAULT 8192
#define WINDIVERT_PARAM_QUEUE_TIME_DEFAULT 2048

tun_t * tun_open(struct addr *src, struct addr *dst, int mtu)
{
	tun_t *tun;

	if ((tun = calloc(1, sizeof(*tun))) == NULL)
		return (NULL);
		
	tun->dst = *dst;
	tun->src = *src;
	tun->handle = INVALID_HANDLE_VALUE;

	char ssrc[24], sdst[24];
	addr_ntop(src, ssrc, sizeof(ssrc));
	addr_ntop(dst, sdst, sizeof(sdst));

	char* filter_template = "outbound and (ip.DstAddr == %s and ip.SrcAddr == %s)";
	char filter[90];

	snprintf(filter, sizeof(filter), filter_template, sdst, ssrc);
	tun->handle = WinDivertOpen(filter, WINDIVERT_LAYER_NETWORK, 0, 0);
	
	if (tun->handle == INVALID_HANDLE_VALUE){
		fprintf(stderr, "error: failed to open divert device (%d)\n", GetLastError());
		return (tun_close(tun));
	}

	// Max-out the packet queue:
	if (!WinDivertSetParam(tun->handle, WINDIVERT_PARAM_QUEUE_LEN, WINDIVERT_PARAM_QUEUE_LEN_DEFAULT)){
		fprintf(stderr, "error: failed to set packet queue length (%d)\n", GetLastError());
		return (tun_close(tun));
	}

	if (!WinDivertSetParam(tun->handle, WINDIVERT_PARAM_QUEUE_TIME, WINDIVERT_PARAM_QUEUE_TIME_DEFAULT)){
		fprintf(stderr, "error: failed to set packet queue time (%d)\n", GetLastError());
		return (tun_close(tun));
	}

	return (tun);
}

static void
_tun_recv(int fd, short event, void *arg)
{
	struct event_iov *eio = (struct event_iov *)fd;
	tun_t *tun = (tun_t *)arg;
	struct ip_hdr *ip;
	
	ip = (struct ip_hdr *)(eio->buf);

	if (eio->len > IP_HDR_LEN && ip->ip_dst == tun->dst.addr_ip) {
		//calling fragroute_process
		(*tun->callback)(eio->buf, eio->len, tun->arg);
	}
}

int
tun_register(tun_t *tun, tun_handler callback, void *arg)
{
	tun->callback = callback;
	tun->arg = arg;
	
	event_set(&tun->ev, (int)tun->handle, EV_READ, _tun_recv, tun);	
	tun->ev.thread = CreateThread(NULL, 1, (LPTHREAD_START_ROUTINE)event_add_thread, (LPVOID)&tun->ev, 0, NULL);

	if (tun->ev.thread == NULL){
		fprintf(stderr, "error: failed to start thread (%d)\n", GetLastError());
		return -1;
	}

	return (0);
}

tun_t *
tun_close(tun_t *tun)
{
	if (event_initialized(&tun->ev))
		event_del(&tun->ev);

	if (tun->handle != INVALID_HANDLE_VALUE)
		WinDivertClose(tun->handle);

	if (tun->ev.thread != INVALID_HANDLE_VALUE)
		CloseHandle(tun->ev.thread);

	free(tun);
	
	return (NULL);
}
