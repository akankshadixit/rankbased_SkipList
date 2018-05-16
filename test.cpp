#include<stdio.h>
#include<rank_skiplist.h>

int main()
{
  SkipList s;

  for(int i = 0; i< 5; i++)
  {
    s.insert(i , i+1);
  } 

  s.print();
  s.search(3);

  s.remove(2);
  s.print();

 return 0;
}
