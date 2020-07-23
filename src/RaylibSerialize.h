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

namespace nlohmann
{
	//Vector2
	template <>
	struct adl_serializer<Vector2>
	{
		static void to_json(json& j, const Vector2& opt)
		{
			j = {
				{"Vector2.x", opt.x},
				{"Vector2.y", opt.y}
			};
		}

		static void from_json(const json& j, Vector2& opt)
		{
			j.at("Vector2.x").get_to(opt.x);
			j.at("Vector2.y").get_to(opt.y);
		}
	};

	//Vector3
	template <>
	struct adl_serializer<Vector3>
	{
		static void to_json(json& j, const Vector3& opt)
		{
			j = {
				{"Vector3.x", opt.x},
				{"Vector3.y", opt.y},
				{"Vector3.z", opt.z}
			};
		}

		static void from_json(const json& j, Vector3& opt)
		{
			j.at("Vector3.x").get_to(opt.x);
			j.at("Vector3.y").get_to(opt.y);
			j.at("Vector3.z").get_to(opt.z);
		}
	};

	//Vector4
	template <>
	struct adl_serializer<Vector4>
	{
		static void to_json(json& j, const Vector4& opt)
		{
			j = {
				{"Vector4.x", opt.x},
				{"Vector4.y", opt.y},
				{"Vector4.z", opt.z},
				{"Vector4.w", opt.w}
			};
		}

		static void from_json(const json& j, Vector4& opt)
		{
			j.at("Vector4.x").get_to(opt.x);
			j.at("Vector4.y").get_to(opt.y);
			j.at("Vector4.z").get_to(opt.z);
			j.at("Vector4.w").get_to(opt.w);
		}
	};

	//Matrix 4x4
	template <>
	struct adl_serializer<Matrix>
	{
		static void to_json(json& j, const Matrix& opt)
		{
			j = {
				{"Matrix.m0", opt.m0},
				{"Matrix.m1", opt.m1},
				{"Matrix.m2", opt.m2},
				{"Matrix.m3", opt.m3},
				{"Matrix.m4", opt.m4},
				{"Matrix.m5", opt.m5},
				{"Matrix.m6", opt.m6},
				{"Matrix.m7", opt.m7},
				{"Matrix.m8", opt.m8},
				{"Matrix.m9", opt.m9},
				{"Matrix.m10", opt.m10},
				{"Matrix.m11", opt.m11},
				{"Matrix.m12", opt.m12},
				{"Matrix.m13", opt.m13},
				{"Matrix.m14", opt.m14},
				{"Matrix.m15", opt.m15}
			};
		}

		static void from_json(const json& j, Matrix& opt)
		{
			j.at("Matrix.m0").get_to(opt.m0);
			j.at("Matrix.m1").get_to(opt.m1);
			j.at("Matrix.m2").get_to(opt.m2);
			j.at("Matrix.m3").get_to(opt.m3);
			j.at("Matrix.m4").get_to(opt.m4);
			j.at("Matrix.m5").get_to(opt.m5);
			j.at("Matrix.m6").get_to(opt.m6);
			j.at("Matrix.m7").get_to(opt.m7);
			j.at("Matrix.m8").get_to(opt.m8);
			j.at("Matrix.m9").get_to(opt.m9);
			j.at("Matrix.m10").get_to(opt.m10);
			j.at("Matrix.m11").get_to(opt.m11);
			j.at("Matrix.m12").get_to(opt.m12);
			j.at("Matrix.m13").get_to(opt.m13);
			j.at("Matrix.m14").get_to(opt.m14);
			j.at("Matrix.m15").get_to(opt.m15);
		}
	};

	//Color
	template <>
	struct adl_serializer<Color>
	{
		static void to_json(json& j, const Color& opt)
		{
			j = {
				{"Color.r", opt.r},
				{"Color.g", opt.g},
				{"Color.b", opt.b},
				{"Color.a", opt.a}
			};
		}

		static void from_json(const json& j, Color& opt)
		{
			j.at("Color.r").get_to(opt.r);
			j.at("Color.g").get_to(opt.g);
			j.at("Color.b").get_to(opt.b);
			j.at("Color.a").get_to(opt.a);
		}
	};

	//Rectangle
	template <>
	struct adl_serializer<Rectangle>
	{
		static void to_json(json& j, const Rectangle& opt)
		{
			j = {
				{"Rectangle.x", opt.x},
				{"Rectangle.y", opt.y},
				{"Rectangle.width", opt.width},
				{"Rectangle.height", opt.height}
			};
		}

		static void from_json(const json& j, Rectangle& opt)
		{
			j.at("Rectangle.x").get_to(opt.x);
			j.at("Rectangle.y").get_to(opt.y);
			j.at("Rectangle.width").get_to(opt.width);
			j.at("Rectangle.height").get_to(opt.height);
		}
	};

	//NPatchInfo
	template <>
	struct adl_serializer<NPatchInfo>
	{
		static void to_json(json& j, const NPatchInfo& opt)
		{
			j = {
				{"NPatchInfo.sourceRec", opt.sourceRec},
				{"NPatchInfo.left", opt.left},
				{"NPatchInfo.top", opt.top},
				{"NPatchInfo.right", opt.right},
				{"NPatchInfo.bottom", opt.bottom},
				{"NPatchInfo.type", opt.type}
			};
		}

		static void from_json(const json& j, NPatchInfo& opt)
		{
			j.at("NPatchInfo.sourceRec").get_to(opt.sourceRec);
			j.at("NPatchInfo.left").get_to(opt.left);
			j.at("NPatchInfo.top").get_to(opt.top);
			j.at("NPatchInfo.right").get_to(opt.right);
			j.at("NPatchInfo.bottom").get_to(opt.bottom);
			j.at("NPatchInfo.type").get_to(opt.type);
		}
	};

	//Camera3D
	template <>
	struct adl_serializer<Camera3D>
	{
		static void to_json(json& j, const Camera3D& opt)
		{
			j = {
				{"Camera3D.position", opt.position},
				{"Camera3D.target", opt.target},
				{"Camera3D.up", opt.up},
				{"Camera3D.fovy", opt.fovy},
				{"Camera3D.type", opt.type}
			};
		}

		static void from_json(const json& j, Camera3D& opt)
		{
			j.at("Camera3D.position").get_to(opt.position);
			j.at("Camera3D.target").get_to(opt.target);
			j.at("Camera3D.up").get_to(opt.up);
			j.at("Camera3D.fovy").get_to(opt.fovy);
			j.at("Camera3D.type").get_to(opt.type);

		}
	};

	//Camera2D
	template <>
	struct adl_serializer<Camera2D>
	{
		static void to_json(json& j, const Camera2D& opt)
		{
			j = {
				{"Camera2D.offset", opt.offset},
				{"Camera2D.target", opt.target},
				{"Camera2D.rotation", opt.rotation},
				{"Camera2D.zoom", opt.zoom},
			};
		}

		static void from_json(const json& j, Camera2D& opt)
		{
			j.at("Camera2D.offset").get_to(opt.offset);
			j.at("Camera2D.target").get_to(opt.target);
			j.at("Camera2D.rotation").get_to(opt.rotation);
			j.at("Camera2D.zoom").get_to(opt.zoom);

		}
	};

	//Transform
	template <>
	struct adl_serializer<Transform>
	{
		static void to_json(json& j, const Transform& opt)
		{
			j = {
				{"Transform.translation", opt.translation},
				{"Transform.rotation", opt.rotation},
				{"Transform.scale", opt.scale}
			};
		}

		static void from_json(const json& j, Transform& opt)
		{
			j.at("Transform.translation").get_to(opt.translation);
			j.at("Transform.rotation").get_to(opt.rotation);
			j.at("Transform.scale").get_to(opt.scale);
		}
	};

	//Ray
	template <>
	struct adl_serializer<Ray>
	{
		static void to_json(json& j, const Ray& opt)
		{
			j = {
				{"Ray.position", opt.position},
				{"Ray.direction", opt.direction}
			};
		}

		static void from_json(const json& j, Ray& opt)
		{
			j.at("Ray.position").get_to(opt.position);
			j.at("Ray.direction").get_to(opt.direction);
		}
	};

	//RayHitInfo
	template <>
	struct adl_serializer<RayHitInfo>
	{
		static void to_json(json& j, const RayHitInfo& opt)
		{
			j = {
				{"RayHitInfo.hit", opt.hit},
				{"RayHitInfo.distance", opt.distance},
				{"RayHitInfo.position", opt.position},
				{"RayHitInfo.normal", opt.normal}
			};
		}

		static void from_json(const json& j, RayHitInfo& opt)
		{
			j.at("RayHitInfo.hit").get_to(opt.hit);
			j.at("RayHitInfo.distance").get_to(opt.distance);
			j.at("RayHitInfo.position").get_to(opt.position);
			j.at("RayHitInfo.normal").get_to(opt.normal);
		}
	};

	//BoundingBox
	template <>
	struct adl_serializer<BoundingBox>
	{
		static void to_json(json& j, const BoundingBox& opt)
		{
			j = {
				{"BoundingBox.min", opt.min},
				{"BoundingBox.max", opt.max}
			};
		}

		static void from_json(const json& j, BoundingBox& opt)
		{
			j.at("BoundingBox.min").get_to(opt.min);
			j.at("BoundingBox.max").get_to(opt.max);
		}
	};

	//VrDeviceInfo
	template <>
	struct adl_serializer<VrDeviceInfo>
	{
		static void to_json(json& j, const VrDeviceInfo& opt)
		{
			j = {
				{"VrDeviceInfo.hResolution", opt.hResolution},
				{"VrDeviceInfo.vResolution", opt.vResolution},
				{"VrDeviceInfo.hScreenSize", opt.hScreenSize},
				{"VrDeviceInfo.vScreenSize", opt.vScreenSize},
				{"VrDeviceInfo.vScreenCenter", opt.vScreenCenter},
				{"VrDeviceInfo.eyeToScreenDistance", opt.eyeToScreenDistance},
				{
					"VrDeviceInfo.lensSeparationDistance",
					opt.lensSeparationDistance
				},
				{
					"VrDeviceInfo.interpupillaryDistance",
					opt.interpupillaryDistance
				},
				{
					"VrDeviceInfo.lensDistortionValues0",
					opt.lensDistortionValues[0]
				},
				{
					"VrDeviceInfo.lensDistortionValues1",
					opt.lensDistortionValues[1]
				},
				{
					"VrDeviceInfo.lensDistortionValues2",
					opt.lensDistortionValues[2]
				},
				{
					"VrDeviceInfo.lensDistortionValues3",
					opt.lensDistortionValues[3]
				},
				{
					"VrDeviceInfo.chromaAbCorrection0",
					opt.chromaAbCorrection[0]
				},
				{
					"VrDeviceInfo.chromaAbCorrection1",
					opt.chromaAbCorrection[1]
				},
				{
					"VrDeviceInfo.chromaAbCorrection2",
					opt.chromaAbCorrection[2]
				},
				{
					"VrDeviceInfo.chromaAbCorrection3",
					opt.chromaAbCorrection[3]
				}
			};
		}

		static void from_json(const json& j, VrDeviceInfo& opt)
		{
			j.at("VrDeviceInfo.hResolution").get_to(opt.hResolution);
			j.at("VrDeviceInfo.vResolution").get_to(opt.vResolution);
			j.at("VrDeviceInfo.hScreenSize").get_to(opt.hScreenSize);
			j.at("VrDeviceInfo.vScreenSize").get_to(opt.vScreenSize);
			j.at("VrDeviceInfo.vScreenCenter").get_to(opt.vScreenCenter);
			j.at("VrDeviceInfo.eyeToScreenDistance")
				.get_to(opt.eyeToScreenDistance);
			j.at("VrDeviceInfo.lensSeparationDistance")
				.get_to(opt.lensSeparationDistance);
			j.at("VrDeviceInfo.interpupillaryDistance")
				.get_to(opt.interpupillaryDistance);
			j.at("VrDeviceInfo.lensDistortionValues0")
				.get_to(opt.lensDistortionValues[0]);
			j.at("VrDeviceInfo.lensDistortionValues1")
				.get_to(opt.lensDistortionValues[1]);
			j.at("VrDeviceInfo.lensDistortionValues2")
				.get_to(opt.lensDistortionValues[2]);
			j.at("VrDeviceInfo.lensDistortionValues3")
				.get_to(opt.lensDistortionValues[3]);
			j.at("VrDeviceInfo.chromaAbCorrection0")
				.get_to(opt.chromaAbCorrection[0]);
			j.at("VrDeviceInfo.chromaAbCorrection1")
				.get_to(opt.chromaAbCorrection[1]);
			j.at("VrDeviceInfo.chromaAbCorrection2")
				.get_to(opt.chromaAbCorrection[2]);
			j.at("VrDeviceInfo.chromaAbCorrection3")
				.get_to(opt.chromaAbCorrection[3]);
		}
	};

}

#endif /* SRC_RAYLIBSERIALIZE_H_ */
