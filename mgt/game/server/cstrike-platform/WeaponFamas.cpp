/// @file

#include "BaseWeapon.hpp"

class CWeaponFamas final : public CBaseWeapon
{
public:
	CWeaponFamas();
	~CWeaponFamas();
	
	void OnSpawn() override;
	
	void Attack() override;
	
	void Reload() override;
private:
};

CWeaponFamas::CWeaponFamas() = default;
CWeaponFamas::~CWeaponFamas() = default;

void CWeaponFamas::OnSpawn()
{
};

void CWeaponFamas::Attack()
{
};

void CWeaponFamas::Reload()
{
};