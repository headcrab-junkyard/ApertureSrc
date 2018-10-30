/// @file

#pragma once

#include "sound/ISoundEmitter.hpp"

// Maximum number of SoundChannels for a single SoundEmitter.
// This is probably excessive...
const int MAX_CHANNELS_PER_EMITTER{16};

/*
================================================
CSoundEmitter 
================================================
*/
class CSoundEmitter : public ISoundEmitter
{
public:
	// sound emitters are only allocated by the soundWorld block allocator
					CSoundEmitter();
	virtual			~CSoundEmitter();
	
	virtual void	Free( bool immediate );

	virtual void	Reset();

	virtual void	UpdateEmitter( const idVec3 &origin, int listenerId, const soundShaderParms_t *parms );

	virtual int		StartSound( const idSoundShader *shader, const s_channelType channel, float diversity = 0, int shaderFlags = 0, bool allowSlow = true );

	virtual void	ModifySound( const s_channelType channel, const soundShaderParms_t *parms );
	virtual void	StopSound( const s_channelType channel );

	virtual void	FadeSound( const s_channelType channel, float to, float over );

	virtual bool	CurrentlyPlaying( const s_channelType channel = SCHANNEL_ANY ) const;

	virtual	float	CurrentAmplitude();

	virtual	int		Index() const;

	//----------------------------------------------

	void			Init( int i, CSoundWorld * sw );

	// Returns true if the emitter should be freed.
	bool			CheckForCompletion( int currentTime );

	void			OverrideParms( const soundShaderParms_t * base, const soundShaderParms_t * over, soundShaderParms_t * out );

	void			Update( int currentTime );
	void			OnReloadSound( const idDecl *decl );

	//----------------------------------------------

	CSoundWorldLocal *		soundWorld;						// the world that holds this emitter

	int			index;							// in world emitter list
	bool		canFree;						// if true, this emitter can be canFree (once channels.Num() == 0)

	// a single soundEmitter can have many channels playing from the same point
	CStaticList<ISoundChannel *, MAX_CHANNELS_PER_EMITTER> channels;

	//----- set by UpdateEmitter -----
	CVec3				origin;
	soundShaderParms_t	parms;
	int					emitterId;						// sounds will be full volume when emitterId == listenerId

	//----- set by Update -----
	int			lastValidPortalArea;
	float		directDistance;
	float		spatializedDistance;
	CVec3		spatializedOrigin;
};