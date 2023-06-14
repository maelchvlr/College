#include <stdio.h>
#include "image_ppm.h"

int main(int argc, char* argv[])
{
  char nomImageY[250], nomImageCr[250], nomImageCb[250], nomImageRgb[250];
  int nH, nW, nTaille;
  
  if (argc != 5) 
     {
       printf("Usage: ImageIn.ppm ImageOutY.pgm ImageOutCr.pgm ImageOutCb.pgm\n"); 
       exit (1) ;
     }
   
   sscanf (argv[1],"%s",nomImageY) ;
   sscanf (argv[2],"%s",nomImageCr);
   sscanf (argv[3],"%s",nomImageCb);
   sscanf (argv[4],"%s",nomImageRgb);

   OCTET *ImgRGB, *ImgY, *ImgCr, *ImgCb;
   
   lire_nb_lignes_colonnes_image_pgm(nomImageY, &nH, &nW);
   nTaille = (nH * nW) ;

    allocation_tableau(ImgY, OCTET, nTaille);
    allocation_tableau(ImgCr, OCTET, nTaille);
    allocation_tableau(ImgCb, OCTET, nTaille);

   lire_image_pgm(nomImageY, ImgY, nH * nW);
   lire_image_pgm(nomImageCr, ImgCr, nH * nW);
   lire_image_pgm(nomImageCb, ImgCb, nH * nW);
   

   allocation_tableau(ImgRGB, OCTET, nTaille*3);

  
  for (int i=0; i < nTaille*3; i+=3){

    int Y = ImgY[i/3];
    int Cr = ImgCr[i/3];
    int Cb = ImgCb[i/3];

    int nr = Y + 1.402 * (Cr - 128);
    int nv = Y - 0.34414 * ( Cb - 128) - 0.714414 * (Cr - 128);
    int nb = Y + 1.772 * (Cb - 128);
    
    if (nr<0) nr = 0; else if (nr>255) nr = 255;
    if (nv<0) nv = 0; else if (nv>255) nv = 255;
    if (nb<0) nb = 0; else if (nb>255) nb = 255;

    ImgRGB[i] = nr;
    ImgRGB[i+1] = nv;
    ImgRGB[i+2] = nb;

  }
  ecrire_image_ppm(nomImageRgb, ImgRGB,  nH, nW);
  free(ImgRGB); free(ImgY); free(ImgCr); free(ImgCb);

   return 1;
}


