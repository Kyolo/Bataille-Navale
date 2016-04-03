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
#include <QPixmap>
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

}
MainWindow::~MainWindow()
{
    delete ui;
}
//**********************************************************************************
//**********************************************************************************
//**********************************************************************************


//*****Quitter*********************************************************
//A partir du menu
void MainWindow::on_actionQuitter_triggered ()
    {
    int reponseQuitter = QMessageBox::information(this, "Fermeture du programme", "Voulez vous fermer le programme et quitter la partie en cours?", QMessageBox::Yes | QMessageBox::No);
       if (reponseQuitter == QMessageBox::Yes)
       {
           if(connecte==1)
           {
                connexion->send(GiveUpHeader+me->getName());
           }
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
               if(connecte==1)
               {
                    connexion->send(GiveUpHeader+me->getName());
               }
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
    me=new Joueur(nomJoueur);
    me->setPerson(true);
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
    ui->fondBateaux->setGeometry(initialPosBoatX,initialPosBoatY,475,95);
    ui->bateau5->setGeometry(initialPosBoatX+15,initialPosBoatY+10,125,25);
    ui->bateau5->setPixmap(QPixmap(":/bateau5.png"));
    ui->bateau41->setGeometry(initialPosBoatX+155,initialPosBoatY+10,100,25);
    ui->bateau41->setPixmap(QPixmap(":/bateau4.png"));
    ui->bateau42->setGeometry(initialPosBoatX+270,initialPosBoatY+10,100,25);
    ui->bateau42->setPixmap(QPixmap(":/bateau4.png"));
    ui->bateau31->setGeometry(initialPosBoatX+385,initialPosBoatY+10,75,25);
    ui->bateau31->setPixmap(QPixmap(":/bateau3.png"));
    ui->bateau32->setGeometry(initialPosBoatX+92,initialPosBoatY+45,75,25);
    ui->bateau32->setPixmap(QPixmap(":/bateau3.png"));
    ui->bateau33->setGeometry(initialPosBoatX+185,initialPosBoatY+45,75,25);
    ui->bateau33->setPixmap(QPixmap(":/bateau3.png"));
    ui->bateau21->setGeometry(initialPosBoatX+272,initialPosBoatY+45,50,25);
    ui->bateau21->setPixmap(QPixmap(":/bateau2.png"));
    ui->bateau22->setGeometry(initialPosBoatX+337,initialPosBoatY+45,50,25);
    ui->bateau22->setPixmap(QPixmap(":/bateau2.png"));
    ui->RAZBateaux->setGeometry(initialPosBoatX-19,initialPosBoatY-2,20,95);
    // bouton validation position bateaux -> ui->validBoatPosition->setGeometry(initialPosBoatX,initialPosBoatY,475,95);
    }

//**************************************************************

//*************Abandon de la partie********************************
void MainWindow::on_actionAbandon_triggered()
    {
    int reponseAbandon = QMessageBox::information(this, "Abandon de la partie ?", "Voulez vraiment abandonner la partie en cours? Toutes vos données seront perdues.", QMessageBox::Yes | QMessageBox::No);
       if (reponseAbandon == QMessageBox::Yes)
       {
         connexion->send(GiveUpHeader+me->getName());
         ui->actionNewGame->setEnabled(true);
         ui->actionAbandon->setEnabled(false);
         ui->graphicsView->setEnabled(false);
         fond = scene->addPixmap(QPixmap(":/mer.gif"));
         QMessageBox::information(this, "Fin de la partie", "Vous avez abandonné la partie");
         ui->fondBateaux->setEnabled(false);
       }
    }
//**********Entrer du texte dans le tchat*******************************
void MainWindow::on_pushButtonOKTchat_clicked ()
   {
    connexion->send(MessageHeader+me->getName()+" : "+ui->lineEditChat->text());
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
    int posX=(int)pt.x();
    int posY = (int)pt.y();
    updateLabelsPositions();
    if(boatIsSelected==false)
    {
        for (int i=0; i<8; i++)
        {
            if(posX>labelRects[i].x() && posX<labelRects[i].width()+labelRects[i].x() && posY>labelRects[i].y()+33 && posY<labelRects[i].height()+labelRects[i].y()+33)
            {
                boatClicked=i;
                boatIsSelected=true;
            }
        }
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    QPointF pt = ui->graphicsView->mapToScene(e->pos());//récupération de la position
    int posX=(int)pt.x();
    int posY = (int)pt.y();
    if(posX>=100 && posX<=534 && posY>=30 && posY<=487)
    {
        pressedX=((int)pt.x()-GWposX-1)/27;
        pressedY=((int)pt.y()-32-GWposY-1)/27;
        cout<<"positionX "<<pressedX<<endl;
        cout <<"positionY "<<pressedY<<endl;

        switch(state){
        case STATE_ATWAR:

            break;
        case STATE_PREPARATION:
            break;
        }
    }
    boatIsSelected=false;
    boatClicked=-1;
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
if(boatIsSelected==true)
{
    QPointF pt = ui->graphicsView->mapToScene(e->pos());//récupération de la position
    int posX=(int)pt.x();
    int posY = (int)pt.y();

    updateBoatGeometry(boatClicked, posX, posY);
}
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    QPointF pt = ui->graphicsView->mapToScene(e->pos());//récupération de la position
    int posX=(int)pt.x();
    int posY = (int)pt.y();
    updateLabelsPositions();
    for (int i=0; i<8; i++)
    {
        if(posX>labelRects[i].x() && posX<labelRects[i].width()+labelRects[i].x() && posY>labelRects[i].y()+33 && posY<labelRects[i].height()+labelRects[i].y()+33)
        {
            boatClicked=i;
            turnImage(i, posX, posY);
        }
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

void MainWindow::updateLabelsPositions()
{
    labelRects[0]=ui->bateau21->geometry();
    labelRects[1]=ui->bateau22->geometry();
    labelRects[2]=ui->bateau31->geometry();
    labelRects[3]=ui->bateau32->geometry();
    labelRects[4]=ui->bateau33->geometry();
    labelRects[5]=ui->bateau41->geometry();
    labelRects[6]=ui->bateau42->geometry();
    labelRects[7]=ui->bateau5->geometry();
}

void MainWindow::updateBoatGeometry(int boat, int x, int y)
{
    y=y-33;
    switch(boat){
    case 0:
              ui->bateau21->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
        break;
    case 1:
        ui->bateau22->setGeometry(x, y, 50, 25);
        break;
    case 2:
        ui->bateau31->setGeometry(x, y, 75, 25);
        break;
    case 3:
        ui->bateau32->setGeometry(x, y, 75, 25);
        break;
    case 4:
        ui->bateau33->setGeometry(x, y, 75, 25);
        break;
    case 5:
        ui->bateau41->setGeometry(x, y, 100, 25);
        break;
    case 6:
        ui->bateau42->setGeometry(x, y, 100, 25);
        break;
    case 7:
        ui->bateau5->setGeometry(x, y, 125, 25);
        break;
    }
}

void MainWindow::turnImage(int boat, int x, int y)
{
    y=y-33;//correction des pixels de la barre de menu
    switch(boat){
    case 0:
        if(labelRects[boat].width()>labelRects[boat].height())
        {
             ui->bateau21->setGeometry(x, y, labelRects[boat].height(),labelRects[boat].width());
             ui->bateau21->setPixmap(QPixmap(":/bateau2Vertical.png"));
        }
        else
        {
            ui->bateau21->setGeometry(x, y, labelRects[boat].height(), labelRects[boat].width());
            ui->bateau21->setPixmap(QPixmap(":/bateau2.png"));
        }
        break;
    case 1:
        ui->bateau22->setGeometry(x, y, 50, 25);
        break;
    case 2:
        ui->bateau31->setGeometry(x, y, 75, 25);
        break;
    case 3:
        ui->bateau32->setGeometry(x, y, 75, 25);
        break;
    case 4:
        ui->bateau33->setGeometry(x, y, 75, 25);
        break;
    case 5:
        ui->bateau41->setGeometry(x, y, 100, 25);
        break;
    case 6:
        ui->bateau42->setGeometry(x, y, 100, 25);
        break;
    case 7:
        ui->bateau5->setGeometry(x, y, 125, 25);
        break;
        updateLabelsPositions();
    }
}

void MainWindow::on_RAZBateaux_clicked()
{
    ui->bateau5->setGeometry(initialPosBoatX+15,initialPosBoatY+10,125,25);
    ui->bateau5->setPixmap(QPixmap(":/bateau5.png"));
    ui->bateau41->setGeometry(initialPosBoatX+155,initialPosBoatY+10,100,25);
    ui->bateau41->setPixmap(QPixmap(":/bateau4.png"));
    ui->bateau42->setGeometry(initialPosBoatX+270,initialPosBoatY+10,100,25);
    ui->bateau42->setPixmap(QPixmap(":/bateau4.png"));
    ui->bateau31->setGeometry(initialPosBoatX+385,initialPosBoatY+10,75,25);
    ui->bateau31->setPixmap(QPixmap(":/bateau3.png"));
    ui->bateau32->setGeometry(initialPosBoatX+92,initialPosBoatY+45,75,25);
    ui->bateau32->setPixmap(QPixmap(":/bateau3.png"));
    ui->bateau33->setGeometry(initialPosBoatX+185,initialPosBoatY+45,75,25);
    ui->bateau33->setPixmap(QPixmap(":/bateau3.png"));
    ui->bateau21->setGeometry(initialPosBoatX+272,initialPosBoatY+45,50,25);
    ui->bateau21->setPixmap(QPixmap(":/bateau2.png"));
    ui->bateau22->setGeometry(initialPosBoatX+337,initialPosBoatY+45,50,25);
    ui->bateau22->setPixmap(QPixmap(":/bateau2.png"));
}
