#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "QString"
#include "QDebug"
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>
#include <QMovie>
#include <QGraphicsScene>
#include <QMouseEvent>

#include <stdio.h>
#include <iostream>

#include "comunicationconstants.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     ui->labelImageFond->setPixmap(QPixmap (":/carteFondFlou.jpg"));
     ui->graphicsView->setGeometry(GWposX,GWposY,434,434);
     scene = new QGraphicsScene(this);
        scene->setSceneRect(QRectF(0,0,434,434));
        ui->graphicsView->setScene(scene);
       // fond = scene->addPixmap(QPixmap(":/Damiereau.jpg"));
        ui->textChat->setText(""); // descativation du chat tant que pas connecté
     ui->textChat->setEnabled(false);
     ui->lineEditChat->setText("");
     ui->lineEditChat->setEnabled(false);
     ui->pushButtonOKTchat->setEnabled(false);
     ui->labelTchatDisable->setText("Tchat désactivé! \n \n Veuillez d'abord \n vous connecter !");
     state=0;
   // descativation du chat tant que pas connecté
}
MainWindow::~MainWindow()
{
    delete ui;
}
//**********************************************************************************
//**********************************************************************************
//**********************************************************************************
int connecte=0;

//*****Quitter*********************************************************
//A partir du menu
void MainWindow::on_actionQuitter_triggered ()
    {
    int reponseQuitter = QMessageBox::information(this, "Fermeture du programme", "Voulez vous fermer le programme et quitter la partie en cours?", QMessageBox::Yes | QMessageBox::No);
       if (reponseQuitter == QMessageBox::Yes)
       {
           connexion->send(GiveUpHeader+this->nomJoueur);
           isClosed=true;
        close();
       }
       else if (reponseQuitter == QMessageBox::No)
       {
       }
    }
//Depuis la croix
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(isClosed==false)
    {
        int reponseQuitter = QMessageBox::information(this, "Fermeture du programme", "Voulez vous fermer le programme et quitter la partie en cours?", QMessageBox::Yes | QMessageBox::No);
           if (reponseQuitter == QMessageBox::Yes)
           {
               connexion->send(GiveUpHeader+this->nomJoueur);
               event->accept();

           }
           else if (reponseQuitter == QMessageBox::No)
           {event->ignore();
               close();
           }
    }
   if(isClosed==true)
   {
       event->accept();
   }
}

//********************************************************************
//*************Nouvelle partie*******************************



void MainWindow::on_actionNewGame_triggered()
    {
    bool ok;
    nomJoueur = QInputDialog::getText(this, tr("Nom du joueur"), tr("Votre nom :"), QLineEdit::Normal,QDir::home().dirName(), &ok);
    ui->actionNewGame->setEnabled(false);
    ui->actionAbandon->setEnabled(true);
    ui->graphicsView->setEnabled(true);
    fond = scene->addPixmap(QPixmap(":/merCarre.jpg"));
    draw();
    QString adresseIP = QInputDialog::getText(this, tr("Adresse IP de connexion"), tr("Veuillez entrer l'adresse IP pour vous connecter : "), QLineEdit::Normal);
    qDebug()<<adresseIP;
    QString portConnexion = QInputDialog::getText(this, tr("Port de connexion"), tr("Veuillez entrer le port pour vous connecter : "), QLineEdit::Normal);
    qDebug()<<portConnexion;
    connexion = new client(adresseIP.toStdString(), portConnexion.toInt());
    ui->actionConnexion->setEnabled(false);
    connect(connexion, SIGNAL(serverError(QString)), this,SLOT(serverError(QString)));
    connect(connexion, SIGNAL(tchatRecive(QString)), this , SLOT(writeInTchat(QString)));
    connecte=1;
    ui->textChat->setText("");
    ui->textChat->setEnabled(true);
    ui->lineEditChat->setText("");
    ui->lineEditChat->setEnabled(true);
    ui->pushButtonOKTchat->setEnabled(true);
    ui->labelTchatDisable->setText("");
    }

//**************************************************************

//*************Abandon de la partie********************************
void MainWindow::on_actionAbandon_triggered()
    {
    int reponseAbandon = QMessageBox::information(this, "Abandon de la partie ?", "Voulez vraiment abandonner la partie en cours? Toutes vos données seront perdues.", QMessageBox::Yes | QMessageBox::No);
       if (reponseAbandon == QMessageBox::Yes)
       {
         connexion->send(GiveUpHeader+this->nomJoueur);
         ui->actionNewGame->setEnabled(true);
         ui->actionAbandon->setEnabled(false);
         ui->graphicsView->setEnabled(false);
         fond = scene->addPixmap(QPixmap(":/mer.gif"));
         QMessageBox::information(this, "Fin de la partie", "Vous avez abandonné la partie");
       }
    }
//**********Entrer du texte dans le tchat*******************************
void MainWindow::on_pushButtonOKTchat_clicked ()
   {
    connexion->send(MessageHeader+nomJoueur+" : "+ui->lineEditChat->toPlainText());
    ui->lineEditChat->setText("");
   }
//*******************Ecriture des messages dans le tchat*****************************************************
void MainWindow::writeInTchat(QString message)
{
    ui->textChat->append(message);
}

//********* Désactiver le Tchat **********************************************
void MainWindow::on_actionTchatDisable_triggered()
{
     ui->textChat->setText("");
     ui->textChat->setEnabled(false);
     ui->lineEditChat->setText("");
     ui->lineEditChat->setEnabled(false);
     ui->pushButtonOKTchat->setEnabled(false);
     ui->labelTchatDisable->setText("Tchat désactivé !");
     ui->actionRactiveTchat->setEnabled(true);
     ui->actionTchatDisable->setEnabled(false);
}
//**************************************************************************

//****************** Réactiver le tchat*************************************
void MainWindow::on_actionRactiveTchat_triggered()
{
    ui->textChat->setText("");
    ui->textChat->setEnabled(true);
    ui->lineEditChat->setText("");
    ui->lineEditChat->setEnabled(true);
    ui->pushButtonOKTchat->setEnabled(true);
    ui->labelTchatDisable->setText("");
    ui->actionRactiveTchat->setEnabled(false);
    ui->actionTchatDisable->setEnabled(true);
}

//*************************************************************************
//Gestion des erreurs de communication Reseau
void MainWindow::serverError(QString error)
{
    QMessageBox::critical(this, "Erreur",error, QMessageBox::Ok);
}
//**********************Clic de la souris********************
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QPointF pt = ui->graphicsView->mapToScene(e->pos());//récupération de la position
    pressedX=((int)pt.x()-GWposX-1)/27;
    pressedY=((int)pt.y()-32-GWposY-1)/27;
    cout<<"positionX "<<pressedX<<endl;
    cout <<"positionY "<<pressedY<<endl;

    if(pressedX>15||pressedX<0||pressedY>15||pressedY<0)
        return;

    //Gestion de l'attaque et/ou du placement des bateaux
    switch(state){
    case STATE_ATWAR:

        break;
    case STATE_PREPARATION:
        break;
    }


}

void MainWindow::draw()
{
    QPen pen;
    pen.setColor(QColor(0,0,0));
    for(int a=0; a<16; a++)
    {
            scene->addRect(a*27+1,0,28,432,pen);
            scene->addRect(0,a*27+1,432,28,pen);
    }

}
