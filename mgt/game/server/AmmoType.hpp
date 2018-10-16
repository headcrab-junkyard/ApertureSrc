/// @file

#pragma once

class CAmmoType
{
public:
	CAmmoType(const tString &asName, int anDmgType)
		: msName(asName), mnDmgType(anDmgType){}
	~CAmmoType();
private:
	tString msName{""};
	int mnDmgType{0};
};