//action che scatta ogni 0.5 sec
//passo il puntatore a snake e le coordinate x e y dell'elemento puntato dal puntatore

//suppngo che il quadrato in cui si muove snake sia un 15x15
int ALTEZZA = 15;
int LUNGHEZZA = 15;

// -------------------------------------|
// 	1 2 3 4 5 6 7 8 9 10 11 12 13 14 15	|
// 1									|
// 2									|
// 3									|
// 4									|
// 5									|
// 6									|
// 7									|
// 8									|
// 9									|
// 10									|
// 11									|
// 12									|
// 13									|
// 14									|
// 15									|
// -------------------------------------|

//DEFINE
int UP = 1;
int DOWN = 2;
int LEFT = 3;
int RIGHT = 4;

//definisco la posizione di snake come una struttura di x e y
struct Position {
	int x = 0;
	int y = 0;
};

void moving(int* snake, Position currentPosition) {
	//utilizzare una funzione che permette di scegliere randomicamente un numero da 1 a 4 compresi
	int direction = rand(1,4); //NON PENSO SIA QUESTA

	if (direction==1) {

		//dobbiamo salire: x resta uguale e y si decrementa di 1
		if ((currentPosition.y)-1==0) {
			currentPosition.y = ALTEZZA;
		} else 
			currentPosition.y--;

	} else if (direction==2) {

		//dobbiamo scendere: x resta uguale e y si incrementa di 1
		if ((currentPosition.y)+1==ALTEZZA+1) {
			currentPosition.y = 0;
		} else 
			currentPosition.y++;

	} else if (direction==3) {

		//dobbiamo andare a sx: y resta uguale e x si decrementa di 1
		if ((currentPosition.x)-1==0) {
			currentPosition.x = LUNGHEZZA;
		} else 
			currentPosition.x--;

	} else if (direction==4) {

		//dobbiamo andare a dx: y resta uguale e x si incrementa di 1
		if ((currentPosition.x)+1==LUNGHEZZA+1) {
			currentPosition.x = 0;
		} else 
			currentPosition.x++;
	}

	return;
}

void eating(int length) {
	length++;
	newBodyPart(length, currentPosition);
	return;
}

void newBodyPart(int length, Position currentPosition) {
	
}






