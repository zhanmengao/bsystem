package com.knowledge;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Main {
    //������ո�ָ���ַ���
    public static void main(String args[])
    {
        BufferedReader br=new BufferedReader(new InputStreamReader(System.in));  
        String str = null;
		try {
			str = br.readLine();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}finally{
	        try {
				br.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
        //����
        char []arr=str.toCharArray();
        int j=arr.length-1;
        for(int i=arr.length-1;i>=0;i--)
        {
        	//���������һ���ո�
        	if(arr[i]==' '||i==0)
        	{
        		if(i>=arr.length-1)
        		{
        			continue;
        		}
        		//��i��ʼ������ַ�����i+1��j��������
        		for(int k=i+1;k<=j;k++)
        		{
        			if(k-1==0)
        			{
        				System.out.print(arr[i]);
        			}
        			if(arr[k]!=' ')
        			{
        				System.out.print(arr[k]);
        				if(k==j && k!=0)
        				{
        					System.out.print(" ");
        				}
        			}
        		}
        		if(i==0)
        		{
        			break;
        		}
        		//�ҵ���һ���ǿո�
        		while(arr[i]==' ')
        		{
        			i--;
        			if(i<=0)
        			{
        				break;
        			}
        		}
        		j=i;
        		
        	}
        }
        

    }

}
