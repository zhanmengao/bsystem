package com.sina.service.imp;

import java.util.List;

import com.sina.basic.BasicService;
import com.sina.domain.Type;
import com.sina.service.inter.TypeServiceInter;

public class TypeService extends BasicService implements TypeServiceInter {

	public List<Type> showType() {
		// TODO Auto-generated method stub
		return this.executeQuery("from Type", null);
	}
}
