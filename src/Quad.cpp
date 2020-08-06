/*
 * Quad.cpp
 *
 *  Created on: Jul 23, 2020
 *      Author: rei de vries
 */

#include "Quad.hpp"

using namespace std;

Quad::Quad(Rectangle rect, VectorMath::Orthog dir)
{
	float h_half = rect.height/2;
	float w_half = rect.width/2;

	switch (dir) {
	case VectorMath::Orthog::up:
	case VectorMath::Orthog::down:
		p[0] = {rect.x-w_half, 0.0, rect.y-h_half};
		p[1] = {rect.x+w_half, 0.0, rect.y-h_half};
		p[2] = {rect.x+w_half, 0.0, rect.y+h_half};
		p[3] = {rect.x-w_half, 0.0, rect.y+h_half};
		break;
	case VectorMath::Orthog::north:
	case VectorMath::Orthog::south:
		p[0] = {0.0, rect.y+h_half, rect.x+w_half};
		p[1] = {0.0, rect.y+h_half, rect.x-w_half};
		p[2] = {0.0, rect.y-h_half, rect.x-w_half};
		p[3] = {0.0, rect.y-h_half, rect.x+w_half};
		break;
	case VectorMath::Orthog::east:
	case VectorMath::Orthog::west:
		p[0] = {rect.x-w_half, rect.y+h_half, 0.0};
		p[1] = {rect.x+w_half, rect.y+h_half, 0.0};
		p[2] = {rect.x+w_half, rect.y-h_half, 0.0};
		p[3] = {rect.x-w_half, rect.y-h_half, 0.0};
		break;
	case VectorMath::Orthog::facing_cam:
		p[0]= {rect.x-w_half, rect.y+h_half, rect.x-w_half};
		p[1] = {rect.x+w_half, rect.y+h_half, rect.x+w_half};
		p[2] = {rect.x+w_half, rect.y-h_half, rect.x+w_half};
		p[3] = {rect.x-w_half, rect.y-h_half, rect.x-w_half};
		break;
	}
}

Quad::Quad(Rectangle rect, Vector3 dir)
{
	Quad(rect, VectorMath::Orthog::up);
	rotate(orthogToVector3(VectorMath::Orthog::up), dir);
}

Quad::Quad(const Vector3 set_p[4])
{
	for (int i = 0; i < 4; ++i ) p[i] = set_p[i];
}


void Quad::scale(float scale)
{
	p[0] = VectorMath::scale(p[0], scale);
	p[1] = VectorMath::scale(p[1], scale);
	p[2] = VectorMath::scale(p[2], scale);
	p[3] = VectorMath::scale(p[3], scale);
}
void Quad::translate(Vector3 offset)
{
	p[0]= VectorMath::add(p[0], offset);
	p[1] = VectorMath::add(p[1], offset);
	p[2] = VectorMath::add(p[2], offset);
	p[3] = VectorMath::add(p[3], offset);
}

void Quad::rotate(Quaternion q)
{
	p[0]= Vector3RotateByQuaternion(p[0],q);
	p[1] = Vector3RotateByQuaternion(p[1],q);
	p[2] = Vector3RotateByQuaternion(p[2],q);
	p[3] = Vector3RotateByQuaternion(p[3],q);
}

void Quad::rotate(float roll,float pitch,float yaw)
{
	rotate(QuaternionFromEuler(roll,pitch,yaw));
}

void Quad::rotate(Vector3 from, Vector3 to)
{
	rotate(QuaternionFromVector3ToVector3(from,to));
}

Vector3 Quad::getPos3D(Vector2 pos) const
{
	return VectorMath::fade(
		VectorMath::fade(p[3],p[2],pos.x),
		VectorMath::fade(p[0],p[1],pos.x),
		pos.y
	);
}

Vector3 Quad::getCenter() const
{
	return {
		(p[0].x+p[1].x+p[2].x+p[3].x)/4.0f, 
		(p[0].y+p[1].y+p[2].y+p[3].y)/4.0f, 
		(p[0].z+p[1].z+p[2].z+p[3].z)/4.0f
	};
}

// TODO: make sure this works, dont think it does rn
RayHitInfo Quad::getRayHit(const Ray& r) const
{
	// find if the ray hit the plane that the quad's points lie on
	RayHitInfo plane_hit = VectorMath::intersectRayPlane(r, p[0], getNormal());
	if (!plane_hit.hit) return plane_hit;
	else {
		// if the ray at least hits the plane, check if the hit position is
		// inside the triangle formed by either p[0], p[1], p[2] 
		// or p[0], p[2], p[3]
		// determing which triangle to check based on which side of the line
		// p[0]-p[2] that the hit position lies on
		Vector3 i = plane_hit.position;
		
		// these vars are shared in both cases
		Vector3 p0_2 = p[2]-p[0];
		Vector3 p0_i = i-p[0];
		float p22 = p0_2*p0_2;
		float pi2 = p0_i*p0_2;
		
		Vector3 p0_1 = p[1]-p[0];
		Vector3 m = p[0]+VectorMath::project(p0_1, p0_2);
		// tests if vector m->i is within 180 degrees of the perpendicular
		// vector facing p[1], in other words, is i on the same side of p0->p2
		// as p1. If so, look at triangle p0,p1,p2, if not, look at p0,p2,p3
		if ((i-m) * (p[1]-m) > 0.0f) {
			float p11 = p0_1*p0_1;
			float p12 = p0_1*p0_2;
			float pi1 = p0_i*p0_1;
			
			float denom = p11*p22-p12*p12;
			
			// compute barycentric coords to check if point is in the triangle
			// denominator not used because we don't need the coords
			// themselves, only need to check if they are negative (outside)
			// or positive (inside)
			float v = p22*pi1-p12*pi2;
			if ( v>0 == denom>0 ) {
				float w = p11*pi2-p12*pi1;
				if ( w>0 == denom>0 ) {
					float u = 1.0f-v/denom-w/denom;
					if ( u>0 ) {
						return plane_hit;
					}
				}
			}
		//same again, for other triangle
		} else {
			Vector3 p0_3 = p[3]-p[0];
			
			float p23 = p0_2*p0_3;
			float p33 = p0_3*p0_3;
			float pi3 = p0_i*p0_3;
			
			float denom = p22*p33-p23*p23;
			
			float v = p33*pi2-p23*pi3;
			if ( v>0 == denom>0 ) {
				float w = p22*pi3-p23*pi2;
				if ( w>0 == denom>0 ) {
					float u = 1.0f-v/denom-w/denom;
					if ( u>0 ) {
						return plane_hit;
					}
				}
			}
		}
		
		// if neither of the triangle checks ever returned true, return false
		plane_hit.hit = false;
		return plane_hit;
	}
}

RayHitInfo Quad::getRayHit(const Ray& r, const Vector3& q_pos) const
{
	Quad q(p);
	q.translate(q_pos);
	return q.getRayHit(r);
}

BoundingBox Quad::getBoundingBox() const
{
	BoundingBox bounding_box;
	bool fits = false;
	// iterate through points to use as max/min of boundingbox
	for (int u = 0; u < 4; ++u) {
		bounding_box.max = p[u];
		for (int v = 0; v < 4; ++v) {
			bounding_box.min = p[(u+v) % 4];
			
			fits = true;
			// now iterate through points to check quad is
			// fully contained in bounding_box
			for (int i = 0; i < 4; ++i) {
				if (!VectorMath::boxContains(bounding_box, p[i])) {
					fits = false;
					break;
				}
			}
			if (fits) break;
		}
		if (fits) break;
	}
	
	return bounding_box;
}

BoundingBox Quad::getBoundingBox(Vector3 origin) const
{
	BoundingBox bb = getBoundingBox();
	bb.max = bb.max+origin;
	bb.min = bb.min+origin;
	return bb;
}

bool Quad::checkCollisionRay(const Ray& ray) const
{
	return getRayHit(ray).hit;
}

string Quad::print() const
{
	return  "p0=" + VectorMath::printVector(p[0]) + "\n"
		+ "\tp1=" + VectorMath::printVector(p[1]) + "\n"
		+ "\tp2=" + VectorMath::printVector(p[2]) + "\n"
		+ "\tp3=" + VectorMath::printVector(p[3]) + "\n";
}

void to_json(json& j, const Quad& q)
{
	j["Quad"] = {
		{"0", q.p[0]},
		{"1", q.p[1]},
		{"2", q.p[2]},
		{"3", q.p[3]}
	};
}

void from_json(const json& j, Quad& q)
{
	const json& quad = j.at("Quad");
	quad.at("0").get_to(q.p[0]);
	quad.at("1").get_to(q.p[1]);
	quad.at("2").get_to(q.p[2]);
	quad.at("3").get_to(q.p[3]);
}
