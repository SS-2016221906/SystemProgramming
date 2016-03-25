#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>

#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE];
static int num_recs;
static int cur_res;
static int fd_utmp = -1;

utmp_open(char *filename)
{
	fd_utmp = open(filename, O_RDONLY);
	cur_res = num_recs = 0;
	return fd_utmp;
}

struct utmp *utmp_next()
{
	struct utmp *recp;
	if(-1 == fd_utmp)
		return NULLUT;
	if(cur_res == num_recs && utmp_reload() == 0)
		return NULLUT;
		
	recp = (struct utmp *) &utmpbuf[cur_res * UTSIZE];
	cur_res++;
	return recp;
}

int utmp_reload()
{
	int atm_read;
	
	atm_read = read(fd_utmp, utmpbuf,NRECS * UTSIZE);
	
	num_recs = atm_read/UTSIZE;
	
	cur_res = 0;
	return num_recs;
}

void utmp_close()
{
	if(-1 != fd_utmp)
		close(fd_utmp);
}