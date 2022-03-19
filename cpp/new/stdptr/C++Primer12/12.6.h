#ifndef __126__
#define __126__
#pragma once
#include<vector>
#include<stdio.h>
#include<memory>
#include<iostream>
#include<string>
#include<stdlib.h>
#ifdef __linux
#define scanf_s scanf
#endif
using namespace std;

namespace new126
{
	vector<int>* VecintFactory()
	{
		return new vector<int>;
	}
	void Read(vector<int> *vec)
	{
		int val;
        scanf_s("%d",&val);
		vec->push_back(val);
	}
	void Write(vector<int> *vec)
	{
		for (auto it = vec->begin(); it != vec->end(); it++)
		{
			printf("%d", *it);
		}
	}
	template<class T>
	shared_ptr<vector<T>> SharedVecFactory()
	{
		return make_shared<vector<T>>();
	}
	template<class T>
	void Write(shared_ptr<vector<T>> vec)
	{
		int val;
		scanf_s("%d", &val);
		vec->push_back(val);
	}
	template<class T>
	void Read(shared_ptr<vector<T>> vec)
	{
		for (auto it = vec->begin(); it != vec->end(); it++)
		{
			printf("%d", *it);
		}
	}

	void main126()
	{
		auto pVec = VecintFactory();
		Read(pVec);
		Write(pVec);
		delete pVec;

		auto sVec = SharedVecFactory<int>();
		Write<int>(sVec);
		Read<int>(sVec);

		int *q = new int(42), *r = new int(100);
		r = q;				//r指向q的内存，原本指向r的指针丢失，内存泄露
		auto q2 = make_shared<int>(42), r2 = make_shared<int>(100);
		r2 = q2;			//没有智能指针指向r2原本内存，原r2被释放。r2 q2共同指向一块内存
	}
}
#endif
