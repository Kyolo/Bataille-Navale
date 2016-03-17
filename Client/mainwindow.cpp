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
    close();
    }

//**********Entrer du texte dans le chat*******************************
void MainWindow::on_pushButtonOKChat_clicked ()
   {ui->textChat->append(nomJoueur+" : "+ui->lineEditChat->text());
    ui->lineEditChat->setText("");
   }


//*************Nouvelle partie*******************************



void MainWindow::on_actionNewGame_triggered()
    {
    bool ok;
    nomJoueur = QInputDialog::getText(this, tr("Nom du joueur"), tr("User name:"), QLineEdit::Normal,QDir::home().dirName(), &ok);
    ui->actionNewGame->setEnabled(false);




    }







void MainWindow::on_actionAbandon_triggered()
    {
     ui->actionNewGame->setEnabled(true);
    }
