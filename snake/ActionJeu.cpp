
#include "ActionJeu.h"
#include"ConsoleKeyReader.h"
#include "Console"
#include "ElapsedTimer.h"

using namespace std;

ActionJeu::ActionJeu()
{
}

ActionJeu::~ActionJeu()
{
}

void ActionJeu::lancer()
{
	ElapsedTimer<> chronometre;
	chronometre.start();
	partieEnCours= true;
	while (partieEnCours) {
		if (chronometre.elapsedSeconds() >= 0.1) {
			obtenirClavier();
			partieEnCours = traiter(chronometre.restartSeconds());
			afficherJeu();
		}

	}


}

void ActionJeu::obtenirClavier()
{
	ConsoleKeyReader & reader{ Console::getInstance().keyReader() };
	reader.installFilter(new ConsoleKeyFilterUp);			// Filtre qui enleve les touche qui monte
	reader.installFilter(new ConsoleKeyFilterModifiers);	//Filtre qui enleve les modifier "Ctrl + touche"
	ConsoleKeyReader::KeyEvents keyEvents;								//Classe event de la classe KeyReader

	keyEvents.clear();

	reader.read(keyEvents);
	//mToucheClavier = 0;

	

	for (auto event : keyEvents) {
		if (event.keyV() == char(37)) {				//Si on a peser la fleche gauche
			mToucheClavier = 37;
		}

		else if (event.keyV() == char(38)) {		//Si on a peser la fl�che haut
			mToucheClavier = 38;
		}

		else if (event.keyV() == char(39)) {		//Si on a peser la fleche droite
			mToucheClavier = 39;
		}

		else if (event.keyV() == char(40)) {			//Si on a peser la fleche bas
			mToucheClavier = 40;
		}
	}

	
}

bool ActionJeu::traiter(double tempsEcoule)
{

	if (mSalazar.tete().x() == mPomme.position().x() && mSalazar.tete().y() == mPomme.position().y())
	{
		mSalazar.setMange(true);
	}

	if (mToucheClavier == 37)
	{
		mSalazar.bougeAGauche();

	}
	else if (mToucheClavier == 38)
	{
		mSalazar.bougeEnHaut();
	}
	else if (mToucheClavier == 39)
	{
		mSalazar.bougeADroite();
	}
	else if (mToucheClavier == 40)
	{
		mSalazar.bougeEnBas();
	}

	if (mSalazar.tete().x() <= 4 || mSalazar.tete().x() >= 55 || mSalazar.tete().y() <= 4 || mSalazar.tete().y() >= 55)
	{
		return false;
	}

	if (mSalazar.mange() == true)
	{
		mCompteur += 1;
		mSalazar.ajouterPoint(mPomme.position());
		mSalazar.setMange(false);

		Point temp(mPomme.aleatoire(5, 55), mPomme.aleatoire(5, 55));
		mPomme.setPosition(temp);
	}



	return true;
}

void ActionJeu::afficherJeu()
{
	ConsoleWriter & writer{ Console::getInstance().writer() };



	writer.createImage("background");

	size_t a{5};
	size_t b{ 50 };
	size_t c{ 57 };
	size_t d{ 16 };
	size_t e{ 22 };



	writer.image("background").fill(a, a, b, b,char(219), ConsoleColor::by + ConsoleColor::ty);
	writer.image("background").fill(e, c, d, a, char(219), ConsoleColor::bw + ConsoleColor::tw);
	writer.image("background").drawText(e + 1, c + 2, "Point: " +to_string(mCompteur) , ConsoleColor::bw + ConsoleColor::tk, true);
	writer.createImage("imageJeu");
	writer.push("background", "imageJeu");

	writer.image("imageJeu").drawPoint(mPomme.position().x(), mPomme.position().y(), mPomme.dessin(), mPomme.couleur());

	for (Point & p : mSalazar.corps())
	{
		writer.image("imageJeu").drawPoint(p.x(), p.y(), mSalazar.forme(), mSalazar.couleur());
		writer.push("imageJeu");
	}

	

	if (partieEnCours == false)
	{

		size_t px{ 21 };
		size_t py{28 };
		size_t width{ 18 };
		size_t heigth{ 5 };

		writer.createImage("finDePartie");
		writer.image("finDePartie").fill(px, py, width, heigth, char(219), ConsoleColor::by + ConsoleColor::ty);
		writer.image("finDePartie").drawText(px+4, py+2, "GAME OVER!", ConsoleColor::by + ConsoleColor::tk, true);
		writer.push("finDePartie");
	}

}

int ActionJeu::compteur()
{
	return mCompteur;
}

void ActionJeu::setCompteur(int compteur)
{
	mCompteur = compteur;
}