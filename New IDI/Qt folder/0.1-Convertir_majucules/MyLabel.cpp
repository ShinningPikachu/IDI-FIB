#include "MyLabel.h"
#include <iostream>
MyLabel::MyLabel(QWidget *parent=0):QLabel(parent)
{
}

double notaParcial = 0.0;
double notalab = 0.0;
double notaFinal = 0.0;
int numEntrega = 0;
bool altpress = false;
bool rpress = false;

double MyLabel::calcNota(){
    double notaextra = 0.0;
	if(notalab >= 4.5){
		if (numEntrega==4){
            notaextra = 1;
		}else if(numEntrega == 3){
            notaextra = 0.53;
		}else if(numEntrega == 2){
            notaextra = 0.22;
		}else if(numEntrega == 1){
            notaextra = 0.05;
		}
	}
    double nota = notaParcial*0.25 + notaFinal*0.5 + (notalab + notaextra) * 0.25;
    QFont font ("Arial", 20);
    font.setPointSize(20);
    setFont(font);
    if (nota < 5) setStyleSheet("color: red");
    else setStyleSheet("color: green");
    //if(notaFinal == 0.0) sentmarkneeded((5 - (notalab*0.25 + notaParcial*0.25))/0.5);
    //else sentmarkneeded(0.0);
    return nota;
}

void MyLabel::setTextMajuscules(QString s)
{
	QString str = s.toUpper();
	setText(str); // heredat de QLabel
}

void MyLabel::ChangeNotaParcial(double n){
	notaParcial = n;
    if(notaFinal == 0.0) sentmarkneeded((5 - (notalab*0.25 + notaParcial*0.25))/0.5);
    else sentmarkneeded(0.0);

    setText(QString::number(calcNota()));
}
void MyLabel::ChangeNotaFinal(double n){
	notaFinal = n;
    if(notaFinal == 0.0) sentmarkneeded((5 - (notalab*0.25 + notaParcial*0.25))/0.5);
    else sentmarkneeded(0.0);
    setText(QString::number(calcNota()));
}
void MyLabel::ChangeNotaLab(double n){
	notalab = n;
    if(notaFinal == 0.0) sentmarkneeded((5 - (notalab*0.25 + notaParcial*0.25))/0.5);
    else sentmarkneeded(0.0);
    setText(QString::number(calcNota()));
}
void MyLabel::ChangeEntregaMenysMes(int check){
    if(check == 2) numEntrega = numEntrega + 1;
    else numEntrega = numEntrega - 1;
    setText(QString::number(calcNota()));
}
void MyLabel::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_R){
        rpress = true;
    }else if(event->key() == Qt::Key_Alt){
        altpress = true;
    }
    if(rpress and altpress) Reset();
}

void MyLabel::keyReleaseEvent (QKeyEvent *event){
    if(event->key() == Qt::Key_R){
        rpress = false;
    }else if(event->key() == Qt::Key_Alt){
        altpress = false;
    }
}
void MyLabel::Reset(){
    notaParcial = 0.0;
    notalab = 0.0;
    notaFinal = 0.0;
    numEntrega = 0;
    setNotaParcial(notaParcial);
    setNotaFinal(notaFinal);
    setNotaLab(notalab);
    setEntrega1(false);
    setEntrega2(false);
    setEntrega3(false);
    setEntrega4(false);
}

