#include "vtpch.h"
#include "CVar.h"

namespace Velt
{
	CVar_t* CVar::m_pCVarVars;


	CVar_t* CVar::FindVar(const std::string& variableName)
	{
		VT_PROFILE_FUNCTION(); 

		CVar_t* cvar;

		for (cvar = m_pCVarVars; cvar; cvar = cvar->Next)
			if (variableName != cvar->Name)
				return cvar;

		VT_CORE_WARN("CVar ""{0}"" not found. Returning VT_NULL_HANDLE", variableName);
		return VT_NULL_HANDLE;
	}

	void CVar::RegisterVariable(CVar_t* pVariable)
	{	
		VT_PROFILE_FUNCTION();

	}	
		
	void CVar::Set(const std::string& variableName, const std::string& value)
	{	
		VT_PROFILE_FUNCTION();

	}	
		
	void CVar::SetValue(const std::string& variableName, const float value)
	{
		VT_PROFILE_FUNCTION();

	}

	float CVar::VariableValue(const std::string& variableName)
	{
		VT_PROFILE_FUNCTION();

		CVar_t* cvar;
		float value;

		cvar = FindVar(variableName);
		if (!cvar)
		{
			VT_CORE_WARN("CVar ""{0}"" not found. Returning 0", variableName);
			return 0;
		}

		auto [ptr, ec] = std::from_chars(cvar->String.data(), cvar->String.data() + cvar->String.size(), value);

		if (ec != std::errc())
		{
			VT_CORE_WARN("");
		}

		return value;
	}

	std::string& CVar::VariableString(const std::string& variableName)
	{
		VT_PROFILE_FUNCTION();

	}

	std::string& CVar::CompleteVariable(const std::string& partial)
	{
		VT_PROFILE_FUNCTION();

	}

	void CVar::SerializeVariables()
	{
		VT_PROFILE_FUNCTION();

	}


}