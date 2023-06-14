// filtre_flou1.cpp : floute l'image

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcrite[250];
  int nH, nW, nTaille, nTailleGrey,k;
  
  if (argc != 4) 
     {
       printf("Usage: ImageIn.pgm ImageOut.pgm k \n"); 
       exit (1) ;
     }
   
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);
    sscanf (argv[3],"%d",&k);

   OCTET *ImgIn, *ImgOut;
   
   lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &nH, &nW);
   nTaille = (nH * nW) ;

   allocation_tableau(ImgIn, OCTET, nTaille);
   lire_image_pgm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille);

  
  for (int i=0; i < nTaille; i++){

    int r = ImgIn[i] + k ;
    if (r<0) r = 0; else if (r>255) r = 255;
    ImgOut[i] = r;
    

  }

   ecrire_image_pgm(cNomImgEcrite, ImgOut,  nH, nW);
   free(ImgIn); free(ImgOut);

   return 1;
}


