#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/mman.h>
#include<netinet/in.h>

// Xanos Stefanos 1573 
int file_exists(const char *fname)
{
	FILE *file;
	if((file=fopen(fname, "r")))
	{
		fclose(file);
	return 1;
	}
return 0;
}

void red()
{
	printf("\033[1;31m");
}
void yellow()
{
        printf("\033[1;33m");
}
void reset()
{
        printf("\033[1;0m");
}

int main(int argc,char *argv[])
{
        //elegxos oti exv toulaxiston mia parametro
        if(argc < 3)
        {	
		red();
                printf("Need at least 2 argument to write thr file(filename kai address) \n");
               reset();
		 exit(1);
	
        }
	
	int textsize=100;
        //char *text = argv[1];
        //printf("Text to be written is : %s\n",text);

        const char *filepath =argv[1];
	int pos = atoi(argv[2]);
	
	int fd;

	if ( file_exists(filepath))
	{
		 fd=open(filepath, O_RDWR);
	}
	else
	{
        	 fd=open(filepath,O_RDWR | O_CREAT,(mode_t)0600);
	}

	 if( fd==-1)
        {
                perror("Error opening file for writting \n");
                exit(2);
        }

       

        if( lseek(fd, textsize-1, SEEK_SET)== -1)
        {
                close(fd);
                perror("Error calling lseek()\n");
                exit(3);
        }

        if( write(fd,"",1) == -1)
        {
                close(fd);
                perror("Error writting last  bite\n");
                exit(4);
        }

        char*map = mmap(0,textsize,PROT_READ | PROT_WRITE, MAP_SHARED,fd,0);
 	
	if( map == MAP_FAILED)
        {
                close(fd);
                perror("Error maping the file \n");
                exit(5);
        }

/*
        int i=0;

        for(i=0; i<textsize; i++)
        {
                printf("writting character %c at position %d\n",text[i],i);
                map[i] = text[i];
        }

        if( msync(map,textsize,MS_SYNC) == -1)
        {
                perror("Could not sync the file to disk\n");
        }
*/

	int oldvalue=map[pos];
	yellow();
	printf("old value %d \n",oldvalue);
	reset();
	oldvalue++;
	map[pos]=oldvalue;
        if( munmap(map,textsize) == -1)
        {
                close(fd);
                perror("Error unmaping the file\n");
                exit(6);
        }
        close(fd);

 	 return 0;
}
