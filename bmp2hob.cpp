//joric (á)2000 8-O

#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct{
  char filename[9];
  short start_addr;
  short sect_length;
  short length;
  short CRC;
}HobetaHdr;

unsigned short CRC_hobeta(char *p)
     {
     unsigned short i, CRC;
     for (i=CRC=0; i<15; CRC = CRC + (p[i] * 257) + i, i++);
     return CRC;
     }

char *mkname(char* filename, char *ext){
   char *name="            ";  //"filename.ext"
   strcpy(name,filename);
   char* where=strchr(name,'.'); if (where!=NULL) *where=0;
   strcat(name,ext);
   return (name);
}

char *mkzxname(char* filename){
   char *zxname="        C"; // "filenameC"
   for (int i=0; (filename[i]!=0 & filename[i]!='.'); zxname[i]=filename[i], i++);
   return (zxname);
}

char main(short argc, char **argv){
  HobetaHdr Header;
  FILE *fs, *fd;
  char *raw;
  char *zxp;
  char *attrs;
  int i,j,k,n; char b;

 if ( argc < 2 ) {
   printf("Adobe photoshop uncompressed B/W bmp (256x192) to ZX screens converter\n");
   printf("Usage: BMP2ZX file.BMP \n");
   return(0);
 }


  strcpy(Header.filename,mkzxname(argv[1]));
  Header.start_addr=0x4000;
  Header.length=0x1B00;
  Header.sect_length=0x1B00;
  Header.CRC=CRC_hobeta((char*)&Header);

  raw=(char*)malloc(6144);
  zxp=(char*)malloc(6144);
  attrs=(char*)malloc(768);
  memset(attrs,7,768);

  fs = fopen (argv[1], "rb" );
  fseek (fs, 62, SEEK_SET);
  fread (raw,6144,1,fs);

  for (i=0; i<192; i++){
      n=1792*(i/64)-2016*(i/8)+256*i;
      memcpy( zxp+(191-i)*32, raw+n, 32);
    }

  fd = fopen (mkname(argv[1],".$c"), "wb" );
  fwrite(&Header,sizeof(Header),1,fd);
  fwrite(zxp,6144,1,fd);
  fwrite(attrs,768,1,fd);

  return(1);

}