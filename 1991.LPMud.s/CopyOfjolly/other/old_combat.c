/* Old combat system, extracted from living.c,
 * modified a bit in its functionality.  -lynx
 */

/* These variables do not get stored after quit (static) -lynx */
static object attacker_ob;	/* Name of player attacking us. */
static object alt_attacker_ob;	/* Name of other player also attacking us. */
static object hunted, hunter;	/* used in the hunt mode */
#ifdef HUNT_TIME
static int hunting_time;
#endif

/* this is called from reset at creation of living */
init_combat_vars() {}

#ifdef HUNT_TIME
update_hunt() {
    hunting_time -= 1;
    if (hunting_time == 0) stop_hunting_mode();
}
#endif

leave_room() {
#ifdef NEW_STATS
    if (!can_flee()) return 0;
#endif
    if (attacker_ob && present(attacker_ob)) {
#ifdef HUNT_TIME
	hunting_time = HUNT_TIME;
#endif
	hunter = attacker_ob;
        attacker_ob=0;
    }
    else if (alt_attacker_ob && present(alt_attacker_ob)) {
#ifdef HUNT_TIME
	hunting_time = HUNT_TIME;
#endif
        hunter = alt_attacker_ob;
        alt_attacker_ob=0;
    }
#ifdef HUNT_TIME
    if (hunter && hunting_time == HUNT_TIME)
	TELL_ME("You are now hunted by " +
		call_other(hunter, "query_name", 0) + ".\n");
#endif
    return 1;
}

enter_room() {
    check_hunted();
    if (hunter && present(hunter)) {
        call_other(hunter, "attack_object", this_object());
        hunter = 0;
    }
}

check_hunted() { if (hunted && present(hunted)) attack_object(hunted); }

stop_fight() {
	attacker_ob = alt_attacker_ob;
	alt_attacker_ob = 0;
	stop_hunting_mode();
}

stop_hunting_mode() {
	if (hunted) {
		/* this looks paranoid, but it seems to happen sometimes */
		if (objectp(hunted) && objectp(me))
		    TELL_ME("You stop hunting "+hunted->stop_hunter()+".\n");
		hunted = 0; /* hunter = 0; */
		return 1;
	}
}

stop_hunter() {
    hunter = 0;
    TELL_ME("You are no longer hunted.\n");
    return cap_name;	/* one call_other less, *grin* */
}

killer_gift(align, exp) {
	write("You kill "+short()+".\n");
	if(attacker_ob) {
		call_other(attacker_ob, "add_alignment", align);
		call_other(attacker_ob, "add_exp", exp);
#ifdef MONITOR_MONSTER_KILLS
		MON(name+" killed by "+attacker_ob->query_name())
#endif
	}
}

/*
 * This routine is called when we are attacked by a player.
 * changes by Kiri (real alt_attacker)
 */
attacked_by(ob) {
    if (!attacker_ob) {
	attacker_ob = ob;
	set_heart_beat(1);
	return;
    }
#if 0
    if (!alt_attacker_ob && ob != attacker_ob) {
#endif
    if (ob != attacker_ob) {
	alt_attacker_ob = attacker_ob;
	attacker_ob = ob;
    }
}

damage_done(by, damage) { attacked_by(by); }

show_enemies() {
    if (attacker_ob)
	write("Attacked by:\t" + call_other(attacker_ob, "query_name")+"\n");
    if (alt_attacker_ob)
	write("Alt-Attack:\t" + call_other(alt_attacker_ob, "query_name")+"\n");
}

/*
 * Return true if there still is a fight.
 * changes by Kiri (real alt_attacker)
 */
attack() {
    int tmp, whit, num;
    string name_of_attacker, name_of_alt_attacker;

    if (environment()) no_fight_room=environment()->no_fight();
    if (no_fight_room) {
      stop_fight();
      return 0; }
    if ((!attacker_ob && !alt_attacker_ob) || no_fight_room) {
	spell_cost = 0;
	return 0;
    }
    if ((alt_attacker_ob && !random(3)) || !attacker_ob) {
	object tmp_ob;

	tmp_ob = attacker_ob;
	attacker_ob = alt_attacker_ob;
	alt_attacker_ob = tmp_ob;
    }
    name_of_attacker = call_other(attacker_ob, "query_name", 0);
    if (alt_attacker_ob)
	name_of_alt_attacker = call_other(alt_attacker_ob, "query_name", 0);
    if (!name_of_attacker || attacker_ob->query_ghost() ||
	attacker_ob == this_object() ||
	environment(attacker_ob) != environment()) {
	if (/* !hunted && */ name_of_attacker &&
	    !call_other(attacker_ob, "query_ghost", 0)) {
#ifdef HUNT_TIME
	    TELL_ME("You are now hunting " +
			call_other(attacker_ob, "query_name", 0) + ".\n");
	    hunting_time = HUNT_TIME;
#endif
	    hunted = attacker_ob;
	}
	attacker_ob = 0;
	if (!alt_attacker_ob) return 0;
	attacker_ob = alt_attacker_ob;
	alt_attacker_ob = 0;
	name_of_attacker = name_of_alt_attacker;
	if (!name_of_attacker || attacker_ob->query_ghost() ||
	    attacker_ob == this_object() ||
	    environment(attacker_ob) != environment(this_object())) {
	    if (/* !hunter && */ name_of_attacker &&
		!call_other(attacker_ob, "query_ghost", 0)) {
#ifdef HUNT_TIME
		TELL_ME("You are now hunting " +
			call_other(attacker_ob, "query_name", 0) + ".\n");
		hunting_time = HUNT_TIME;
#endif
		hunted = attacker_ob;
	    }
	    attacker_ob = 0;
	    return 0;
	}
    }
    if (alt_attacker_ob && (!name_of_alt_attacker ||
		name_of_alt_attacker == NAME_OF_GHOST ||
		alt_attacker_ob == this_object() ||
		environment(alt_attacker_ob) != environment(this_object())))
	alt_attacker_ob = 0;
    if (!attacker_ob && !alt_attacker_ob) return 0;
    if (spell_cost) {
	sp -= spell_cost;
	tell_object(attacker_ob, "You are hit by a "+spell_name+".\n");
	write("You cast a "+spell_name+".\n");
    }
    if(weapon) {
	whit = call_other(weapon,"hit",attacker_ob);
#ifdef MULTI_HIT_WEAPONS
	if(whit=="end") return;
#else
	if (!attacker_ob) {
	    TELL_ME("CRACK!\nYour weapon broke!\n");
	    log_file("BAD_SWORD", weapon->short() + ", " +
		     creator(weapon) + " XX !\n");
	    spell_cost = 0;
	    spell_dam = 0;
	    destruct(weapon);
	    weapon_class = 0;
	    return 1;
	}
#endif
    }
    if(whit != "miss") {
		/* No calculation change for Dex in new stat systems -lynx */
	tmp = ((weapon_class + whit) * 2 + Dex) / 3;
	if (tmp == 0)
	    tmp = 1;
	tmp = call_other(attacker_ob, "hit_player", random(tmp) + spell_dam);
    } else
	tmp = 0;
    tmp -= spell_dam;
    if (!is_npc && weapon && tmp > 20 &&
      random(100) < weapon_class - level * 2 / 3 - 14) {
	TELL_ME("CRACK!\nYour weapon broke!\n" +
		"You are too unexperienced for such a weapon.\n");
	log_file("BAD_SWORD", weapon->short() + ", " +
		 creator(weapon) + "\n");
	spell_cost = 0;
	spell_dam = 0;
	destruct(weapon);
	weapon_class = 0;
	return 1;
    }
    tmp += spell_dam;
    if (tmp == 0) {
	TELL_ME("You miss.\n");
	say(cap_name+" misses "+name_of_attacker+".\n");
	spell_cost = 0;
	spell_dam = 0;
	return 1;
    }
    experience += tmp;
    tmp -= spell_dam;
    spell_cost = 0;
    spell_dam = 0;
    /* Does the enemy still live ? */
    if (!attacker_ob || attacker_ob->query_ghost()) {
	/* TELL_ME("You kill "+name_of_attacker+".\n"); */
	attacker_ob = alt_attacker_ob;
	alt_attacker_ob = 0;
	if (attacker_ob) return 1;
    } else {
#ifdef COMBAT_D
	/* New combat message system, -lynx */
	if (weapon) num=weapon->query_type();
	COMBAT_D->combat_message(tmp, attacker_ob, name_of_attacker, cap_name,
	num); /* Variable combat messages - junky */
#else
	/* Attack version by Lynx */
	string how, what, he;
	how = " to small fragments";
	what = "massacre";
	he = "s";
	if (tmp < 25) {
	    how = " with a bone crushing sound";
	    what = "smash";
	    he = "es";
	}
	if (tmp < 20) {
	    how = " very hard";
	    what = "smash";
	    he = "es";
	}
	if (tmp < 15) {
	    how = " very hard";
	    what = "hit";
	    he = "s";
	}
	if (tmp < 10) {
	    how = " hard";
	    what = "hit";
	    he = "s";
	}
	if (tmp < 7) {
	    how = " in the stomach";
	    what = "punch";
	    he = "es";
	}
	if (tmp < 5) {
	    how = "";
	    what = "hit";
	    he = "s";
	}
	if (tmp < 3) {
	    how = "";
	    what = "graze";
	    he = "s";
	}
	if (tmp == 1) {
	    how = "";
	    what = "tickle";
	    he = "s";
	}
	TELL_ME("You " + what + " " + name_of_attacker +
		    how + ".\n");
	tell_object(attacker_ob, cap_name + " " + what+he + " you" + how +
		    ".\n");
	say(cap_name + " " + what+he + " " + name_of_attacker + how +
		    ".\n", attacker_ob);
#endif
	return 1;
    }
}

query_attack() { return attacker_ob; }

attack_object(ob) {
    if (call_other(ob, "query_ghost", 0)) return;
    set_heart_beat(1);	/* For monsters, start the heart beat */
    if (attacker_ob == ob) {
	return;
    }
    if (alt_attacker_ob == ob) {
	alt_attacker_ob = attacker_ob;
	attacker_ob = ob;
	return;
    }
    if (!alt_attacker_ob && attacker_ob)
	alt_attacker_ob = attacker_ob;
    attacker_ob = ob;
	/* Snake's multikill fix equivalent */
    if (ob->query_attack() == this_object()) return;
    call_other(attacker_ob, "attacked_by", this_object());
    attack();
    return 1;
}

query_hunted() { return hunted; }
query_hunter() { return hunter; }

#ifdef NEW_STATS
can_flee() {
	if (attacker_ob && present(attacker_ob))
		if (hp > FLEE_THRESHOLD &&
				attacker_ob->query_flex() > query_flex()+17) {
			TELL_ME(attacker_ob->query_name()+
				" blocks your way!\n");
			return;
		}
		else TELL_ME("You manage to escape.\n\n");
	return 1;
}
#endif
