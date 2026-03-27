#include "room.h"

#undef  EXTRA_RESET
#define EXTRA_RESET extra_reset(arg);

ONE_EXIT("players/jolly/workroom","east",
         "Jolly's new workroom",
         "You can see nothing but dust on the floor.\n"+
         "",1)

extra_reset(arg) {
  object ob;
  if ( !present("herman munster") ) {
  ob = clone_object("obj/monster");
    /* The parameters must be set in the correct order because some of them
       affect others as well (eg. set_short() sets the long description too). */
    ob->set_name    ("herman munster");
    ob->set_alias   ("herman"        );  /* These four strings are checked */
    ob->set_alt_name("munster"       );  /* in the id() of the monster.    */
    ob->set_race    ("monster"       );
    ob->set_short("The well known herman munster");
    ob->set_long ("You see jolly is watching you from inside herman.\n");
    ob->set_level(5);
    ob->set_hp(100);
    ob->set_ep(20000);
    ob->set_al(-200);
    ob->set_wc(10);
    ob->set_ac(6);
    ob->set_male();
    ob->set_move_at_reset(); /* war eingeklammert !*/
    ob->set_aggressive(0);
    ob->set_chance     (50);  /* chance for spells (50% is very much) */
    ob->set_spell_dam  (20);
    ob->set_spell_mess1("Herman makes you crying."       );
               /* Spell message for the other players ('\n' will be appended) */
    ob->set_spell_mess2("You hear yourself crying.");
               /* Spell message for the attacker      ('\n' will be appended) */
    /* no ob->set_frog();   */
          ob->set_whimpy(); /*war auch eingeklammert*/
    ob->set_random_pick(70);  /* kleptomaniac =:-) */
    ob->load_chat(10,({"Herman says : Hey ! Jolly said U go out of this room ! .\n",
                       "Herman forces u to go.\n"}));
    ob->load_a_chat(10,({"Yell 1.\n",
                         "Yell 2.\n"}));
    /* no set_match, set_dead_ob, set_init_ob */
    move_object(ob,this_object());
  }
}
