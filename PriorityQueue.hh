#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include<set>
#include<utility>
#include<algorithm>
#include<boost/shared_ptr.hpp>

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
class PriorityQueue {
  private :
    typedef boost::shared_ptr< std::pair<K,V> > qElement;

    class keysOrder{ //Klasy służące do porządkowania elementów
      public :
        bool operator()(const PriorityQueue::qElement & a, const PriorityQueue::qElement & b){
          if (a->first < b->first)
            return true;
          else
          if (a->second < b->second) //TODO dodać porządek na adresach
            return true;
          else
            if (a < b)
              return true;
            else
              return false;
        }
    } ;

    class valuesOrder{
      public :
        bool operator()(const PriorityQueue::qElement & a, const PriorityQueue::qElement & b){
          if (a->second < b->second)
            return true;
          else
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
   
    void swap(PriorityQueue<K, V> & queue);
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
   typename std::set<qElement, keysOrder >::iterator *it;
   qElement b;
   b = new std::pair<K,V>;
   try{
     *b = std::make_pair(a->first, a->second);
   //sądzę, że można też: *b = *a;
   }
   catch(...){
     delete b;
     throw;
   }
   try{
     *it = keys.insert(b);
   }
   catch(...){
     //b->~pair();
     delete b;
     throw;
   }
   try{
     values.insert(b);
   }
   catch(...){
     keys.erase(*it);
     //b->~pair();
     delete b;
     throw;
   }
}

template< typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(PriorityQueue<K, V> const & queue){
/*        keys = std::set<qElement,keysOrder>();
        values = std::set<qElement,valuesOrder>();
        // jw. chyba niepotrzebne */

// std::const_itereator<qElement> p = queue.keys.begin();
 //int done = 0;

 typename std::set<qElement, keysOrder >::const_iterator p = queue.keys.begin();
 typename std::set<qElement, keysOrder >::const_iterator k = queue.keys.end();

 typedef typename std::set<qElement, valuesOrder >::iterator two;
 typedef typename std::set<qElement, keysOrder >::iterator one;

 one kinput;
 two vinput;
 /*kinput = new one;
 try{
   vinput = new two;
 }
 catch(...){
   delete kinput;
   throw;
 }*/
 //try{ //obsługa tych k/v-input
   try{
     for ( ; p!=k; ++p ){
       //fragment niemal żywcem z funkcji copy:
       qElement b;
       b = new std::pair<K,V>;
       try{
       *b = std::make_pair((*p)->first, (*p)->second); //drobna zmiana a na (*p)
       }
       catch(...){
        delete b;
        throw;
       }//koniec fragmentu
       try{
         kinput = keys.insert(kinput, b);
         //if (done==0) done = 1;
         vinput = values.insert(vinput, b);
         //if (done == 1) done = 2;
       } catch(...){
          //b->~pair();
          delete b;
          throw;
       }
     } //for
   } catch(...){ //try
     keys.clear(); //na to trzeba niestety uważać.
     values.clear();
     throw;
   }
 /*} //zewnętrzny try
 catch(...){
 //  switch (done){
 //    case 2: vinput->~two();
 //    case 1: kinput->~one(); break;
 //  }
   delete kinput;
   delete vinput;
   throw;
 }*/
 //vinput->~two();
 //kinput->~one();
 //delete kinput;
 //delete vinput;
}

template< typename K, typename V>
PriorityQueue<K,V>& PriorityQueue<K, V>::operator= (PriorityQueue<K, V> const & queue){
  //wersja z czytanki, dla mnie bomba:
  PriorityQueue<K,V> temp (queue); // Copy-constructor -- RAII
  temp.swap (*this); // Non-throwing swap
  return *this;

  /*
  //operator= to prostsza wersja merga - bo w mergu musimy odtwarzać, \
  a tu wystarczy przywrócić poprzedni stan
  std::set<qElement, keysOrder > k2;
  std::set<qElement, valuesOrder > v2;
  //jak się powyżej przy alokacji coś wysypie to nie mój problem
  keys.swap(k2);
  values.swap(v2);
  //^ ten fragment się nie wysypie
  try{
    typename std::set<qElement, keysOrder >::const_iterator p = queue.keys.begin();
    typename std::set<qElement, keysOrder >::const_iterator k = queue.keys.end();
    for ( ; p!=k; ++p ) copyQElement(*p);
    //to już bezpieczne:
    k2.clear();
    v2.clear(); //TODO mądre kasowanie!
  }
  catch (...){
    std::set<qElement, keysOrder >.swap(keys,k2);
    std::set<qElement, valuesOrder >.swap(values,v2);
    k2.clear();
    v2.clear(); //czy to jest w ogóle potrzebne czy destruktor sam się odpali?
    throw;
  }
  return *this;
  */
  
}

template< typename K, typename V>
bool PriorityQueue<K,V>::empty() const{
  return keys.empty();
}

template< typename K, typename V>
typename PriorityQueue<K,V>::size_type PriorityQueue<K,V>::size() const{ 
  return keys.size();
}

template< typename K, typename V>
void PriorityQueue<K,V>::insert(K const & key, V const & value){
  //tu oczywiście też jest za dużo try-catch, ale to jest forma a nie treść.
  qElement a (new std::pair<K,V>);
  typedef typename std::multiset<qElement, keysOrder >::iterator itek;
  itek atkeys;
  /*try{
    atkeys = new typename std::multiset<qElement, keysOrder >::iterator;
  }
  catch(...){
    delete a;
    throw;
  }*/
  try{
    *a = std::make_pair(key, value);
  }
  catch(...){
    delete a;
    //atkeys->~itek();
    //delete atkeys;
    throw;
  }
  try{
    atkeys = keys.insert(a);
  } catch(...){
    //a->~pair();
    delete a;
    //atkeys->~itek();
    //delete atkeys;
    throw;
  }
  try{
    values.insert(a);
  }
  catch (...){
    keys.erase(atkeys); //tu powinno być bez gwiazdki!!! (przy poprawkach się zapodziała)
    //a->~pair();
    delete a;
    //atkeys->~itek();
    //delete atkeys;
    throw;
  }
  //a->~pair();
  delete a;
  //atkeys->~itek();
  //delete atkeys;

   /*
   qElement a= new std::pair<K,V>; //po co tworzyć taką pustą parę, skoro za chwilę tworzymy inną?
   *a = std::make_pair(key, value);
   keys.insert(a);
   values.insert(a);
   */
}
//TODO: wyjątki
template< typename K, typename V>
V const & PriorityQueue<K,V>::minValue() const{
   if (keys.empty()) throw emptyException;
   return (*values.begin())->second; 
}

template< typename K, typename V>
V const & PriorityQueue<K,V>::maxValue() const{
   if (keys.empty()) throw emptyException;
   return (*values.end())->second; 
}

template< typename K, typename V>
K const & PriorityQueue<K,V>::minKey() const{
   if (keys.empty()) throw emptyException;
   return (*values.begin())->first; 
}

template< typename K, typename V>
K const & PriorityQueue<K,V>::maxKey() const{
   if (keys.empty()) throw emptyException;
   return (*values.end())->first; 
}

template< typename K, typename V>
void PriorityQueue<K,V>::merge(PriorityQueue<K, V> & queue){
  int i,j = queue.size(), done;
  typedef typename std::set<qElement, keysOrder >::iterator itek1;
  typedef typename std::set<qElement, valuesOrder >::iterator itek2;
  itek1* otherkeys;

  itek1** tabk;
  itek2** tabv;
  tabk = new itek1* [j]; //może się nie powieść, nie ma problemu
  try{ //znów będzie nieelegancko z wieloma try
     tabv = new itek2* [j];
     //to już się uda:
     for(i=0; i<j; ++i){
       tabk[i] = tabv[i] = NULL;
     }
  }
  catch(...){
    delete [] tabk;
    throw;
  }
  try{
    *otherkeys = queue.keys.begin();
    done = 0;
    for(i=0; i<j; ++i){
      ++(*otherkeys);
      tabk[i] = new itek1;
      done = 1;
      *tabk[i]=keys.insert(**otherkeys);
      done = 2;
      tabv[i] = new itek2;
      done = 3;
      *tabv[i]=values.insert(**otherkeys);
      done = 0;
    }
    //to już jest bezpieczne:
    queue.keys.clear();
    queue.values.clear(); //do poprawki jak wszystkie usuwania
  }
  catch(...){
    switch (done){
      case 3: delete tabv[i];
      case 2: keys.erase(*(tabv[i]));
      case 1: delete tabk[i];
      case 0: --i; break;
    }
    for(; i>=0; --i){
      keys.erase(*(tabk[i]));
      //tabk[i]->~itek1();
      delete tabk[i];
      values.erase(*(tabv[i]));
      //tabv[i]->~itek2();
      delete tabv[i];
    }
    delete[] tabk;
    delete[] tabv;
    throw;
  }
  --i; //bo było j.
  for(; i>=0; --i){
    //tabk[i]->~itek1();
    delete tabk[i];
    //tabv[i]->~itek2();
    delete tabv[i];
  }
  delete[] tabk;
  delete[] tabv;
}


template< typename K, typename V>
void PriorityQueue<K,V>::swap(PriorityQueue<K, V> & queue){
  keys.swap(queue.keys);
  values.swap(queue.values);
}

template <typename K, typename V>
void PriorityQueue<K,V>::deleteMin(){ //ślady poprzedniej interpretacji w kodzie
  if (keys.empty()) throw emptyException;
 
  typedef typename std::multiset<qElement, keysOrder >::iterator itek;
  //itek *i = new itek;
  //try{
  itek i = values.begin();
  //} catch (...){
  //  delete i;
  //}
  //try{
    keys.erase(*i); //usuwamy poprzez wartość, może się sypnąć
  //}
  /*catch(...){
    i->~itek(); //to wszystko zbędne, jeżeli robimy bez wskaźników
    delete i;
  }*/
  values.erase(i); //to się już uda, bo usuwamy poprzez iterator
}

template <typename K, typename V>
void PriorityQueue<K,V>::deleteMax(){ //wersja bez śladów jak wyżej:
  if (keys.empty()) throw emptyException;

  typedef typename std::multiset<qElement, keysOrder >::iterator itek;
  itek i = values.end();
  keys.erase(*i); //usuwamy poprzez wartość, może się sypnąć
  values.erase(i); //to się już uda, bo usuwamy poprzez iterator
}

template<typename K, typename V>
void swap(PriorityQueue<K, V> & queue0, PriorityQueue<K, V> & queue){
        queue0.swap(queue);
}
 





#endif