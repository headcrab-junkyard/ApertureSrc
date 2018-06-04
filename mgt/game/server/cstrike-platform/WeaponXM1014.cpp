/// @file

#include "BaseWeapon.hpp"

class CWeaponXM1014 final : public CBaseWeapon
{
public:
	CWeaponXM1014();
	~CWeaponXM1014();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponXM1014::CWeaponXM1014() = default;
CWeaponXM1014::~CWeaponXM1014() = default;

void CWeaponXM1014::OnSpawn()
{
};

void CWeaponXM1014::Attack()
{
};

void CWeaponXM1014::Reload()
{
};