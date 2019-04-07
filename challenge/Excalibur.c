#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* ---- Base64 Encoding/Decoding Table --- */
char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* decodeblock - decode 4 '6-bit' characters into 3 8-bit binary bytes */
void decodeblock(unsigned char in[], char *clrstr) {
  unsigned char out[4];
  out[0] = in[0] << 2 | in[1] >> 4;
  out[1] = in[1] << 4 | in[2] >> 2;
  out[2] = in[2] << 6 | in[3] >> 0;
  out[3] = '\0';
  strncat(clrstr, out, sizeof(out));
}

void b64_decode(char *b64src, char *clrdst) {
  int c, phase, i;
  unsigned char in[4];
  char *p;

  clrdst[0] = '\0';
  phase = 0; i=0;
  while(b64src[i]) {
    c = (int) b64src[i];
    if(c == '=') {
      decodeblock(in, clrdst); 
      break;
    }
    p = strchr(b64, c);
    if(p) {
      in[phase] = p - b64;
      phase = (phase + 1) % 4;
      if(phase == 0) {
        decodeblock(in, clrdst);
        in[0]=in[1]=in[2]=in[3]=0;
      }
    }
    i++;
  }
}

/* encodeblock - encode 3 8-bit binary bytes as 4 '6-bit' characters */
void encodeblock( unsigned char in[], char b64str[], int len ) {
    unsigned char out[5];
    out[0] = b64[ in[0] >> 2 ];
    out[1] = b64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? b64[ ((in[1] & 0x0f) << 2) |
             ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? b64[ in[2] & 0x3f ] : '=');
    out[4] = '\0';
    strncat(b64str, out, sizeof(out));
}

/* encode - base64 encode a stream, adding padding if needed */
void b64_encode(char *clrstr, char *b64dst) {
  unsigned char in[3];
  int i, len = 0;
  int j = 0;

  b64dst[0] = '\0';
  while(clrstr[j]) {
    len = 0;
    for(i=0; i<3; i++) {
     in[i] = (unsigned char) clrstr[j];
     if(clrstr[j]) {
        len++; j++;
      }
      else in[i] = 0;
    }
    if( len ) {
      encodeblock( in, b64dst, len );
    }
  }
}





/*
Excalibur or Caliburn is the legendary sword of King Arthur, sometimes attributed with magical powers or associated with the rightful sovereignty of Great Britain
Sometimes Excalibur and the Sword in the Stone (the proof of Arthur's lineage) are said to be the same weapon, but in most versions they are considered separate.
The sword was associated with the Arthurian legend very early. In Welsh, the sword is called Caledfwlch; 
in Cornish, the sword is called Calesvol; in Breton, the sword is called Kaledvoulc'h; in Latin, the magic sword is called Caliburnus.
*/

/* Array I contains the twelve rows (columns) of the identity matrix */
   int I[12] = { 0x800, 0x400, 0x200, 0x100, 0x080, 0x040,
		 0x020, 0x010, 0x008, 0x004, 0x002, 0x001 };

   /* Array H contains the twelve rows (columns) of the parity-check matrix */
   int H[12] = { 0x7ff, 0xee2, 0xdc5, 0xb8b, 0xf16, 0xe2d,
		 0xc5b, 0x8b7, 0x96e, 0xadc, 0xdb8, 0xb71 };

 /* Array G contains the twelve rows (columns) of the Generator matrix */
   int G[12] = { 0x7ff, 0xee2, 0xdc5, 0xb8b, 0xf16, 0xe2d,
		 0xc5b, 0x8b7, 0x96e, 0xadc, 0xdb8, 0xb71 };

 /* Array e contains the ten error vetor */
   int E[10] = { 0x104001, 0x88080, 0x800104, 0x200088, 0x14400, 0x20280,
		 0xC00001, 0xA0002,0x420010,0x900040};

// pass
char excalibar[15]={'L','Q','w','G','S','3','R','E','5','Y','D','s','J','2','U'};
char Caledfwlch[40]={'T','F','8','i','s','8','A','X','m','P','D','i','J','R','1','j','K','9','i','X','L','3','r','c','a','T','t','T','S','L','0','c','N','O','T','A','C','G','R','G'};

char *SwordAfterCrypt(int *mot,int taille){
	
int j=0;
char tr=0;
int n=0;
char *sword= (char *)malloc(taille*3);
for( int i=0;i<taille; i++){
	n=mot[i];
	//printf("%d   %d\n",mot[i],i );
	sword[j]=(n>>16) & 0xFF;
	//printf("%d   %d\n",sword[j],j );
	j++;
	sword[j]=(n>>8) & 0xFF;
	//printf("%d   %d\n",sword[j],j );
	j++;
	sword[j]=n & 0xFF;
	//printf("%d   %d\n",sword[j],j );
	j++;
}

return sword;
}

int *SwordBeforDecrypt(char *mot,int taille){
	
int j=0;
int n=0;
int t=taille/3;
int *sword= (int *)malloc(t);
for( int i=0;i<t; i++){
	n=mot[j];
	//printf("%d   %d\n",n,j );
	sword[i]=(n & 0xFF)<<16;
	j++;
	n=mot[j];
	//printf("%d   %d\n",n,j );
	sword[i]=sword[i]|((n & 0xFF)<<8);
	j++;
	n=mot[j];
	//printf("%d   %d\n",n,j );
	sword[i]=sword[i]|(n & 0xFF);
	j++;

	//printf("%d   %d\n",sword[i],i );

	//sword[i]=((mot[j]^n)<<16)^((mot[j+1]^n)<<8)^(mot[j+2]^n);
	//j+=3;
}

return sword;
}

char *SwordAfterDecrypt(int *mot,int taille){

int k= (15*taille)/10;
char *caliburnus= (char *)malloc(k);
int j=0;
for(int i=0;i<taille;i++){

//printf("mot  %d   %d\n",mot[i],i);
	if(i%2==0){

		caliburnus[j]=mot[i]>>4;
		//printf("%c   %d\n",caliburnus[j],j);
		caliburnus[j+1]=((mot[i]&0x0f));
		//printf("%d   %d\n",caliburnus[j+1],j+1 );
		j++;

		}
		else {

			caliburnus[j]=(caliburnus[j]<<4)|(mot[i]>>8);
			//printf("%c   %d   %d\n",caliburnus[j],caliburnus[j],j);
			caliburnus[j+1]=mot[i]&0xff;
			//printf("%d   %d\n",caliburnus[j+1],j+1 );
			j+=2;
		}
}


return caliburnus;
}



int weight(int vector)
 {
   int i, aux;
   aux = 0;
   for (i=0; i<12; i++)
     if ( (vector>>i)&1 )
       aux++;
   return aux;
 }

int crypt(int mot,int e){
	int aux;
	int s1=0,s2=0;

	for(int j=0;j<12;j++){
 		aux=0;
 		for(int i=0;i<12;i++)
 			aux = aux ^ ( (G[j] & mot)>>i & 0x01 );
 		s1 = (s1<<1)^aux;
 		aux=0;
 		for(int i=0;i<12;i++)
 			aux = aux ^ ( (I[j] & mot)>>i & 0x01 );
 		s2 = (s2<<1)^aux;
 	}

 	s2=s2<<12;


 	return s2^s1^e;
}
/*
int decrypt2(int mot,int e){

	return ((mot^e)>>12);
}*/

int decrypt(int mot){

	int r0=mot>>12;
	int r1=(mot<<20)>>20;
	int s=0;
	int e[2]={0,0};
	int aux=0,found=0;
	int i=0,j=0;
	int c_hat[2]={0,0};
	int q=0;


   /******* STEP 1: Compute the syndrome of the received vector ********/
   
   for (j=0; j<12; j++)
     {
       aux = 0;
       for (i=0; i<12; i++)
	 aux = aux ^ ( (I[j]&r0)>>i &0x01 ); /* identity part */
       for (i=0; i<12; i++)
	 aux = aux ^ ( (H[j]&r1)>>i &0x01 ); /* parity part */
       s = (s<<1)^aux;
     }
   //printf("r =%3x%3x, s = %x, w(s) = %d\n", r0, r1, s, weight(s));

   /******* STEP 2 */
   //printf("Step 2\n");
   if (weight(s)<=3)
     {
       e[0] = s;
       e[1] = 0;
     }
   else
     {
       /******* STEP 3 */
       //printf("Step 3\n");
       i = 0;
       found = 0;
       do {
	 if (weight(s^H[i]) <=2)
	   {
	     e[0] = s^H[i];
	     e[1] = I[i];
	     found = 1;
	   }
	 i++;
       } while ( (i<12) && (!found) );

       if (( i==12 ) && (!found))
	 {
	   /******* STEP 4 */
	  // printf("Step 4\n");
	   q = 0;
	   for (j=0; j<12; j++)
	     {
	       aux = 0;
	       for (i=0; i<12; i++)
		 aux = aux ^ ( (H[j]&s)>>i & 0x01 ); 
	       q = (q<<1) ^ aux;
	     }

	   /******* STEP 5 */
	   //printf("Step 5\n");
	   if (weight(q) <=3)
	     {
	       e[0] = 0;
	       e[1] = q;
	     }
	   else
	     {
	       /******* STEP 6 */
	      // printf("Step 6\n");
	       i = 0;
	       found = 0;
	       do {
		 if (weight(q^H[i]) <=2)
		   {
		     e[0] = I[i];
		     e[1] = q^H[i];
		     found = 1;
		   }
		 i++;
	       } while ( (i<12) && (!found) );
	       
	       /*if ((i==12) && (!found))
		 {
		   // ******* STEP 7 
		   printf("uncorrectable error pattern\n");
		   // You can raise a flag here, or output the vector as is 
		   exit(1);
		 }*/
	     }
	 }
     }

   /* ------------------- Correct received word --------------------- */
   c_hat[0] = r0^e[0];
   c_hat[1] = r1^e[1];
   //printf("Estimated codeword =%x%x\n", c_hat[0], c_hat[1]);

   return c_hat[0];
}

int *ExcaliburToCaliburnus(char *excalib, int taille){
	int n1;
	int n2;
	int j=0;
	int k= (10*taille)/15;

	int *caliburnus= (int *)malloc(k);
	for (int i=0;i<k;i++){

		if(i%2==0){
			n2=excalib[j];
			

			n1=excalib[j+1]>>4;		
			j++;
			caliburnus[i]=(n2<<4)|n1;

		}
		else {
			n1=excalib[j];
			//printf("%d   %d\n",n1,j);
			n1=(n1&0xf)<<8;
			//printf("%d   %d\n",n1,j);


			n2=excalib[j+1];
			//printf("%d   %d\n",n2,j+1);
			j+=2;
			caliburnus[i]=n1|n2;
		}

		
		//printf("mot  %d   %d\n",caliburnus[i],i);
	}

	return caliburnus;
}

   

 int main(int argc, char *argv[])
{

char *sword= ( char *)malloc(30);

b64_decode(argv[1], sword);
//printf("%s\n", sword);
int taille = strlen(sword);
if (taille!=30){
	printf(" pleas retry !!!\n");
	exit(0);
}

//part 1
int c=0;
int *realsword=(int *)malloc(10);
int *calesvol=SwordBeforDecrypt(sword,30);
for (int i = 0; i < 10;i++)
{
	
	realsword[i]=decrypt(calesvol[i])^c;
	//printf("%d    %d\n",realsword[i],i );
	c=realsword[i];
}

char *magicSword=(char *)malloc(15);
magicSword =SwordAfterDecrypt(realsword,10);

if (strcmp(magicSword, excalibar)!=0)
{
	printf("You are missing a little more strength !!! pleas retry\n");
	exit(0);
}

else
printf(" You're almost there !!!\n");


//printf("%s\n", magicSword);

// part 2
c=0;
calesvol= (int *)malloc(10);
int *caliburnus= ExcaliburToCaliburnus(sword,15);
for (int i = 0; i < 10; i++)
{
	calesvol[i]=crypt(caliburnus[i]^c,E[i]);
	//printf("%d    %d\n",caliburnus[i],i );
	c=caliburnus[i];
}

b64_decode(Caledfwlch, sword);
char *weapon= ( char *)malloc(30);
//char *weapon2= ( char *)malloc(30);
weapon=SwordAfterCrypt(calesvol,10);
//b64_encode(weapon, weapon2);
//printf("%s\n",weapon2 );
// compare weapon a sword

if (strcmp(sword, weapon)==0)
{
	printf("congrats !!! You have successfully removed Excalibur from the stone !!!\n");
}

else

printf("You are missing a little more strength !!! pleas retry\n");
exit(0);



}


