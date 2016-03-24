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



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     ui->labelImageFond->setPixmap(QPixmap (":/carteFondFlou.jpg"));
     scene = new QGraphicsScene(this);
        scene->setSceneRect(QRectF(0,0,483,483));
        ui->graphicsView->setScene(scene);
       // fond = scene->addPixmap(QPixmap(":/Damiereau.jpg"));
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
       { close();
       }
       else if (reponseQuitter == QMessageBox::No)
       {
       }
    }
//Depuis la croix
void MainWindow::closeEvent(QCloseEvent *event)
{
    int reponseQuitter = QMessageBox::information(this, "Fermeture du programme", "Voulez vous fermer le programme et quitter la partie en cours?", QMessageBox::Yes | QMessageBox::No);
       if (reponseQuitter == QMessageBox::Yes)
       { event->accept();
       }
       else if (reponseQuitter == QMessageBox::No)
       {event->ignore();
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
    fond = scene->addPixmap(QPixmap(":/Damiereau.jpg"));
    }

//**************************************************************

//*************Abandon de la partie********************************
void MainWindow::on_actionAbandon_triggered()
    {
    int reponseAbandon = QMessageBox::information(this, "Abandon de la partie ?", "Voulez vraiment abandonner la partie en cours? Toutes vos données seront perdues.", QMessageBox::Yes | QMessageBox::No);
       if (reponseAbandon == QMessageBox::Yes)
       { ui->actionNewGame->setEnabled(true);
         ui->actionAbandon->setEnabled(false);
         QMessageBox::information(this, "Fin de la partie", "Vous avez abandonné la partie");
       }
       else if (reponseAbandon == QMessageBox::No)
       {
       }
    ui->graphicsView->setEnabled(false);
    fond = scene->addPixmap(QPixmap(":/mer.gif"));

    }
//********************************************************************

//**********Connexion************************************************
void MainWindow::on_actionConnexion_triggered ()
{
    QString adresseIP = QInputDialog::getText(this, tr("Adresse IP de connexion"), tr("Veuillez entrer l'adresse IP pour vous connecter : "), QLineEdit::Normal);
    qDebug()<<adresseIP;
    QString portConnexion = QInputDialog::getText(this, tr("Port de connexion"), tr("Veuillez entrer le port pour vous connecter : "), QLineEdit::Normal);
    qDebug()<<portConnexion;
    connexion = new client(adresseIP.toStdString(), portConnexion.toInt());
    ui->actionConnexion->setEnabled(false);
    connect(connexion, SIGNAL(serverError(QString)), this,SLOT(serverError(QString)));
}
//***********************************************************************

//**********Entrer du texte dans le tchat*******************************
void MainWindow::on_pushButtonOKTchat_clicked ()
   {ui->textChat->append(nomJoueur+" : "+ui->lineEditChat->text());
    connexion->send(nomJoueur+" : "+ui->lineEditChat->text());
    ui->lineEditChat->setText("");
   }
//************************************************************************

//********* Désactiver le Tchat **********************************************
void MainWindow::on_actionTchatDisable_triggered()
{ui->textChat->setText("");
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
{   ui->textChat->setText("");
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
