#include "Core/Core.h"
#include "Model.h"

namespace Velt::Renderer {
	
	
	Ref<Model> Model::Create(const ModelCreateInfo& info)
	{
		Ref<Model> model = CreateRef<Model>();
		model->m_Submeshes.reserve(info.Parts.size());

		for (const auto& part : info.Parts)
		{
			Submesh sm{};
			sm.Mesh = CreateRef<Mesh>(part.Vertices, part.Indices);
			sm.MatIndex = part.MaterialIndex;
			model->m_Submeshes.push_back(std::move(sm));
		}

		return model;
	}
}