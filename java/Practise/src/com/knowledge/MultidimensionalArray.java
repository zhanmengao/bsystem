
/*��ʮ�Ž� ��ά����
 1.��ά���飬���壺
   �﷨������ ������[][]=new ����[��С][��С] int a[][]=new int [2][3]
*/
package com.knowledge;
public class MultidimensionalArray
{
 
    public static void main(String[] args)
    {
       int a[][]=new int[4][6];
       a[1][2]=1;
       a[2][1]=2;
       a[2][3]=3;
      
       for (int i=0;i<4;i++)
       {
           //��
           for (int j=0;j<3;j++)
              System.out.print(a[i][j]+" ");
              System.out.println();          
       }
      
    }
}