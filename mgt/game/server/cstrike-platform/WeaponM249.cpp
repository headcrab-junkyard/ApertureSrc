/// @file

#include "BaseWeapon.hpp"

class CWeaponM249 final : public CBaseWeapon
{
public:
	CWeaponM249();
	~CWeaponM249();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponM249::CWeaponM249() = default;
CWeaponM249::~CWeaponM249() = default;

void CWeaponM249::OnSpawn()
{
};

void CWeaponM249::Attack()
{
};

void CWeaponM249::Reload()
{
};