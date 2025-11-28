#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer> //para el loop o el tiempo del nivel//
#include <QKeyEvent> //para implementar el tecleado, jugar con el teclado//
#include "vehiculo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void actualizarJuego(); //loop del juego

private:
    Ui::MainWindow *ui;
    QGraphicsScene *escena;
    QTimer *timer;
    Vehiculo *jugador;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    //Funciones graficas, especicifamente para el nivel 1//
    void configurarNivel1();
    void verificarColisionesNivel1();
};
#endif // MAINWINDOW_H
