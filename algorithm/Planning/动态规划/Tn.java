package 动态规划;

public class Tn {
	static int T[];
	//Tn=sum(2*T(i)*T(i-1)),i=1->n
	static int TN(int n)
	{
		//自底向上动态规划
		T=new int[n+1];
		T[0]=2;
		T[1]=2;
		T[2]=T[0]*T[1]*2;
		for(int i=3;i<=n;i++)
		{
			T[i]=T[i-1]+2*T[i-1]*T[i-2];
		}
		return T[n];
	}
	
	public static void main(String args[])
	{
		
		int size=5;
		TN(size);
		for(int i=0;i<=size;i++)
		{
			System.out.println(T[i]);
		}
	}
}

