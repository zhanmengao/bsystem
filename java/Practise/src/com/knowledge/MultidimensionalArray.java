
/*第十九讲 多维数组
 1.多维数组，定义：
   语法：类型 数组名[][]=new 类型[大小][大小] int a[][]=new int [2][3]
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
           //列
           for (int j=0;j<3;j++)
              System.out.print(a[i][j]+" ");
              System.out.println();          
       }
      
    }
}