
public class TableSort
{
  static int[] array = {9,8,7,6,3,2,1};

  public static void main(String[] args)
  {
    sort(array);

    for(int e: array)
      System.out.print(e);
    System.out.println();
  }

  public static int[] sort(int[] a)
  {
    //1st pass: get biggest int in array
    int max = 0;
    for(int e: a)
      if(e > max)
        max = e;

    // create new array with max val
    Integer[] temp = new Integer[max];

    //2nd pass: fill temp array
    for(int e: a)
    {
      Integer obj = temp[e];
      if(obj == null) // no int w/ that value is in the table already
        temp[e] = (Integer)e;
      else if(obj instanceof ArrayList<?>) // more than 1 item w/ that priority in the table
      {
      	ArrayList<Integer> val = (ArrayList<Integer>)temp[e];
        val.add(e);
      }
      else // (int)  1 item w/ that priority exists already;
      {
        ArrayList<Integer> list = new ArrayList<Integer>();
        list.add(e);
        list.add(e);
        temp[e] = list;
      }
    }

    //3rd pass: create output array from temp array
    int[] output = new int[a.length];
    int i = 0;
    while(i < output.length)
    {
      if(temp[i] instanceof ArrayList<?>)
      {
        ArrayList<Integer> list = (ArrayList<Integer>)temp[i];
        for(Integer e: list)
          output[i++] = e;
      }
      else if(temp[i] instanceof Integer)
      {
        output
      }
    }
  }
}
