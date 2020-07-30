/*
 * RaylibSerialize.h
 *
 *  Created on: Jul 23, 2020
 *      Author: rei de vries
 *      nlohmann::json serializers for Raylib structs
 *		Doesn't include these:
 *			Image
 *			Texture2D
 *			RenderTexture2D
 *			CharInfo (contains Image)
 *			Font (contains Texture2D)
 *			Mesh
 *			Shader
 *			MaterialMap (contains Texture2D)
 *			Material (contains MaterialMap)
 *			BoneInfo (used inside ModelAnimation)
 *			Model (contains Materials and Meshes)
 *			ModelAnimation
 *			Wave
 *			AudioStream
 *			Sound
 *			Music
 *		Because all these types don't make sense to serialise into Json,
 *		they should be loaded from their own filetype.
 *		VrDeviceInfo, Ray, and RayHitInfo also don't seem to make much sense
 *		for serialization, but I included them anyway since it might be useful
 *		for someone.
 */

#ifndef SRC_RAYLIBSERIALIZE_H_
#define SRC_RAYLIBSERIALIZE_H_

#include "CatConf.h"
#include <nlohmann/json.hpp>
#include <iostream>

namespace nlohmann
{
//Vector2
template <>
struct adl_serializer<Vector2>
{
	static void to_json(json& j, const Vector2& opt)
	{
		
		j["Vector2"] = {
			{"x", opt.x},
			{"y", opt.y}
		};
	}

	static void from_json(const json& j, Vector2& opt)
	{
		const json& vector_2 = j.at("Vector2");
		vector_2.at("x").get_to(opt.x);
		vector_2.at("y").get_to(opt.y);
	}
};

//Vector3
template <>
struct adl_serializer<Vector3>
{
	static void to_json(json& j, const Vector3& opt)
	{
		
		j["Vector3"] = {
			{"x", opt.x},
			{"y", opt.y},
			{"z", opt.z}
		};
	}

	static void from_json(const json& j, Vector3& opt)
	{
		const json& vector_3 = j.at("Vector3");
		vector_3.at("x").get_to(opt.x);
		vector_3.at("y").get_to(opt.y);
		vector_3.at("z").get_to(opt.z);
	}
};

//Vector4
template <>
struct adl_serializer<Vector4>
{
	static void to_json(json& j, const Vector4& opt)
	{
		
		j["Vector4"] = {
			{"x", opt.x},
			{"y", opt.y},
			{"z", opt.z},
			{"w", opt.w}
		};
	}

	static void from_json(const json& j, Vector4& opt)
	{
		const json& vector_4 = j.at("Vector4");
		vector_4.at("x").get_to(opt.x);
		vector_4.at("y").get_to(opt.y);
		vector_4.at("z").get_to(opt.z);
		vector_4.at("w").get_to(opt.w);
	}
};

//Matrix 4x4
template <>
struct adl_serializer<Matrix>
{
	static void to_json(json& j, const Matrix& opt)
	{
		
		j["Matrix"] = {
			{"m0", opt.m0},
			{"m1", opt.m1},
			{"m2", opt.m2},
			{"m3", opt.m3},
			{"m4", opt.m4},
			{"m5", opt.m5},
			{"m6", opt.m6},
			{"m7", opt.m7},
			{"m8", opt.m8},
			{"m9", opt.m9},
			{"m10", opt.m10},
			{"m11", opt.m11},
			{"m12", opt.m12},
			{"m13", opt.m13},
			{"m14", opt.m14},
			{"m15", opt.m15}
		};
	}

	static void from_json(const json& j, Matrix& opt)
	{
		const json& matrix = j.at("Matrix");
		matrix.at("m0").get_to(opt.m0);
		matrix.at("m1").get_to(opt.m1);
		matrix.at("m2").get_to(opt.m2);
		matrix.at("m3").get_to(opt.m3);
		matrix.at("m4").get_to(opt.m4);
		matrix.at("m5").get_to(opt.m5);
		matrix.at("m6").get_to(opt.m6);
		matrix.at("m7").get_to(opt.m7);
		matrix.at("m8").get_to(opt.m8);
		matrix.at("m9").get_to(opt.m9);
		matrix.at("m10").get_to(opt.m10);
		matrix.at("m11").get_to(opt.m11);
		matrix.at("m12").get_to(opt.m12);
		matrix.at("m13").get_to(opt.m13);
		matrix.at("m14").get_to(opt.m14);
		matrix.at("m15").get_to(opt.m15);
	}
};

//Color
template <>
struct adl_serializer<Color>
{
	static void to_json(json& j, const Color& opt)
	{
		
		j["Color"] = {
			{"r", opt.r},
			{"g", opt.g},
			{"b", opt.b},
			{"a", opt.a}
		};
	}

	static void from_json(const json& j, Color& opt)
	{
		const json& color = j.at("Color");
		color.at("r").get_to(opt.r);
		color.at("g").get_to(opt.g);
		color.at("b").get_to(opt.b);
		color.at("a").get_to(opt.a);
	}
};

//Rectangle
template <>
struct adl_serializer<Rectangle>
{
	static void to_json(json& j, const Rectangle& opt)
	{
		
		j["Rectangle"] =  {
			{"x", opt.x},
			{"y", opt.y},
			{"width", opt.width},
			{"height", opt.height}
		};
	}

	static void from_json(const json& j, Rectangle& opt)
	{
		const json& rect = j.at("Rectangle");
		rect.at("x").get_to(opt.x);
		rect.at("y").get_to(opt.y);
		rect.at("width").get_to(opt.width);
		rect.at("height").get_to(opt.height);
	}
};

//NPatchInfo
template <>
struct adl_serializer<NPatchInfo>
{
	static void to_json(json& j, const NPatchInfo& opt)
	{
		
		j["NPatchInfo"] = {
			{"sourceRec", opt.sourceRec},
			{"left", opt.left},
			{"top", opt.top},
			{"right", opt.right},
			{"bottom", opt.bottom},
			{"type", opt.type}
		};	
	}

	static void from_json(const json& j, NPatchInfo& opt)
	{
		const json& n_patch_info = j.at("NPatchInfo");
		n_patch_info.at("sourceRec").get_to(opt.sourceRec);
		n_patch_info.at("left").get_to(opt.left);
		n_patch_info.at("top").get_to(opt.top);
		n_patch_info.at("right").get_to(opt.right);
		n_patch_info.at("bottom").get_to(opt.bottom);
		n_patch_info.at("type").get_to(opt.type);
	}
};

//Camera3D
template <>
struct adl_serializer<Camera3D>
{
	static void to_json(json& j, const Camera3D& opt)
	{
		
		j["Camera3D"] = {
			{"position", opt.position},
			{"target", opt.target},
			{"up", opt.up},
			{"fovy", opt.fovy},
			{"type", opt.type}
		};
	}

	static void from_json(const json& j, Camera3D& opt)
	{
		const json& cam_3d = j.at("Camera3D");
		cam_3d.at("position").get_to(opt.position);
		cam_3d.at("target").get_to(opt.target);
		cam_3d.at("up").get_to(opt.up);
		cam_3d.at("fovy").get_to(opt.fovy);
		cam_3d.at("type").get_to(opt.type);

	}
};

//Camera2D
template <>
struct adl_serializer<Camera2D>
{
	static void to_json(json& j, const Camera2D& opt)
	{
		
		j["Camera2D"] = {
			{"offset", opt.offset},
			{"target", opt.target},
			{"rotation", opt.rotation},
			{"zoom", opt.zoom}
		};
	}

	static void from_json(const json& j, Camera2D& opt)
	{
		const json& cam_2d = j.at("Camera2D");
		cam_2d.at("offset").get_to(opt.offset);
		cam_2d.at("target").get_to(opt.target);
		cam_2d.at("rotation").get_to(opt.rotation);
		cam_2d.at("zoom").get_to(opt.zoom);
	}
};

//Transform
template <>
struct adl_serializer<Transform>
{
	static void to_json(json& j, const Transform& opt)
	{
		
		j["Transform"] = {
			{"translation", opt.translation},
			{"rotation", opt.rotation},
			{"scale", opt.scale}
		};
	}

	static void from_json(const json& j, Transform& opt)
	{
		const json& transform = j.at("Transform");
		transform.at("translation").get_to(opt.translation);
		transform.at("rotation").get_to(opt.rotation);
		transform.at("scale").get_to(opt.scale);
	}
};

//Ray
template <>
struct adl_serializer<Ray>
{
	static void to_json(json& j, const Ray& opt)
	{
		
		j["Ray"] = {
			{"position", opt.position},
			{"direction", opt.direction}
		};
	}

	static void from_json(const json& j, Ray& opt)
	{
		const json& ray = j.at("Ray");
		ray.at("position").get_to(opt.position);
		ray.at("direction").get_to(opt.direction);
	}
};

//RayHitInfo
template <>
struct adl_serializer<RayHitInfo>
{
	static void to_json(json& j, const RayHitInfo& opt)
	{
		
		j["RayHitInfo"] = {
			{"hit", opt.hit},
			{"distance", opt.distance},
			{"position", opt.position},
			{"normal", opt.normal}
		};
	}

	static void from_json(const json& j, RayHitInfo& opt)
	{
		const json& ray_hit_info = j.at("RayHitInfo");
		ray_hit_info.at("hit").get_to(opt.hit);
		ray_hit_info.at("distance").get_to(opt.distance);
		ray_hit_info.at("position").get_to(opt.position);
		ray_hit_info.at("normal").get_to(opt.normal);
	}
};

//BoundingBox
template <>
struct adl_serializer<BoundingBox>
{
	static void to_json(json& j, const BoundingBox& opt)
	{
		
		j["BoundingBox"] = {
			{"min", opt.min},
			{"max", opt.max}
		};
	}

	static void from_json(const json& j, BoundingBox& opt)
	{
		const json& bounding_box = j.at("BoundingBox");
		bounding_box.at("min").get_to(opt.min);
		bounding_box.at("max").get_to(opt.max);
	}
};

//VrDeviceInfo
template <>
struct adl_serializer<VrDeviceInfo>
{
	static void to_json(json& j, const VrDeviceInfo& opt)
	{
		
		j["VrDeviceInfo"] = {
			{"hResolution", opt.hResolution},
			{"vResolution", opt.vResolution},
			{"hScreenSize", opt.hScreenSize},
			{"vScreenSize", opt.vScreenSize},
			{"vScreenCenter", opt.vScreenCenter},
			{"eyeToScreenDistance", opt.eyeToScreenDistance},
			{
				"lensSeparationDistance",
				opt.lensSeparationDistance
			},
			{
				"interpupillaryDistance",
				opt.interpupillaryDistance
			},
			{
				"lensDistortionValues",
				{
					opt.lensDistortionValues[0], 
					opt.lensDistortionValues[1],
					opt.lensDistortionValues[2],
					opt.lensDistortionValues[3]
				}
			},
			{
				"chromaAbCorrection",
				{
					opt.chromaAbCorrection[0],
					opt.chromaAbCorrection[1],
					opt.chromaAbCorrection[2],
					opt.chromaAbCorrection[3]
				}
			}
		};
	}

	static void from_json(const json& j, VrDeviceInfo& opt)
	{
		const json& vr_device_info = j.at("VrDeviceInfo");
		vr_device_info.at("hResolution").get_to(opt.hResolution);
		vr_device_info.at("vResolution").get_to(opt.vResolution);
		vr_device_info.at("hScreenSize").get_to(opt.hScreenSize);
		vr_device_info.at("vScreenSize").get_to(opt.vScreenSize);
		vr_device_info.at("vScreenCenter").get_to(opt.vScreenCenter);
		vr_device_info.at("eyeToScreenDistance")
			.get_to(opt.eyeToScreenDistance);
		vr_device_info.at("lensSeparationDistance")
			.get_to(opt.lensSeparationDistance);
		vr_device_info.at("interpupillaryDistance")
			.get_to(opt.interpupillaryDistance);
		vr_device_info.at("lensDistortionValues")
			.get_to(opt.lensDistortionValues);
		vr_device_info.at("chromaAbCorrection")
			.get_to(opt.chromaAbCorrection);
	}
};
}

#endif /* SRC_RAYLIBSERIALIZE_H_ */
