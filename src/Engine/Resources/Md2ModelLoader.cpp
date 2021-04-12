#include "Md2ModelLoader.hpp"
#include "../../Math/Vector2.hpp"
#include "../../Math/Vector3.hpp"
#include "Md2Normals.hpp"
#include "Util/FileUtils.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <memory>
#include <sstream>

namespace Engine
{
	namespace Resources
	{
		using namespace Math;

		std::string Md2ModelLoader::GetNamePart(const std::string& frame_name) const
		{
			std::string ret = "";

			for (uint8_t i = 0; i < 16; i++)
			{
				if (frame_name[i] == '\0' || isdigit(frame_name[i]))
					break;

				ret.push_back(frame_name[i]);
			}

			return ret;
		}

		bool Md2ModelLoader::LoadStaticModel(const std::string& filename,
											 std::vector<Vertex>& out_vertices,
											 std::vector<uint32_t>& out_indices,
											 ModelLoadingOptions options)
		{
			// TODO load only one frame's vertices
			return false;
		}

		bool Md2ModelLoader::LoadAnimatedModel(const std::string& filename,
											   std::vector<Frame>& out_frames,
											   std::vector<uint32_t>& out_indices,
											   std::map<std::string, Animation>& out_animations,
											   ModelLoadingOptions options)
		{
			std::ifstream file_stream;

			file_stream.open(filename, std::ios::binary);

			if (!file_stream.is_open())
				return false;

			const uint8_t md2_header_bytes = sizeof(Md2Header);

			Md2Header header;

			file_stream.read((char*)&header, sizeof(Md2Header));

			if (header.magicnumber != 844121161)
				return false;

			if (header.version != 8)
				return false;

			/*
				Texture coordinates
			*/
			std::vector<Vector2> texturecoords;

			file_stream.seekg(header.textcoords_offset);

			std::vector<unsigned char> texturecoords_chunk;
			uint32_t texturecoords_size = header.textcoords_count * (sizeof(uint16_t) * 2);
			texturecoords_chunk.resize(texturecoords_size);

			file_stream.read((char*)texturecoords_chunk.data(), texturecoords_size);

			for (uint16_t i = 0; i < header.textcoords_count; i++)
			{
				uint32_t offset = i * sizeof(uint16_t) * 2;

				uint16_t texturecoord_x = *reinterpret_cast<uint16_t*>(&texturecoords_chunk[offset]);
				uint16_t texturecoord_y = *reinterpret_cast<uint16_t*>(&texturecoords_chunk[offset + sizeof(uint16_t)]);

				texturecoords.push_back(Vector2((float)texturecoord_x / (float)header.skinwidth, (float)texturecoord_y / (float)header.skinheight));
			}

			/*
				Triangles (indices with texture coordnidates)
			*/

			// a map of texture coordinates indices, indexed by vertex index
			std::map<uint16_t, uint16_t> vertices_to_texturecoords;

			file_stream.seekg(header.triangles_offset);

			std::vector<unsigned char> triangles_chunk;
			uint32_t triangles_size = header.triangles_count * (sizeof(uint16_t) * 6);
			triangles_chunk.resize(triangles_size);

			file_stream.read((char*)triangles_chunk.data(), triangles_size);

			for (uint16_t i = 0; i < header.triangles_count; i++)
			{
				uint32_t offset = i * sizeof(uint16_t) * 3 * 2;

				uint16_t vertex_index0 = *reinterpret_cast<uint16_t*>(&triangles_chunk[offset]);
				uint16_t vertex_index1 = *reinterpret_cast<uint16_t*>(&triangles_chunk[offset + sizeof(uint16_t)]);
				uint16_t vertex_index2 = *reinterpret_cast<uint16_t*>(&triangles_chunk[offset + sizeof(uint16_t) * 2]);

				uint16_t texturecoord_index0 = *reinterpret_cast<uint16_t*>(&triangles_chunk[offset + sizeof(uint16_t) * 3]);
				uint16_t texturecoord_index1 = *reinterpret_cast<uint16_t*>(&triangles_chunk[offset + sizeof(uint16_t) * 4]);
				uint16_t texturecoord_index2 = *reinterpret_cast<uint16_t*>(&triangles_chunk[offset + sizeof(uint16_t) * 5]);

				out_indices.push_back((uint32_t)vertex_index2);
				out_indices.push_back((uint32_t)vertex_index1);
				out_indices.push_back((uint32_t)vertex_index0);

				vertices_to_texturecoords.insert_or_assign(vertex_index0, texturecoord_index0);
				vertices_to_texturecoords.insert_or_assign(vertex_index1, texturecoord_index1);
				vertices_to_texturecoords.insert_or_assign(vertex_index2, texturecoord_index2);
			}

			/*
				Frames
			*/
			file_stream.seekg(header.frames_offset);

			std::vector<unsigned char> frames_chunk;
			uint32_t frames_size = header.frame_size * header.frames_count;
			frames_chunk.resize(frames_size);

			file_stream.read((char*)frames_chunk.data(), frames_size);

			for (uint16_t i = 0; i < header.frames_count; i++)
			{
				uint32_t offset = i * header.frame_size;

				std::vector<Vertex> vertices;
				Vector3 scale;
				Vector3 translation;
				char name[16];

				scale.x = *reinterpret_cast<float*>(&frames_chunk[offset]);
				scale.y = *reinterpret_cast<float*>(&frames_chunk[offset + sizeof(float)]);
				scale.z = *reinterpret_cast<float*>(&frames_chunk[offset + sizeof(float) * 2]);

				translation.x = *reinterpret_cast<float*>(&frames_chunk[offset + sizeof(float) * 3]);
				translation.y = *reinterpret_cast<float*>(&frames_chunk[offset + sizeof(float) * 4]);
				translation.z = *reinterpret_cast<float*>(&frames_chunk[offset + sizeof(float) * 5]);

				for (uint8_t chr = 0; chr < 16; chr++)
					name[chr] = *reinterpret_cast<char*>(&frames_chunk[offset + (sizeof(float) * 6) + chr]);

				uint16_t vertices_offset = (sizeof(float) * 6) + (sizeof(char) * 16);
				uint16_t vertices_size	 = sizeof(uint8_t) * 4;

				for (uint32_t j = 0; j < header.vertices_count; j++)
				{
					uint32_t vertex_offset = offset + vertices_offset + (vertices_size * j);

					uint8_t pos_x		 = *reinterpret_cast<uint8_t*>(&frames_chunk[vertex_offset]);
					uint8_t pos_y		 = *reinterpret_cast<uint8_t*>(&frames_chunk[vertex_offset + sizeof(uint8_t)]);
					uint8_t pos_z		 = *reinterpret_cast<uint8_t*>(&frames_chunk[vertex_offset + sizeof(uint8_t) * 2]);
					uint8_t normal_index = *reinterpret_cast<uint8_t*>(&frames_chunk[vertex_offset + sizeof(uint8_t) * 3]);

					const Vector2& texturecoord = texturecoords[vertices_to_texturecoords.at(j)];

					Vertex vertex = Vertex(Vector3(((float)pos_x * scale.x) + translation.x,
												   ((float)pos_y * scale.y) + translation.y,
												   ((float)pos_z * scale.z) + translation.z),
										   texturecoord);

					if (options.normal_options == GenerationCondition::NEVER || options.normal_options == GenerationCondition::IF_MISSING)
						vertex.SetNormal(md2_normals[normal_index]);

					vertices.push_back(vertex);
				}

				Frame frame = { std::string(name), std::make_shared<std::vector<Vertex>>() };

				*frame.vertices.get() = vertices;

				out_frames.push_back(frame);
			}

			/*
				Animations
			*/
			std::string example_name = "";
			uint32_t start_frame	 = 0;
			uint32_t end_frame		 = 0;

			for (uint16_t i = 0; i < out_frames.size(); i++)
			{
				const Frame& frame = out_frames[i];

				std::string name_part = GetNamePart(frame.name);

				bool is_last_frame = i == (out_frames.size() - 1);

				if (name_part != example_name || is_last_frame)
				{
					end_frame = is_last_frame ? i : i - 1;

					if (i != 0)
					{
						Animation animation = { start_frame, end_frame };

						out_animations.insert_or_assign(example_name, animation);
					}

					example_name = name_part;
					start_frame	 = i;
				}
			}

			for (Frame& frame : out_frames)
			{
				if (options.normal_options == GenerationCondition::ALWAYS)
					Vertex::CalculateNormals(*frame.vertices.get(), out_indices);

				// md2 does not support tangents
				if (options.tangent_options == GenerationCondition::ALWAYS || options.tangent_options == GenerationCondition::IF_MISSING)
					Vertex::CalculateTangents(*frame.vertices.get(), out_indices);
			}

			return true;
		}
	}
}
