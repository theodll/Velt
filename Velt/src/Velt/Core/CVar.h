#pragma once
#include "Core/Core.h"
#include <string>

/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

This is heavily based on the original CVAR code by Id Software, Inc. from 1996-1997 (as stated in the license) but modified to be fitting for this engine and modernized to use safe C++ features.

*/


namespace Velt 
{
	struct CVar_t
	{
		std::string Name;
		std::string String;
		float Value;
		bool Serialize;
		bool Server; // notifies player when 
		struct CVar_t* Next;
	};

	class VELT_API CVar
	{
	public:
		static void RegisterVariable(CVar_t* pVariable);
		static void Set(const std::string& variableName, const std::string& value);
		static void SetValue(const std::string& variableName, const float value);
		static float VariableValue(const std::string& variableName);
		static const std::string& VariableString(const std::string& variableName);
		static const std::string& CompleteVariable(const std::string& partial);
		static CVar_t* FindVar(const std::string& variableName);

		static void SerializeVariables();
	private:
		static CVar_t* m_pCVarVars;
	}; 


}