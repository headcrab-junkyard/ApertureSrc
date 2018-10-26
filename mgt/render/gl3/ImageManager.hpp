/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

#pragma once

class CImageManager final
{
public:
	CImageManager() = default;

	void Init();
	void Shutdown();

	/// If the exact combination of parameters has been asked for already, an existing
	/// image will be returned, otherwise a new image will be created.
	/// Be careful not to use the same image file with different filter / repeat / etc parameters
	/// if possible, because it will cause a second copy to be loaded.
	/// If the load fails for any reason, the image will be filled in with the default
	/// grid pattern.
	/// Will automatically execute image programs if needed.
	idImage *LoadFromFile(const char *name, textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage, cubeFiles_t cubeMap = CF_2D);

	/// look for a loaded image, whatever the parameters
	idImage *GetImage(const char *name) const;

	/// look for a loaded image, whatever the parameters
	idImage *GetImageWithParameters(const char *name, textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage, cubeFiles_t cubeMap) const;

	/// The callback will be issued immediately, and later if images are reloaded or vid_restart
	/// The callback function should call one of the idImage::Generate* functions to fill in the data
	idImage *ImageFromFunction(const char *name, void (*generatorFunction)(idImage *image));

	/// scratch images are for internal renderer use.  ScratchImage names should always begin with an underscore
	idImage *ScratchImage(const char *name, idImageOpts *imgOpts, textureFilter_t filter, textureRepeat_t repeat, textureUsage_t usage);

	/// purges all the images before a vid_restart
	void PurgeAllImages();

	/// reloads all apropriate images after a vid_restart
	void ReloadImages(bool all);

	/// unbind all textures from all texture units
	void UnbindAll();

	/// disable the active texture unit
	void BindNull();

	/// Called only by renderSystem::BeginLevelLoad
	void BeginLevelLoad();

	/// Called only by renderSystem::EndLevelLoad
	void EndLevelLoad();

	void Preload(const idPreloadManifest &manifest, const bool &mapPreload);

	/// Loads unloaded level images
	int LoadLevelImages(bool pacifier);

	/// used to clear and then write the dds conversion batch file
	void StartBuild();
	void FinishBuild(bool removeDups = false);

	void PrintMemInfo(MemInfo_t *mi);

	/// built-in images
	void CreateIntrinsicImages();
	
	idImage *defaultImage;
	idImage *flatNormalMap;   // 128 128 255 in all pixels
	idImage *alphaNotchImage; // 2x1 texture with just 1110 and 1111 with point sampling
	idImage *whiteImage;      // full of 0xff
	idImage *blackImage;      // full of 0x00
	idImage *noFalloffImage;  // all 255, but zero clamped
	idImage *fogImage;        // increasing alpha is denser fog
	idImage *fogEnterImage;   // adjust fogImage alpha based on terminator plane
	idImage *scratchImage;
	idImage *scratchImage2;
	idImage *accumImage;
	idImage *currentRenderImage;         // for SS_POST_PROCESS shaders
	idImage *currentDepthImage;          // for motion blur
	idImage *originalCurrentRenderImage; // currentRenderImage before any changes for stereo rendering
	idImage *loadingIconImage;           // loading icon must exist always
	idImage *hellLoadingIconImage;       // loading icon must exist always

	//--------------------------------------------------------

	idImage *AllocImage(const char *name);
	idImage *AllocStandaloneImage(const char *name);

	bool ExcludePreloadImage(const char *name);

	idList<idImage *, TAG_IDLIB_LIST_IMAGE> images;
	idHashIndex imageHash;

	bool insideLevelLoad{false};     // don't actually load images now
	bool preloadingMapImages{false}; // unless this is set
};

extern CImageManager *globalImages; // pointer to global list for the rest of the system