/// @file

#pragma once

#include "CommonTypes.hpp"

interface ISystem;
interface ICmdArgs;

class CCmdBuffer;

enum cmd_source_t : int;

class CCmdProcessor
{
public:
	CCmdProcessor(ISystem *apSystem, CCmdBuffer *apBuffer);
	
	/// Parses a single line of text into arguments and tries to execute it.
	/// The text can come from the command buffer, a remote client, or stdin
	void ExecuteString(const char *asText, cmd_source_t aeCmdSrc);
	
	const ICmdArgs &GetCurrentCmdArgs() const; // for backwards compat with legacy code
private:
	ISystem *mpSystem{nullptr};
	CCmdBuffer *mpBuffer{nullptr};
	
	ICmdArgs *mpCurCmdArgs{nullptr};
};