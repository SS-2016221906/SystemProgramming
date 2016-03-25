#include <stdio.h>
#include <sys/types.h>
#include <utmp.h>
#include <fcntl.h>
#include <time.h>

#define SHOWHOST
#define NULLUT ((struct utmp *)NULL)

void show_info(struct utmp *);
void showtime(time_t);

int main(){
	struct utmp *utbufp, *utmp_next();
	
	if(-1==utmp_open(UTMP_FILE)){
		perror(UTMP_FILE);
		return 1;
	}
	while(NULLUT != (utbufp = utmp_next()))
		show_info(utbufp);
	utmp_close();
	return 0;
}

void show_info(struct utmp *utbufp)
{
	if(USER_PROCESS != utbufp->ut_type)
		return;
	
	printf("%-12.12s",utbufp->ut_name);
	printf(" ");
	printf("%-8.8s",utbufp->ut_line);
	printf(" ");
	showtime(utbufp->ut_time);
#ifdef SHOWHOST
	if('\0' != utbufp->ut_host[0])
		printf(" (%s)",utbufp->ut_host);
#endif
	printf("\n");
}

void showtime(time_t timeval)
{
	char *cp;
	cp = ctime(&timeval);
	
	printf("%12.12s",cp+4);
}