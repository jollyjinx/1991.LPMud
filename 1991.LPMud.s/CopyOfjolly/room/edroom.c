/* Jollys edroom Version 1.0   12.July 1991 */

#include "room.h"


#define OBJ   this_player()
#define TEST call_other(obj,"query_real_name")
#define LEVEL call_other(this_player(),"query_level")
#undef EXTRA_RESET
#define EXTRA_RESET extra_reset(arg);


ONE_EXIT("players/jolly/workroom"     ,"west",


"Jolly's edroom",

"Jolly's edroom.\n"+
"",1)


init() {
		object mas;
::init();
               if(bin_drin()) {
		move_object( OBJ ,"players/jolly/workroom");
		tell_object( OBJ , "\nYou failed to enter Jolly's EDroom,\n"+
				  "cause Jolly is workin' with the famous ed.\n");
		mas = find_player("jolly");
		tell_object( mas,"");
		}
}                              


extra_reset(arg) {
			}

bin_drin() { object obj;
             int    flag;
             obj=first_inventory(this_object());
             flag=0;
             while(obj)
             {
             if(TEST=="jolly") flag=1;
             obj=next_inventory(obj);
             }
             obj=this_player();
             if(TEST=="jolly") flag=0;
             if(flag) return 1;
           }
