import java.util.Arrays;

public class MergeSort
{
 	static int[] A = {};

  	public static void main(String[] args)
  	{
    	int[] B = sort(A);
    	// prints array
    	for(int e: B)
      		System.out.print(e + " ");
    	System.out.println();
  	}

  	public static int[] merge(int[] A, int[] B)
  	{
    	int n = A.length;
    	int m = B.length;
    	int[] output = new int[n+m];
    	int Aindex = 0;
    	int Bindex = 0;

    	for(int outindex=0; outindex<n+m; outindex++)
    	{
      		if((Aindex != n) && (Bindex == m || A[Aindex] <= B[Bindex]))
        		output[outindex] = A[Aindex++];
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
