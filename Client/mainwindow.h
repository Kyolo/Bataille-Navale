#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QInputDialog>
#include <QGraphicsScene>
#include <QMouseEvent>


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
 void on_actionConnexion_triggered ();
 void on_actionRactiveTchat_triggered ();

private:

    Ui::MainWindow *ui;
      QGraphicsScene *scene;
      QGraphicsPixmapItem *fond;
    QString nomJoueur = "Joueur 1";
};


#endif // MAINWINDOW_H
