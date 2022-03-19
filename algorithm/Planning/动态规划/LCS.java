package 动态规划;
//LCS：最长公共子序列
public class LCS {
	public static void main(String args[])
	{
		String X="ABCBDAB";
		String Y="BDCABA";
		System.out.println(new LCS().LcS_LENGTH(X, Y));
	}
	//自顶向下构造LCS
	public int LcS_LENGTH(String X,String Y)
	{
		//记录X[0...i]与Y[0...j]的最优解
		int c[][]=new int[X.length()][Y.length()];
		//初始化
		for(int i=0;i<c.length;i++)
		{
			for(int j=0;j<c[0].length;j++)
			{
				if(j==0 || i==0)
				{
					c[i][j]=0;
				}else{
					c[i][j]=-1;
				}		
			}
		}
		//自顶向下构造
		return LcS_LENGTH(c, X.toCharArray(), Y.toCharArray(), X.length()-1, Y.length()-1);
	}
	private int LcS_LENGTH(int [][]c,char []X,char []Y,int i,int j)
	{
		//如果已经求解过了
		if(c[i][j]>=0)
		{
			return c[i][j];
		}
		//求解
		if(X[i]==Y[j])
		{
			//如果相等，则原问题=1(当前匹配)+子问题的解
			c[i][j]=1+LcS_LENGTH(c,X,Y,i-1,j-1);
		}
		else{
			//如果不相等 当前的则取最大值
			c[i][j]=Math.max(LcS_LENGTH(c,X,Y,i-1,j),LcS_LENGTH(c, X, Y, i, j-1));
		}
		return c[i][j];	
	}
}
