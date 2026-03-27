/* 
     Nemesis Press Agency      (farouls branch)   o2. Oktober 1991
                             by Jolly alias Pat  
*/
      


#include "room.h"

#define PLAYERNAME call_other(this_player(),"query_name")
#undef  EXTRA_RESET
#define EXTRA_RESET extra_reset(arg);






/***************** roomverwaltung ******************/


ONE_EXIT("players/faroul/shopping/road1"     ,"east",

         "npa (farouls branch)",

         "Nemesis Press Agency. (farouls branch) \n"+
         "A sign reads : Issues of the famous Joker Journal\n"+
         "               will cost you only 20 coins !\n"+
         "               Type : gimme\n"+
         "",1)

extra_reset(arg) { if(arg) return;
                   items =({"sign","U should know what it reads !"});
                 }

init() { ::init();
         add_action("give","gimme");
       }                              

give() { object ob;
         if( call_other(this_player(),"query_money") < 20 ) return 1;
         if( present("joker journal",this_player()) )
         {  write("You cannot buy two journals.\n");
            return 1;  
         }
         call_other(this_player(),"add_money",-20);
         ob = clone_object("players/jolly/obj/jj");         
         move_object(ob,this_player());             
         write("You have now the latest issue of the Joker - Journal\n");
         say( (this_player()->query_name())+" owns now a Joker-Journal\n");
         return 1;
       }





