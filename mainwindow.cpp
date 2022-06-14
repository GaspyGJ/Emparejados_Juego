#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <algorithm>
#include <random>
#include <QThread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //conecto el temporizador con esta clase e invoco al metodo
    connect(temporizador, SIGNAL(timeout()), this, SLOT(actualizarEstado()));
                   //evento=se termino el tiempo
                   // y se vuelve a repetir
    connect(ui->tarjeta1, SIGNAL(clicked()), this, SLOT(imagenClickeada()));
    connect(ui->tarjeta2, SIGNAL(clicked()), this, SLOT(imagenClickeada()));
    connect(ui->tarjeta3, SIGNAL(clicked()), this, SLOT(imagenClickeada()));
    connect(ui->tarjeta4, SIGNAL(clicked()), this, SLOT(imagenClickeada()));
    connect(ui->tarjeta5, SIGNAL(clicked()), this, SLOT(imagenClickeada()));
    connect(ui->tarjeta6, SIGNAL(clicked()), this, SLOT(imagenClickeada()));
    connect(ui->tarjeta7, SIGNAL(clicked()), this, SLOT(imagenClickeada()));
    connect(ui->tarjeta8, SIGNAL(clicked()), this, SLOT(imagenClickeada()));
    connect(ui->tarjeta9, SIGNAL(clicked()), this, SLOT(imagenClickeada()));
    connect(ui->tarjeta10, SIGNAL(clicked()), this, SLOT(imagenClickeada()));
    connect(ui->tarjeta11, SIGNAL(clicked()), this, SLOT(imagenClickeada()));
    connect(ui->tarjeta12, SIGNAL(clicked()), this, SLOT(imagenClickeada()));

    iniciarJuego();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::iniciarJuego(){

    this->jugadaIniciada=false;

    this->parejasRestantes=6;

    this->temporizador->start(1000);//el temporizador es de 1 segundo, cada 1 segundo se invoca a timeout()
    this->tiempo.setHMS(0,1,0,0); // el tiempo comienza en 1 minuto (h,m,s,ms)
    ui->cronometro->setText(tiempo.toString("m:ss")); // al lebel le agrego el tiempo en formato "minuto:segundos"

    this->puntaje=0;
    this->ui->label_puntaje->setText(QString::number(puntaje)); // QString::number(numero)--> transforma el numero a QString

    mezclar();

    mapear();

    //@@@@@@@@@@@ PARA CUANDO QUEREMOS VOLVER A JUAGR
    //HACER ESTO ui->layoutGrid->setEnable(true);
    /*QList<QPushButton*> botones = ui->layoutGrid->findChildren<QPushButton*>();
        foreach(QPushButton* b , botones){
                b->setEnabled(true);
                b->setStyleSheet("#"+b->objectName()+"{background-color:green}");
        }*/
}


void MainWindow::actualizarEstado(){
    actualizarCronometro();
    verificarResultado();
}

void MainWindow::actualizarCronometro(){
    //se llama cada un segundo
    tiempo= tiempo.addSecs(-1);  // resto un segundo al tiempo
   //al cronometro le agrego el tiempo convertido en string con cierto formato
    ui->cronometro->setText(tiempo.toString("m:ss"));
}

void MainWindow::verificarResultado(){
    //me fijo si termino el tiempo o si di vuelta todas las imagenes

    QMessageBox *msj= new QMessageBox(this);

    msj->setIcon(QMessageBox::Information);
    msj->addButton(QMessageBox::Yes);
    msj->addButton(QMessageBox::No);

    if(parejasRestantes!=0){
        if(tiempo.toString()=="00:00:00"){
            temporizador->stop();

            msj->setWindowTitle("Perdiste");
            msj->setText("Perdiste Perdise NO HAY NADIE PEOR QUE VOS \n Volver a jugar?");

            //@@@@@@@@@@@ HACER ESTO ui->FrameGeneral->setEnable(false);


            if(msj->exec() == QMessageBox::Yes){
              //msj->exec() ejecuta la ventana y retorna el boton precionado.
                reiniciarTarjetas();
                iniciarJuego();
            }
            else{
                //cierro el programa
                QCoreApplication::quit();
            }
        }

        //si no termino el tiempo , el juego sigue
    }
    else{//parejasRestantes==0
        //gano
        temporizador->stop();

        msj->setWindowTitle("Ganaste");

        msj->setText("Ganaste!!!!!! Alto crack pa \n Volver a jugar?");

        if(QMessageBox::Yes == msj->exec()){
          //msj->exec() ejecuta la ventana y retorna el boton precionado.
            reiniciarTarjetas();
            iniciarJuego();
        }
        else{
            //cierro el programa
            QCoreApplication::quit();
        }
    }

}


void MainWindow::mezclar(){

    //utilizamos la funcion shuffle q nos ofrece C++
    //#include <algorithm>
    //#include <random>

    //tomo un valor aleatorio, q se usa como semilla para el algoritmo de aleatoriedad
    unsigned  semilla_Aleatoriedad=std::chrono::system_clock::now().time_since_epoch().count();

    shuffle(this->tarjetas.begin(),this->tarjetas.end(),std::default_random_engine( semilla_Aleatoriedad));

    /* LO MISMO Q HACER :
    std::random_device numeroAleatorio;
    std::default_random_engine semilla_Aleatoriedad(numeroAleatorio);
        //lo mismo q hacer:
        //std::default_random_engine semilla_Aleatoriedad =std::default_random_engine(numeroAleatorio);
    shuffle(this->imagenes.begin(), this->imagenes.end(), semilla_Aleatoriedad);
    */
}

void MainWindow::mapear(){

        /*cada par de boton es asignado a una imagen*/

       //QVector<QString>::iterator iterador = imagenes.begin();
       //auto toma el tipo de valor q devuelve v.begin();
       auto iterador = tarjetas.begin();

       /* Es como una tabla de clave:valor
          QHash<QString,QString> mapeoDeImagenes;
          en clave mapeo[imagenX] tiene como valor = nombrbeImagen
          *Iterator devuelve lo q este en esa posicion*/

        QString file_name;

        for (int i=1; i<=6; i++){

        file_name="color"+QString::number(i)+".png";

        HashDeTarjetas[(*iterador)]=file_name;

        iterador++;
        //dos veces porq dos tarjetas tienen la misma imagen
        HashDeTarjetas[(*iterador)]=file_name;

        iterador++;
        }   
}


void MainWindow::imagenClickeada(){


     if(jugadaIniciada){
         jugadaIniciada=false;

                       //devuelve el objeto q invoco el SLOT
         parJugado[1]=qobject_cast<QPushButton*>(sender());

         mostrarImagen(1);

         deshabilitar_tarjetas();

         comprobarImagenesIguales();

     }
     else{
         //en la primera posicion guardo el boton que fue precionado
         jugadaIniciada=true;
                       // sender() retorna QObject creo , devuelve el objeto q invoco el SLOT
         parJugado[0]=qobject_cast<QPushButton*>(sender());

         parJugado[0]->setEnabled(false);// deshabilito la tarjeta para q no se pueda volver a clickear

         mostrarImagen(0);
     }

}

void MainWindow::mostrarImagen(unsigned posicion){

    QString nombreTarjeta = parJugado[posicion]->objectName();

    QString imagen = HashDeTarjetas[nombreTarjeta];

    parJugado[posicion]->setStyleSheet("#"+nombreTarjeta+"{background-image: url(:/imagenes/"+imagen+");}");
}

void MainWindow::comprobarImagenesIguales(){

    QString imagen1 = HashDeTarjetas[parJugado[0]->objectName()];
    QString imagen2 = HashDeTarjetas[parJugado[1]->objectName()];

    if(imagen1==imagen2){
        //acerto
        puntaje+=15;
        ui->label_puntaje->setText(QString::number(puntaje));
        parejasRestantes--;

        verificarResultado();

        habilitar_tarjetas();

    }
    else{
        //no acerto
        puntaje-=5;
        ui->label_puntaje->setText(QString::number(puntaje));
            //despues de un segundo , llamara a ese SLOT
        QTimer::singleShot(1000,this,SLOT(ocultarImagenes()));
        //funciona como una promesa , sigue ejecutandose las siguientes lineas del programa antes de q pase el tiempo
    }



}


void MainWindow::ocultarImagenes(){

    QString nombreImagen1 = parJugado[0]->objectName();

    QString nombreImagen2 = parJugado[1]->objectName();

    parJugado[0]->setStyleSheet("#"+nombreImagen1+"{}");

    parJugado[1]->setStyleSheet("#"+nombreImagen2+"{}");

    habilitar_tarjetas();
}

void MainWindow::reiniciarTarjetas(){

    ui->tarjeta1->setStyleSheet("#tarjeta1{}");
     ui->tarjeta2->setStyleSheet("#tarjeta2{}");
      ui->tarjeta3->setStyleSheet("#tarjeta3{}");
       ui->tarjeta4->setStyleSheet("#tarjeta4{}");
        ui->tarjeta5->setStyleSheet("#tarjeta5{}");
         ui->tarjeta6->setStyleSheet("#tarjeta6{}");
          ui->tarjeta7->setStyleSheet("#tarjeta7{}");
           ui->tarjeta8->setStyleSheet("#tarjeta8{}");
            ui->tarjeta9->setStyleSheet("#tarjeta9{}");
             ui->tarjeta10->setStyleSheet("#tarjeta10{}");
              ui->tarjeta11->setStyleSheet("#tarjeta11{}");
               ui->tarjeta12->setStyleSheet("#tarjeta12{}");

}


void MainWindow::habilitar_tarjetas(){

    ui->tarjeta1->setEnabled(true);
     ui->tarjeta2->setEnabled(true);
      ui->tarjeta3->setEnabled(true);
       ui->tarjeta4->setEnabled(true);
        ui->tarjeta5->setEnabled(true);
         ui->tarjeta6->setEnabled(true);
          ui->tarjeta7->setEnabled(true);
           ui->tarjeta8->setEnabled(true);
            ui->tarjeta9->setEnabled(true);
             ui->tarjeta10->setEnabled(true);
              ui->tarjeta11->setEnabled(true);
               ui->tarjeta12->setEnabled(true);
}

void MainWindow::deshabilitar_tarjetas(){

    ui->tarjeta1->setEnabled(false);
     ui->tarjeta2->setEnabled(false);
      ui->tarjeta3->setEnabled(false);
       ui->tarjeta4->setEnabled(false);
        ui->tarjeta5->setEnabled(false);
         ui->tarjeta6->setEnabled(false);
          ui->tarjeta7->setEnabled(false);
           ui->tarjeta8->setEnabled(false);
            ui->tarjeta9->setEnabled(false);
             ui->tarjeta10->setEnabled(false);
              ui->tarjeta11->setEnabled(false);
               ui->tarjeta12->setEnabled(false);
}

