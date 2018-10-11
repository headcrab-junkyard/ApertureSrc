void CWeaponRocketLauncher::OnAttack()
{
	if(self.weapon == IT_ROCKET_LAUNCHER)
	{
		player_rocket1();
		self.attack_finished = time + 0.8f;
		W_FireRocket();
	};
};