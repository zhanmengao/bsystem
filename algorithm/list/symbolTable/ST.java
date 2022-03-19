package com.symbolTable;

public interface ST<K extends Comparable<K>,V> {
	public void put(K key,V value);
	public V get(K key);
	public int size();
	public V remoove(K key);

}
