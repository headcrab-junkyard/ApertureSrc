/// @file

#include "BaseWeapon.hpp"

class CWeaponGlock final : public CBaseWeapon
{
public:
	CWeaponGlock();
	~CWeaponGlock();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponGlock::CWeaponGlock() = default;
CWeaponGlock::~CWeaponGlock() = default;

void CWeaponGlock::OnSpawn()
{
};

void CWeaponGlock::Attack()
{
};

void CWeaponGlock::Reload()
{
};