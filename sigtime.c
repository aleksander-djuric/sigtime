/*
 * sigtime.c
 *
 * Description: Linux Signal Handling Example
 * Copyright (c) 2017 Aleksander Djuric. All rights reserved.
 * Distributed under the GNU Lesser General Public License (LGPL).
 * The complete text of the license can be found in the LICENSE
 * file included in the distribution.
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <syslog.h>

#define LOGFILE "/tmp/curtime"
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "sigtime"
#endif

static int terminate = 0;

void signal_handler (int sig) {
	char buffer[26];
	FILE *fp;
	time_t t;

	if (sig == SIGUSR1) {
		syslog(LOG_INFO, "received SIGUSR1");
		time(&t);
		strftime(buffer, 26, "%Y-%m-%d %H:%M:%S.000\n", localtime(&t));
		if (!(fp = fopen(LOGFILE, "a"))) {
			syslog(LOG_ERR, "couldn't open logfile: "LOGFILE);
			return;
		}
		if (fputs(buffer, fp) == EOF)
			syslog(LOG_ERR, "couldn't write to logfile: "LOGFILE);
		fclose(fp);
	} else {
		syslog(LOG_INFO, "received signal: %d", sig);
		terminate = 1;
	}
}

void signal_setup (void) {
	struct sigaction sa;

	sa.sa_handler = &signal_handler;
	sa.sa_flags = SA_RESTART;
	sigfillset(&sa.sa_mask);
	sigdelset(&sa.sa_mask, SIGUSR1);
	sigdelset(&sa.sa_mask, SIGTERM);
	sigprocmask(SIG_SETMASK, &sa.sa_mask, NULL);

	if (sigaction(SIGUSR1, &sa, NULL) < 0 ||
		sigaction(SIGTERM, &sa, NULL) < 0)
		syslog(LOG_ERR, "can't setup signal hanler");
}

int main () {
	openlog(PACKAGE_NAME, 0, LOG_USER|LOG_DAEMON);

	if (!fork()) {

		openlog(PACKAGE_NAME, 0, LOG_DAEMON);

		signal_setup();

		while (!terminate) sleep(1);
		if (terminate) {
			syslog(LOG_INFO, "stopping..");
		}

		closelog();
		return 0;
	}

	syslog(LOG_INFO, "daemon started..");

	closelog();
	return 0;
}
