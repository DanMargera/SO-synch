#include "Dispatcher.h"

DWORD WINAPI startDispatcherThread( LPVOID arg );


Dispatcher::Dispatcher(Database* db, FILE* src) {
  for (size_t i = 0; i < MAX_WORKERS; i++) {
    workers.push_front(new Worker(db, this));  // Inicializa a lista de workers
  }
  this->source = src;

  listSemaphore = CreateSemaphore(NULL, MAX_WORKERS, MAX_WORKERS, NULL);
  listMutex = CreateMutex(NULL, FALSE, NULL);

  if (listSemaphore == NULL)
    ErrorHandler::Error(UNABLE_TO_CREATE_SEMAPHORE);
  if (listMutex == NULL)
    ErrorHandler::Error(UNABLE_TO_CREATE_MUTEX);
}

void Dispatcher::start() {
  thread = CreateThread(NULL, 0,
               startDispatcherThread, // Método inicial da nova thread criada
               this,       // Passa uma referência deste objeto para chamada do método run()
               0,          // Flag de criação padrão
               &threadID); // Armazena ID da thread criada
}

DWORD WINAPI startDispatcherThread( LPVOID  arg ) {
  if (!arg)
    return 0;

  Dispatcher* d = (Dispatcher*)arg;
  d->run();
  return TRUE;
}

/*
* Método com a lógica de execução da Thread despachante,
* Recebe requisições do arquivo para serem processadas.
**/
void Dispatcher::run() {
  char op;
  int position, executionTime, value;
  // Lê requisições do arquivo
  while (fscanf(source, "%c %d %d %d\n", &op, &position, &executionTime, &value) > 2) {
    printf("%c %d %d %d\n", op, position, executionTime, value);
    requestFromSource(op, position, executionTime, value); // Processa requisições recebidas
  }

  // Espera a finalização das threads dos workers
  printf("End of requests.\nFinishing execution...\n");
  while (workers.size() < MAX_WORKERS) {
    Sleep(DISPATCHER_SLEEP_INTERVAL);
  }
}

/*
* Processa as requisições recebidas.
**/
void Dispatcher::requestFromSource(char op, int position, int executionTime, int value) {
  switch(op) {
    case 'L':
      dispatchReader(position, executionTime);
      break;
    case 'E':
      dispatchWriter(position, executionTime, value);
      break;
  }
}

void Dispatcher::dispatchReader(int position, int executionTime) {
  dispatch(position, executionTime, 0, FALSE);
}

void Dispatcher::dispatchWriter(int position, int executionTime, int value) {
  dispatch(position, executionTime, value, TRUE);
}

/*
* Despacha workers para leitura/escrita no banco de dados.
* Mantém a lista de trabalhadores protegida de acesso simultâneo.
**/
void Dispatcher::dispatch(int position, int executionTime, int value, bool writer) {
  DWORD dwWaitSemaphore, dwWaitMutex;

  printf("Seeking/Waiting available workers...\n");
  // Semáforo de controle do número de workers disponíveis
  dwWaitSemaphore = WaitForSingleObject(listSemaphore, INFINITE);
  dwWaitMutex = WaitForSingleObject(listMutex, INFINITE);  // Down no mutex de acesso à lista

  // Delega a requisição ao primeiro worker da lista de disponíveis e remove-o da lista
  Worker* w = workers.front();
  workers.pop_front();
  if (writer) {
    w->startWriting(position, executionTime, value);
  }
  else {
    int target;
    w->startReading(position, executionTime, &target);
  }

  ReleaseMutex(listMutex);
  if (writer)
    printf("Writer dispatched.\n");
  else
    printf("Reader dispatched.\n");
}

/*
* Método usado para receber a notificação de que uma thread trabalhadora
* terminou sua execução. Reinsere o worker ocioso na lista de disponíveis.
**/
void Dispatcher::update(Worker* w) {
  DWORD dwWaitMutex;

  dwWaitMutex = WaitForSingleObject(listMutex, INFINITE);  // Down no mutex de acesso à lista
  workers.push_back(w);
  ReleaseMutex(listMutex);

  ReleaseSemaphore(listSemaphore, 1, NULL);  // Worker adicionado de volta, up no semáforo da lista
}
