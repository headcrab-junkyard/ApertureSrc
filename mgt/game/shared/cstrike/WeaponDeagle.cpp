/// @file

#include "BaseWeapon.hpp"

class CWeaponDeagle final : public CBaseWeapon
{
public:
	CWeaponDeagle();
	~CWeaponDeagle();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponDeagle::CWeaponDeagle() = default;
CWeaponDeagle::~CWeaponDeagle() = default;

void CWeaponDeagle::OnSpawn()
{
};

void CWeaponDeagle::Attack()
{
};

void CWeaponDeagle::Reload()
{
};