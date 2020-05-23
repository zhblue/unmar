#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
static struct{
  char marc[4];
  int ver;
  int num; 
} mar_header;
typedef struct{
  char filename[56];
  unsigned int length;
  unsigned int date;
  unsigned int start;
} pack_file;
void writefile(pack_file file,FILE * fmar){
    char dname[56];
    strcpy(dname,file.filename);
    char *dir=dirname(dname);     
    if(strlen(dir)>0) mkdir(dir,0700);
#ifdef __linux__ 
    //linux code goes here
    for(int i=0;i<strlen(file.filename);i++){
	if(file.filename[i]=='\\') file.filename[i]='/';
    }
#endif
    FILE * fdata=fopen(file.filename,"wb");
    if(fdata==NULL) {
		printf("fail to open file %s.\n",file.filename);
    }
    char data;
    for(int i=0;i<file.length;i++){
      fread(&data,1L,1L,fmar);
      fwrite(&data,1L,1L,fdata);	
    }
    fclose(fdata);
}
int main(int argc, char * argsv[]){
  if(argc!=2){
	printf("usage: unmar <.mar file>\n");
	return -1;
  }
  printf("extracting... %s \n",argsv[1]);
  char * mar_filename=argsv[1];
  FILE * fmar=fopen(mar_filename,"rb");
  fread(&mar_header,sizeof(mar_header),1L,fmar);

  printf("total files:%d\n",mar_header.num);
  pack_file files[mar_header.num]; 
  for(int i=0;i<mar_header.num;i++){
      fread(&files[i],sizeof(pack_file),1L,fmar);
  }

  for(int i=0;i<mar_header.num;i++){
      printf("%d:[%-20s]\t\t pos:%u \t\t size:%u \n",i+1,files[i].filename,files[i].start,files[i].length);
      writefile(files[i],fmar);
  }

  fclose(fmar);  
  return 0;
}
