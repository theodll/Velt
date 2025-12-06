namespace Velt::RenderAPI
{
	enum class API
	{
		None = 0,
		VULKAN = 1,
		D3D12 = 2,
		METAL = 3
	};

	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;
		virtual void Init() = 0;

		static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}