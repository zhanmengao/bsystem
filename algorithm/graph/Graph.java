package com.Graph;

import java.util.ArrayList;

import com.Graph.AdjacencyList.Edge;

public interface Graph {
	public void DFS();
	public void GFS();
	public int V();
	public int E();
	public void addEdge(int v,int w);
	public ArrayList<Edge> adj(int v);
	public ArrayList<Edge> edges();
	
}
