#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "Worker.h"
#include <list>

#define MAX_WORKERS 10
#define DISPATCHER_SLEEP_INTERVAL 15

class Dispatcher: public Observer {
public:
  Dispatcher(Database* db, FILE* src);
  ~Dispatcher();
  void start();
  void run();
  void update(Worker* w);
  HANDLE getThread() { return thread; };
private:
  FILE* source; // Fonte das requisições de leitura/escrita.
  HANDLE listSemaphore, listMutex;
  HANDLE thread; // Handle da thread dispachante
  DWORD threadID;
  std::list<Worker*> workers; // Lista de Workers disponíveis no momento.
  void requestFromSource(char op, int position, int executionTime, int value);
  void dispatchReader(int position, int executionTime);
  void dispatchWriter(int position, int executionTime, int value);
  void dispatch(int position, int executionTime, int value, bool writer);
};

#endif
