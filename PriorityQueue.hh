#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include<set>
#include<utility>
#include<algorithm>
#include<boost/shared_ptr.hpp>
#include<boost/scoped_ptr.hpp>
#include<boost/scoped_array.hpp>

// Covention: 2 spaces, no tabs
#include <exception>

class PriorityQueueEmptyException: public std::exception {};
class PriorityQueueNotFoundException: public std::exception {};

/* TODO
 * boost i jakieś mądre kasowanie obiektów, bo erase wykonane na secie nie załatwia nam sprawy, jeśli chodzi o
 * usuwanie
 * bardziej elegancka obsługa wyjątków - teraz po prostu "jest"
 */

template <typename K, typename V>
class PriorityQueue;

template <typename K, typename V>
bool operator<(PriorityQueue<K, V> & queue0, PriorityQueue<K, V> & queue);


template <typename K, typename V>
class PriorityQueue {
  private :
    typedef boost::shared_ptr< std::pair<K,V> > qElement;

    class keysOrder{ //Klasy służące do porządkowania elementów
      public :
        bool operator()(const PriorityQueue::qElement & a,
                        const PriorityQueue::qElement & b) throw()
        //zakładamy, że K, V nie rzucają wyjątków z porównania (shared_ptr nie rzuca)
        {
          //można użyć porównania na parze, ale ze względu na symetrię  jest tak:
          if (a->first < b->first)
            return true;
          else if (!(a->first > b->first))
          if (a->second < b->second)
            return true;
          else
            if (a < b)
              return true;
            else
              return false;
        }
    } ;

    class keysOrder0{ //porównanie standardowe
      public :
        bool operator()(const PriorityQueue::qElement & a,
                        const PriorityQueue::qElement & b) throw(){
          if (a->first < b->first)
            return true;
          else if (!(a->first > b->first))
          if (a->second < b->second) //TODO dodać porządek na adresach
            return true;
          else
              return false; //równe czyli nie nierówne
        }
    } ;

    class valuesOrder{
      public :
        bool operator()(const PriorityQueue::qElement & a,
                        const PriorityQueue::qElement & b) throw(){
          if (a->second < b->second)
            return true;
          else if (!(a->second > b->second))
            if (a->first < b->first)
              return true;
            else
              if (a < b)
                return true;
              else
                return false;
        }
    } ;
    
        /* TODO
         * Można wyrzucić te klasy poza PriorityQueue, ale wymaga to zabawy z kolejnościa deklaracji itd.,
         * zostawiam to na potem
         */
     
    void copyQElement(qElement a);
    std::set<qElement, keysOrder > keys;
    std::set<qElement, valuesOrder > values;

    PriorityQueueEmptyException emptyException;
    PriorityQueueNotFoundException notFoundException;
    
  public :
    typedef K key_type;
    typedef V value_type;
    typedef int size_type;

    PriorityQueue(); //non-parameter constructor [O(1)];
    PriorityQueue(PriorityQueue<K, V> const & queue); //copy constructor [O(queue.size())];
    PriorityQueue<K, V> & operator=(PriorityQueue<K, V> const & queue);
    
    bool empty() const;
    
    size_type size() const;
    
    void insert(K const & key, V const & value);
    
    V const & minValue() const;
    V const & maxValue() const;
    
    K const & minKey() const;
    K const & maxKey() const;
    
    void deleteMin();
    void deleteMax();
    
    void changeValue(K const & key, V const & value);
    
    void merge(PriorityQueue<K, V> & queue);
   
    void swap(PriorityQueue<K, V> & queue) throw();

    friend bool operator< <K,V>(PriorityQueue<K, V> & queue0,
                                PriorityQueue<K, V> & queue);
    //pozostałe operatory generujemy z pomocą tego jednego
} ;


//-----------------------------------------------------------------------------------------------------

template< typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(){
/*        keys = std::set<qElement,keysOrder>();
        values = std::set<qElement,valuesOrder>();
      //  chyba niepotrzebne */
}

template< typename K, typename V>
void PriorityQueue<K,V>::copyQElement(qElement a){
   typename std::set<qElement, keysOrder >::iterator it;
   qElement b (new std::pair<K,V>);
   *b = std::make_pair(a->first, a->second);
   it = keys.insert(b);//.first
   try{
     values.insert(b);
   }
   catch(...){
     keys.erase(it);
     throw;
   }
}

template< typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(PriorityQueue<K, V> const & queue){

 typename std::set<qElement, keysOrder >::const_iterator p = queue.keys.begin();
 typename std::set<qElement, keysOrder >::const_iterator k = queue.keys.end();

 typedef typename std::set<qElement, valuesOrder >::iterator two;
 typedef typename std::set<qElement, keysOrder >::iterator one;

 one kinput = keys.begin();
 two vinput = values.begin(); //optymalizują działanie.
 for ( ; p!=k; ++p ){
   //fragment niemal żywcem z funkcji copy:
   qElement b (new std::pair<K,V>);
   *b = std::make_pair((*p)->first, (*p)->second); //drobna zmiana a na (*p)
   kinput = keys.insert(kinput, b);
   vinput = values.insert(vinput, b);
 }
}

template< typename K, typename V>
PriorityQueue<K,V>& PriorityQueue<K, V>::operator= (PriorityQueue<K, V> const & queue){
  //wersja z czytanki, dla mnie bomba:
  PriorityQueue<K,V> temp (queue); // Copy-constructor -- RAII
  temp.swap (*this); // Non-throwing swap
  return *this;
}

template< typename K, typename V>
bool PriorityQueue<K,V>::empty() const{
  return keys.empty();
}

template< typename K, typename V> //no-throw??
typename PriorityQueue<K,V>::size_type PriorityQueue<K,V>::size() const{ 
  return keys.size();
}

template< typename K, typename V>
void PriorityQueue<K,V>::insert(K const & key, V const & value){
  qElement a (new std::pair<K,V>);
  typedef typename std::set<qElement, keysOrder >::iterator itek;
  itek atkeys;
  *a = std::make_pair(key, value);
  atkeys = keys.insert(a).first;
  try{
    values.insert(a);
  }
  catch (...){
    keys.erase(atkeys); //tu powinno być bez gwiazdki!!! (przy poprawkach się zapodziała)
    throw;
  }
}

template< typename K, typename V>
V const & PriorityQueue<K,V>::minValue() const{
   if (keys.empty()) throw emptyException;
   return (*values.begin())->second; 
}

template< typename K, typename V>
V const & PriorityQueue<K,V>::maxValue() const{
   if (keys.empty()) throw emptyException;
   return (*--values.end())->second; 
}

template< typename K, typename V>
K const & PriorityQueue<K,V>::minKey() const{
   if (keys.empty()) throw emptyException;
   return (*values.begin())->first; 
}

template< typename K, typename V>
K const & PriorityQueue<K,V>::maxKey() const{
   if (keys.empty()) throw emptyException;
   return (*--values.end())->first; 
}

template< typename K, typename V>
void PriorityQueue<K,V>::merge(PriorityQueue<K, V> & queue){
  int i,j = queue.size(), done;
  typedef typename std::set<qElement, keysOrder >::iterator itek1;
  typedef typename std::set<qElement, valuesOrder >::iterator itek2;
  itek1 otherkeys;

  boost::scoped_array < boost::scoped_ptr<itek1> > tabk (new boost::scoped_ptr<itek1>[j]);
  boost::scoped_array < boost::scoped_ptr<itek2> > tabv (new boost::scoped_ptr<itek2>[j]);
  try{
    otherkeys = queue.keys.begin();
    done = 0;
    for(i=0; i<j; ++i){
      tabk[i].reset ( new itek1 );
      *tabk[i]=keys.insert(*otherkeys).first;
      done = 2;
      tabv[i].reset( new itek2 );
      *tabv[i]=values.insert(*otherkeys).first;
      done = 0;
      ++otherkeys;
    }
    //to już jest bezpieczne:
    queue.keys.clear();
    queue.values.clear();
  }
  catch(...){
    switch (done){
      case 2: keys.erase(*(tabv[i]));
      case 0: --i; break;
    }
    for(; i>=0; --i){
      keys.erase(*(tabk[i]));
      values.erase(*(tabv[i]));
    }
  }
}


template< typename K, typename V>
void PriorityQueue<K,V>::swap(PriorityQueue<K, V> & queue) throw(){
  keys.swap(queue.keys);
  values.swap(queue.values);
}

template <typename K, typename V>
void PriorityQueue<K,V>::deleteMin(){
  if (keys.empty()) return;
 
  typedef typename std::set<qElement, keysOrder >::iterator itek;

  itek i = values.begin();
  keys.erase(*i); //usuwamy poprzez wartość, może się sypnąć
  values.erase(i); //to się już uda, bo usuwamy poprzez iterator
}

template <typename K, typename V>
void PriorityQueue<K,V>::deleteMax(){
  if (keys.empty()) return;

  typedef typename std::set<qElement, keysOrder >::iterator itek;
  itek i = --values.end();
  keys.erase(*i); //usuwamy poprzez wartość, może się sypnąć
  values.erase(i); //to się już uda, bo usuwamy poprzez iterator
}

template<typename K, typename V>
void swap(PriorityQueue<K, V> & queue0, PriorityQueue<K, V> & queue) throw(){
        queue0.swap(queue);
}

template< typename K, typename V>
bool operator==(PriorityQueue<K, V> & queue0, PriorityQueue<K, V> & queue){
  return (!(queue0<queue) && !(queue<queue0));
 /*można przepisać z użyciem:
 bool equal ( InputIterator1 first1, InputIterator1 last1,
               InputIterator2 first2, BinaryPredicate pred );
               ale po co pisać predykat, jak można nie pisać^^
 */
}

template< typename K, typename V>
bool operator!=(PriorityQueue<K, V> & queue0, PriorityQueue<K, V> & queue){
  return !(queue0==queue);
}

template< typename K, typename V>
bool operator<=(PriorityQueue<K, V> & queue0, PriorityQueue<K, V> & queue){
  return (queue0 < queue || queue0==queue);
  //oczywiście można przepisać wprost i będzie optymalniej bo będą 2 porównania\
    zamiast 3 (dwóch takich samych) (wymagałoby to friend)
}

template< typename K, typename V>
bool operator>=(PriorityQueue<K, V> & queue0, PriorityQueue<K, V> & queue){
  return queue <= queue0;
}

template< typename K, typename V>
bool operator<(PriorityQueue<K, V> & queue0, PriorityQueue<K, V> & queue){
  return lexicographical_compare (queue0.keys.begin(), queue0.keys.end(),
                                 queue.keys.begin(), queue.keys.end(),
                                 typename PriorityQueue<K, V>::keysOrder0() );
}

template< typename K, typename V>
bool operator>(PriorityQueue<K, V> & queue0, PriorityQueue<K, V> & queue){
  return queue < queue0;
}
 





#endif
