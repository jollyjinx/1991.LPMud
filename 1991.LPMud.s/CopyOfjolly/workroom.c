 /* Jollys workroom Version 2.0   12.July 1991 */

#include "room.h"

#define TEST2 call_other(this_player(),"query_real_name")
#define TEST  call_other(this_player(),"query_name")
#define LEVEL call_other(this_player(),"query_level")
#undef  EXTRA_RESET
#define EXTRA_RESET extra_reset(arg);

object monster;
FOUR_EXIT("players/ryuken/workroom"     ,"south",
          "players/jolly/room/npa"       ,"west",
          "room/shop"                   ,"north",
          "players/jolly/room/edroom"    ,"east",

"Jolly's workroom",

"Jolly's workroom.\n"+
"First of all you can see a large sign reading: Beware of working Jolly !\n"+
"                                               Don't touch anything    !\n"+
"Near the sign you can see the skeletons of illiterates.\n"+
"Above your head is a camera observing you. A large board\n"+
"is fixed at the eastern wall, and a blackboard at the north-\n"+
"ern wall. A desk is standing near the window at the western\n"+ 
"wall.\n"+
"",1)


init() {
           string str;
::init();
     if((TEST!="jolly") && (TEST2!="jolly") ) {
     if(!bin_da()) {
         object jo;
         jo=find_player("jolly");
         if (jo) { 
               if (file_name(environment(jo))!="players/jolly/room/edroom"){
                   str=call_other(this_player(),"short");
        if(str) tell_object( jo ,"Master ! "+TEST+" entered your workroom !\n");
        if(!str) tell_object(jo,"Master ! "+TEST2+" entered your wk(invis).\n");
         }
      else tell_object(jo,"");
       }
    }
     }
}                              




extra_reset(arg) {
   

if ( !arg ) {  items = ({"sign"       ,"U should know what it reads",   
                         "large sign" ,"U should know what it reads" ,
                         "skeletons"  ,"They have only broken bones",
                         "bones"      ,"They're all broken",
                         "camera"     ,"It's a Sony ",
                         "blackboard"      ,"It's full of chalk",
                         "board"        ,"It's crowded with blueprints",
                         "chalk"      ,"It's from Dover",
                         "desk"       ,"Documents are lying on the desk",
                         "documents"  ,"They are crypted",
                         "window"     ,"It's a Sander-Sprossen-Fenster"                             });
            }


  



      /* Again we have to test the presence of the monster */
if ( !(monster) || !present("butler") ) {

     monster = clone_object("players/jolly/monster/butler");
     move_object( monster , this_object() );
  }


}

bin_da() { object obj;
           int    flag;
           obj=first_inventory(this_object());
           while(obj)
           {
           if( (obj->query_real_name())=="jolly") flag=1;
           obj=next_inventory(obj);
           }
           return flag;
         }
