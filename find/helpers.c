/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    int first = 0;
    int last = n-1;
    int middle = (first+last)/2;
    
    while(first <= last)
    {
      if(values[middle] < value)
      {
       first = middle +1;
      }else if(values[middle] == value)
      {
        return true;
      }else
      {
       last = middle-1;
       middle = (first+last)/2;
      } 
    }
    if(first>last)
    {
       return false;
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm: SELECTION SORT!!!
    
    int j;
    int smallest;
    int temp;
    for(int i = 0;i<(n-1);i++)
    {
      smallest = i;
      
      for(j=(i+1);j<n;j++)
      {
        if(values[j] < values[smallest])
        {
          smallest = j;
        }
      }
      if(smallest != i)
      {
         temp = values[i];
         values[i] = values[smallest];
         values[smallest] = temp;
      }
    }
}
