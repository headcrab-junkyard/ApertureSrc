/// @file

#pragma once

static const int MAX_RENDER_CROPS = 8;

class idParallelJobList;

// TODO: previously known as trGlobals_t
/*
** Most renderer globals are defined here.
** backend functions should never modify any of these fields,
** but may read fields that aren't dynamically modified
** by the frontend.
*/
class CRenderSystemLocal : public idRenderSystem
{
public:
	// internal functions
	CRenderSystemLocal();
	~CRenderSystemLocal();
	
	void Clear();
	
	void GetCroppedViewport(idScreenRect *viewport);
	
	void PerformResolutionScaling(int &newWidth, int &newHeight);
	
	int GetFrameCount() const { return frameCount; };
	
	// external functions
	virtual void Init();
	virtual void Shutdown();
	virtual void ResetGuiModels();
	virtual void InitOpenGL();
	virtual void ShutdownOpenGL();
	virtual bool IsOpenGLRunning() const;
	virtual bool IsFullScreen() const;
	virtual stereo3DMode_t GetStereo3DMode() const;
	virtual bool HasQuadBufferSupport() const;
	virtual bool IsStereoScopicRenderingSupported() const;
	virtual stereo3DMode_t GetStereoScopicRenderingMode() const;
	virtual void EnableStereoScopicRendering(const stereo3DMode_t mode) const;
	virtual int GetWidth() const;
	virtual int GetHeight() const;
	virtual float GetPixelAspect() const;
	virtual float GetPhysicalScreenWidthInCentimeters() const;
	virtual idRenderWorld *AllocRenderWorld();
	virtual void FreeRenderWorld(idRenderWorld *rw);
	virtual void BeginLevelLoad();
	virtual void EndLevelLoad();
	virtual void LoadLevelImages();
	virtual void Preload(const idPreloadManifest &manifest, const char *mapName);
	virtual void BeginAutomaticBackgroundSwaps(autoRenderIconType_t icon = AUTORENDER_DEFAULTICON);
	virtual void EndAutomaticBackgroundSwaps();
	virtual bool AreAutomaticBackgroundSwapsRunning(autoRenderIconType_t *usingAlternateIcon = NULL) const;

	virtual idFont *RegisterFont(const char *fontName);
	virtual void ResetFonts();
	virtual void PrintMemInfo(MemInfo_t *mi);

	virtual void SetColor(const idVec4 &color);
	virtual uint32 GetColor();
	virtual void SetGLState(const uint64 glState);
	virtual void DrawFilled(const idVec4 &color, float x, float y, float w, float h);
	virtual void DrawStretchPic(float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial *material);
	virtual void DrawStretchPic(const idVec4 &topLeft, const idVec4 &topRight, const idVec4 &bottomRight, const idVec4 &bottomLeft, const idMaterial *material);
	virtual void DrawStretchTri(const idVec2 &p1, const idVec2 &p2, const idVec2 &p3, const idVec2 &t1, const idVec2 &t2, const idVec2 &t3, const idMaterial *material);
	virtual idDrawVert *AllocTris(int numVerts, const triIndex_t *indexes, int numIndexes, const idMaterial *material, const stereoDepthType_t stereoType = STEREO_DEPTH_TYPE_NONE);
	virtual void DrawSmallChar(int x, int y, int ch);
	virtual void DrawSmallStringExt(int x, int y, const char *string, const idVec4 &setColor, bool forceColor);
	virtual void DrawBigChar(int x, int y, int ch);
	virtual void DrawBigStringExt(int x, int y, const char *string, const idVec4 &setColor, bool forceColor);

	virtual void WriteDemoPics();
	virtual void DrawDemoPics();
	virtual const emptyCommand_t *SwapCommandBuffers(uint64 *frontEndMicroSec, uint64 *backEndMicroSec, uint64 *shadowMicroSec, uint64 *gpuMicroSec);

	virtual void SwapCommandBuffers_FinishRendering(uint64 *frontEndMicroSec, uint64 *backEndMicroSec, uint64 *shadowMicroSec, uint64 *gpuMicroSec);
	virtual const emptyCommand_t *SwapCommandBuffers_FinishCommandBuffers();

	virtual void RenderCommandBuffers(const emptyCommand_t *commandBuffers);
	virtual void TakeScreenshot(int width, int height, const char *fileName, int downSample, renderView_t *ref);
	virtual void CropRenderSize(int width, int height);
	virtual void CaptureRenderToImage(const char *imageName, bool clearColorAfterCopy = false);
	virtual void CaptureRenderToFile(const char *fileName, bool fixAlpha);
	virtual void UnCrop();
	virtual bool UploadImage(const char *imageName, const byte *data, int width, int height);
public:
	// renderer globals
	bool registered; // cleared at shutdown, set at InitOpenGL (beginRegistration)

	bool takingScreenshot;

	int frameCount; // incremented every frame
	int viewCount;  // incremented every view (twice a scene if subviewed)
	                // and every R_MarkFragments call

	float frameShaderTime; // shader time for all non-world 2D rendering

	CVec4 ambientLightVector; // used for "ambient bump mapping"

	CList<CRenderWorldLocal *> worlds;

	CRenderWorldLocal *primaryWorld;
	renderView_t primaryRenderView;
	viewDef_t *primaryView;
	// many console commands need to know which world they should operate on

	const idMaterial *whiteMaterial;
	const idMaterial *charSetMaterial;
	const idMaterial *defaultPointLight;
	const idMaterial *defaultProjectedLight;
	const idMaterial *defaultMaterial;
	idImage *testImage;
	idCinematic *testVideo;
	int testVideoStartTime;

	idImage *ambientCubeImage; // hack for testing dependent ambient lighting

	viewDef_t *viewDef;

	performanceCounters_t pc; // performance counters

	viewEntity_t identitySpace; // can use if we don't know viewDef->worldSpace is valid

	idScreenRect renderCrops[MAX_RENDER_CROPS];
	int currentRenderCrop;

	// GUI drawing variables for surface creation
	int guiRecursionLevel; // to prevent infinite overruns
	uint32 currentColorNativeBytesOrder;
	uint64 currentGLState;
	class idGuiModel *guiModel;

	idList<idFont *, TAG_FONT> fonts;

	unsigned short gammaTable[256]; // brightness / gamma modify this

	srfTriangles_t *unitSquareTriangles;
	srfTriangles_t *zeroOneCubeTriangles;
	srfTriangles_t *testImageTriangles;

	// these are allocated at buffer swap time, but
	// the back end should only use the ones in the backEnd stucture,
	// which are copied over from the frame that was just swapped.
	drawSurf_t unitSquareSurface_;
	drawSurf_t zeroOneCubeSurface_;
	drawSurf_t testImageSurface_;

	idParallelJobList *frontEndJobList;

	unsigned timerQueryId; // for GL_TIME_ELAPSED_EXT queries
};

extern CRenderSystemLocal tr;