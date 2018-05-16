#ifndef RANK_SKIPLIST_H
#define RANK_SKIPLIST_H

#include <vector>

using namespace std;

class SkipList
{

private:

    struct node{
          
    int level;
    int rnk;
    int bit;
    const char* rhash;
    const char* dhash;
    const char* hash ;
    node* rgt;
    node* dwn;

    node():rnk(0),bit(0),rgt(0),dwn(0),level(0) 
    {}
   };

            // generates hash strings from byte strings
            char* hexstring(unsigned char*, int );

            //calculate hash value of rgt and dwn node
            const char* set_level_hash(int, int, int, const char*, const char*);
  
            //calculate hash value of any two values
            const char* set_hash(int);

            //generates the node level in tha range [1,maxLevel).
            int coinToss() const;

            //checks if it is possible to go below a certain level
            bool canGoBelow(node *x, int key); 

            //returns a set of pointers to the location at each node where new links are to be created
            std::vector<node*> update(int searchKey);

            //creates a new node and returns a pointer to it
            static node* makeNode();

            //calcutaes hash for each node in skiplist
            void calc_rank();

            //calculates rank for each node in skiplist
            void calc_hash();

            const float probability;
            const int maxLevel;

            // head and tail vectors
            vector<node*> head;
            vector<node*> nil;


public:

            SkipList();
           ~SkipList();

            void insert(int searchKey, int data);

            void search(int searchKey);

            void remove(int searchKey);

            void print() const;

};


#endif
