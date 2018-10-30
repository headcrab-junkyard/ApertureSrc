/// @file

#pragma once

/*
===============================================================================

	SOUND EMITTER

===============================================================================
*/

// sound channels
static const int SCHANNEL_ANY = 0;	// used in queries and commands to effect every channel at once, in
									// startSound to have it not override any other channel
static const int SCHANNEL_ONE = 1;	// any following integer can be used as a channel number
typedef int s_channelType;	// the game uses its own series of enums, and we don't want to require casts

struct ISoundEmitter
{
	//virtual					~ISoundEmitter() = default;

	// a non-immediate free will let all currently playing sounds complete
	// soundEmitters are not actually deleted, they are just marked as
	// reusable by the soundWorld
	virtual void			Free( bool immediate ) = 0;

	// the parms specified will be the default overrides for all sounds started on this emitter.
	// NULL is acceptable for parms
	virtual void			UpdateEmitter( const idVec3 &origin, int listenerId, const soundShaderParms_t *parms ) = 0;

	// returns the length of the started sound in msec
	virtual int				StartSound( const idSoundShader *shader, const s_channelType channel, float diversity = 0, int shaderFlags = 0, bool allowSlow = true ) = 0;

	// pass SCHANNEL_ANY to effect all channels
	virtual void			ModifySound( const s_channelType channel, const soundShaderParms_t *parms ) = 0;
	virtual void			StopSound( const s_channelType channel ) = 0;
	// to is in Db, over is in seconds
	virtual void			FadeSound( const s_channelType channel, float to, float over ) = 0;

	// returns true if there are any sounds playing from this emitter.  There is some conservative
	// slop at the end to remove inconsistent race conditions with the sound thread updates.
	// FIXME: network game: on a dedicated server, this will always be false
	virtual bool			CurrentlyPlaying( const s_channelType channel = SCHANNEL_ANY ) const = 0;

	// returns a 0.0 to 1.0 value based on the current sound amplitude, allowing
	// graphic effects to be modified in time with the audio.
	// just samples the raw wav file, it doesn't account for volume overrides in the
	virtual	float			CurrentAmplitude() = 0;

	// for save games.  Index will always be > 0
	virtual	int				Index() const = 0;
};