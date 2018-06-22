/// @file

#include "BaseWeapon.hpp"

class CWeaponKnife final : public CBaseWeapon
{
public:
	CWeaponKnife();
	~CWeaponKnife();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponKnife::CWeaponKnife() = default;
CWeaponKnife::~CWeaponKnife() = default;

void CWeaponKnife::OnSpawn()
{
};

void CWeaponKnife::Attack()
{
};

void CWeaponKnife::Reload()
{
};