/// @file

#include "BaseWeapon.hpp"

class CWeaponM3 final : public CBaseWeapon
{
public:
	CWeaponM3();
	~CWeaponM3();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponM3::CWeaponM3() = default;
CWeaponM3::~CWeaponM3() = default;

void CWeaponM3::OnSpawn()
{
};

void CWeaponM3::Attack()
{
};

void CWeaponM3::Reload()
{
};