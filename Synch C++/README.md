# Implementação em C++

Um vetor de 100 inteiros é lido do arquivo database.txt, e em seguida, são lidas instruções de leitura ou escrita em posições do vetor.
A proteção de acesso simultâneo é feita usando *mutex* e semáforos, e é implementada dentro da classe Database.
A classe Dispatcher processa os pedidos contidos no arquivo e despacha *threads* trabalhadoras para atender às requisições.
Utiliza-se o padrão observador para notificar o fim de uma *thread* trabalhadora.

Obs.: Junto às requisições é passado um "tempo de execução" para simular
o tempo gasto por escritores e leitores,como pedido no enunciado do trabalho.
