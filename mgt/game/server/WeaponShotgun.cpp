void CWeaponShotgun::OnAttack()
{
	if(self.weapon == IT_SHOTGUN)
	{
		player_shot1();
		self.attack_finished = time + 0.5f;
		W_FireShotgun();
	};
};