/// @file

#include "BaseWeapon.hpp"

class CWeaponAK47 final : public CBaseWeapon
{
public:
	CWeaponAK47();
	~CWeaponAK47();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponAK47::CWeaponAK47() = default;
CWeaponAK47::~CWeaponAK47() = default;

void CWeaponAK47::OnSpawn()
{
};

void CWeaponAK47::Attack()
{
};

void CWeaponAK47::Reload()
{
};