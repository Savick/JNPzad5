#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include<set>
#include<utility>
#include<algorithm>
// Covention: 4 spaces, no tabs

/* TODO
 * boost i jakieś mądre kasowanie obiektów, bo erase wykonane na secie nie załatwia nam sprawy, jeśli chodzi o
 * usuwanie
 * bardziej elegancka obsługa wyjątków - teraz po prostu "jest"
 */

template <typename K, typename V>
class PriorityQueue;

template<typename K, typename V>
void swap(PriorityQueue<K,V>&, PriorityQueue<K,V>&);

template <typename K, typename V>
class PriorityQueue {
  private :
    typedef std::pair<K,V>* qElement;

    class keysOrder{ //Klasy służące do porządkowania elementów
      public :
        bool operator()(const PriorityQueue::qElement & a, const PriorityQueue::qElement & b){
          if (a->first < b->first)
            return true;
          else
          if (a->second < b->second) //TODO dodać porządek na adresach
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
            return false;
        }
    } ;
    
        /* TODO
         * Można wyrzucić te klasy poza PriorityQueue, ale wymaga to zabawy z kolejnościa deklaracji itd.,
         * zostawiam to na potem
         */
     
    void copyQElement(qElement a);
    std::multiset<qElement, keysOrder > keys;
    std::multiset<qElement, valuesOrder > values;

  public :
    typedef K key_type;
    typedef V value_type;
    typedef int size_type;

    PriorityQueue(); //non-parameter constructor [O(1)];
    PriorityQueue(PriorityQueue<K, V> const & queue); //copy constructor [O(queue.size())];
    PriorityQueue<K, V> & operator=(PriorityQueue<K, V> const & queue);
    //operator przypisania [O(queue.size())];
    bool empty() const;
    // metoda zwracająca true wtw, gdy kolejka jest pusta [O(1)];
    size_type size() const;
    // metoda zwracająca liczbę par klucz – wartość przechowywanych w kolejce [O(1)];
    void insert(K const & key, V const & value);
    //	metoda wstawiająca do kolejki parę o kluczu key i wartości value [O(log size())];
    V const & minValue() const;
    V const & maxValue() const;
    //metody zwracające odpowiednio najmniejszą i największą wartość przechowywaną w kolejce [O(1)]; w przypadku wywołania którejś z tych metod na pustej strukturze, powinien zostać zgłoszony wyjątek PriorityQueueEmptyException;
    K const & minKey() const;
    K const & maxKey() const;
    //	metody zwracające klucz o przypisanej odpowiednio najmniejszej lub największej wartości [O(1)]; w przypadku wywołania którejś z tych metod na pustej strukturze, powinien zostać zgłoszony wyjątek PriorityQueueEmptyException;
    void deleteMin();
    void deleteMax();
    //	metody usuwające z kolejki parę o odpowiednio najmniejszej lub największej wartości [O(log size())];
    void changeValue(K const & key, V const & value);
    //metoda zmieniająca dotychczasową wartość przypisaną kluczowi key na nową wartość value [O(log size())]; w przypadku gdy w kolejce nie ma pary o kluczu key, powinien zostać zgłoszony wyjątek PriorityQueueNotFoundException();
    void merge(PriorityQueue<K, V> & queue);
    //metoda scalająca zawartość kolejki z podaną kolejką queue; ta operacja usuwa wszystkie elementy z kolejki queue i wstawia je do kolejki *this [O(queue.size() * log (queue.size() + size()))];
    friend void swap<K,V>(PriorityQueue<K, V> & queue0, PriorityQueue<K, V> & queue);
    //metoda zamieniającą zawartość kolejki z podaną kolejką queue (tak jak większość kontenerów w bibliotece standardowej) [O(1)].
    //nie metoda tylko funkcja zewnętrzna
} ;

template< typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(){
/*        keys = std::set<qElement,keysOrder>();
        values = std::set<qElement,valuesOrder>();
      //  chyba niepotrzebne */
}

template< typename K, typename V>
void PriorityQueue<K,V>::copyQElement(qElement a){
   typename std::multiset<qElement, keysOrder >::iterator *it;
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
	
//	std::const_itereator<qElement> p = queue.keys.begin();
 int done = 0;

	typename std::multiset<qElement, keysOrder >::const_iterator p = queue.keys.begin();
	typename std::multiset<qElement, keysOrder >::const_iterator k = queue.keys.end();

 typedef typename std::multiset<qElement, valuesOrder >::iterator two;
 typedef typename std::multiset<qElement, keysOrder >::iterator one;
 
 one *kinput;
 two *vinput;
 kinput = new one;
 try{
   vinput = new two;
 }
 catch(...){
   delete kinput;
   throw;
 }
 try{ //obsługa tych k/v-input
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
         *kinput = keys.insert(*kinput, b);
         if (done==0) done = 1;
         *vinput = values.insert(*vinput, b);
         if (done == 1) done = 2;
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
 } //zewnętrzny try
 catch(...){
   switch (done){
     case 2: vinput->~two();
     case 1: kinput->~one(); break;
   }
   delete kinput;
   delete vinput;
   throw;
 }
 vinput->~two();
 kinput->~one();
 delete kinput;
 delete vinput;
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
  std::multiset<qElement, keysOrder > k2;
  std::multiset<qElement, valuesOrder > v2;
  //jak się powyżej przy alokacji coś wysypie to nie mój problem
  keys.swap(k2);
  values.swap(v2);
  //^ ten fragment się nie wysypie
  try{
    typename std::multiset<qElement, keysOrder >::const_iterator p = queue.keys.begin();
    typename std::multiset<qElement, keysOrder >::const_iterator k = queue.keys.end();
    for ( ; p!=k; ++p ) copyQElement(*p);
    //to już bezpieczne:
    k2.clear();
    v2.clear(); //TODO mądre kasowanie!
  }
  catch (...){
    std::multiset<qElement, keysOrder >.swap(keys,k2);
    std::multiset<qElement, valuesOrder >.swap(values,v2);
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
int PriorityQueue<K,V>::size() const{ // TODO int zamienić na size_type - coś nie chciało działać
  return keys.size();
}

template< typename K, typename V>
void PriorityQueue<K,V>::insert(K const & key, V const & value){
  //tu oczywiście też jest za dużo try-catch, ale to jest forma a nie treść.
  qElement a;
  typename std::multiset<qElement, keysOrder >::iterator *atkeys;
  a = new std::pair<K,V>;
  try{
    atkeys = new typename std::multiset<qElement, keysOrder >::iterator;
  }
  catch(...){
    delete a;
    throw;
  }
  try{
    *a = std::make_pair(key, value);
  }
  catch(...){
    delete a;
    delete atkeys;
    throw;
  }
  try{
    *atkeys = keys.insert(a);
  } catch(...){
    //a->~pair();
    delete a;
    delete atkeys;
    throw;
  }
  try{
    values.insert(a);
  }
  catch (...){
    keys.erase(*atkeys);
    //a->~pair();
    delete a;
    delete atkeys;
    throw;
  }
  delete a;
  delete atkeys;
  
   /*
   qElement a= new std::pair<K,V>; //po co tworzyć taką pustą parę, skoro za chwilę tworzymy inną?
   *a = std::make_pair(key, value);
   keys.insert(a);
   values.insert(a);
   */
}

template< typename K, typename V>
V const & PriorityQueue<K,V>::minValue() const{
   return (*values.begin())->second; //TODO czy to na pewno dobrze?
}

template< typename K, typename V>
V const & PriorityQueue<K,V>::maxValue() const{
   return (*values.end())->second; //TODO czy to na pewno dobrze?
}

template< typename K, typename V>
K const & PriorityQueue<K,V>::minKey() const{
   return (*values.begin())->first; //TODO czy to na pewno dobrze?
}

template< typename K, typename V>
K const & PriorityQueue<K,V>::maxKey() const{ //klucz o przypisanej maksymalnej wartości - czyli nie zmieniamy nakeys
   return (*values.end())->first; //TODO czy to na pewno dobrze?
   //nie rozumiem, czemu to ma być ok ?!?!
}

template< typename K, typename V>
void PriorityQueue<K,V>::merge(PriorityQueue<K, V> & queue){
  int i,j = queue.size(), done;
  typedef typename std::multiset<qElement, keysOrder >::iterator itek1;
  typedef typename std::multiset<qElement, valuesOrder >::iterator itek2;
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
      tabk[i]->~itek1();
      delete tabk[i];
      values.erase(*(tabv[i]));
      tabv[i]->~itek2();
      delete tabv[i];
    }
    delete[] tabk;
    delete[] tabv;
    throw;
  }
  --i; //bo było j.
  for(; i>=0; --i){
    tabk[i]->~itek1();
    delete tabk[i];
    tabv[i]->~itek2();
    delete tabv[i];
  }
  delete[] tabk;
  delete[] tabv;
}

template< typename K, typename V>
void swap(PriorityQueue<K, V> & queue0, PriorityQueue<K, V> & queue){
  queue0.keys.swap(queue.keys);
  queue0.values.swap(queue.values);
}





#endif
