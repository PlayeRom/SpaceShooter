#pragma once

#include "../SpaceShooter_LevelsEditor/LevelsStructs.h"

class CLevelsManager
{
public:
	CLevelsManager();
	~CLevelsManager();

	inline GLint GetLevelsSize()			{ return static_cast< GLint >( m_aLevels.size() ); }
	inline SLevel& GetLevel( GLint iIndex ) { return m_aLevels[ iIndex ]; }
	inline SShipsGroups& GetLavelGroup( GLint iIndexLevel, GLint iIndexGroup ) {
		return m_aLevels[ iIndexLevel ].psShipsGroups[ iIndexGroup ];
	}
	GLint GetIndexLevel( GLint iLevel );

private:
	GLboolean ReadLevels( LPCTSTR lpFileName );
	GLvoid Clear();

	std::vector<SLevel> m_aLevels;

	SLevel sErrorReturn;
};
