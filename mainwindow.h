#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QVector>
#include <QHash>
#include <QString>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTimer *temporizador = new QTimer(); // este va a contar desde 1min para abajo
    //para q algo ocurra de forma repetitiva
    //o para que algo pase cuando halla pasado un determinado tiempo

    QTime tiempo; // por cada segundo del temporizador , se le resta un segundo al tiempo

    unsigned parejasRestantes;
    short int puntaje;
    bool jugadaIniciada;
    QPushButton* parJugado[2];

    QVector<QString> tarjetas{"tarjeta1","tarjeta2","tarjeta3","tarjeta4","tarjeta5",
                              "tarjeta6","tarjeta7","tarjeta8","tarjeta9","tarjeta10",
                              "tarjeta11","tarjeta12"};

    // Es como una tabla de clave:valor
    QHash<QString,QString> HashDeTarjetas;



private slots:

    void iniciarJuego();
    void mezclar();
    void mapear();

    void actualizarEstado();
    void actualizarCronometro();
    void verificarResultado();

    void imagenClickeada();
    void mostrarImagen(unsigned posicion);
    void ocultarImagenes();
    void comprobarImagenesIguales();

    void deshabilitar_tarjetas();
    void habilitar_tarjetas();
    void reiniciarTarjetas();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
