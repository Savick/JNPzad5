#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include<set>
#include<utility>
#include<algorithm>
// Covention: 4 spaces, no tabs

/* TODO
 * boost i jakieś mądre kasowanie obiektów, bo erase wykonane na secie nie załatwia nam sprawy, jeśli chodzi o
 * usuwanie
 */

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
    void swap(PriorityQueue<K, V> & queue);
    //metoda zamieniającą zawartość kolejki z podaną kolejką queue (tak jak większość kontenerów w bibliotece standardowej) [O(1)].
} ;

template< typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(){
/*        keys = std::set<qElement,keysOrder>();
        values = std::set<qElement,valuesOrder>();
      //  chyba niepotrzebne */
}

template< typename K, typename V>
void PriorityQueue<K,V>::copyQElement(qElement a){
   qElement b = std::copy(*a);
   keys.insert(b);
   values.insert(b);
}

template< typename K, typename V>
PriorityQueue<K,V>::PriorityQueue(PriorityQueue<K, V> const & queue){
/*        keys = std::set<qElement,keysOrder>();
        values = std::set<qElement,valuesOrder>();
        // jw. chyba niepotrzebne */
  std::for_each(queue.keys.begin(),queue.keys.end(),copyQElement); //coś nie działa!!!!!!
}

template< typename K, typename V>
PriorityQueue<K,V>& PriorityQueue<K, V>::operator= (PriorityQueue<K, V> const & queue){
  keys.clear(); 
  values.clear(); //TODO mądre kasowanie!
  std::for_each(queue.keys.begin(),queue.keys.end(),copyQElement); //jw. do przejrzenia
  return *this;
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
   qElement a= new std::pair<K,V>;
   *a = std::make_pair(key, value);
   keys.insert(a);
   values.insert(a);
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
}



#endif
