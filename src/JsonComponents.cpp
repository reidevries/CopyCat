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

	return j;
}



//-----------------public functions to load and save levels/game state------//

json JsonComponents::loadLevel(entt::registry& reg, std::string file_name)
{
	json j;
	ifstream f(file_name);
	if (f.good()) {
		f >> j;
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

	json j;
	j["test1"] = v2;
	j["test2"] = v3;
	j["test3"] = v4;
	j["test4"] = m;
	j["test5"] = c;
	j["test6"] = r;
	j["test7"] = n;
	j["test8"] = q;
	j["test9"] = sa;
	cout << j.dump(4) << endl;

	Vector2 v2_test = j["test1"].get<Vector2>();
	Vector3 v3_test = j["test2"].get<Vector3>();
	Vector4 v4_test = j["test3"].get<Vector4>();
	Matrix m_test = j["test4"].get<Matrix>();
	Color c_test = j["test5"].get<Color>();
	Rectangle r_test = j["test6"].get<Rectangle>();
	NPatchInfo n_test = j["test7"].get<NPatchInfo>();
	Quad q_test = j["test8"].get<Quad>();
	SpriteAnim sa_test = j["test9"].get<SpriteAnim>();
	cout << "test1: " << VectorMath::printVector(v2_test) << endl;
	cout << "test2: " << VectorMath::printVector(v3_test) << endl;
	cout << "test3: " << VectorMath::printVector(v4_test) << endl;
	cout << "test4: " << VectorMath::printMat(m_test) << endl;
	cout << "test5: " << VectorMath::printColor(c_test) << endl;
	cout << "test6: " << VectorMath::printRect(r_test) << endl;
	cout << "test7: "
		<< "sourceRec is " << VectorMath::printRect(n_test.sourceRec)
		<< endl
		<< "\tleft,top,right,bottom offsets are "
		<< n.left << "," << n.top << "," << n.right << "," << n.bottom
		<< endl
		<< "\tNPatch type is " << n.type << endl;
	cout << "test8: " << q_test.print();
	cout << "test9: " << sa_test.region_name << endl;

	cout << "------JsonComponents::test()------FINISHED-------------" << endl;
	cout << endl;

}
