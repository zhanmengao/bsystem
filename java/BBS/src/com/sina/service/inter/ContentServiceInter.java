package com.sina.service.inter;

import java.util.List;

import com.sina.basic.BasicServiceInter;
import com.sina.domain.Content;

public interface ContentServiceInter extends BasicServiceInter{

	public List<Content> showContents();
}
