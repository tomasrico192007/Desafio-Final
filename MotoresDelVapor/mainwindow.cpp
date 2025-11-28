#include "mainwindow.h"
#include "ui_mainwindow.h"

//Destructor para el manejo de memoria//
MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarJuego);

    //SE empieza la configuracion para el primer nivel//
    configurarNivel1();

    timer->start(16);
}

void MainWindow::configurarNivel1()
{
    //Se define una vista lateral fija o en 2d, en este caso con la dimension o como yo lo llamo, resolucion 1000x600//
    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 1000, 600);
    ui->graphicsView->setScene(escena);

    //Se crea el vehiculo en la escena y lo ponemos en el piso//
    jugador = new Vehiculo();
    escena->addItem(jugador);
    jugador->setPosicion(50, 400);

    //Se dibuja el piso, para que el carro sepa dond delimita//
    QGraphicsRectItem *piso = new QGraphicsRectItem(0, 430, 1000, 10);
    piso->setBrush(Qt::darkGreen);
    escena->addItem(piso);
}


void MainWindow::actualizarJuego()
{
    jugador->actualizarFisicaNivel1();

    verificarColisionesNivel1();
}


void MainWindow::verificarColisionesNivel1() { //Se verifican los bordes para delimitar la colision, y por donde anda el carro, primero en izquierda con 00, y despues en derecha con 1000//

    if (jugador->getX() < 0) {
        jugador->setPosicion(0, jugador->getY());
        jugador->setVelX(0);
    }

    if (jugador->getX() > 1000 - jugador->boundingRect().width()) {
        jugador->setPosicion(1000 - jugador->boundingRect().width(), jugador->getY());
        jugador->setVelX(0);
    }
}

//COnrtoles para el vehiculo//
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //Se acelera con D, aunque principalmente se tenia pensado con W, con D queda bien//
    if(event->key() == Qt::Key_D) {
        jugador->setFuerzaMotor(200);
        jugador->acelerar(true);
    }
    //Se frena o se le mete reveresa o cambio a atras con la A, reemplazando la S//
    if(event->key() == Qt::Key_A) {
        jugador->setFuerzaMotor(-100);
        jugador->acelerar(true);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_D || event->key() == Qt::Key_A) {
        jugador->setFuerzaMotor(0);
        jugador->acelerar(false);
    }
}
