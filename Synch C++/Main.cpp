#include "Dispatcher.h"

using namespace std;

int main(int argc, char const* argv[]) {

  FILE* fp;
  if ((fp = fopen("database.txt", "r")) == NULL) { // Abre o arquivo de entrada para leitura
    printf("No such file\n");
    return 1;
  }

  Database* db = new Database(fp);  // Inicializa o banco de dados à partir do arquivo

  Dispatcher* dispatcher = new Dispatcher(db, fp);  // Inicializa o despachante
  dispatcher->start();  // Dá início ao processo de despacho

  DWORD waitDispatcher;
  // Espera até que o despachante termine sua execução
  waitDispatcher = WaitForSingleObject(dispatcher->getThread(), INFINITE);

  db->print();
  fclose(fp);


  return 0;
}
