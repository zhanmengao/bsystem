package com.Graph;

import com.Graph.ListDigraph.Edge;

public interface SP {
	double distTo(int v);
	boolean hasPathTo(int v);
	Iterable<Edge> pathTo(int b);
	
}
