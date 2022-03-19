package com.pra;

public class Throws{
	public static void main(String[] args)throws Exception{
		try{
			throw new Exception();
		}catch(Exception e){
			System.out.println("Caught in main()");
			e.printStackTrace();
		}
		System.out.println("nothing");
	}
}
