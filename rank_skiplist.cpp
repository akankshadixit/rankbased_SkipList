#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<algorithm>
#include<sstream>
#include <vector>
#include <string.h>


#include<rank_skiplist.h>
#include<sha256.h>

using namespace std;

//===================================================Private Member Functions=========================================


char* SkipList::hexstring(unsigned char* bytes, int len)
{
    char* buffer    = (char*)malloc(sizeof(char)*((2*len)+1));
    char* temp      = buffer;
    for(int i=0;i<len;i++) {
        sprintf(temp,"%02x",bytes[i]);
        temp += 2;
    }
    *temp = '\0';
    return buffer;
}


const char* SkipList::set_level_hash(int level, int rank, int bit, const char* x, const char* y)
{ 
   int len = strlen(x) + strlen(y);
   char* result = (char*)malloc (sizeof(char)*(2*(len)+10));


   char int_str[32];
   sprintf(int_str,"%d", level);
   strcpy(result, int_str);

   sprintf(int_str,"%d", rank);
   strcat(result,int_str);

   sprintf(int_str,"%d", bit);
   strcat(result,int_str);

   strcat(result,x);
   strcat(result,y);
  
   sha256_hash_t buffer[32];
   sha256(buffer,result,8*(len+10));
   char* string = hexstring((unsigned char*)buffer,32);
   return string;
}


const char* SkipList::set_hash(int data)
{
    char* str = (char*)malloc(sizeof(char));
    sprintf(str, "%d", data);
    int len = strlen(str);
    char* result = (char*)malloc (sizeof(char)*(2*(len)+1));

    strcpy(result,str);
   
    sha256_hash_t buffer[32];
    sha256(buffer,result,8*(len));
    char* string = hexstring((unsigned char*)buffer,32);

    return string;

}



int SkipList::coinToss() const
{
    int flip = 1 + rand() % 2;    // 1 = heads , 2 = tails
    return flip;
}


bool SkipList::canGoBelow(node *x, int searchKey)
{
  int dkey;

  if(x->dwn == 0)
  return false;
  else
  {
   dkey = x->dwn->rnk;
   if(searchKey <= dkey)
   return true;
   else
   return false;
  }
}


vector<SkipList::node*>SkipList::update(int searchKey)
{
  vector<node*> result(maxLevel);
  node *x;
  int key;
  x = head[maxLevel-1];

  // handles the case when first element is being inserted
  if(searchKey == 1)
     {
         for(int i = maxLevel-1; i>= 0; i--)
         {
           x = head[i];
           result[i] = x;
         }
     }
   else
      {
         key = searchKey;
         for(int i = maxLevel-1; i>=0; i--)
         {
           while( x->rgt->rnk != 0 && x->rgt != 0 && (canGoBelow(x,key) == false) && key != 1) 
           {
             key = key - (x->rnk - x->rgt->rnk);
             x = x->rgt;
           }
           result[i] = x;

           if(i>0)
           {
             x = x->dwn;
           }
         }
       }

   return result;
}


SkipList::node* SkipList::makeNode()
{
   return new node;
}


void SkipList::calc_rank()
{

 node *x ;
 for(int i = 0; i<maxLevel; i++)
  {
    vector<node*> list;
    x = head[i];

    while(x->rgt != 0)
    {
       list.push_back(x);
       x = x->rgt;
    }

    if(i == 0)
    {
      for(int j = list.size()-1; j>=0;j--)
      {
         if(list[j]->bit == 0)
         {
           if(list[j] == head[i])
           {
            list[j]->rnk = 0;
           }
           else
           {
             list[j]->rnk = 1;
           }
         }
         else
         {
           if(list[j]->rgt->rnk == 0)
           {
             list[j]->rnk = 1;
           }
           else if(list[j] == head[i])
           {
             list[j]->rnk = list[j]->rgt->rnk;
           }
           else
           {
             list[j]->rnk = list[j]->rgt->rnk + 1;
           }
         }
      }

    }
    else
    {
      for(int j = list.size()-1; j>=0; j--)
      {

        if(list[j]->bit == 0)
        {
          list[j]->rnk = list[j]->dwn->rnk;  
        }
       else
        {
          list[j]->rnk = list[j]->rgt->rnk + list[j]->dwn->rnk;
        }
      }
    }

  }

}



void SkipList::calc_hash()
{
  node* x;

  for(int i = 0;i<maxLevel ; i++)
  {
    vector<node*> list;
    x = head[i];
   
    while(x->rgt !=0)
    {
      list.push_back(x);
      x = x->rgt;
    }

    
    for(int j = list.size()-1; j>=0;j--)
    {
      if(i != 0)
       list[j]->dhash = list[j]->dwn->hash;

      list[j]->rhash = list[j]->rgt->hash; 
      list[j]->hash = set_level_hash(list[j]->level, list[j]->rnk, list[j]->bit, list[j]->dhash,list[j]->rhash); 
    }
  }
}


//==============================================Public Member Functions======================================================



SkipList::SkipList():
 probability(0.5), maxLevel(16){

     head.resize(maxLevel, 0);
     nil.resize(maxLevel,0);
    
     for(int i = 0; i < maxLevel ;i++)
     {
        head[i] = new node;
        nil[i] = new node;

 	head[i]->level = i;
	nil[i]->level = i;
        nil[i]->hash = set_hash(1);
        nil[i]->rhash = set_hash(1);

        head[i]->rgt = nil[i];
        head[i]->bit = 1;
     
       
        if(i == 0)
        {
	 nil[i]->dhash = set_hash(1);
         head[i]->dhash = set_hash(1);      
	}

        if(i>0)
        {
          head[i]->dwn = head[i-1];
          nil[i] ->dwn = nil[i-1];
  
	  nil[i]->dhash = nil[i]->dwn->hash;        
        }
           
      }
}

SkipList::~SkipList()
{
   node* temp, *d, *r;
   d = head[maxLevel-1];

   while(d != head[0])
   {
     r = d;
     temp = r;
     r = r->rgt;
     d = d->dwn;
     delete temp;
   }
   delete d;
   delete r;
}


void SkipList::insert(int searchKey, int data)
{
   vector<node*> result = update(searchKey + 1);
   node *ptr, *x;
   int coin = 1 ;
   int i = 0;
  
  do
   { 
      ptr = makeNode();
      x = result[i]->rgt;
      result[i]->rgt = ptr;
      ptr->rgt = x;
   
      ptr->bit = result[i]->bit;
      result[i]->bit = 1;
      ptr->level = i;  

     //insert leaf node hash for tag
      if(i == 0)
      {
        const char* z = set_hash(data);
        ptr->dhash = z;             
      }
      
    //join down pointers
      if(i>0)
      {
        x = result[i-1]->rgt;
        ptr->dwn = x;
        result[i-1]->bit = 0;
      }
  
      i++;
      coin = coinToss();
   }while(coin == 1);


calc_rank(); 
calc_hash();

}


void SkipList::search(int searchKey)
{
  node *x;
  int key;
  x = head[maxLevel-1];

  if(searchKey == 1)
  {
    printf("Hash of element present at %d is: %s\n" , searchKey+1 , head[0]->rgt->hash);
  }
  else
  {
       key = searchKey + 1;
       for(int i = maxLevel-1; i>=0; i--)
        {
          while( x->rgt->rnk != 0 && x->rgt != 0 && (canGoBelow(x,key) == false) && key != 1) //&& key != 1
          {
            key = key - (x->rnk - x->rgt->rnk);
            x = x->rgt;
          }

          if(i>0)
            x = x->dwn;    
        } 

    printf("Hash of element present at %d is: %s\n" , searchKey+1, x->hash);    
  }
}



void SkipList::remove(int searchKey)
{
 vector<node*> prev = update(searchKey+1);
 vector<node*> cur = update(searchKey+2);

 for(int i = 0;i<maxLevel;i++)
 {
  if(prev[i] != cur[i])
  {
    prev[i]->rgt = cur[i]->rgt;
    prev[i]->bit = cur[i]->bit;
    free(cur[i]);
  }
 }
  calc_rank();
  calc_hash();
}



void SkipList::print() const
{
 node *ptr;

   for(int i = 0; i < maxLevel ;i++)
   {
    ptr = head[i];
    cout<<"LEVEL: "<<i<<endl;

   while(ptr->rgt != 0)
   {
     cout<<"Bit: "<<ptr->bit<<"  Rank:  "<<ptr->rnk<<"  Hash:  "<<ptr->hash<<"  Right: "<<ptr->rgt<<"  Down: "<<ptr->dwn<<endl;
     ptr = ptr->rgt;
   }
    cout<<endl;
   }
    cout<<endl;
}
