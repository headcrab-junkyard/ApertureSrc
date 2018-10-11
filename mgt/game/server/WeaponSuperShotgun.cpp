void CWeaponSuperShotgun::OnAttack()
{
	if(self.weapon == IT_SUPER_SHOTGUN)
	{
		player_shot1();
		self.attack_finished = time + 0.7f;
		W_FireSuperShotgun();
	};
};