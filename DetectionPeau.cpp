/*
                  Etudiant:      Ginel DORLEON
                                 P20 IFI 2016
                 Cours   :  Vision par Ordinateur
                            TP1

*/

//#include "DetectionPeau.h"
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



//
// Espacedenometdefinitiondesdifferentesfonctions@1Starthere
//

// Definition des espaces de nom
using namespace cv;
using namespace std;

//Apprentissage: fonction pour calculer l'histogramme de l'image  peau
void histogra_Peau(Mat &histogr_Peau, double &nbPixelPeau, int nb_espace_couleur, int number_image_base);
//Apprentissage: fonction pour calculer l'histogramme de l'image  non peau
void histogra_NonPeau(Mat &histogr_NonPeau,double &nbPxNonPeau, int nb_espace_couleur, int number_image_base);
//Apprentissage: fonction pour normaliser l'hstogramme
void normalisationHistogramme(Mat &histogramme, double nbPx);
//Apprentissage: fonction pour faire le lissage de l'hstogramme
void lissage(Mat &histogramme,int nb_espace_couleur,int type );
//Apprentissage: fonction pourla detection de la peau suivant l'image
void detection_de_la_Peau (Mat &imageResult, const Mat histogr_Peau, const Mat histogr_NonPeau, string imageFileDect, int nb_espace_couleur, bool write);

/*
 *
 */

//Ecriture@defonctionprincipale
int main(int argc, char** argv) {

	//Initialisation: assigner le parametre -p --> le nombre de octet pour representer les images


	double nbPixelPeau (0); //variable contenant le nb de pixel peau
	double nbPxNonPeau (0);
	int nb_espace_couleur (4); // valeur de p utilisee par defaut
	int nbImageBase (25); // nombre d'image dans la base

	// construction du modele histogramme pour une image peau avec la fonction Mat histogr_Peau
	Mat histogr_Peau;
	histogra_Peau(histogr_Peau,nbPixelPeau, nb_espace_couleur, nbImageBase);

	// construction du modele histogramme pour une image non-peau
	Mat histogr_NonPeau;
	histogra_NonPeau(histogr_NonPeau,nbPxNonPeau, nb_espace_couleur, nbImageBase);

	// la fonction ci permet la normalisation de l'histogramme Peau 
	normalisationHistogramme(histogr_Peau, nbPixelPeau);
        // la fonction ci permet la normalisation de l'histogramme Peau 
	normalisationHistogramme(histogr_NonPeau, nbPxNonPeau);

	//Lissage afin d'ameliorer les qualites/la detectionl'histogramme
	lissage(histogr_Peau, nb_espace_couleur,1);
	lissage(histogr_NonPeau, nb_espace_couleur,2);

	// Faire l'evaluation des resultat (la phase d'apprentissage)
	float tauxBienDetecte;
	double nbPixelPeauBienDect = 0;


	// ********//

	if(argc==2){

		string fileName = (argv[1]);
		Mat img_Peau_base = imread(argv[1], CV_LOAD_IMAGE_COLOR);
		Mat img_Resultat;

					detection_de_la_Peau(img_Resultat, histogr_Peau, histogr_NonPeau, fileName, nb_espace_couleur,  false);
					imshow("Image_detecte", img_Resultat);
					//comparer

					double pxPeauBienDect (0);
					double pxPeauRef (0);
					double pxPeauDect (0);

					vector<Mat> planesResultBase;
					vector<Mat> planesPeauBase;
					split(img_Resultat, planesResultBase);
					split(img_Peau_base, planesPeauBase);

for (int i = 0; i < img_Resultat.rows; i++)
{
for (int j = 0; j < img_Resultat.cols; j++)
{
//ici on demande au programme de calculer le nombre de pixel Peau detecte dans chaque image apprentissage
if ((planesResultBase[0].at<uchar>(i,j) != 0) || (planesResultBase[1].at<uchar>(i,j) != 0) || (planesResultBase[2].at<uchar>(i,j) != 0)) 
{  pxPeauDect ++; 
}
//et maintenant on calcule le nombre de pixel bien detecte dans chaque image apprentissage
if ((planesPeauBase[0].at<uchar>(i,j) != 0) || (planesPeauBase[1].at<uchar>(i,j) != 0) || (planesPeauBase[2].at<uchar>(i,j) != 0)) {
								pxPeauRef ++;
if ((planesResultBase[0].at<uchar>(i,j) != 0) || (planesResultBase[1].at<uchar>(i,j) != 0) || (planesResultBase[2].at<uchar>(i,j) != 0)) {
									pxPeauBienDect ++;
								}
							}

						}
					}
			cout << "Résultat de "<< fileName  <<endl;
			
			// La fonction suivante permet la detection de la peau
	 Mat imgDetectResult;
			cout << "Le pourcentage bien detecte est: " << (pxPeauBienDect / pxPeauRef) * 100<<"%" << endl;
	}
		else{  // Generer les resultats dans un fichier txt
	string apprentissageFileName = "ResultatApprentissage.txt";
	ofstream apprentissageTextFile(apprentissageFileName.data());

	if (apprentissageTextFile.is_open())
	{
		for (int l = 0; l < nbImageBase; l++) {
			stringstream fileImageBase;
			fileImageBase << "apprentissage/" << l + 1 << ".jpg";
			string fileName = fileImageBase.str();
			Mat img_Resultat;
			detection_de_la_Peau(img_Resultat, histogr_Peau, histogr_NonPeau, fileName, nb_espace_couleur,  false);
			
                        //comparer les images de la base non peau
			stringstream fileimg_Peau_base;
			fileimg_Peau_base << "images_peau/" << l + 1 << ".jpg";
			string fileNamePeauBase = fileimg_Peau_base.str();
			Mat img_Peau_base = imread(fileNamePeauBase, -1);
			if (!img_Peau_base.data) { // si on ne peut pas charger l'image
				cout << "Chargement impossible de l'image " << fileNamePeauBase << "..." << endl;
				continue;
			}
			if ((img_Resultat.cols != img_Peau_base.cols) || (img_Resultat.rows != img_Peau_base.rows)) {
				
				continue;
			}
			double pxPeauBienDect (0);
			double pxPeauRef (0);
			double pxPeauDect (0);

			vector<Mat> planesResultBase;
			vector<Mat> planesPeauBase;
			split(img_Resultat, planesResultBase);
			split(img_Peau_base, planesPeauBase);
			for (int i = 0; i < img_Resultat.rows; i++) {
				for (int j = 0; j < img_Resultat.cols; j++) {
					//calculer le nombre de pixel Peau detecte dans chaque image apprentissage
					if ((planesResultBase[0].at<uchar>(i,j) != 0) || (planesResultBase[1].at<uchar>(i,j) != 0) || (planesResultBase[2].at<uchar>(i,j) != 0)) {
						pxPeauDect ++;
					}
					//calculer le nombre de pixel bien detecte dans chaque image apprentissage
					if ((planesPeauBase[0].at<uchar>(i,j) != 0) || (planesPeauBase[1].at<uchar>(i,j) != 0) || (planesPeauBase[2].at<uchar>(i,j) != 0)) {
						pxPeauRef ++;
						if ((planesResultBase[0].at<uchar>(i,j) != 0) || (planesResultBase[1].at<uchar>(i,j) != 0) || (planesResultBase[2].at<uchar>(i,j) != 0)) {
							pxPeauBienDect ++;
						}
					}
				}
			}
			//calculer le nombre de pixels bien detecte dans tous les images apprentissage
			nbPixelPeauBienDect += pxPeauBienDect;
			//enregistrer dans le fichier text pour chaque image apprentissage
			apprentissageTextFile << fileName << '\r';
			apprentissageTextFile << "% Taux Pixel Peau bien detect: " << (pxPeauBienDect / pxPeauRef) * 100 << "%." << '\r';
			apprentissageTextFile << "% taux Pixel Peau correct: " << (pxPeauBienDect / pxPeauDect) * 100 << "%." << '\r';
		}
		apprentissageTextFile.close();
	}

	tauxBienDetecte = (nbPixelPeauBienDect / nbPixelPeau) * 100;
	cout << "Pourcentage bien detecte: " << tauxBienDetecte << "%." << endl;
	//Etape 4: detection de la peau
	Mat imgDetectResult;
	
	
	cout << "Le nombre de Pixel Peau: " << nbPixelPeau << endl;
	cout << "Le nombre de Pixel Non Peau: " << nbPxNonPeau << endl;
	
	cout << "Le Nb Pixel bien detecte est: " << nbPixelPeauBienDect << endl;
	
		}
	waitKey();

	return (EXIT_SUCCESS);
}
// On commence par calculer l'hstogramme de nos images apres la segmentation manuelle effectuee.
void histogra_Peau(Mat &histogr_Peau,double &nbPixelPeau, int nb_espace_couleur, int number_image_base) {

	int taille = pow(2, nb_espace_couleur);
	histogr_Peau = Mat::zeros(taille, taille, CV_64FC1);
	Mat imageHist = Mat::zeros(taille, taille, CV_8UC3);
	for (int l = 0; l < number_image_base; l++) {
		stringstream filename;
		filename << "images_peau/" << l + 1 << ".jpg";
		string fname = filename.str();

		
                //chargement de l'image
                 Mat image_peau = imread(fname, -1);
		if (!image_peau.data) {  //si ca echoue ??
			cout << "chargement de l'image a echoue " << fname << "..." << endl;
			continue;
		}

		//Conversion des espaces de couleurs RVB --> L*A*B
                // Le code de onversion CV_BGR2Lab dans l'espace lab . Dans chaque cas, nous convertissons l'ensemble des images de
               // l'espace de couleurs RGB à l’espace de couleur Lab
                 Mat imageLAB;
		cvtColor(image_peau, imageLAB, CV_RGB2Lab);
		vector<Mat> planesLAB;
		split(imageLAB, planesLAB);
		//reduire l'espace de couleur
		for (int i = 0; i < imageLAB.rows; i++) {
			for (int j = 0; j < imageLAB.cols; j++) {
				for (int p = 1; p < 3; p++) {
					planesLAB[p].at<uchar>(i,j) = planesLAB[p].at<uchar>(i,j) / pow(2, (8 - nb_espace_couleur));
				}
			}
		}
		merge(planesLAB, imageLAB);
		vector<Mat> planesPeau;
		split(image_peau, planesPeau);

		//Construction de la matrice qui va contenir l'histogramme peau
		for (int i = 0; i < imageLAB.rows; i ++) {
			for (int j = 0; j < imageLAB.cols; j++) {

				if (planesPeau[0].at<uchar>(i,j) == 0 && planesPeau[1].at<uchar>(i,j) == 0 && planesPeau[2].at<uchar>(i,j) == 0) {
					continue;
				}
				else {
					nbPixelPeau++;
					int x = planesLAB[1].at<uchar>(i,j);
					int y = planesLAB[2].at<uchar>(i,j);
					histogr_Peau.at<double>(x,y) = histogr_Peau.at<double>(x,y) + 1;
				}
			}
		}
	}
	//Affichage de l'histogramme de peau
	double maxHist;
	minMaxLoc(histogr_Peau, NULL, &maxHist);

	int ratio = maxHist/256;
	vector<Mat> planesHist;
	split(imageHist, planesHist);
	for (int i = 0; i < imageHist.rows; i++) {
		for (int j = 0; j < imageHist.cols; j++) {
			planesHist[0].at<uchar>(i,j) = planesHist[1].at<uchar>(i,j) = planesHist[2].at<uchar>(i,j) = histogr_Peau.at<double>(i,j) / ratio;
		}
	}
	merge(planesHist, imageHist);
	Mat imageResize;
	resize(imageHist, imageResize, cvSize(256, 256));
	imwrite("histogr_Peau.png", imageResize); //Sauvergarde de l'histo dans la base
	namedWindow("Histogramme Peau", 1);
	imshow("Histogramme Peau", imageResize);// Affichage de l'hsto a l'ecran
	


}

// // calculer l'hstogramme des images non-peau apres la segmentation manuelle effectuee.
void histogra_NonPeau(Mat &histogr_NonPeau,double &nbPxNonPeau, int nb_espace_couleur, int number_image_base) {
	int sizeHist = pow(2, nb_espace_couleur);
	histogr_NonPeau = Mat::zeros(sizeHist, sizeHist, CV_64FC1);
	Mat imageHist = Mat::zeros(sizeHist, sizeHist, CV_8UC3);

	for (int l = 0; l < number_image_base; l++) {
		stringstream filename;
		filename << "images_non_peau/" << l + 1 << ".jpg";
		string fname = filename.str();
		//Chargement de l'image
		Mat image_peau = imread(fname, -1);
		if (!image_peau.data) {
			cout << "Impossible de charger " << fname << "..." << endl;
			continue;
		}

		//Conversion de RVB --> LAB avec le code de conversion CV_RGB2Lab
		Mat imageLAB;
		cvtColor(image_peau, imageLAB, CV_RGB2Lab);
		vector<Mat> planesLAB;
		split(imageLAB, planesLAB);

		//réduction de l'espace des valeurs des pixels de 256*256*256 
		for (int i = 0; i < imageLAB.rows; i++) {
			for (int j = 0; j < imageLAB.cols; j++) {
				for (int p = 1; p < 3; p++) {
					planesLAB[p].at<uchar>(i,j) = planesLAB[p].at<uchar>(i,j) / pow(2, (nb_espace_couleur));
				}
			}
		}
		merge(planesLAB, imageLAB);
		vector<Mat> planesPeau;
		split(image_peau, planesPeau);

		//Construire de la matrice qui va contenir l'histogramme peau
		for (int i = 0; i < imageLAB.rows; i ++) {
			for (int j = 0; j < imageLAB.cols; j++) {
				if (planesPeau[0].at<uchar>(i,j) == 0 && planesPeau[1].at<uchar>(i,j) == 0 && planesPeau[2].at<uchar>(i,j) == 0) {
					continue;
				}
				else {
					nbPxNonPeau++;
					int x = planesLAB[1].at<uchar>(i,j);
					int y = planesLAB[2].at<uchar>(i,j);
					histogr_NonPeau.at<double>(x,y) = histogr_NonPeau.at<double>(x,y) + 1;
				}
			}
		}
	}

	//Affichage des differents histogrammes de peau
	double maxHist;
	minMaxLoc(histogr_NonPeau, NULL, &maxHist);
	int ratio = maxHist/256;
	vector<Mat> planesHist;
	split(imageHist, planesHist);
	for (int i = 0; i < imageHist.rows; i++) {
		for (int j = 0; j < imageHist.cols; j++) {
			planesHist[0].at<uchar>(i,j) = planesHist[1].at<uchar>(i,j) = planesHist[2].at<uchar>(i,j) = histogr_NonPeau.at<double>(i,j) / ratio;
		}
	}
	merge(planesHist, imageHist);
	Mat imageResize;
	resize(imageHist, imageResize, cvSize(256, 256));
	imwrite("histogr_NonPeau.png", imageResize); //Sauvegarder l'histogramme de non peau
	namedWindow("Histogramme Non Peau", 1);
	imshow("Histogramme Non Peau", imageResize); // affichage de l'histogramme de non peau
	
}

//Apprentissage: fonction pour normaliser l'hstogramme
void normalisationHistogramme (Mat &histogramme, double nbPx) {
	for (int i = 0; i < histogramme.rows; i++) {
		for (int j = 0; j < histogramme.cols; j++) {
			histogramme.at<double>(i,j) = histogramme.at<double>(i,j) / nbPx;
		}
	}
}

//Apprentissage: fonction de detection de l'hstogramme
void detection_de_la_Peau (Mat &imageResult, const Mat histogr_Peau, const Mat histogr_NonPeau, string imageFileDect, int nb_espace_couleur,   bool write) {
	Mat imageDectRGB = imread(imageFileDect, -1);
	if (!imageDectRGB.data) {
		cout << "Impossible de charger l' image " << imageFileDect << "..." << endl;
		return;
	}
	Mat imageDect;
	cvtColor(imageDectRGB, imageDect, CV_RGB2Lab);
	vector<Mat> planesDect;
	vector<Mat> planesDectRGB;
	split(imageDectRGB, planesDectRGB);
	split(imageDect, planesDect);
	for (int i = 0; i < imageDect.rows; i++) {
		for (int j = 0; j < imageDect.cols; j++) {
			for (int p = 1; p < 3; p++) {
				planesDect[p].at<uchar>(i,j) = planesDect[p].at<uchar>(i,j) / pow(2, (8 - nb_espace_couleur));
			}
		}
	}
	merge(planesDect, imageDect);

	imageResult = Mat::zeros(imageDectRGB.size(), imageDectRGB.type());
	vector<Mat> planesResult;
	split(imageResult, planesResult);
	for (int i = 0; i < imageDect.rows; i++) {
		for (int j = 0; j < imageDect.cols; j++) {


			//calculer la probabilite
/*
  Après l'obtention des histogrammes de couleurs de pixels de peau et de non-
  peau, nous calculons la probabilité conditionnelle pour chaque couleur sachant que
  cette couleur est une couleur de peau ou non
   Dans cette étape nous calculons pour chacun des pixels de notre base d'images et
selon chaque combinaison d'axes, la probabilité qu'il soit un pixel de peau

*/
			int x = planesDect[1].at<uchar>(i,j);
			int y = planesDect[2].at<uchar>(i,j);
			if  (histogr_Peau.at<double>(x,y) > histogr_NonPeau.at<double>(x,y)){
				for (int p = 0; p < 3; p++) {
					planesResult[p].at<uchar>(i,j) = planesDectRGB[p].at<uchar>(i,j);
				}
			}
		}
	}
	merge(planesResult, imageResult);
	imwrite(imageFileDect.append("Img_detecter.jpg"), imageResult);
	if (write) {
		namedWindow("Resultat Detection", 1);
		imshow("Resultat Detection", imageResult);

	}
}
  //   La fonction ci permet de faire le lissage de l'histogramme, afin d'ameliorer la qualite de la detection
void lissage(Mat &histogramme,int nb_espace_couleur, int type ){
	int sizeHist = pow(2, nb_espace_couleur);
	Mat a=histogramme;
	int max=0;
	//moyenne de la valeur du pixel avec les valeurs des voisins
	for (int i = 1; i < sizeHist; i++) {
	for (int j = 1; j < sizeHist; j++) {
	histogramme.at<double>(i,j)=    (a.at<double>(i-1,j-1)    +a.at<double>(i-1,j)    +a.at<double>(i-1,j+1)
					+a.at<double>(i,j-1)      +a.at<double>(i,j)      +a.at<double>(i,j+1)
					+a.at<double>(i+1,j-1)    +a.at<double>(i+1,j)    +a.at<double>(i+1,j+1))/9;

		}
	}
	//determination de la plus grande valeur max possible de l'histogramme
	for (int i = 0; i < sizeHist; i++)
		for (int j = 0; j < sizeHist; j++) {
			if (histogramme.at<double>(i,j) > max)
				max = histogramme.at<double>(i,j);
		}
	Mat histo;
	resize(histogramme, histo, cvSize(256, 256));
		
}
