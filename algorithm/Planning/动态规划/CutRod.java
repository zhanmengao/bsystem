package 动态规划;

public class CutRod {
	//输入：价格表，钢条长度
	public double cutRod(double[] p,double n)
	{
		if(n==0)
		{
			return 0;
		}
		//q保存当前最大值
		double q=Double.MIN_NORMAL;
		//每次循环得到切割i时得到的最大值
		for(int i=0;i<=n;i++)
		{
			//q取当前最大的q和切割i+切割剩下段后的最大值
			q=Math.max(q, p[i]+cutRod(p,n-i));
		}
		return q;
	}
	//输入：价格表，总长度
	//是算法对外接口，负责初始化
	public double TopToBottom(double []price,int length)
	{
		//子问题的解：r[i]储存TopToBottom(price,i)的最优解
		double []r=new double[length];
		for(int i=0;i<r.length;i++)
		{
			r[i]=Double.MIN_NORMAL;
		}
		return TopToBottom(price,length,r);
	}
	private double TopToBottom(double []price,int n,double []r)
	{
		//如果备忘录中有
		if(r[n]>0)
		{
			return r[n];
		}
		//如果长度为0 则直接返回
		double q;
		if(n==0)
		{
			q=0;
		}
		//否则 去计算
		else {
			q=Double.MIN_NORMAL;
			for(int j=0;j<price.length;j++)
			{
				q=Math.max(q,price[j]+TopToBottom(price,n-j,r));
			}
		}
		//记录
		r[n]=q;
		return q;
		
	}
	//自底向上求最优解
	public double BottomToTop(double []price,int length)
	{
		//存放剩余i时的最优解
		double r[]=new double[length];
		r[0]=0;
		r[1]=price[1];
		//外层循环：解决每个i
		for(int i=2;i<length;i++)
		{
			double q=Double.MIN_NORMAL;
			//内层循环：解决每次切割
			for(int j=0;j<=i;i++)
			{
				q=Math.max(q,price[i]+r[i-j]);
			}
			r[i]=q;
		}
		return r[length];
	}
}





