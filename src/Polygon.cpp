#include "Polygon.hpp"

# define M_PI           3.14159265358979323846

using namespace std;

void Polygon::updateBound(const Vector2 new_pos)
{
	if (new_pos.x < bounds_min.x) {
		bounds_min.x = new_pos.x;
	} else if (new_pos.x > bounds_max.x) {
		bounds_max.x = new_pos.x;
	}
	
	if (new_pos.y < bounds_min.y) {
		bounds_min.y = new_pos.y;
	} else if (new_pos.y > bounds_max.y) {
		bounds_max.y = new_pos.y;
	}
}

void Polygon::recalculateBound()
{
	bounds_max = (Vector2){0,0};
	bounds_min = (Vector2){999999999.0,999999999.0};
	for (auto vertex : vertices) {
		updateBound(vertex);
	}
}

void Polygon::recalculateBound(const Vector2 removed_point)
{
	//if we're removing one point,
	//only need to recalculate if the removed point is on a boundary
	if (   removed_point.x <= bounds_min.x || removed_point.y <= bounds_min.y
		|| removed_point.x >= bounds_max.x || removed_point.y >= bounds_max.y) {
		recalculateBound();
	} 
}

void Polygon::recalculateData()
{
	Vector2 centre_accumulator = (Vector2){0,0};
	double denominator;
	for (auto vertex : vertices) {
		centre_accumulator = VectorMath::add(centre_accumulator, vertex);
		denominator += 1.0;
	}
	
	centre_pos = VectorMath::scale(centre_accumulator, 1/denominator);
	
	recalculateBound();
}

Polygon::Polygon(vector<Vector2> vertices)
{
	this->vertices = vertices;
	recalculateData();
}

Polygon::Polygon(Vector2 centre, const float radius, const int num_vertices)
{
	centre_pos = centre;
	
	bounds_max = (Vector2){0,0};
	bounds_min = (Vector2){0,0};
	for (int i = 0; i < num_vertices; i++) {
		double radians = 2*M_PI*i/num_vertices;
		Vector2 edge = {
			static_cast<float>(centre.x + radius*cos(radians)),
			static_cast<float>(centre.y - radius*sin(radians))
		};
		vertices.push_back(edge);
		updateBound(edge);
	}
}

int Polygon::getNearestIndex(const Vector2 pos) const
{
	float mindist = -1;
	int savedmin = 0;
	for (auto it = vertices.begin(); it != vertices.end(); ++it) {
		float sqrdist = VectorMath::sqrdist(pos, *it);
		if (mindist == -1 || sqrdist < mindist) {
			mindist = sqrdist;
			savedmin = it-vertices.begin();
		}
	}
	return savedmin;
}

int Polygon::getNearestClockwiseIndex(const Vector2 pos) const
{
	int nearestindex = getNearestIndex(pos);
	if (nearestindex >= getNumVertices()) {
		return nearestindex;
	}
	Vector2 nearest = vertices[nearestindex];
	Vector2 nearestd = VectorMath::sub(nearest, centre_pos);
	Vector2 thisd = VectorMath::sub(pos, centre_pos);
	
	bool is_clockwise = false;
	if ((nearestd.x > 0  && thisd.x > 0) 
		|| (nearestd.y > 0 && thisd.y > 0) 
		|| (nearestd.y < 0 && thisd.y < 0)) {
		if (atan2(thisd.y, thisd.x) - atan2(nearestd.y, nearestd.x) > 0) {
			is_clockwise = true;
		}
	} else {
		if (atan2(nearestd.y, nearestd.x) - atan2(thisd.y, thisd.x) > 0) {
			is_clockwise = true;
		}
	}
	int nextindex = (nearestindex+1)%(getNumVertices()+1);
	
	if (is_clockwise) return nearestindex;
	else return nextindex;
}

array<Vector2, 2> Polygon::getNearestEdge(const Vector2 pos)
{
	vector<Vector2>::iterator next_it;
	float nearest_dist = 100000;
	array<Vector2, 2> nearest_edge = {
		(Vector2){-1,-1},
		(Vector2){-1,-1}
	};

	for (auto it = vertices.begin(); it != vertices.end(); it++) {
		next_it = next(it);
		if (next_it == vertices.end()) {
			next_it = vertices.begin();
		}

		float dist = VectorMath::distanceToSegment(*it, *next_it, pos);
		if (dist < nearest_dist) {
			nearest_dist = dist;
			nearest_edge = {*it, *next_it};
		}
	}
	return nearest_edge;
}

Vector2 Polygon::getVertexPos(const int index) const
{
	if (index < vertices.size() && index >= 0) {
		return vertices[index];
	} else {
		cout << "hey! ur trying to access vertex " << index
			<< ", which doesn't exist in Polygon " << this << "!" << endl;
		return vertices[0];
	}
}

void Polygon::moveVertex(const int index, const Vector2 new_pos)
{
	if (vertices.size() > 0) {
		float scalar = 1/vertices.size();
		centre_pos = VectorMath::sub(centre_pos, VectorMath::scale(vertices[index], scalar));
		vertices[index] = new_pos;
		centre_pos = VectorMath::add(centre_pos, VectorMath::scale(new_pos, scalar));
		updateBound(new_pos);
	}
}

void Polygon::addVertex(const int index, const Vector2 point)
{
	if (vertices.size() == 0) {
		centre_pos = point;
		bounds_max = point;
		bounds_min = point;
	} else {
		float newsize = vertices.size()+1;
		float scalar = vertices.size()/newsize;
		centre_pos = VectorMath::add(
			VectorMath::scale(centre_pos, scalar),
			VectorMath::scale(point, 1/newsize)
		);
		updateBound(point);
	}
	vertices.insert(vertices.begin()+index, point);
}

void Polygon::removeVertex(const int index)
{
	if (vertices.size() > 0) {
		if (vertices.size() == 1) {
			centre_pos = (Vector2){0,0};
			bounds_max = (Vector2){0,0};
			bounds_min = (Vector2){0,0};
		} else {
			float oldsize = vertices.size();
			centre_pos = VectorMath::scale(
				VectorMath::sub(
					centre_pos,
					VectorMath::scale(vertices[index], 1/oldsize)
				),
				(oldsize+1)/oldsize
			);
		}
		Vector2 removed_point = vertices[index];
		vertices.erase(vertices.begin()+index);
		recalculateBound(removed_point);
	}
}

void Polygon::reverse()
{
	std::reverse(vertices.begin(), vertices.end());
}

void Polygon::clear()
{
	vertices.clear();
}

bool Polygon::containsPoint(const Vector2 point) const
{
	if (point.x < bounds_min.x 		|| point.x > bounds_max.x
		|| point.y < bounds_min.y 	|| point.y > bounds_max.y) {
		return false;
	} else {
		return VectorMath::checkPointInPolygon(point, vertices);
	}
}

float Polygon::findVertexCoverage(const Polygon b) const
{
	vector<Vector2> bv = b.getVertices();
	int bn = b.getNumVertices()+getNumVertices();
	float accumulator = 0;
	for (auto b_vertex : bv) {
		if (containsPoint(b_vertex)) accumulator++;
	}
	for (auto this_vertex : vertices) {
		if (b.containsPoint(this_vertex)) accumulator++;
	}
	return accumulator/bn;
}

vector<Vector2> Polygon::findOverlapShape(const Polygon b) const
{
	vector<Vector2> b_vertices = b.getVertices();
	return b_vertices;
}

vector<Vector2> Polygon::getArrayScaled(float screen_scale) const
{
	vector<Vector2> return_vec;
	for (auto const& vertex : vertices) {
		return_vec.push_back(VectorMath::scale(vertex, screen_scale));
	}
	return return_vec;
}

string Polygon::printArray() const
{
	stringstream ss;
	for (auto const& vertex : vertices) {
		ss << vertex.x << "," << vertex.y << " ";
	}
	ss << endl;
	return ss.str();
}
