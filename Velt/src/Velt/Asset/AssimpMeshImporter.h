#pragma once
#include "Core/Core.h"
#include "Renderer/Model.h"
#include <filesystem>

namespace Velt 
{
	class AssimpMeshImporter 
	{
	public:
		AssimpMeshImporter(const std::filesystem::path& path);
		Ref<Mesh> ImportToMesh();
	private:
		void TraverseNodes(Ref<Mesh> meshSource, void* pAssimpNode, u32 nodeIndex, const Matrix& parentTranform = Matrix(1.0f), u32 level = 0);

		std::filesystem::path m_Path;
	};
} 