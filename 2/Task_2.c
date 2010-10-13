#include<stdio.h>
#include<io.h>
#include<stdlib.h>
#include<malloc.h>
int code = 0;
void cry_out(char* message, char* message_2, int code)
{
	fprintf(stdout,"%s\n",message);
	if (code>0) {fprintf(stderr,"%s - ",message); fprintf(stderr,"Error occured: %s\n",message_2);}
	return;
}
long int munch(FILE* pointer, FILE* pointer_2, char* fname, unsigned int* exit_buf)
{
	unsigned int n;
	long int m = 0;
	int s;
	do
		{
		s=fscanf(pointer,"%d",&n);
		exit_buf[m++]= n;
		}
    while(s>0);
    if (s!=EOF) 
		{
		code=4;
		cry_out("File is corrupted",fname,1);
		}
	return m-1;
}
int main(int argc,char *argv[])
{
    signed int i;
    if (argc<3) 
		{
		cry_out("Failed","not enough args",1);
		return 1;
		};
    FILE* fp[argc-1];
;
    if ((fp[argc-2] = fopen(argv[argc-1],"w"))!=NULL) {}
    else 
		{
		cry_out("Failed to create file",argv[argc-1],1);
		code=2;
		return code;
		}
;
    FILE* tmp_store[3];
    long int size[3];
    long int j;
    for (j=0;j<3;j++) 
		{
		size[j]=0;
		}
;

    for (j=0;j<3;j++)
        {
        if ((tmp_store[j] =tmpfile())!=NULL) {}
        else {cry_out("Failed to create tempfile","tempfile",1);code=4;goto finish_1;}
        }
;
    for (i=0;i<argc-2;i++)
		{
		fp[i] = NULL;
		if ((fp[i] = fopen(argv[i+1],"r"))!=NULL) 
			{
			unsigned int* w_buf;
			if ((w_buf =(unsigned int*)malloc(filelength(fileno(fp[i]))*sizeof(int)))!=NULL)
				{
				size[0] = munch(fp[i],fp[argc-2],argv[i+1],w_buf);
				long int j;
				for (j=size[0]-1;j>0;j--)
					{
					long int k;
					for (k=0;k<j;k++)
						{
						if (w_buf[k]>w_buf[k+1])
							{
							unsigned int temp = w_buf[k];
							w_buf[k] = w_buf[k+1];
							w_buf[k+1] = temp;
							}			
						}
					}
				for (j=0;j<size[0];j++)
				{fprintf(tmp_store[0],"%d ",w_buf[j]);}
				fseek(tmp_store[0],0,SEEK_SET);
				fseek(tmp_store[1],0,SEEK_SET);
				fseek(tmp_store[2],0,SEEK_SET);
;
				long int a = size[0]-1;
				long int b = size[1]-1;
				unsigned int _a = 0;
				fscanf(tmp_store[0],"%d",&_a);
				unsigned int _b =0;
				fscanf(tmp_store[1],"%d",&_b);
				while ((a>=0)&&(b>=0))
					{
					unsigned int _c;
					if (_a>_b)
						{
						_c=_b;
						fscanf(tmp_store[1],"%d",&_b);
						--b;
						}
					else
						{
						_c=_a;
						fscanf(tmp_store[0],"%d",&_a);
						--a;
						}
					fprintf(tmp_store[2],"%d ",_c);
					}
				while (a>=0)
					{
					fprintf(tmp_store[2],"%d ",_a);
					fscanf(tmp_store[0],"%d",&_a);
					--a;
					}
				while (b>=0)
					{
					fprintf(tmp_store[2],"%d ",_b);
					fscanf(tmp_store[1],"%d",&_b);
					--b;
					}
					;
					free(w_buf);
					FILE* swap = tmp_store[1];
					tmp_store[1] = tmp_store[2];
					tmp_store[2] = swap;
					size[1]+=size[0];
					size[0]=size[2]=0;
					if (((tmp_store[0]=tmpfile())!=NULL)&&((tmp_store[2]=tmpfile())!=NULL)) {}
					else 
						{
						cry_out("Failed to create tempfile","tempfile",1);
						code=4;
						goto finish_1;
						}
					}
				else
					{
					cry_out("Out of memory","Out of memory",1);
					code=5;
					goto finish_1;
					}
				}
            else {cry_out("Failed to open file",argv[i+1],1);code=3;goto finish_1;}
		}
;
	fseek(tmp_store[1],0,SEEK_SET);
	for (j=0;j<size[1];j++)
		{
		unsigned int a;
		fscanf(tmp_store[1],"%d",&a);
		fprintf(fp[argc-2],"%d ",a);
		}
;
finish_1:
	for (;i>=0;i--)
		{
		if (fp[i]!=NULL) {fclose(fp[i]);}
		}
	fclose(fp[argc-2]);
	rmtmp();
	return code;
}
