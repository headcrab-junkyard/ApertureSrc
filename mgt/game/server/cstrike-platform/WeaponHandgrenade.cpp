/// @file

#include "BaseWeapon.hpp"

class CWeaponHegrenade final : public CBaseWeapon
{
public:
	CWeaponHegrenade();
	~CWeaponHegrenade();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponHegrenade::CWeaponHegrenade() = default;
CWeaponHegrenade::~CWeaponHegrenade() = default;

void CWeaponHegrenade::OnSpawn()
{
};

void CWeaponHegrenade::Attack()
{
};

void CWeaponHegrenade::Reload()
{
};