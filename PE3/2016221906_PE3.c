#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

ino_t get_inode(char *);
void printpathto(ino_t);
void inum_to_name(ino_t, char *, int );
int top=0;

typedef struct names{
	char its_name[BUFSIZ];
	struct names *next;
}names;

int main(){
	ino_t this_inode;
	ino_t my_inode;
    char its_name[BUFSIZ];
	struct names *Head;
	struct names *temp;

	Head = (names*)malloc(sizeof(names));
//	Head->its_name = NULL;
	Head->next = NULL;

    this_inode = get_inode(".");

	while(get_inode("..") != this_inode){
		chdir("..");
		inum_to_name(this_inode, its_name, BUFSIZ);

		temp = (names*)malloc(sizeof(names));
		strncpy(temp->its_name,its_name,BUFSIZ);
		temp->next = Head;
		Head = temp;

		my_inode = get_inode(".");
		this_inode = my_inode;
	}

	temp = Head;
	while(temp->next){
		printf("/%s", temp->its_name);
		temp = temp->next;
	}

	putchar('\n');
	return 0;
}

//void printpathto(ino_t this_inode){
//
//    ino_t my_inode;
//    char its_name[BUFSIZ];
//
//    if(get_inode("..") != this_inode){
//		chdir("..");
//		inum_to_name(this_inode, its_name, BUFSIZ);
//		my_inode = get_inode(".");
//		printpathto(my_inode);
//		printf("/%s", its_name);
//    }
//}

void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen){

    DIR *dir_ptr;
    struct dirent *direntp;

    dir_ptr = opendir(".");

    if(dir_ptr == NULL){
		perror(".");
		exit(1);
    }

    while((direntp = readdir(dir_ptr)) != NULL)
		if(direntp->d_ino == inode_to_find){
			strncpy(namebuf, direntp->d_name, buflen);
			namebuf[buflen-1] = '\0';
			closedir(dir_ptr);
			return;
		}

    fprintf(stderr, "error looking for inum %d\n",(int) inode_to_find);
    exit(1);
}

ino_t get_inode (char *fname){

    struct stat info;

    if(stat(fname, &info) == -1){
		fprintf(stderr, "Cannot stat ");
		perror(fname);
		exit(1);
    }

    return info.st_ino;
}
