#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

#define TRUE 1
#define FALSE 0

void do_ls(char[]);
void dostat(char *);
void show_file_info(char *,struct stat *);
void mode_to_letters(int ,char[]);
char *uid_to_name(uid_t);
char *gid_to_name(gid_t);
int Option_R = FALSE;

void main(int ac, char *av[])
{
	if(ac == 1)
		do_ls(".");
	else
		while(--ac)
		{
			if(0 == strcmp(*++av,"-R"))
			{
				--ac;
				av[0] = ".";
				Option_R = TRUE;
			}
			printf("%s:\n",*av);
			do_ls(*av);
		}
}

void do_ls(char dirname[])
{
	DIR *dir_ptr;
	struct dirent *direntp;
	if(NULL == (dir_ptr = opendir(dirname)))
		fprintf(stderr,"ls1: cannot open %s\n",dirname);
	else
	{
		while(NULL != (direntp = readdir(dir_ptr)))
			dostat(direntp->d_name);
		closedir(dir_ptr);
	}
}

void dostat(char *filename)
{
	struct stat info;
	if(-1 == stat(filename, &info))
		perror(filename);
	else
		show_file_info(filename,  &info);
}

void show_file_info(char *filename, struct stat *info_p)
{
	char *uid_to_name(), *ctime(), *gid_to_name(), *filemode();
	void mode_to_letters();
	char modestr[11];
	
	mode_to_letters(info_p->st_mode, modestr);
	
	printf("%s"		,modestr);
	printf("%4d "	,(int)info_p->st_nlink);
	printf("%-8s "	,uid_to_name(info_p->st_uid));
	printf("%-8s "	,gid_to_name(info_p->st_gid));
	printf("%8ld "	,(long)info_p->st_size);
	printf("%.12s "	,4+ctime(&info_p->st_mtime));
	printf("%s\n"	,filename);
	
	if(Option_R){
		if(modestr[0] == 'd')
			if(!(	(0==strcmp(filename,".."))||(0==strcmp(filename,"."))	))
				do_ls(filename);
	}
}

void mode_to_letters(int mode, char str[])
{
	strcpy(str,"----------");
	
	if(S_ISDIR(mode)) str[0] = 'd';
	if(S_ISCHR(mode)) str[0] = 'c';
	if(S_ISBLK(mode)) str[0] = 'b';
	
	if(mode & S_IRUSR) str[1] = 'r';
	if(mode & S_IWUSR) str[2] = 'w';
	if(mode & S_IXUSR) str[3] = 'x';
	
	if(mode & S_IRGRP) str[4] = 'r';
	if(mode & S_IWGRP) str[5] = 'w';
	if(mode & S_IXGRP) str[6] = 'x';
	
	if(mode & S_IROTH) str[7] = 'r';
	if(mode & S_IWOTH) str[8] = 'w';
	if(mode & S_IXOTH) str[9] = 'x';
}

char *uid_to_name (uid_t uid)
{
	struct passwd *getpwuid(), *pw_ptr;
	static char namstr[10];
	if(NULL == (pw_ptr = getpwuid(uid)))
	{
		sprintf(namstr,"%d",uid);
		return namstr;
	}
	else
		return pw_ptr->pw_name;
}

char *gid_to_name(gid_t gid)
{
	struct group *getgrgid(), *grp_ptr;
	static char numstr[10];
	if(NULL == (grp_ptr = getgrgid(gid)))
	{
		sprintf(numstr,"%d",gid);
		return numstr;
	}
	else
		return grp_ptr->gr_name;
}
