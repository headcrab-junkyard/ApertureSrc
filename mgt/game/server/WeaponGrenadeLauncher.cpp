void CWeaponGrenadeLauncher::OnAttack()
{
	if(self.weapon == IT_GRENADE_LAUNCHER)
	{
		player_rocket1();
		self.attack_finished = time + 0.6f;
		W_FireGrenade();
	};
};