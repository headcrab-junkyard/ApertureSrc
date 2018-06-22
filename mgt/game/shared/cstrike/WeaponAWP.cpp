/// @file

#include "BaseWeapon.hpp"

class CWeaponAWP final : public CBaseWeapon
{
public:
	CWeaponAWP();
	~CWeaponAWP();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponAWP::CWeaponAWP() = default;
CWeaponAWP::~CWeaponAWP() = default;

void CWeaponAWP::OnSpawn()
{
};

void CWeaponAWP::Attack()
{
};

void CWeaponAWP::Reload()
{
};