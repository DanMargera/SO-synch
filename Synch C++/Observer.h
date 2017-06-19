#ifndef OBSERVER_H
#define OBSERVER_h

class Worker;

/*
* Interface usada para o padrão observador.
**/
class Observer {
public:
  virtual void update(Worker* w) = 0;
};

#endif
