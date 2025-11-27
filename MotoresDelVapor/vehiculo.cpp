#include "vehiculo.h"

Vehiculo::Vehiculo() {
    masa = 50.0;
    fuerzaMotor = 150.0;
    friccion = 0.08;
    motorEncendido = false;

    gravedad = 9.8;
    enElAire = false;
}

void Vehiculo::acelerar(bool activado) {
    motorEncendido = activado;
}

void Vehiculo::saltar() {
    if (!enElAire) {
        velocidad_y = -60; //Impulso hacia arriba cuando salta//
        enElAire = true;
    }
}

//Fisica de aceleracion y inceria para el vehiculo o jugador, como sea//
void Vehiculo::actualizarFisicaNivel1() {
    double fuerzaActual = 0;
    if (motorEncendido) {
        fuerzaActual = fuerzaMotor;
    }

    //friccion//
    double fuerzaRozamiento = velocidad_x * friccion;

    double fuerzaNeta = fuerzaActual - fuerzaRozamiento;
    double aceleracion = fuerzaNeta / masa;

    double dt = 0.1;
    velocidad_x += aceleracion * dt;
    x_pos += velocidad_x * dt;

    setPosicion(x_pos, y_pos);
}

//Fisica para el movimiento parabolico, un poco mas facil
void Vehiculo::actualizarFisicaNivel2() {
    double dt = 0.1;

    //Se aplica la graverdad constante en parabolica//
    velocidad_y += gravedad * dt;
    y_pos += velocidad_y * dt;

    //Se simula el suelo en 400//
    if (y_pos >= 400) {
        y_pos = 400;
        velocidad_y = 0;
        enElAire = false;
    }

    //Y se mantiene una constatne actulizacion de las fisicas dependiendo del nivel, en este caso el 1//
    actualizarFisicaNivel1();
}

void Vehiculo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(Qt::blue); //Se hace que el jugador o carro sea de azul//
    painter->drawRect(0, 0, 60, 30); //Se setea o se pone la forma del carro, o las medidaas//
}
