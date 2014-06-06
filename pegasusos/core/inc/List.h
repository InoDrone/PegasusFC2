/*
 * List.h
 *
 *  Created on: 5 juin 2014
 *      Author: alienx
 */

#ifndef UTIL_LIST_H_
#define UTIL_LIST_H_

//#include "FreeRTOS.h"

namespace util {

  template<class T>
  class List {

    public:
      List();

      bool append(T obj);
      bool remove(T obj);

      void start();
      bool hasNext();
      T next();

      //T operator[](uint16_t index);

    private:
      struct Objects {
	Objects* next;
	T obj;
      };

      Objects* _mObjects;
      Objects* head;
      Objects* current;
      Objects* loopPtr;
  };

  template<class T>
  List<T>::List() :
    _mObjects(nullptr),
    head(nullptr),
    current(nullptr),
    loopPtr(nullptr) {}


  template<class T>
  bool List<T>::append(T obj) {
    Objects* entry = new Objects;// (Objects *)pvPortMalloc(sizeof(Objects));

    entry->obj = obj;
    entry->next = nullptr;

    if (current) {
	current->next = entry;
	current = entry;
    } else {
	current = entry;
	head = current;
    }

    return true;
  }

  template<class T>
  void List<T>::start() {
    loopPtr = head;
  }
  template<class T>
  bool List<T>::hasNext() {
    return loopPtr != nullptr;
  }

  template<class T>
  T List<T>::next() {
    T c = loopPtr->obj;
    loopPtr = loopPtr->next;

    return c;
  }

}



#endif /* UTIL_LIST_H_ */
