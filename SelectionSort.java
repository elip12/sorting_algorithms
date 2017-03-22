/*
SelectionSort: algorithm that sorts an array. Slowest sort method. Not to be used in real life
*/

public class SelectionSort
{
  static int[] A = {1,2,3,4,66,4,2,11,99,0};

  public static void main(String[] args)
  {
    sort(A);
    // prints array
    for(int e: A)
      System.out.print(e + " ");
    System.out.println();
  }

  // gets the ith minimum element, and puts it in the ith position of the array
  public static void sort(int[] A)
  {
    for(int i=0; i<A.length; i++)
    {
      int min = i; //index of min element, not the element itself
      for(int j=i; j<A.length; j++)
        if(A[j] < A[min])
          min = j;
      int temp = A[min];
      A[min] = A[i];
      A[i] = temp;
    }
  }
}
