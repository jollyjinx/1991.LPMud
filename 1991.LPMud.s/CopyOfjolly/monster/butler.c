/* Jolly's workroommonster */

inherit "obj/monster";     /* 'inherit' is different from '#include' */

reset(arg) {
  object ob;

  ::reset(arg);  /* Always remember to call the redefined function of the
                    inherited object so it can perform necessary operations*/
                        
  if ( arg )  return;     /* We only have to set the monster up once */

  /* As we have inherited the standard monster
     we can call the functions directly.       */
  set_name("butler");
  set_race("monster");
  set_short("Jolly's butler is here");
  set_long ("He's very angry, cause you are not in your own workroom.\n");
  set_level(19);
  set_male();

  ob = clone_object("obj/treasure");
  ob->set_name("instruction book");
  ob->set_alias("book");
  ob->set_short("Jolly's instruction book");
  ob->set_long("The book reads : Jolly will be very angry,\n"+
               "                 cause u killed his butler.\n");
  ob->set_class(12);
  ob->set_weight(2);
  ob->set_value(999);
  move_object( ob , this_object() );        /* Give book to butler.    */
  command("wield book",this_object() );
 }
