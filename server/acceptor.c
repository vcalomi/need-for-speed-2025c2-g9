#include "./acceptor.h"

Acceptor::Acceptor(const char* port): listen_socket_(port){}

Acceptor::run() {
    while(should_keep_running()){
        Socket client = listen_socket_.accept() // en caso de error lanza excepcion
        // creo queue que va a usar el handler con el cliente
        // creo client hanlder que toma como parametro y socket, y la queue de conexion con el cliente
        // agrego la queue del cliente al monitor
        // lanzo el client handler
        // lo agrego la vector de clients handlers -> esto puede o ser un vector de unique pointers(heap) o una lista (stack) que no nos hace 
        // perder la referencia si se mueve de lugar
        // reap
    }
    //clear
}


Acceptor::~Acceptor(){
    if(this->joinable){
        this->join();
    }
}