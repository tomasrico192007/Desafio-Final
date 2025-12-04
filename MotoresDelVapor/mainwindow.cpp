#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vehiculo.h"
#include "obstaculo.h"


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

    //Esto lo encontre en internet, es para desactivar las barras laterales o el scroll por decirlo asi//
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::actualizarJuego);

    //SE empieza la configuracion para el primer nivel//
    //configurarNivel1();
    //configurarNivel2();
    configurarNivel3();
    verificarColisionesNivel2();

    timer->start(16);
}

void MainWindow::configurarNivel1()
{
    juegoTerminado = false;
    vueltasJugador = 0;

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

    //Se crea el obstaculo o la penalizacion que es el objeto rojo//
    muro_rojo = new Obstaculo(
        400, 350, 0.0,0.0 );
    muro_rojo->setPos(400, 350);
    escena->addItem(muro_rojo);

    //Se dibuja la meta//
    meta = new QGraphicsRectItem(0, 0, 10, 600);
    meta->setPos(0, 0);
    escena->addItem(meta);
}


void MainWindow::configurarNivel2(){
    juegoTerminado = false;
    vueltasJugador = 0;

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 1000, 600);
    ui->graphicsView->setScene(escena);

    //Se inicia la lista que con tiene los obstaculos del nivel 2//
    obstaculosNivel2.clear();

    // Se crea el jugador//
    jugador = new Vehiculo();
    escena->addItem(jugador);
    jugador->setPosicion(50, 400);

    //Se hace el piso //
    QGraphicsRectItem *piso = new QGraphicsRectItem(0, 430, 1000, 10);
    piso->setBrush(Qt::darkGreen);
    escena->addItem(piso);

    meta = new QGraphicsRectItem(0, 0, 10, 600);
    meta->setPos(0, 0);
    meta->setBrush(Qt::yellow);
    escena->addItem(meta);

    muro_rojo=nullptr;

    Obstaculo *obs1 = new Obstaculo(300, 350, 0, 0);
    escena->addItem(obs1);
    obstaculosNivel2.append(obs1);

    Obstaculo *obs2 = new Obstaculo(550, 350, 0, 0);
    escena->addItem(obs2);
    obstaculosNivel2.append(obs2);

    Obstaculo *obs3 = new Obstaculo(700, 350, 0, 0);
    escena->addItem(obs3);
    obstaculosNivel2.append(obs3);

}

void MainWindow::configurarNivel3()
{
    juegoTerminado = false;
    vueltasJugador = 0;

    escena = new QGraphicsScene(this);
    escena->setSceneRect(0, 0, 1000, 600);
    ui->graphicsView->setScene(escena);

    jugador = new Vehiculo();
    jugador->setPosicion(50, 400);
    escena->addItem(jugador);

    QGraphicsRectItem *piso = new QGraphicsRectItem(0, 430, 1000, 10);
    piso->setBrush(Qt::darkGray);
    escena->addItem(piso);

    obstaculosNivel2.clear();

    Obstaculo *obs1 = new Obstaculo(250, 250, 100, 1.5);
    escena->addItem(obs1);
    obstaculosNivel2.append(obs1);

    Obstaculo *obs2 = new Obstaculo(450, 300, 50, 4.0);
    escena->addItem(obs2);
    obstaculosNivel2.append(obs2);

    Obstaculo *obs3 = new Obstaculo(650, 200, 150, 2.0);
    escena->addItem(obs3);
    obstaculosNivel2.append(obs3);

    Obstaculo *obs4 = new Obstaculo(850, 300, 50, 4.0);
    escena->addItem(obs4);
    obstaculosNivel2.append(obs4);
}


void MainWindow::actualizarJuego()
{
    if (juegoTerminado) return;

    static double tiempoTotal = 0.0;
    double dt = 16.0 / 1000.0;

    foreach (QGraphicsItem *item, obstaculosNivel2) {

        Obstaculo *obs = dynamic_cast<Obstaculo*>(item);
        if (obs) {
            obs->actualizarMovimientoOscilatorio(tiempoTotal);
        }
    }

    verificarLimites(jugador);
    verificarMeta(jugador, vueltasJugador);

    jugador->actualizarFisicaNivel2();
    verificarColisionesNivel2();

    //verificarColisionesNivel1();

    tiempoTotal +=dt;
}


void MainWindow::verificarColisionesNivel1() {

    const double VELOCIDAD_MAXIMA_SEGURA = 20.0;

    if (jugador->collidesWithItem(muro_rojo)) {

        double velXActual = jugador->getVelX();

        if (velXActual > VELOCIDAD_MAXIMA_SEGURA) {

            jugador->setVelX(velXActual * 0.25);

        } else {

            jugador->setVelX(velXActual * 0.95);
        }

        jugador->setPos(jugador->getX() - 10, jugador->getY());
    }
}

void MainWindow::verificarColisionesNivel2() {
    if (obstaculosNivel2.isEmpty()) return;

    //double x_previo = jugador->getX();

    foreach (QGraphicsItem *obs, obstaculosNivel2) {
        if (jugador->collidesWithItem(obs)) {

            jugador->setVelX(0);
            jugador->setVelY(0);

            jugador->setPosicion(50,400);

            return;
        }
    }
}

void MainWindow::verificarMeta(Vehiculo *v, int &vueltas)
{
    if (juegoTerminado) return;
}

void MainWindow::verificarLimites(Vehiculo *v)
{
    double anchoEscena = 1000;

    //Se veirifican los limites externos izquierdo//
    if (v->getX() < 0) {
        v->setPosicion(0, v->getY());
        v->setVelX(0);
    }

    //Se verifican los limites externos derechos//
    if (v->getX() > anchoEscena - v->boundingRect().width()) {

        if (!juegoTerminado) {
            vueltasJugador++;
            qDebug() << "Total de vueltas:" << vueltasJugador;

            if (vueltasJugador >= 5) {
                juegoTerminado = true;
                qDebug() << "Ganaste";
            }
        }

        //Se le reinicia la posicion//
        v->setPosicion(5, v->getY());

         //Se le aplica la penalizacionh de 25% por el obstaculo //
        v->setVelX(v->getVelX() * 0.75);
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
        jugador->setFuerzaMotor(-300);
        jugador->acelerar(true);
    }

    //Se usa mayoritariamente para el nivel 2, y consiste en saltar para esquivar os obstaculos//
    if(event->key() == Qt::Key_Space) {
        jugador->saltar();
    }

}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_D || event->key() == Qt::Key_A) {
        jugador->setFuerzaMotor(-200);
        jugador->acelerar(false);
    }
}
