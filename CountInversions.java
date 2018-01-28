/*
  CountInversions.java
  Author: Eli Pandolfo

  counts the number of inersions in an array, 
  using a divide and conquer strategy. Run time O(nlogn).
  Uses a global variable to allow mergesort code to remain basically unchanged.
*/

import java.util.Arrays;

public class CountInversions
{
 	static int[] A = {1, 2, 3, 4, 1}; // 3 
  static int inv = 0;

  	public static void main(String[] args)
  	{
      int[] sorted = sort(A);
      for(int e: sorted)
    	  System.out.print(e + " ");
      System.out.println("\ninversions: " + inv);
  	}

  	public static int[] merge(int[] A, int[] B)
  	{
    	int n = A.length;
    	int m = B.length;
    	int[] output = new int[n + m];
    	int Aindex = 0;
    	int Bindex = 0;

    	for(int outindex = 0; outindex < n + m; outindex++)
    	{
      		if((Aindex != n) && (Bindex == m || A[Aindex] <= B[Bindex]))
        		output[outindex] = A[Aindex++];
      		else if((Aindex != n))
          {
            output[outindex] = B[Bindex++];
            inv++;
          }
          else
        		output[outindex] = B[Bindex++];
    	}
    	return output;
  	}

  	public static int[] sort(int[] A)
  	{
    	int n = A.length;
      if(n == 1)
          return A;
      int[] C = sort(Arrays.copyOfRange(A, 0, n/2));
      int[] D = sort(Arrays.copyOfRange(A, n/2, n));

      int[] B = merge(C, D);
      return B;
    }
} 
