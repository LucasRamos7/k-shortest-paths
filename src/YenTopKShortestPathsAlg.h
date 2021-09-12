///////////////////////////////////////////////////////////////////////////////
///  YenTopKShortestPathsAlg.h
///  The implementation of Yen's algorithm to get the top k shortest paths 
///  connecting a pair of vertices in a graph. 
///
///  @remarks <TODO: insert remarks here>
///
///  @author Yan Qi @date 7/10/2010
/// 
///  $Id: YenTopKShortestPathsAlg.h 65 2010-09-08 06:48:36Z yan.qi.asu $
///
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <set>
#include <map>
#include <queue>
#include <vector>
#include <memory>

#include "GraphElements.h"
#include "Graph.h"
#include "DijkstraShortestPathAlg.h"
#include "YenTopKShortestPathsAlg.h"

using namespace std;


class YenTopKShortestPathsAlg
{
	Graph* m_pGraph;
	Graph& cGraph;
	vector<BasePath*> m_vResultList;
	map<BasePath*, BaseVertex*> m_mpDerivationVertexIndex;
	multiset<BasePath*, WeightLess<BasePath> > m_quPathCandidates;

	BaseVertex* m_pSourceVertex;
	BaseVertex* m_pTargetVertex;

	int m_nGeneratedPathNum;

private:

	void _init();

public:
	YenTopKShortestPathsAlg(Graph &graph) : m_pSourceVertex(NULL), m_pTargetVertex(NULL), cGraph(graph)
	{
	}

	YenTopKShortestPathsAlg(Graph &graph, BaseVertex *pSource, BaseVertex *pTarget)
		: m_pSourceVertex(pSource), m_pTargetVertex(pTarget), cGraph(graph)
	{
		m_pGraph = new Graph(graph);
		_init();
	}

	~YenTopKShortestPathsAlg(void){clear();}

	//void run(BaseVertex *pSource, BaseVertex *pTarget, double Kf, int* nPaths);
	void run(BaseVertex* pSource, BaseVertex* pTarget, int Kf, int* nPaths);

	void clear();
	bool has_next();	
	BasePath* next();

	BasePath* get_shortest_path(BaseVertex* pSource, BaseVertex* pTarget);
	void get_shortest_paths(BaseVertex* pSource, BaseVertex* pTarget, int top_k, 
		vector<BasePath*>&);
};
