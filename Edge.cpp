#ifndef _EDGE_H_
#define _EDGE_H_
class Edge
{
public:
	Edge(const int& point1ID, const int& point2ID, const std::vector<Point> & pointVector = null, const int & count = 1) {
		this->point1ID = point1ID;
		this->point2ID = point2ID;
		this->count = count;
	}
	~Edge(){
		
	}
	int point1ID, point2ID;
	int count;
};
#endif