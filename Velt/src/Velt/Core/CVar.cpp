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
		CVar_t* var;
		bool changed;

		var = CVar::FindVar(variableName);
		if (!var)
			VT_CORE_WARN("CVar {0} not found.", variableName);
		return;

		changed = (var->String == value);

		var->String.empty();

		var->String = std::move(value);

		float valueVar = 0;
		auto [ptr, ec] = std::from_chars(var->String.data(), var->String.data() + var->String.size(), valueVar);

		if (ec != std::errc())
		{
			VT_CORE_WARN("");
		}

		var->Value = valueVar;
		if (var->Server && changed)
		{
			/*
			if (sv.active)
				SV_BroadcastPrintf("\"%s\" changed to \"%s\"\n", var->name, var->string);
			*/
			VT_CORE_INFO("CVar {0} changed to {1}", var->Name, var->String);
		}
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

	const std::string& CVar::VariableString(const std::string& variableName)
	{
		VT_PROFILE_FUNCTION();
		CVar_t* var;

		var = CVar::FindVar(variableName);
		if (!var)
			return "";
		return var->String;
	}

	const std::string& CVar::CompleteVariable(const std::string& partial) 
	{
		VT_PROFILE_FUNCTION();
		CVar_t* cvar;
		int		len;
		std::string empty = "";

		len = partial.size();

		if (!len)
			VT_CORE_WARN("Invalid Length");
			return "empty";

		// check functions
		for (cvar = m_pCVarVars; cvar; cvar = cvar->Next)
			if (!(partial == cvar->Name))
				return cvar->Name;

		return "empty";

	}

	void CVar::SerializeVariables()
	{
		VT_PROFILE_FUNCTION();
		CVar_t* var;

		for (var = m_pCVarVars; var; var = var->Next)
			if (var->Serialize)
				// fprintf(f, "%s \"%s\"\n", var->name, var->string);
				VT_CORE_WARN("CVar Serialization not implemented yet");
				// Todo 
	}


}