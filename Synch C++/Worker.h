#ifndef READER_H
#define READER_H

#include "Database.h"
#include "Observer.h"

class Worker {
public:
  Worker(Database* db, Observer* parent);
  void startReading(int position, int executionTime, int* target);
  void startWriting(int position, int executionTime, int value);
  void read();
  void write();
private:
  Database* db;
  Observer* parent;  // Observador à ser notificado
  int* target;  // Ponteiro para o endereço de escrita ao realizar uma leitura
  int position, time, value;
  HANDLE thread;  // Handle da thread trabalhadora
  DWORD threadID;
  void done();
};

#endif
