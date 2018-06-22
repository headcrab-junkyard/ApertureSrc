/// @file

#include "BaseWeapon.hpp"

class CWeaponUSP final : public CBaseWeapon
{
public:
	CWeaponUSP();
	~CWeaponUSP();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponUSP::CWeaponUSP() = default;
CWeaponUSP::~CWeaponUSP() = default;

void CWeaponUSP::OnSpawn()
{
};

void CWeaponUSP::Attack()
{
};

void CWeaponUSP::Reload()
{
};