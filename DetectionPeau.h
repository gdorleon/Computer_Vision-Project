
/*
   Etudiant:      Ginel DORLEON
                  P20 IFI 2016
   Cours   :  Vision par Ordinateur
              TP1

*/


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/basic_string.h>


using namespace cv;
using namespace std;

void histogr_Peau(Mat &histogr_Peau, double &nbPixelPeau, int nb_espace_couleur, int number_image_base);
void histogr_NonPeau(Mat &histogr_NonPeau,double &nbPxNonPeau, int nb_espace_couleur, int number_image_base);
void normalisationHistogramme(Mat &histogramme, double nbPx);
void lissage(Mat &histogramme,int nb_espace_couleur,int type );

void detection_de_la_Peau (Mat &imageResult, const Mat histogr_Peau, const Mat histogr_NonPeau, string imageFileDect, int nb_espace_couleur, bool write);
