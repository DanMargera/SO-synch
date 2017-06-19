#include "Worker.h"

DWORD WINAPI startReadingThread( LPVOID arg );
DWORD WINAPI startWritingThread( LPVOID arg );

Worker::Worker(Database* db, Observer* parent) {
  this->db = db;
  this->parent = parent;
}

// Cria uma thread para leitura.
void Worker::startReading(int position, int executionTime, int* target) {
  this->position = position;
  this->time = executionTime;
  this->target = target;
  thread = CreateThread(NULL, 0,
               startReadingThread, // Método inicial da nova thread criada
               this,       // Passa uma referência deste objeto para chamada do método read()
               0,          // Flag de criação padrão
               &threadID); // Armazena o identificador da thread criada

  if (thread == NULL) {
    ErrorHandler::Error(UNABLE_TO_CREATE_THREAD);
  }
}

// Cria uma thread para escrita.
void Worker::startWriting(int position, int executionTime, int value) {
  this->position = position;
  this->time = executionTime;
  this->value = value;
  thread = CreateThread(NULL, 0,
               startWritingThread, // Método inicial da nova thread criada
               this,       // Passa uma referência deste objeto para chamada do método write()
               0,          // Flag de criação padrão
               &threadID); // Armazena o identificador da thread criada
  if (thread == NULL) {
    ErrorHandler::Error(UNABLE_TO_CREATE_THREAD);
  }
}

DWORD WINAPI startReadingThread( LPVOID arg ) {
  if (!arg)
    return 0;

  Worker* w = (Worker*)arg;
  w->read();
  return TRUE;
}

DWORD WINAPI startWritingThread( LPVOID arg ) {
  if (!arg)
    return 0;

  Worker* w = (Worker*)arg;
  w->write();
  return TRUE;
}

// Lê do banco de dados e armazena no endereço apontado por target.
void Worker::read() {
  *(this->target) = this->db->read(position, this->time);
  done();
}

// Escreve no banco de dados o valor indicado na posição indicada.
void Worker::write() {
  this->db->write(position, this->time, value);
  done();
}

// Método chamado após finalizar a leitura/escrita.
void Worker::done() {
  CloseHandle(thread); // Libera a handle
  parent->update(this); // Notifica o observador que esse worker acabou o trabalho.
}
