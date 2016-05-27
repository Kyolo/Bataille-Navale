#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QInputDialog>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QRect>
#include "client.h"
#include "joueur.h"
#include "adversaire.h"
#include <QLabel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
 void on_actionQuitter_triggered ();
 void on_pushButtonOKTchat_clicked ();
 void on_actionNewGame_triggered ();
 void on_actionAbandon_triggered ();
 void on_actionTchatDisable_triggered ();
 void on_actionRactiveTchat_triggered ();
 void on_RAZBateaux_clicked ();
 void on_ButtonDone_clicked();
 void mousePressEvent(QMouseEvent *e);
 void mouseMoveEvent(QMouseEvent *e);
 void mouseDoubleClickEvent(QMouseEvent *e);
 void mouseReleaseEvent(QMouseEvent *e);

private:

    Ui::MainWindow *ui;
      QGraphicsScene *scene;
      QGraphicsPixmapItem *fond;
    QString nomJoueur = "Joueur 1";
    client *connexion;
    int pressedX;
    int pressedY;
    bool isClosed=false;
    void draw();
    static const int GWposX=100;
    static const int GWposY=30;
    int state;
    static const int initialPosBoatX=80;
    static const int initialPosBoatY=464;
    static const int STATE_PREPARATION = 0;
    static const int STATE_ATWAR = 1;
    Joueur *me;
    int connecte=0;
    void updateLabelsPositions();
    QRect labelRects[8];
    bool isHorizontal[8];
    int boatClicked=-1;
    bool boatIsSelected=false;
    void updateBoatGeometry(int boat, int x, int y);
    void turnImage(int boat, int x, int y);
    void updatePlayerBoats();
    TableauAdversaire adversaires;
    bool started=false;
    QLabel labelResult[16][16];

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void serverError(QString);
    void writeInTchat(QString);
    void rename();
    void NewNameSlot(QString name);
    void AttackReceived(QString, uchar, uchar, bool);
    void GameStarted();
    void ComboBoxChanged(QString name);
    void gestionTours();
};


#endif // MAINWINDOW_H
