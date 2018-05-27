/// @file

#include "quakedef.h"
#include "network/INetwork.hpp"

class CNetworkNull final : public INetwork
{
public:
	CNetworkNull();
	~CNetworkNull();

	bool Init() override { return true; }
	void Shutdown() override {}
};