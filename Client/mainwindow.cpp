#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "QString"
#include "QDebug"


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
    QString nomJoueur = "Joueur 1";
void MainWindow::on_pushButtonOKChat_clicked ()
   {
    ui->textChat->setText(ui->textChat->toPlainText()+nomJoueur+" : "+ui->lineEditChat->text()+"\n");
    ui->lineEditChat->setText("");
   }


//*************Reglage du  nom du joueur*******************************



void MainWindow::on_actionNomDuJoueur_triggered()
    {

    }
