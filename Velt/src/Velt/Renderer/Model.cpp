#include "Core/Core.h"
#include "Model.h"

namespace Velt {
	
	
	Ref<Model> Model::Create(const ModelCreateInfo* pInfo)
	{
		Ref<Model> model = CreateRef<Model>();
		model->m_Submeshes.reserve(pInfo->Parts.size());

		for (const auto& part : pInfo->Parts)
		{
			Submesh sm{};
			sm.Mesh = CreateRef<Mesh>(part.Vertices, part.Indices);
			sm.MatIndex = part.MaterialIndex;
			model->m_Submeshes.push_back(std::move(sm));
		}

		return model;
	}
}