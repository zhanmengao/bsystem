package com.hsp.inter;

public class UpperLetter implements ChangeLetter {

	private String str;
	
	
	public String change() {
		//��Сд��ĸ->��д
		return str.toUpperCase();
	}


	public String getStr() {
		return str;
	}


	public void setStr(String str) {
		this.str = str;
	}

}
