#include "rpggame.h"

void rpgtrigger::update()
{
	//hack to make models passthrough. (eg, doors)
	if(flags & F_TRIGGERED)
		state = (lastmillis - lasttrigger < 750) ? CS_ALIVE : CS_DEAD;
	else
		state = (lastmillis - lasttrigger < 750) ? CS_DEAD : CS_ALIVE;
}

void rpgtrigger::render()
{
	if(flags & F_INVIS) return;

	rendermodel(temp.mdl, (flags & F_TRIGGERED) ? ANIM_TRIGGER : (lasttrigger - lastmillis > 1500 ? ANIM_MAPMODEL|ANIM_LOOP : ANIM_TRIGGER|ANIM_REVERSE), vec(o).sub(vec(0, 0, eyeheight)), yaw, pitch, roll, MDL_CULL_DIST|MDL_CULL_OCCLUDED, NULL, NULL, lasttrigger, 1500, temp.alpha);
}

void rpgtrigger::hit(rpgent *attacker, use_weapon *weapon, use_weapon *ammo, float mul, int flags, vec dir)
{
	loopv(weapon->effects)
	{
		if(!game::statuses.inrange(weapon->effects[i]->status)) continue;
		seffects.add(new victimeffect(attacker, weapon->effects[i], weapon->chargeflags, mul));
	}

	if(ammo) loopv(ammo->effects)
	{
		if(!game::statuses.inrange(ammo->effects[i]->status)) continue;
		seffects.add(new victimeffect(attacker, ammo->effects[i], weapon->chargeflags, mul));
	}

	getsignal("hit", false, attacker);
}

void rpgtrigger::init(int base)
{
	game::loadingrpgtrigger = this;
	rpgscript::config->setref(this, true);

	defformatstring(file)("%s/%i.cfg", game::datapath("triggers"), base);
	execfile(file);

	rpgscript::config->setnull(true);
	game::loadingrpgtrigger = NULL;
}
