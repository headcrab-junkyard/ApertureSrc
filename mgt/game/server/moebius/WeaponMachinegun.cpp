

void CWeaponNailgun::OnAttack()
{
	if(self.weapon == IT_NAILGUN)
		player_nail1();
};

void CWeaponSuperNailgun::OnAttack()
{
	if(self.weapon == IT_SUPER_NAILGUN)
		player_nail1();
};