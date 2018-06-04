/// @file

#include "BaseWeapon.hpp"

class CWeaponGalil final : public CBaseWeapon
{
public:
	CWeaponGalil();
	~CWeaponGalil();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponGalil::CWeaponGalil() = default;
CWeaponGalil::~CWeaponGalil() = default;

void CWeaponGalil::OnSpawn()
{
};

void CWeaponGalil::Attack()
{
};

void CWeaponGalil::Reload()
{
};