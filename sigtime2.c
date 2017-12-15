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
#include <errno.h>
#include <string.h>
#include <syslog.h>

#define LOGFILE "/tmp/curtime"
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "sigtime"
#endif

sigset_t sset;

void write_time() {
	char buffer[26];
	FILE *fp;
	time_t t;

	time(&t);
	strftime(buffer, 26, "%Y-%m-%d %H:%M:%S.000\n", localtime(&t));
	if (!(fp = fopen(LOGFILE, "a"))) {
		syslog(LOG_ERR, "couldn't open logfile: "LOGFILE);
		return;
	}
	if (fputs(buffer, fp) == EOF)
		syslog(LOG_ERR, "couldn't write to logfile: "LOGFILE);
	fclose(fp);
}

int main () {
	openlog(PACKAGE_NAME, 0, LOG_USER|LOG_DAEMON);

	if (!fork()) {
		int sig = 0;

		openlog(PACKAGE_NAME, 0, LOG_DAEMON);

		sigfillset(&sset);
		sigprocmask(SIG_SETMASK, &sset, NULL);

                while (sig != SIGTERM) {
            		if (sigwait(&sset, &sig)) {
			    syslog(LOG_ERR, "%s", strerror(errno));
			    break;
			}
	
			if (sig == SIGUSR1) {
				syslog(LOG_INFO, "received SIGUSR1");
				write_time();
			} else {
				syslog(LOG_INFO, "received signal: %d", sig);
			}
		}

		syslog(LOG_INFO, "stopping..");
		closelog();
		return 0;
	}

	syslog(LOG_INFO, "daemon started..");

	closelog();
	return 0;
}
