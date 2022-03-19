package com.Graph;

import java.util.ArrayList;

import com.Graph.ListDigraph.Edge;

public interface Digraph {
	public int V();
	public int E();
	public void addEdge(int v,int w);
	public ArrayList<Edge> adj(int v);
	public Digraph reverse();
	

}
