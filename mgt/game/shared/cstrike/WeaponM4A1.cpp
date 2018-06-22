/// @file

#include "BaseWeapon.hpp"

class CWeaponM4A1 final : public CBaseWeapon
{
public:
	CWeaponM4A1();
	~CWeaponM4A1();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponM4A1::CWeaponM4A1() = default;
CWeaponM4A1::~CWeaponM4A1() = default;

void CWeaponM4A1::OnSpawn()
{
};

void CWeaponM4A1::Attack()
{
};

void CWeaponM4A1::Reload()
{
};