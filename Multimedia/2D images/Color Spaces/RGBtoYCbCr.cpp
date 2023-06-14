// filtre_flou1.cpp : floute l'image

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250], cNomImgEcriteY[250], cNomImgEcriteCr[250], cNomImgEcriteCb[250];
  int nH, nW, nTaille;
  
  if (argc != 5) 
     {
       printf("Usage: ImageIn.ppm ImageOutY.pgm ImageOutCr.pgm ImageOutCb.pgm\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   sscanf (argv[2],"%s",cNomImgEcriteY);
   sscanf (argv[3],"%s",cNomImgEcriteCr);
   sscanf (argv[4],"%s",cNomImgEcriteCb);

   OCTET *ImgIn, *ImgY, *ImgCr, *ImgCb;
   
   lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = (nH * nW) ;

   allocation_tableau(ImgIn, OCTET, nTaille*3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   
   allocation_tableau(ImgY, OCTET, nTaille);
   allocation_tableau(ImgCr, OCTET, nTaille);
   allocation_tableau(ImgCb, OCTET, nTaille);

  
  for (int i=0; i < nTaille; i++){
    int nr = ImgIn[i*3];
    int nv = ImgIn[i*3+1];
    int nb = ImgIn[i*3+2];
    int Y = 0.299 * nr + 0.587 * nv + 0.114 * nb;
    int Cb = - 0.1687 * nr - 0.3313 * nv + 0.5 * nb + 128;
    int Cr = 0.5 * nr - 0.4187 * nv - 0.0813 * nb + 128;
    ImgY[i] = Y;
    ImgCb[i] = Cb;
    ImgCr[i] = Cr;

  }
  ecrire_image_pgm(cNomImgEcriteY, ImgY,  nH, nW);
  ecrire_image_pgm(cNomImgEcriteCr, ImgCr,  nH, nW);
  ecrire_image_pgm(cNomImgEcriteCb, ImgCb,  nH, nW);
  free(ImgIn); free(ImgY); free(ImgCr); free(ImgCb);

   return 1;
}


