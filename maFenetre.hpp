#pragma once
#ifndef MA_FENETRE_HPP
#define MA_FENETRE_HPP


#include <QMainWindow>
#include <QtGui>
#include <QWidget>



namespace Ui
{
class MainWindow;
}

class monWidgetGL;

class maFenetre: public QMainWindow
{
    Q_OBJECT

public:

    maFenetre(QWidget *parent=NULL);
    ~maFenetre();

private slots:

    void action_quitter();
    void action_afficher();
    void action_wireframe();

private:

    Ui::MainWindow *ui;
    monWidgetGL *glWidget;



};

#endif
