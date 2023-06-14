// test_couleur.cpp : Seuille une image en niveau de gris

#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char cNomImgLue[250];
  int nH, nW, nTaille,nR,nG,nB;
  
  int histoR[256];
  int histoB[256];
  int histoV[256];
   
   sscanf (argv[1],"%s",cNomImgLue) ;
   
   OCTET *ImgIn, *ImgOut;
   
  lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
   nTaille = nH * nW;
  
   int nTaille3 = nTaille * 3;
   allocation_tableau(ImgIn, OCTET, nTaille3);
   lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
   allocation_tableau(ImgOut, OCTET, nTaille3);
	

   for(int i=0; i<256;i++){
    histoR[i]=0;
    histoB[i]=0;
    histoV[i]=0;
   }

   for (int i=0; i < nTaille3; i+=3)
     {
       nR = ImgIn[i];
       nG = ImgIn[i+1];
       nB = ImgIn[i+2];
       histoR[nR]++;
       histoB[nB]++;
       histoV[nG]++;
  }

   for(int i=0; i<256;i++){
    printf("%d %d %d %d\n", i, histoR[i], histoB[i], histoV[i]);
   }

   return 1;
}
