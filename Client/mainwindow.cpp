#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "QString"
#include "QDebug"
#include <QMessageBox>
#include <QInputDialog>
#include <QDir>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
//**********************************************************************************
//**********************************************************************************
//**********************************************************************************


//*****Quitter*********************************************************
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
//********************************************************************
//*************Nouvelle partie*******************************



void MainWindow::on_actionNewGame_triggered()
    {
    bool ok;
    nomJoueur = QInputDialog::getText(this, tr("Nom du joueur"), tr("User name:"), QLineEdit::Normal,QDir::home().dirName(), &ok);
    ui->actionNewGame->setEnabled(false);
    ui->actionAbandon->setEnabled(true);
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

    }
//********************************************************************

//**********Entrer du texte dans le tchat*******************************
void MainWindow::on_pushButtonOKTchat_clicked ()
   {ui->textChat->append(nomJoueur+" : "+ui->lineEditChat->text());
    ui->lineEditChat->setText("");
   }
//************************************************************************

//********* Vider le tchat **********************************************
void MainWindow::on_actionTchatDisable_triggered()
{ui->textChat->setText("");
 ui->textChat->setEnabled(false);
 ui->lineEditChat->setText("");
 ui->lineEditChat->setEnabled(false);
 ui->pushButtonOKTchat->setEnabled(false);
 ui->labelTchatDisable->setText("Tchat désactivé !");
}
//**************************************************************************
