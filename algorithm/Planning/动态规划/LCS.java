package ��̬�滮;
//LCS�������������
public class LCS {
	public static void main(String args[])
	{
		String X="ABCBDAB";
		String Y="BDCABA";
		System.out.println(new LCS().LcS_LENGTH(X, Y));
	}
	//�Զ����¹���LCS
	public int LcS_LENGTH(String X,String Y)
	{
		//��¼X[0...i]��Y[0...j]�����Ž�
		int c[][]=new int[X.length()][Y.length()];
		//��ʼ��
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
		//�Զ����¹���
		return LcS_LENGTH(c, X.toCharArray(), Y.toCharArray(), X.length()-1, Y.length()-1);
	}
	private int LcS_LENGTH(int [][]c,char []X,char []Y,int i,int j)
	{
		//����Ѿ�������
		if(c[i][j]>=0)
		{
			return c[i][j];
		}
		//���
		if(X[i]==Y[j])
		{
			//�����ȣ���ԭ����=1(��ǰƥ��)+������Ľ�
			c[i][j]=1+LcS_LENGTH(c,X,Y,i-1,j-1);
		}
		else{
			//�������� ��ǰ����ȡ���ֵ
			c[i][j]=Math.max(LcS_LENGTH(c,X,Y,i-1,j),LcS_LENGTH(c, X, Y, i, j-1));
		}
		return c[i][j];	
	}
}
