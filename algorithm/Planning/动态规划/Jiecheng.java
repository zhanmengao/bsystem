package ¶¯Ì¬¹æ»®;

public class Jiecheng {
	static int array[];

	public static int jiecheng(int n)
	{
		if(array[n]!=0)
		{
			return array[n];
		}
		else{
			array[n]=n*jiecheng(n-1);
		}
		return array[n];
	}
	public static void main(String args[])
	{
		int size=8;
		array=new int[size+1];
		array[0]=1;
		array[1]=1;
		System.out.println(jiecheng(size));
	}
}
