#pragma once
#include "Mesh.h"
#include "Tools.h"

namespace Velt::Renderer
{
	struct Submesh
	{
		Ref<Mesh> Mesh;
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

		const TransformComponent& GetTransform() const {return m_Transform; }
		const std::vector<Submesh>& GetSubmeshes() const { return m_Submeshes; }
	private:
		Model() {};
		std::vector<Submesh> m_Submeshes;
		TransformComponent m_Transform{};
	};
}