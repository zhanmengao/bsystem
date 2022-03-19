package com.PQ;

public interface AbPQ<Key> {

	public void insert(Key k);
	public Key M();
	public Key del();
	boolean isEmpty();
	public int size();
	
}
