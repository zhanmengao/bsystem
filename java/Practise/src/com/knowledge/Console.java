package com.knowledge;
import java.io.*;
import java.util.Scanner; 
public class Console {
	    public static void main(String[] args) {  
	        
	          
	    }
	    //����1��BufferedReader��
	    public void Io()
	    {
	    	try {  
	            BufferedReader strin=new BufferedReader(new InputStreamReader(System.in));  
	            System.out.print("������һ���ַ�����");  
	            String str = strin.readLine();         
	            System.out.println("��һ����"+str);  
	              
	            System.out.println("������ڶ����ַ�����");  
	            String str2 = strin.readLine();  
	            System.out.println("��2����"+str2);  
	        } catch (IOException e) {  
	            e.printStackTrace();  
	        }  
	    }
	    //����2��util�����Scanner��
	    public void sr()
	    {
	    	Scanner sc = new Scanner(System.in);   
	        System.out.println("�����һ��booleanֵ(true/false):");  
	        if(sc.nextBoolean()){  
	            System.out.println("���벼�������");  
	        }else{  
	            System.out.println("���벼�����ٵ�");  
	        }  
	          
	          
	        System.out.println("�����һ������:");  
	        System.out.println("�������֣�"+sc.nextInt());  
	  
	        System.out.println("����һ���ַ���:");  
	        System.out.println("�����ַ�����"+sc.next());  
	  
	        System.out.println("����һ��������:");  
	        System.out.println("���볤���ͣ�"+sc.nextLong());  
	          
	    }

	}  








