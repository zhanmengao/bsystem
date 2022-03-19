package ��̬�滮;

public class CutRod {
	//���룺�۸����������
	public double cutRod(double[] p,double n)
	{
		if(n==0)
		{
			return 0;
		}
		//q���浱ǰ���ֵ
		double q=Double.MIN_NORMAL;
		//ÿ��ѭ���õ��и�iʱ�õ������ֵ
		for(int i=0;i<=n;i++)
		{
			//qȡ��ǰ����q���и�i+�и�ʣ�¶κ�����ֵ
			q=Math.max(q, p[i]+cutRod(p,n-i));
		}
		return q;
	}
	//���룺�۸���ܳ���
	//���㷨����ӿڣ������ʼ��
	public double TopToBottom(double []price,int length)
	{
		//������Ľ⣺r[i]����TopToBottom(price,i)�����Ž�
		double []r=new double[length];
		for(int i=0;i<r.length;i++)
		{
			r[i]=Double.MIN_NORMAL;
		}
		return TopToBottom(price,length,r);
	}
	private double TopToBottom(double []price,int n,double []r)
	{
		//�������¼����
		if(r[n]>0)
		{
			return r[n];
		}
		//�������Ϊ0 ��ֱ�ӷ���
		double q;
		if(n==0)
		{
			q=0;
		}
		//���� ȥ����
		else {
			q=Double.MIN_NORMAL;
			for(int j=0;j<price.length;j++)
			{
				q=Math.max(q,price[j]+TopToBottom(price,n-j,r));
			}
		}
		//��¼
		r[n]=q;
		return q;
		
	}
	//�Ե����������Ž�
	public double BottomToTop(double []price,int length)
	{
		//���ʣ��iʱ�����Ž�
		double r[]=new double[length];
		r[0]=0;
		r[1]=price[1];
		//���ѭ�������ÿ��i
		for(int i=2;i<length;i++)
		{
			double q=Double.MIN_NORMAL;
			//�ڲ�ѭ�������ÿ���и�
			for(int j=0;j<=i;i++)
			{
				q=Math.max(q,price[i]+r[i-j]);
			}
			r[i]=q;
		}
		return r[length];
	}
}





