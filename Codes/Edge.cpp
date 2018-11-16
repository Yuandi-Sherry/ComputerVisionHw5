#include "Edge.h"

Edge::Edge() {
	
}

Edge::Edge(const int & point1, const int & point2, const int & count) {
	point1ID = point1;
	point2ID = point2;
	this->count = count;
}

Edge::~Edge() {
}