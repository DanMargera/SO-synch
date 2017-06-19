#include "Database.h"

Database::Database(FILE* src) {
  for (size_t i = 0; i < DATA_VECTOR_LENGTH; i++) {  // Inicializa o vetor com os dados do arquivo
    fscanf(src, "%d\n", &data[i]);
  }

  readerCount = 0;
  writerCount = 0;

  wCountMutex = CreateMutex(NULL, FALSE, NULL);
  rCountMutex = CreateMutex(NULL, FALSE, NULL);
  readMutex = CreateMutex(NULL, FALSE, NULL);
  writeMutex = CreateMutex(NULL, FALSE, NULL);

  if (wCountMutex == NULL || rCountMutex == NULL || readMutex == NULL || writeMutex == NULL) {
    ErrorHandler::Error(UNABLE_TO_CREATE_MUTEX);
  }
}

/*
* Escrita no banco de dados com proteção de acesso simultâneo
**/
void Database::write(int position, int sleepTime, int value) {
  DWORD dwWaitMutex;

  dwWaitMutex = WaitForSingleObject(wCountMutex, INFINITE); // down mutex contador de escritores
  writerCount++;
  if (writerCount == 1)  // se é o primeiro escritor -> down mutex de leitura
    dwWaitMutex = WaitForSingleObject(readMutex, INFINITE);
  ReleaseMutex(wCountMutex);  // up mutex contador de escritores

  dwWaitMutex = WaitForSingleObject(writeMutex, INFINITE);  // down mutex de escrita
  Sleep(sleepTime);
  data[position] = value;
  ReleaseMutex(writeMutex);  // up mutex de escrita

  dwWaitMutex = WaitForSingleObject(wCountMutex, INFINITE);  // down mutex contador de escritores
  writerCount--;
  if (writerCount == 0)  // se é o ultimo escritor -> up mutex de leitura
    ReleaseMutex(readMutex);
  ReleaseMutex(wCountMutex);  // up mutex contador de escritores
}

/*
* Leitura do banco de dados com proteção de acesso simultâneo
**/
int Database::read(int position, int sleepTime) {
  DWORD dwWaitMutex;
  int returnValue;

  dwWaitMutex = WaitForSingleObject(readMutex, INFINITE); // down mutex de leitura
  dwWaitMutex = WaitForSingleObject(rCountMutex, INFINITE); // down mutex contador de leitores
  readerCount++;
  if (readerCount == 1)  // se é o primeiro leitor -> down mutex de escrita
    dwWaitMutex = WaitForSingleObject(writeMutex, INFINITE);
  ReleaseMutex(rCountMutex);
  ReleaseMutex(readMutex);

  Sleep(sleepTime);
  returnValue = data[position];

  dwWaitMutex = WaitForSingleObject(rCountMutex, INFINITE);
  readerCount--;
  if (readerCount == 0)  // se é o último leitor -> up mutex de escrita
    ReleaseMutex(writeMutex);
  ReleaseMutex(rCountMutex);  // up mutex contador de leitores

  return returnValue;
}

// Imprime o vetor de dados em uma linha.
void Database::print() {
  for (size_t i = 0; i < DATA_VECTOR_LENGTH; i++) {
    printf("%d ", data[i]);
  }
  printf("\n");
}
