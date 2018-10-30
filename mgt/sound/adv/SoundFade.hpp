/// @file

#pragma once

class CSoundFade
{
public:
	CSoundFade() { Clear(); }

	void	Clear();
	void	SetVolume( float to );
	void	Fade( float to, int length, int soundTime );

	float	GetVolume( int soundTime ) const;
	
	int		fadeStartTime;
	int		fadeEndTime;
	float	fadeStartVolume;
	float	fadeEndVolume;
};