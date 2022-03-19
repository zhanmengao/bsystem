package com.sina.service.imp;

import java.util.List;

import com.sina.basic.BasicService;
import com.sina.domain.Content;
import com.sina.service.inter.ContentServiceInter;

public class ContentService extends BasicService implements ContentServiceInter {

	public List<Content> showContents() {
		// TODO Auto-generated method stub
		return this.executeQuery("from Content", null);
	}
}
