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

using namespace std; using namespace Header;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     ui->labelImageFond->setPixmap(QPixmap (":/carteFondFlou.jpg"));
     ui->graphicsView->setGeometry(GWposX,GWposY,434,434);
     QRect rect(GWposX+1,GWposY+1,434,434);
     ui->gridLayout->setGeometry(rect);
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
     ui->labelJoueursConnectes->append("Joueurs connectés :\n");
     state=0;
     //ui->tircoule->setPixmap(QPixmap(":/Tircoulé.png"));
    // ui->tircoule->setGeometry(157,157+10,25,25);
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
MainWindow::~MainWindow()
{
    delete ui;
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
    connexion = new client(adresseIP.toStdString(), portConnexion.toInt());
    ui->actionConnexion->setEnabled(false);
    connect(connexion, SIGNAL(serverError(QString)), this,SLOT(serverError(QString)));
    connect(connexion, SIGNAL(tchatRecive(QString)), this , SLOT(writeInTchat(QString)));
    connect(connexion, SIGNAL(rename()), this, SLOT(rename()));
    connect(connexion, SIGNAL(NewNameSignal(QString)), this , SLOT(NewNameSlot(QString)));
    connect(connexion, SIGNAL(AttackReceived(QString,uchar,uchar,bool)), this, SLOT(AttackReceived(QString,uchar,uchar,bool)));
    connect(connexion, SIGNAL(gmeStart()), this, SLOT(GameStarted()));
    connect(ui->nameBox, SIGNAL(currentTextChanged(QString)), this, SLOT(ComboBoxChanged(QString)));
    connecte=1;
    ui->textChat->setText("");
    ui->textChat->setEnabled(true);
    ui->lineEditChat->setText("");
    ui->lineEditChat->setEnabled(true);
    ui->pushButtonOKTchat->setEnabled(true);
    ui->labelTchatDisable->setText("");
    QMessageBox::information(this, "Bataille navale", "Veuillez maintenant placer vos bateaux sur la grille !\nN'oubliez pas de valider votre choix à la fin de l'opération.");
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
    for (int i=0; i<8; i++)
    {
        isHorizontal[i]=true;
    }
    ui->graphicsView->setVisible(true);
    ui->fondBateaux->setVisible(true);
    ui->RAZBateaux->setVisible(true);
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
    //ui->validBoatPosition->setGeometry(initialPosBoatX,initialPosBoatY,475,95);
    }

//**************************************************************

//*************Abandon de la partie********************************
void MainWindow::on_actionAbandon_triggered()
    {
    int reponseAbandon = QMessageBox::information(this, "Abandon de la partie ?", "Voulez vraiment abandonner la partie en cours ?\nToutes vos données seront perdues.", QMessageBox::Yes | QMessageBox::No);
       if (reponseAbandon == QMessageBox::Yes)
       {
         connexion->send(GiveUpHeader+me->getName());
         ui->actionNewGame->setEnabled(true);
         ui->actionAbandon->setEnabled(false);
         ui->graphicsView->setEnabled(false);
         fond = scene->addPixmap(QPixmap(":/mer.gif"));
         QMessageBox::information(this, "Fin de la partie", "Vous avez abandonné la partie");
         ui->graphicsView->setVisible(false);
         ui->fondBateaux->setVisible(false);
         ui->RAZBateaux->setVisible(false);
         ui->bateau5->setVisible(false);
         ui->bateau21->setVisible(false);
         ui->bateau22->setVisible(false);
         ui->bateau31->setVisible(false);
         ui->bateau32->setVisible(false);
         ui->bateau33->setVisible(false);
         ui->bateau41->setVisible(false);
         ui->bateau42->setVisible(false);

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

void MainWindow::AttackReceived(QString nom, uchar x, uchar y, bool etat)
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
}

void MainWindow::ComboBoxChanged(QString name)
{
    cout<< "comboBox changed"<<endl;
    int id=adversaires.getByName(name);
    cout <<"id du joueur"<< id <<endl;
    if(id!=-1)
    {
        for (int a=0; a<16; a++)
        {
            for (int b=0; b<16; b++)
            {
                if(adversaires.adv[id].getState(a,b)==0)
                {
                    labelResult[a][b].setPixmap(QPixmap(":/TirCoule.png"));
                }
                else if(adversaires.adv[id].getState(a,b)==1)
                {
                    labelResult[a][b].setPixmap(QPixmap(":/TirBateau.png"));
                }
                else if(adversaires.adv[id].getState(a,b)==2)
                {
                    labelResult[a][b].setPixmap(QPixmap());
                }
            }
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
               cout<<"attack x"<<AttackX<<"     attack y"<<AttackY<<endl;
               connexion->send((QString)PlayerAttack+":"+nomJoueur+":"+ui->nameBox->currentText()+":"+QString::number(AttackX)+":"+QString::number(AttackY));
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
}

void MainWindow::turnImage(int boat, int x, int y)
{
    if(state==0)
    {
        y=y-33;//correction des pixels de la barre de menu
        switch(boat){
        case 0:
            if(labelRects[boat].width()>labelRects[boat].height())
            {
                 ui->bateau21->setGeometry(x, y, labelRects[boat].height(),labelRects[boat].width());
                 ui->bateau21->setPixmap(QPixmap(":/bateau2Vertical.png"));
                 isHorizontal[boat]=false;
            }
            else
            {
                ui->bateau21->setGeometry(x, y, labelRects[boat].height(), labelRects[boat].width());
                ui->bateau21->setPixmap(QPixmap(":/bateau2.png"));
                isHorizontal[boat]=true;
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
        connexion->send(NewPlayer+me->writeMessage());
        state=1;
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
