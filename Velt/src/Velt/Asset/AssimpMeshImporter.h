#pragma once
#include "Core/Core.h"
#include "Renderer/Model.h"
#include <filesystem>


struct aiNode;


namespace Velt 
{

	class VELT_API AssimpMeshImporter 
	{
	public:
		AssimpMeshImporter(const std::filesystem::path& path);
		Ref<Mesh> ImportToMesh();
	private:
		void TraverseNodes(Ref<Mesh> mesh, aiNode* aNode, uint32_t nodeIndex, const Matrix& parentTransform);

		std::filesystem::path m_Path;
	};
} 