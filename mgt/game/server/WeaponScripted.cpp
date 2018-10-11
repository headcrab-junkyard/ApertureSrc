/// @file
/// @brief scripted weapon that uses a script file to implement its logic

#include "BaseWeapon.hpp"

class CWeaponScripted final : public CBaseWeapon
{
public:
	CWeaponScripted();
	~CWeaponScripted();

	void OnSpawn() override;

	void Attack() override;

	void Reload() override;

private:
	IScriptObject *mpScript{ nullptr };
};

CWeaponScripted::CWeaponScripted() = default;
CWeaponScripted::~CWeaponScripted() = default;

void CWeaponScripted::OnSpawn()
{
	mpScript->CallFunc("OnSpawn");
};

void CWeaponScripted::Attack()
{
	mpScript->CallFunc("Attack");
};

void CWeaponScripted::Reload()
{
	mpScript->CallFunc("Reload");
};