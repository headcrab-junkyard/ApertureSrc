/// @file

#include "BaseWeapon.hpp"

class CWeaponMP5 final : public CBaseWeapon
{
public:
	CWeaponMP5();
	~CWeaponMP5();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponMP5::CWeaponMP5() = default;
CWeaponMP5::~CWeaponMP5() = default;

void CWeaponMP5::OnSpawn()
{
};

void CWeaponMP5::Attack()
{
};

void CWeaponMP5::Reload()
{
};