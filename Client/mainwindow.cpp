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
//#include <QtMultimedia/QSound>
//#include <QtMultimedia/QAudioOutput>

#include "comunicationconstants.h"

using namespace std; using namespace Header;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     MainWindow::RAZIG();

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RAZIG()
{ ui->labelImageFond->setPixmap(QPixmap (":/carteFondFlou.jpg"));
    ui->ImageConnexionStatus->setPixmap(QPixmap (":/ledRouge.png"));
    ui->graphicsView->setGeometry(GWposX,GWposY,434,434);
    QRect rect(GWposX+1,GWposY+1,434,434);
    ui->gridLayout->setGeometry(rect);
    scene = new QGraphicsScene(this);
       scene->setSceneRect(QRectF(0,0,434,434));
       ui->graphicsView->setScene(scene);
       ui->textChat->setText(""); // descativation du chat tant que pas connecté
    ui->textChat->setEnabled(false);
    ui->lineEditChat->setText("");
    ui->lineEditChat->setEnabled(false);
    ui->pushButtonOKTchat->setEnabled(false);
    ui->labelTchatDisable->setText("Tchat désactivé! \n \n Veuillez d'abord \n vous connecter !");
    ui->labelJoueursConnectes->append("Joueurs connectés :\n");
    ui->ButtonDone->setVisible(false);
    ui->RAZBateaux->setVisible(false);
    ui->nameBox->setVisible(false);
    state=0;

   for (int a=0; a<16; a++)
   {
       for( int b=0; b<16; b++)
       {
           labelResult[a][b].setMaximumHeight(27);
            labelResult[a][b].setMinimumHeight(27);
             labelResult[a][b].setMaximumWidth(27);
              labelResult[a][b].setMinimumWidth(27);
           ui->gridLayout->addWidget(&labelResult[a][b], a, b);
       }
   }
   ui->graphicsView->setLayout(ui->gridLayout);

}

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
    ui->labelInfosPartie->setText("Suivez les indications ...");
    nomJoueur = QInputDialog::getText(this, tr("Nom du joueur"), tr("Votre nom :"), QLineEdit::Normal,QDir::home().dirName(), &ok);
    nomJoueur.replace(QString(":"),QString("_"));
    me=new Joueur(nomJoueur);
    me->setPerson(true);
    ui->actionNewGame->setEnabled(false);
    ui->actionAbandon->setEnabled(true);
    ui->graphicsView->setEnabled(true);
    fond = scene->addPixmap(QPixmap(":/fondDamier.png"));
    draw();
    QString adresseIP = QInputDialog::getText(this, tr("Adresse IP de connexion"), tr("Veuillez entrer l'adresse IP pour vous connecter : "), QLineEdit::Normal);
    qDebug()<<adresseIP;
    QString portConnexion = QInputDialog::getText(this, tr("Port de connexion"), tr("Veuillez entrer le port pour vous connecter : "), QLineEdit::Normal);
    qDebug()<<portConnexion;
    ui->ImageConnexionStatus->setPixmap(QPixmap(":/ledVerte.png"));
    connexion = new client(adresseIP.toStdString(), portConnexion.toInt());
    ui->actionConnexion->setEnabled(false);
    connect(connexion, SIGNAL(serverError(QString)), this,SLOT(serverError(QString)));
    connect(connexion, SIGNAL(tchatRecive(QString)), this , SLOT(writeInTchat(QString)));
    connect(connexion, SIGNAL(rename()), this, SLOT(rename()));
    connect(connexion, SIGNAL(NewNameSignal(QString)), this , SLOT(NewNameSlot(QString)));
    connect(connexion, SIGNAL(AttackReceived(QString,uchar,uchar,bool)), this, SLOT(AttackReceived(QString,uchar,uchar,bool)));
    connect(connexion, SIGNAL(gmeStart()), this, SLOT(GameStarted()));
    connect(ui->nameBox, SIGNAL(currentTextChanged(QString)), this, SLOT(ComboBoxChanged(QString)));
    connect(connexion, SIGNAL(GestTours()), this, SLOT(gestionTours()));
    connect(connexion, SIGNAL(signalPlayerLost(QString)), this, SLOT(gameLost(QString)));
    connect(connexion, SIGNAL(signalPlayerWin(QString)), this, SLOT(gameWon(QString)));
    connecte=1;
    state=0;
    ui->nameBox->clear();
    ui->bateau21->setCursor(Qt::ClosedHandCursor);
    ui->bateau22->setCursor(Qt::ClosedHandCursor);
    ui->bateau31->setCursor(Qt::ClosedHandCursor);
    ui->bateau32->setCursor(Qt::ClosedHandCursor);
    ui->bateau33->setCursor(Qt::ClosedHandCursor);
    ui->bateau41->setCursor(Qt::ClosedHandCursor);
    ui->bateau42->setCursor(Qt::ClosedHandCursor);
    ui->bateau5->setCursor(Qt::ClosedHandCursor);
    ui->textChat->setText("");
    ui->labelTour->setText("");
    ui->textChat->setEnabled(true);
    ui->lineEditChat->setText("");
    ui->lineEditChat->setEnabled(true);
    ui->pushButtonOKTchat->setEnabled(true);
    ui->labelTchatDisable->setText("");
    ui->labelInfosPartie->setText("C'est l'heure de placer vos bateaux !");
    QMessageBox::information(this, "Bataille navale", "Veuillez maintenant placer vos bateaux sur la grille !\nN'oubliez pas de valider votre choix à la fin de l'opération.");
    ui->fondBateaux->setGeometry(initialPosBoatX,initialPosBoatY,475,100);
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
    ui->ButtonDone->setGeometry(initialPosBoatX+473,initialPosBoatY-2,20,95);
    for (int i=0; i<8; i++)
    {
        isHorizontal[i]=true;
    }
    ui->graphicsView->setVisible(true);
    ui->fondBateaux->setVisible(true);
    ui->RAZBateaux->setVisible(true);
    ui->ButtonDone->setVisible(true);
    ui->bateau5->setVisible(true);
    ui->bateau21->setVisible(true);
    ui->bateau22->setVisible(true);
    ui->bateau31->setVisible(true);
    ui->bateau32->setVisible(true);
    ui->bateau33->setVisible(true);
    ui->bateau41->setVisible(true);
    ui->bateau42->setVisible(true);
    connexion->send(NewName+me->playerName);
    adversaires = TableauAdversaire();
    }

//**************************************************************

//*************Abandon de la partie********************************
void MainWindow::on_actionAbandon_triggered()
    {
    int reponseAbandon = QMessageBox::information(this, "Abandon de la partie ?", "Voulez vraiment abandonner la partie en cours ?\nToutes vos données seront perdues.", QMessageBox::Yes | QMessageBox::No);
       if (reponseAbandon == QMessageBox::Yes)
       {
         ui->labelInfosPartie->setText("Partie abandonnée !");
         connexion->send(GiveUpHeader+me->getName());
         ui->actionNewGame->setEnabled(true);
         ui->actionAbandon->setEnabled(false);
         ui->graphicsView->setEnabled(false);
         ui->graphicsView->setVisible(false);
         ui->fondBateaux->setVisible(false);
         ui->RAZBateaux->setVisible(false);
         ui->labelTour->setText("");
         ui->bateau5->setVisible(false);
         ui->bateau21->setVisible(false);
         ui->bateau22->setVisible(false);
         ui->bateau31->setVisible(false);
         ui->bateau32->setVisible(false);
         ui->bateau33->setVisible(false);
         ui->bateau41->setVisible(false);
         ui->bateau42->setVisible(false);
         ui->labelJoueursConnectes->setText("Joueurs connectés :\n");
         ui->labelExplicNameBox->setText("");
         ui->labelExplicNameBox->setVisible(false);
         ui->nameBox->setVisible(false);
         ui->ButtonDone->setVisible(false);
         QMessageBox::information(this, "Fin de la partie", "Vous avez abandonné la partie");
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
  // QSound::play("sonTchat.wav");
}

void MainWindow::rename()
{
    nomJoueur = QInputDialog::getText(this, tr("Nom du joueur"), tr("Veuillez changer votre nom. Un autre joueur possede deja celui-ci. :"), QLineEdit::Normal,QDir::home().dirName());
    me->playerName=nomJoueur;
    connexion->send(NewName+me->playerName);
}

void MainWindow::NewNameSlot(QString name)
{
    cout<<"ajout nom au menu deroulant et dans le label joueurs connectés"<<endl;
    if(adversaires.getByName(name)==-1)
    {
    ui->nameBox->addItem(name);
    ui->labelJoueursConnectes->append(" •  "+name);
    adversaires.newAdv(name);
    }
}

void MainWindow::AttackReceived(QString nom, uchar y, uchar x, bool etat)
{
  int id=adversaires.getByName(nom);
  adversaires.adv[id].setState(x,y,etat);
  if(ui->nameBox->currentText()==nom)
  {
      if(etat==1)
      {
         labelResult[x][y].setPixmap(QPixmap(":/TirBateau.png"));
      }
      if(etat==0)
      {
        labelResult[x][y].setPixmap(QPixmap(":/TirCoule.png"));
      }
  }
}

void MainWindow::GameStarted()
{
    started=true;
    ui->textChat->append("Serveur : Tous les joueurs sont prêts, la partie commence...");
    ui->labelInfosPartie->setText("Partie en cours...             ");
}

void MainWindow::ComboBoxChanged(QString name)
{
    cout<< "comboBox changed"<<endl;
    int id=adversaires.getByName(name);
    cout <<"id du joueur"<< id <<endl;
    if(state==STATE_ATWAR)
    {
        if(name==nomJoueur)
        {
            if(isHorizontal[0]==true)
            ui->bateau21->setPixmap(QPixmap(":/bateau2.png"));
            else
            ui->bateau21->setPixmap(QPixmap(":/bateau2Vertical.png"));
            if(isHorizontal[1]==true)
            ui->bateau22->setPixmap(QPixmap(":/bateau2.png"));
            else
            ui->bateau22->setPixmap(QPixmap(":/bateau2Vertical.png"));
            if(isHorizontal[2]==true)
            ui->bateau31->setPixmap(QPixmap(":/bateau3.png"));
            else
            ui->bateau31->setPixmap(QPixmap(":/bateau3Vertical.png"));
            if(isHorizontal[3]==true)
            ui->bateau32->setPixmap(QPixmap(":/bateau3.png"));
            else
            ui->bateau32->setPixmap(QPixmap(":/bateau3Vertical.png"));
            if(isHorizontal[4]==true)
            ui->bateau33->setPixmap(QPixmap(":/bateau3.png"));
            else
            ui->bateau33->setPixmap(QPixmap(":/bateau3Vertical.png"));
            if(isHorizontal[5]==true)
            ui->bateau41->setPixmap(QPixmap(":/bateau4.png"));
            else
            ui->bateau41->setPixmap(QPixmap(":/bateau4Vertical.png"));
            if(isHorizontal[6]==true)
            ui->bateau42->setPixmap(QPixmap(":/bateau4.png"));
            else
            ui->bateau42->setPixmap(QPixmap(":/bateau4Vertical.png"));
            if(isHorizontal[7]==true)
            ui->bateau5->setPixmap(QPixmap(":/bateau5.png"));
            else
            ui->bateau5->setPixmap(QPixmap(":/bateau5Vertical.png"));
        }
        else
        {
           ui->bateau21->setPixmap(QPixmap());
           ui->bateau22->setPixmap(QPixmap());
           ui->bateau31->setPixmap(QPixmap());
           ui->bateau32->setPixmap(QPixmap());
           ui->bateau33->setPixmap(QPixmap());
           ui->bateau41->setPixmap(QPixmap());
           ui->bateau42->setPixmap(QPixmap());
           ui->bateau5->setPixmap(QPixmap());
        }
        if(id!=-1)
        {
            for (int a=0; a<16; a++)

            {
                for (int b=0; b<16; b++)
                {
                    labelResult[a][b].raise();
                    if(adversaires.adv[id].getState(a,b)==0)
                    {
                        labelResult[a][b].setPixmap(QPixmap(""));
                        labelResult[a][b].setPixmap(QPixmap(":/TirCoule.png"));
                    }
                    else if(adversaires.adv[id].getState(a,b)==1)
                    {
                        labelResult[a][b].setPixmap(QPixmap(""));
                        labelResult[a][b].setPixmap(QPixmap(":/TirBateau.png"));
                    }
                    else if(adversaires.adv[id].getState(a,b)==2)
                    {
                        labelResult[a][b].setPixmap(QPixmap());
                    }
                }
            }
            ui->graphicsView->repaint();
        }
    }
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
    ui->ImageConnexionStatus->setPixmap(QPixmap (":/ledRouge.png"));
    ui->labelInfosPartie->setText("Le serveur a été déconnecté !");
    QMessageBox::critical(this, "Erreur",error, QMessageBox::Ok);
}
//**********************Clic de la souris********************
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    int posX;
    int posY;
    QPointF pt = ui->graphicsView->mapToScene(e->pos());//récupération de la position
    switch(state)
    {
    case STATE_ATWAR:
        if(started==true)
        {
            cout <<"clic"<<endl;
             posX=(int)pt.x();
             posY = (int)pt.y();
            if(posX>=100 && posX<=534 && posY>=30 && posY<=487)
            {
               int AttackX=((int)pt.x()-GWposX-1)/27;
               int AttackY=((int)pt.y()-32-GWposY-1)/27;

               if(adversaires.adv[adversaires.getByName(ui->nameBox->currentText())].getState(AttackY, AttackX)==2 && nomJoueur!= ui->nameBox->currentText())
               {   ui->labelTour->setText("C'est au tour de votre adversaire !");
                   cout<<"attack x"<<AttackX<<"     attack y"<<AttackY<<endl;
                   connexion->send((QString)PlayerAttack+":"+nomJoueur+":"+ui->nameBox->currentText()+":"+QString::number(AttackX)+":"+QString::number(AttackY));
               }
                else if (nomJoueur== ui->nameBox->currentText())
               {
                   QMessageBox::warning(this, "Attention !", "Le suicide ne semble pas etre une bonne stratégie !!!",QMessageBox::Ok);
               }
               else if (!adversaires.adv[adversaires.getByName(ui->nameBox->currentText())].getState(AttackY, AttackX)==2)
              {
                  QMessageBox::warning(this, "Attention !", "Vous avez deja attaqué ce joueur a cet endroit",QMessageBox::Ok);
              }
            }
        }
        break;
    case STATE_PREPARATION:
        QPointF pt = ui->graphicsView->mapToScene(e->pos());//récupération de la position
        posX=(int)pt.x();
        posY = (int)pt.y();
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
        break;
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    QPointF pt = ui->graphicsView->mapToScene(e->pos());//récupération de la position
    int posX=(int)pt.x();
    int posY = (int)pt.y();
    cout <<"Mouse release"<<    endl;
    if(posX>=100 && posX<=534 && posY>=30 && posY<=487)
    {
        pressedX=((int)pt.x()-GWposX-1)/27;
        pressedY=((int)pt.y()-32-GWposY-1)/27;
        cout<<"positionX "<<pressedX<<endl;
        cout <<"positionY "<<pressedY<<endl;

        switch(state){
        case STATE_ATWAR:
cout << "at war"<<endl;
            break;
        case STATE_PREPARATION:
            updateLabelsPositions();
            for (int i=0; i<8; i++)
            {
                if(posX>=labelRects[i].x() && posX<=labelRects[i].width()+labelRects[i].x() && posY>=labelRects[i].y()+33 && posY<=labelRects[i].height()+labelRects[i].y()+33)
                {
                    boatClicked=i;
                    if(labelRects[i].width()+GWposX+1+(pressedX)*27<=534&&labelRects[i].height()+GWposY+1+(pressedY)*27<=487)
                    {
                        updateBoatGeometry(boatClicked,GWposX+1+(pressedX)*27,GWposY+33+1+(pressedY)*27 );
                    }
                }
            }
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
    if(state==0)
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
}

void MainWindow::updateBoatGeometry(int boat, int x, int y)
{
    if(state==0)
    {
        y=y-33;
        switch(boat){
        case 0:
            ui->bateau21->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
            break;
        case 1:
            ui->bateau22->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
            break;
        case 2:
            ui->bateau31->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
            break;
        case 3:
            ui->bateau32->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
            break;
        case 4:
            ui->bateau33->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
            break;
        case 5:
            ui->bateau41->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
            break;
        case 6:
            ui->bateau42->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
            break;
        case 7:
            ui->bateau5->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
            break;
        }
    }
}

void MainWindow::turnImage(int boat, int x, int y)
{
    if(state==0)
    {
        y=y-33;//correction des pixels de la barre de menu
        switch(boat){
        case 0:
            if( isHorizontal[boat])
            {
                 ui->bateau21->setGeometry(x, y, labelRects[boat].height(),labelRects[boat].width());
                 int height=labelRects[boat].height();
                 int width=labelRects[boat].width();
                 labelRects[boat].setHeight(width);
                 labelRects[boat].setWidth(height);
                 ui->bateau21->setPixmap(QPixmap(":/bateau2Vertical.png"));
                 isHorizontal[boat]=false;
            }
            else
            {
                ui->bateau21->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
                ui->bateau21->setPixmap(QPixmap(":/bateau2.png"));
                int height=labelRects[boat].height();
                        int width=labelRects[boat].width();
                        labelRects[boat].setHeight(width);
                        labelRects[boat].setWidth(height);
                isHorizontal[boat]=true;
            }
            break;
        case 1:
            if( isHorizontal[boat])
            {
                 ui->bateau22->setGeometry(x, y, labelRects[boat].height(),labelRects[boat].width());
                 int height=labelRects[boat].height();
                         int width=labelRects[boat].width();
                         labelRects[boat].setHeight(width);
                         labelRects[boat].setWidth(height);
                 ui->bateau22->setPixmap(QPixmap(":/bateau2Vertical.png"));
                 isHorizontal[boat]=false;
            }
            else
            {
                ui->bateau22->setGeometry(x, y,labelRects[boat].width(), labelRects[boat].height());
                ui->bateau22->setPixmap(QPixmap(":/bateau2.png"));
                int height=labelRects[boat].height();
                        int width=labelRects[boat].width();
                        labelRects[boat].setHeight(width);
                        labelRects[boat].setWidth(height);
                isHorizontal[boat]=true;
            }
            break;
        case 2:
            if( isHorizontal[boat])
            {
                 ui->bateau31->setGeometry(x, y, labelRects[boat].height(),labelRects[boat].width());
                 int height=labelRects[boat].height();
                         int width=labelRects[boat].width();
                         labelRects[boat].setHeight(width);
                         labelRects[boat].setWidth(height);
                 ui->bateau31->setPixmap(QPixmap(":/bateau3Vertical.png"));
                 isHorizontal[boat]=false;
            }
            else
            {
                ui->bateau31->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
                ui->bateau31->setPixmap(QPixmap(":/bateau3.png"));
                int height=labelRects[boat].height();
                        int width=labelRects[boat].width();
                        labelRects[boat].setHeight(width);
                        labelRects[boat].setWidth(height);
                isHorizontal[boat]=true;
            }
            break;
        case 3:
            if( isHorizontal[boat])
            {
                 ui->bateau32->setGeometry(x, y, labelRects[boat].height(),labelRects[boat].width());
                 int height=labelRects[boat].height();
                         int width=labelRects[boat].width();
                         labelRects[boat].setHeight(width);
                         labelRects[boat].setWidth(height);
                 ui->bateau32->setPixmap(QPixmap(":/bateau3Vertical.png"));
                 isHorizontal[boat]=false;
            }
            else
            {
                ui->bateau32->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
                ui->bateau32->setPixmap(QPixmap(":/bateau3.png"));
                int height=labelRects[boat].height();
                        int width=labelRects[boat].width();
                        labelRects[boat].setHeight(width);
                        labelRects[boat].setWidth(height);
                isHorizontal[boat]=true;
            }
            break;
        case 4:
            if( isHorizontal[boat])
            {
                 ui->bateau33->setGeometry(x, y, labelRects[boat].height(),labelRects[boat].width());
                 int height=labelRects[boat].height();
                         int width=labelRects[boat].width();
                         labelRects[boat].setHeight(width);
                         labelRects[boat].setWidth(height);
                 ui->bateau33->setPixmap(QPixmap(":/bateau3Vertical.png"));
                 isHorizontal[boat]=false;
            }
            else
            {
                ui->bateau33->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
                ui->bateau33->setPixmap(QPixmap(":/bateau3.png"));
                int height=labelRects[boat].height();
                        int width=labelRects[boat].width();
                        labelRects[boat].setHeight(width);
                        labelRects[boat].setWidth(height);
                isHorizontal[boat]=true;
            }
            break;
        case 5:
            if( isHorizontal[boat])
            {
                 ui->bateau41->setGeometry(x, y, labelRects[boat].height(),labelRects[boat].width());
                 int height=labelRects[boat].height();
                         int width=labelRects[boat].width();
                         labelRects[boat].setHeight(width);
                         labelRects[boat].setWidth(height);
                 ui->bateau41->setPixmap(QPixmap(":/bateau4Vertical.png"));
                 isHorizontal[boat]=false;
            }
            else
            {
                ui->bateau41->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
                ui->bateau41->setPixmap(QPixmap(":/bateau4.png"));
                int height=labelRects[boat].height();
                        int width=labelRects[boat].width();
                        labelRects[boat].setHeight(width);
                        labelRects[boat].setWidth(height);
                isHorizontal[boat]=true;
            }
            break;
        case 6:                                                     //#REKT
            if( isHorizontal[boat])
            {
                 ui->bateau42->setGeometry(x, y, labelRects[boat].height(),labelRects[boat].width());
                 int height=labelRects[boat].height();
                         int width=labelRects[boat].width();
                         labelRects[boat].setHeight(width);
                         labelRects[boat].setWidth(height);
                 ui->bateau42->setPixmap(QPixmap(":/bateau4Vertical.png"));
                 isHorizontal[boat]=false;
            }
            else
            {
                ui->bateau42->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
                ui->bateau42->setPixmap(QPixmap(":/bateau4.png"));
                int height=labelRects[boat].height();
                        int width=labelRects[boat].width();
                        labelRects[boat].setHeight(width);
                        labelRects[boat].setWidth(height);
                isHorizontal[boat]=true;
            }
            break;
        case 7:
            if( isHorizontal[boat])
            {
                 ui->bateau5->setGeometry(x, y, labelRects[boat].height(),labelRects[boat].width());
                 int height=labelRects[boat].height();
                         int width=labelRects[boat].width();
                         labelRects[boat].setHeight(width);
                         labelRects[boat].setWidth(height);
                 ui->bateau5->setPixmap(QPixmap(":/bateau5Vertical.png"));
                 isHorizontal[boat]=false;
            }
            else
            {
                ui->bateau5->setGeometry(x, y, labelRects[boat].width(), labelRects[boat].height());
                ui->bateau5->setPixmap(QPixmap(":/bateau5.png"));
                int height=labelRects[boat].height();
                        int width=labelRects[boat].width();
                        labelRects[boat].setHeight(width);
                        labelRects[boat].setWidth(height);
                isHorizontal[boat]=true;
            }
            break;
            updateLabelsPositions();
        }
    }
}

void MainWindow::on_RAZBateaux_clicked()
{
    if(state==0)
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
}

void MainWindow::on_ButtonDone_clicked()
{
    cout <<"clic sur le bouton Done"<<endl;
    updateLabelsPositions();
    updatePlayerBoats();
    int isWrite=-1 ;
    isWrite= me->checkWrite(true);
    switch(isWrite)
    {
    case 0:
        cout <<"tout va bien"<<endl;
        ui->labelInfosPartie->setText("Attente du deuxième joueur !");
        connexion->send(NewPlayer+me->writeMessage());
        state=1;
        ui->bateau21->setCursor(Qt::ArrowCursor);
        ui->bateau22->setCursor(Qt::ArrowCursor);
        ui->bateau31->setCursor(Qt::ArrowCursor);
        ui->bateau32->setCursor(Qt::ArrowCursor);
        ui->bateau33->setCursor(Qt::ArrowCursor);

        ui->bateau41->setCursor(Qt::ArrowCursor);
        ui->bateau42->setCursor(Qt::ArrowCursor);
        ui->bateau5->setCursor(Qt::ArrowCursor);
        ui->ButtonDone->setVisible(false);
        ui->RAZBateaux->setVisible(false);
        ui->fondBateaux->setVisible(false);
        ui->textChat->append("Serveur : Attente du deuxième joueur...");
        ui->labelExplicNameBox->setText("Choix de la grille à afficher :");
        ui->nameBox->setVisible(true);
        break;

    case 1:
        QMessageBox::critical(this, "Erreur", "Des bateaux se superposent", QMessageBox::Ok);
    break;
    case 2:
        QMessageBox::critical(this, "Erreur", "Tous les bateaux ne sont pas placés", QMessageBox::Ok);
    break;
}
}

void MainWindow::updatePlayerBoats()
{
    if(state==0)
    {
        for (int i=0; i<8; i++)
        {
            switch(i){
            case 0:
                me->setBoat((labelRects[i].x()-GWposX-1)/27, (labelRects[i].y()-GWposY-1)/27,isHorizontal[i],i,2);
                cout <<"x: "<<labelRects[i].x()<<"y: "<<labelRects[i].y()<<endl;
                cout <<"x: "<<(labelRects[i].x()-GWposX-1)/27<<" y: "<<(labelRects[i].y()-GWposY-1)/27 <<endl;
                break;
            case 1:
                me->setBoat((labelRects[i].x()-GWposX-1)/27, (labelRects[i].y()-GWposY-1)/27,isHorizontal[i],i,2);
                break;
            case 2:
                me->setBoat((labelRects[i].x()-GWposX-1)/27, (labelRects[i].y()-GWposY-1)/27,isHorizontal[i],i,3);
                break;
            case 3:
                me->setBoat((labelRects[i].x()-GWposX-1)/27, (labelRects[i].y()-GWposY-1)/27,isHorizontal[i],i,3);
                break;
            case 4:
                me->setBoat((labelRects[i].x()-GWposX-1)/27, (labelRects[i].y()-GWposY-1)/27,isHorizontal[i],i,3);
                break;
            case 5:
                me->setBoat((labelRects[i].x()-GWposX-1)/27, (labelRects[i].y()-GWposY-1)/27,isHorizontal[i],i,4);
                break;
            case 6:
               me->setBoat((labelRects[i].x()-GWposX-1)/27, (labelRects[i].y()-GWposY-1)/27,isHorizontal[i],i,4);
                break;
            case 7:
                me->setBoat((labelRects[i].x()-GWposX-1)/27, (labelRects[i].y()-GWposY-1)/27,isHorizontal[i],i,5);
                break;
            }
        }
    }
}

void MainWindow::gestionTours ()
{ ui->labelTour->setText("C'est à votre tour de jouer !");

}


void MainWindow::gameWon(QString name)
{
   if(name==nomJoueur)
   {
    ui->labelInfosPartie->setText("Partie gagnée !");
    ui->labelTour->setText("Vous avez gagné la partie !");
    int reponseNewGame = QMessageBox::question(this, "Nouvelle partie", "Félicitations, vous avez gagné la partie !\nVoulez vous recommencer une nouvelle partie?", QMessageBox::Yes | QMessageBox::No);
    if (reponseNewGame == QMessageBox::Yes)
    {MainWindow::on_actionNewGame_triggered();

    }
    else if (reponseNewGame == QMessageBox::No)
    {
    MainWindow::RAZIG();
    }
    }
}

void MainWindow::gameLost(QString name)
{
    if(name==nomJoueur)
    {
        ui->labelInfosPartie->setText("Partie perdue !");
        ui->labelTour->setText("Vous avez perdu la partie !");
        int reponseNewGame = QMessageBox::question(this, "Nouvelle partie", "Vous avez malheuresement perdu la partie !\nVoulez vous recommencer une nouvelle partie?", QMessageBox::Yes | QMessageBox::No);
           if (reponseNewGame == QMessageBox::Yes)
           {MainWindow::on_actionNewGame_triggered();

           }
           else if (reponseNewGame == QMessageBox::No)
           {
            MainWindow::RAZIG();
           }
    }
}
