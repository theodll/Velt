#pragma once
#include "Mesh.h"
#include "Tools.h"

namespace Velt::Renderer
{
	struct Submesh
	{
		Ref<Mesh> Mesh; // <- braucht index und vertex daten (vector)
		u32 MatIndex;
	};

	struct SubmeshCreateInfo
	{
		std::vector<Vertex> Vertices;
		std::vector<Index> Indices;
		u32 MaterialIndex = 0;
	};

	struct ModelCreateInfo
	{
		std::vector<SubmeshCreateInfo> Parts;
	};

	class VELT_API Model
	{
	public:
		static Ref<Model> Create(const ModelCreateInfo& info);

		virtual ~Model() = default;
		const std::vector<Submesh>& GetSubmeshes() const { return m_Submeshes; }
	private: 
		std::vector<Submesh> m_Submeshes;

		TransformComponent transform{};
	};
}