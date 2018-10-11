void CWeaponLightning::OnAttack()
{
	if(mpOwner->GetCurrentWeapon()->meType == IT_LIGHTNING)
	{
		self.attack_finished = time + 0.1f;
		gpEngine->pfnEmitSound(self, CHAN_AUTO, "weapons/lstart.wav", 1, ATTN_NORM);
		player_light1();
	};
};