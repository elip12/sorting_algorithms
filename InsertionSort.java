

public class InsertionSort
{
 	static int[] A = {};

 	public static void main(String[] args)
 	{
   		sort(A);
   		// prints array
   		for(int e: A)
     	System.out.print(e + " ");
   		System.out.println();
  	}

  	public static void sort(int[] A)
  	{
    	for(int i=1; i<A.length; i++)
    	{
      		int j=i;
      		while(j >= 1 && A[j-1] > A[j])
      		{
        		int temp = A[j];
        		A[j] = A[j-1];
        		A[j-1] = temp;
        		j--;
      		}
    	}
  	}
}
