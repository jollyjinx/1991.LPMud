/* Jolly's workroommonster */

inherit "obj/monster";     /* 'inherit' is different from '#include' */
object owner;
object bgs;
string nowner;

reset(arg) {
  object ob;

  ::reset(arg);  /* Always remember to call the redefined function of the
                    inherited object so it can perform necessary operations*/
                        
  if ( arg )  return;     /* We only have to set the monster up once */

  /* As we have inherited the standard monster
     we can call the functions directly.       */
  set_name("bodyguard");
  set_alias("guard");
  set_race("monster");
  set_short("Jolly's bodyguard is here");
  set_long ("He's very angry, cause you are not in your own workroom.\n");
  set_level(19);
  set_male();

 }
 
but(ob1,ob2){owner=ob1; 
             bgs=ob2;
	nowner=call_other(owner,"query_real_name");
	set_short(nowner+"'s bodyguard is here");
return 1;}

staff() {
	if(present(owner)) return;
	say("The bodyguard of "+nowner+" followed his master.\n");
	move_object(this_object(),environment(owner));
	say(nowner+"'s bodyguard comes in.\n");
}
second_life()
{
::second_life();
bgs->died();
}
