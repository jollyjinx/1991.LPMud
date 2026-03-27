#include "room.h"
#undef EXTRA_RESET
#define EXTRA_RESET x_reset();
#define TOWER "/players/rakido/isle/TOWN/tower"
#define ROOM  "/players/rakido/isle/TOWN/middle_room"
#define tdir "/players/rakido/isle/TOWN/"


TWO_EXIT(	"/players/rakido/isle/TOWN/top.c"		,"up",
			"/players/rakido/isle/TOWN/tower.c"	,"down",
   			"in the resting room",
   "You stand in a small room which contains really nothing. You think this room\n"+
   "has the only purpose to make a little pause during the long way to the top.\n"+
   "The only interesting thing is a little window.\n"
   ,1)


x_reset()
{
	   items = ({"room","Really nothing interesting except the window" });
}

init()
{
::init();
 	add_action("lookd","ld");
   add_action("window","exa");
   add_action("window","examine");
   add_action("window","x");
}

window(str)
{
   if ( !str ) return 0;
  
   if (str != "window") return 0;
   write("Strange, really strange. It seems, that the window is built just under the\n");
   write("ceiling of the entrance hall. I think, you could take a look down to the hall.\n");
   return 1;
}

lookd(str)
{
      write("As you look down, you see this scene:\n\n");
      move_object(this_player(),TOWER);
      call_other(this_player(),"look");
      move_object(this_player(),ROOM);
      return 1;
}
