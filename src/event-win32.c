/*
 * event-win32.c
 *
 * Copyright (c) 2001 Dug Song <dugsong@monkey.org>
 *
 * $Id: event-win32.c,v 1.1 2002/02/25 06:21:59 dugsong Exp $
 */

#include <windows.h>
#include <winsock.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "event.h"
#include "windivert.h"

static HINSTANCE		 lib_inst;
static HANDLE			 timer_queue;

#define create_timer_queue CreateTimerQueue
#define delete_timer_queue DeleteTimerQueue
#define create_timer CreateTimerQueueTimer
#define delete_timer DeleteTimerQueueTimer

int	event_gotsig;
int	(*event_sigcb)(void);

int
os_version(void)
{
	OSVERSIONINFO info;
	
	info.dwOSVersionInfoSize = sizeof(info);

	if (GetVersionEx(&info) == TRUE &&
	    info.dwPlatformId == VER_PLATFORM_WIN32_NT)
		return (info.dwMajorVersion);
	
	return (32767);
}

static int
timeval_to_ms(struct timeval *tv)
{
	return ((tv->tv_sec * 1000) + (tv->tv_usec / 1000));
}

void
event_init(void)
{
	if (os_version() <= 4)
		errx(1, "this program must be run on Windows 2000 or greater");

	timer_queue = create_timer_queue();
}

void
event_set(struct event *ev, int fd, short event, void (*callback)(int, short, void *), void *arg)
{
	memset(ev, 0, sizeof(*ev));
	
	ev->handle = (HANDLE)fd;
	ev->overlap.hEvent = (HANDLE)ev;
	ev->event = event;
	ev->callback = callback;
	ev->arg = arg;
	ev->thread = INVALID_HANDLE_VALUE;
}

DWORD event_add_thread(LPVOID arg)
{
	struct event* ev = (struct event *)arg;

	if (ev->event != EV_READ)
		return (-1);	/* XXX - UNIMPLEMENTED */
	
	while (!event_gotsig){
		//ReadFileEx(ev->handle, ev->buf, sizeof(ev->buf), &ev->overlap, event_callback);
		//TODO: Implement async: 
		//		https://github.com/ffalcinelli/pydivert/blob/master/pydivert/models.py
		//		https://github.com/ffalcinelli/pydivert/blob/master/pydivert/windivert.py

		//Divert tunnel: https://barbatunnel.codeplex.com/
		UINT packet_len = 0;
		if (!WinDivertRecv(ev->handle, ev->buf, sizeof(ev->buf), &ev->addr, &packet_len))
		{
			//warning: failed to read packet
			continue;
		}

		struct event *ev_over = (struct event *)(ev->overlap.hEvent);
		struct event_iov eio;

		eio.buf = ev->buf;
		eio.len = packet_len;

		//calling _tun_recv
		ev->callback((int)&eio, ev_over->event, ev->arg);
	}	

	return 0;
}

int
event_initialized(struct event *ev)
{
	return (ev->handle != INVALID_HANDLE_VALUE);
}

void
event_del(struct event *ev)
{
	/* XXX - UNIMPLEMENTED */
}

void
timeout_set(struct event *ev, void (*callback)(int, short, void *), void *arg)
{
	memset(ev, 0, sizeof(*ev));
	
	ev->event = EV_TIMEOUT;
	ev->callback = callback;
	ev->arg = arg;
}

void CALLBACK
timeout_callback(PVOID arg, BOOL TimerFired)
{
	struct event *ev = (struct event *)arg;

	delete_timer(timer_queue, ev->handle, NULL);
	ev->handle = INVALID_HANDLE_VALUE;
	
	ev->callback(-1, EV_TIMEOUT, ev->arg);
}

void
timeout_add(struct event *ev, struct timeval *tv)
{
	if (create_timer(&ev->handle, timer_queue, timeout_callback, ev, timeval_to_ms(tv), 0, WT_EXECUTEINIOTHREAD) == 0)
		errx(1, "CreateTimerQueueTimer failed");
}

int
event_dispatch(void)
{
	for (;;) {
		while (event_gotsig) {
			event_gotsig = 0;
			if (event_sigcb != NULL) {
				if ((*event_sigcb)() == -1) {
					delete_timer_queue(timer_queue);
					return (-1);
				}
			}
		}
		/* XXX - i'm a lazy bum */
		SleepEx(100, TRUE);
	}
	return (0);
}
