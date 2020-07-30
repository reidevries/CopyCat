/*
 * SaveComponents.cpp
 *
 *  Created on: Jul 21, 2020
 *      Author: rei de vries
 */

#include "JsonComponents.h"

using namespace std;

const std::string JsonComponents::level_directory = "level/";

//-----------------methods to parse entities to json------------------------//

json JsonComponents::fromFloor(entt::registry& reg,
	const entt::entity entity)
{
	json j;

	ReiDV::getToJson<WorldPos>(reg, entity, j, "WorldPos");
	ReiDV::getToJson<SpriteAnim>(reg, entity, j, "SpriteAnim");

	return j;
}



//-----------------public functions to load and save levels/game state------//

json JsonComponents::loadLevel(entt::registry& reg, std::string file_name)
{
	json j;
	ifstream f(file_name);
	if (f.good()) {
		f >> j;
		for (auto& [level_id, components] : j.items()) {
			//floor
			if (auto str_size = level_id.size(); str_size > 5 
				&& level_id.substr(str_size-5,5) == string("floor")) {
				SpriteAnim s = components["SpriteAnim"].get<SpriteAnim>();
				Vector3 pos = components["WorldPos"].get<WorldPos>().pos;
				Factory::makeFloor(
					reg,
					s,
					pos
				);
			}
		}
	} else {
		DebugPrinter::printDebug(0, "JsonComponents::parseLevel",
			file_name + " is an invalid filename, returning empty json");
	}
	return j;
}



void JsonComponents::saveLevel(entt::registry& reg, std::string file_name)
{
	std::string full_name = level_directory + file_name;
	DebugPrinter::printDebug(3, "JsonComponents::saveLevel",
		"saving current level to " + full_name + "...");

	json j;
	auto view = reg.view<LevelID>();
	for (auto entity : view) {
		string level_id = view.get<LevelID>(entity).name;

		//check to see if level id corresponds to a known object, to reduce
		//the amount of necessary data to save as a lot of it can be inferred
		if (level_id.substr(level_id.size()-5, 5) == string("floor")) {
			j[level_id] = fromFloor(reg, entity);
		} else {
			j[level_id] = ReiDV::entityToJson(reg, entity);
		}
	}

	ofstream f(full_name);
	if (f.good()) {
		f << setw(4) << j;
	} else {
		DebugPrinter::printDebug(0, "JsonComponents::saveLevel",
			full_name + " is an invalid filename, printing here instead:\n"
			+ j.dump());
	}
}


void JsonComponents::test()
{
	cout << endl;
	cout << "------JsonComponents::test()------RUNNING--------------" << endl;
	Vector2 v2 = {12.345, 6.789};
	Vector3 v3 = {1.98, 2.76, 3.54};
	Vector4 v4 = {1,2,3,4};
	Matrix m = VectorMath::identity;
	Color c = BEIGE;
	Rectangle r = {11.11, 22.22, 32.0, 32.0};
	NPatchInfo n = {{9,10,11,11}, 4, 3, 2, 1, NPT_3PATCH_HORIZONTAL};
	Quad q(Rectangle{0,0,3,7});
	SpriteAnim sa;
	sa.atlas_name = "test atlas name";
	sa.region_name = "test region name";
	Camera2D cam_2d;
	cam_2d.offset = {1,2};
	cam_2d.target = {3,4};
	cam_2d.rotation = 5;
	cam_2d.zoom = 6;
	Camera3D cam_3d;
	cam_3d.fovy = 123;
	cam_3d.position = {4,5,6};
	cam_3d.target = {7,8,9};
	cam_3d.type = CAMERA_FIRST_PERSON;
	cam_3d.up = {1.9, 2.8, 3.7};
	Transform t;
	t.rotation = v4;
	t.scale = v3;
	t.translation = v3;
	Ray ray;
	ray.direction = {1,2,3};
	ray.position = {4,5,6};
	RayHitInfo ray_hit;
	ray_hit.position = {1,2,3};
	ray_hit.normal = {4,5,6};
	ray_hit.distance = 7.8;
	ray_hit.hit = true;
	BoundingBox bb;
	bb.max = {9,8,7};
	bb.min = {1,2,3};
	VrDeviceInfo vr_dev;
	vr_dev.hResolution = 123;
	vr_dev.chromaAbCorrection[0] = 4;
	vr_dev.chromaAbCorrection[1] = 5;
	vr_dev.chromaAbCorrection[2] = 6;
	vr_dev.chromaAbCorrection[3] = 7;

	json j;
	j["test01"] = v2;
	j["test02"] = v3;
	j["test03"] = v4;
	j["test04"] = m;
	j["test05"] = c;
	j["test06"] = r;
	j["test07"] = n;
	j["test08"] = q;
	j["test09"] = sa;
	j["test10"] = cam_2d;
	j["test11"] = cam_3d;
	j["test12"] = t;
	j["test13"] = ray;
	j["test14"] = ray_hit;
	j["test15"] = bb;
	j["test16"] = vr_dev;
	cout << j.dump(4) << endl;

	Vector2 v2_test = j["test01"].get<Vector2>();
	cout << "test01: " << VectorMath::printVector(v2_test) << endl;
	Vector3 v3_test = j["test02"].get<Vector3>();
	cout << "test02: " << VectorMath::printVector(v3_test) << endl;
	Vector4 v4_test = j["test03"].get<Vector4>();
	cout << "test03: " << VectorMath::printVector(v4_test) << endl;
	Matrix m_test = j["test04"].get<Matrix>();
	cout << "test04: " << VectorMath::printMat(m_test) << endl;
	Color c_test = j["test05"].get<Color>();
	cout << "test05: " << VectorMath::printColor(c_test) << endl;
	Rectangle r_test = j["test06"].get<Rectangle>();
	cout << "test06: " << VectorMath::printRect(r_test) << endl;
	NPatchInfo n_test = j["test07"].get<NPatchInfo>();
	cout << "test07: "
		<< "sourceRec is " << VectorMath::printRect(n_test.sourceRec)
		<< endl
		<< "\tleft,top,right,bottom offsets are "
		<< n.left << "," << n.top << "," << n.right << "," << n.bottom
		<< endl
		<< "\tNPatch type is " << n.type << endl;
	Quad q_test = j["test08"].get<Quad>();
	cout << "test08: " << q_test.print();
	SpriteAnim sa_test = j["test09"].get<SpriteAnim>();
	cout << "test09: '" << sa_test.atlas_name 
		<< "' : '" << sa_test.region_name << "'" << endl;
	Camera2D cam_2d_test = j["test10"].get<Camera2D>();
	cout << "test10:" << endl
		<< " target: " << VectorMath::printVector(cam_2d_test.target)
		<< endl
		<< " offset: " << VectorMath::printVector(cam_2d_test.offset)
		<< endl
		<< " rotation: " << cam_2d_test.rotation 
		<< " zoom: " << cam_2d_test.zoom << endl;
	Camera3D cam_3d_test = j["test11"].get<Camera3D>();
	cout << "test11: " << endl
		<< " position: " << VectorMath::printVector(cam_3d_test.position)
		<< endl
		<< " target: " << VectorMath::printVector(cam_3d_test.target)
		<< endl
		<< " fovy: " << cam_3d_test.fovy 
		<< " type: " << cam_3d_test.type
		<< endl
		<< " up: " << VectorMath::printVector(cam_3d_test.up) << endl;
	Transform t_test = j["test12"].get<Transform>();
	cout << "test12: " << endl
		<< " translation: " << VectorMath::printVector(t_test.translation)
		<< endl
		<< " scale: " << VectorMath::printVector(t_test.scale)
		<< endl
		<< " rotation: " << VectorMath::printVector(t_test.rotation) << endl;
	Ray ray_test = j["test13"].get<Ray>();
	cout << "test13: " << endl
		<< " direction: " << VectorMath::printVector(ray_test.direction) 
		<< endl
		<< " position: " << VectorMath::printVector(ray_test.position) 
		<< endl;
	RayHitInfo ray_hit_test = j["test14"].get<RayHitInfo>();
	cout << "test14: " << endl
		<< " position: " << VectorMath::printVector(ray_hit_test.position)
		<< endl
		<< " normal: " << VectorMath::printVector(ray_hit_test.normal)
		<< endl
		<< " distance: " << ray_hit_test.distance 
		<< " hit? " << ray_hit_test.hit << endl;
	BoundingBox bb_test = j["test15"].get<BoundingBox>();
	cout << "test15: " << endl
		<< " max: " << VectorMath::printVector(bb_test.max)
		<< " min: " << VectorMath::printVector(bb_test.min) << endl;
	VrDeviceInfo vr_dev_test = j["test16"].get<VrDeviceInfo>();
	cout << "test16: " << endl
		<< " hResolution: " << vr_dev_test.hResolution
		<< " chromaAbCorrection: {" << vr_dev_test.chromaAbCorrection[0]
		<< "," << vr_dev_test.chromaAbCorrection[1]
		<< "," << vr_dev_test.chromaAbCorrection[2]
		<< "," << vr_dev_test.chromaAbCorrection[3] << "}" << endl;
	
	cout << "------JsonComponents::test()------FINISHED-------------" << endl;
	cout << endl;

}
