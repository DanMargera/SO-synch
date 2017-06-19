#ifndef DATABASE_H
#define DATABASE_H

#include <Windows.h>
#include <stdio.h>
#include "ErrorHandler.h"

#define DATA_VECTOR_LENGTH 100

class Database {
public:
  Database(FILE* src);
  void write(int position, int sleepTime, int value);
  int read(int position, int sleepTime);
  void print();
private:
  HANDLE rCountMutex, wCountMutex, readMutex, writeMutex;
  int readerCount, writerCount;
  int data[DATA_VECTOR_LENGTH];
};

#endif
