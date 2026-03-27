/* config.h file used in some of my files
				     Jolly
*/

#include "./mud.h"


#ifdef  NeXTWORLD
#define GAMENAME "NeXTWorld"
#define LEVEL_WIZARD 21
#define AGENCYEXITA "/players/jolly/workroom"
#define AGENCYEXITB "east"

#define JJ_VALUE 10
#define JJ_SUBSCOSTS 10
#define JJ_SELLCOSTS 17
#define JJ_CHATCOSTS 3
#define JJ_CLONECOSTS 20
#define JJ_NO_DRINKS
#define JJ_NO_KEYS
#undef JJ_NO_VALUELESS
#undef JJ_AUTOLOAD
#endif
#ifdef FH_MUD
#define GAMENAME "FH-mud"
#define LEVEL_WIZARD 21
#define AGENCYEXITA "/room/forest1"
#define AGENCYEXITB "south"

#define JJ_VALUE 10
#define JJ_SUBSCOSTS 10
#define JJ_SELLCOSTS 5
#define JJ_CHATCOSTS 0
#define JJ_CLONECOSTS 20
#define JJ_NO_DRINKS
#define JJ_NO_KEYS
#undef JJ_NO_VALUELESS
#define JJ_AUTOLOAD
#endif               


#ifdef  NEMESIS
#define GAMENAME "Nemesis"
#include "levels.h"
#define AGENCYEXITA "/players/junky/room/north2"
#define AGENCYEXITB "east"

#define JJ_VALUE 10
#define JJ_SUBSCOSTS 10
#define JJ_SELLCOSTS 17
#define JJ_CHATCOSTS 3
#define JJ_CLONECOSTS 20
#define JJ_NO_DRINKS
#define JJ_NO_KEYS
#undef JJ_NO_VALUELESS
#undef JJ_AUTOLOAD
#endif


#ifdef  PADERMUD
#define GAMENAME "Padermud"
#define LEVEL_WIZARD 21
#define AGENCYEXITA "/room/forest1"
#define AGENCYEXITB "south"

#define JJ_VALUE 10
#define JJ_SUBSCOSTS 1
#define JJ_SELLCOSTS 0
#define JJ_CHATCOSTS 0
#define JJ_CLONECOSTS 0
#undef JJ_NO_DRINKS
#undef JJ_NO_KEYS
#undef JJ_NO_VALUELESS
#define JJ_AUTOLOAD
#endif


#define FALSE 0
#define TRUE 1
#define NEWS 1                                   

#define JJPATHNAME "/players/jolly/obj/jj.c"
#define JJINFO "/players/jolly/text/jjinfo.txt"
#define JJWIZCMD "/players/jolly/text/jjwizcmd.txt"
#define JJPLYCMD "/players/jolly/text/jjplycmd.txt"
#define JJEXPCMD "/players/jolly/text/jjexpcmd.txt"
#define JJSECOND "/players/jolly/text/jjsecond.txt"


#define AGENCY "/players/jolly/room/npa.c"
#define AGENCYPATH "/players/jolly/room/npa.c"
#define MAXHISTORY 10
#define MAXCHANNELS 7


#define POBJ this_player()
#define PNAM call_other(this_player(),"query_real_name")
#define LEVEL call_other(this_player(),"query_level")
#define MAXPLAYERS 50
#define SPACE "                                        "


/*
    LEVEL()    {
        if(!environment(this_object()) ) return this_player()->query_level();
              if( environment(this_object())->query_level() &&
                 !environment(this_object())->query_npc() )
              return (environment(this_object())->query_level());
              return POBJ->query_level();
           }
*/


COST(a)    {
              if(POBJ->query_money()<a)
	      {
                write("You don't have so much money.\n");
		return;
	      }
	      POBJ->add_money(-a);
	      return 1;
	   }


ALOG(str)  {
              if(!str) return;
              write_file("/players/jolly/logfile",
	           ctime(time())+" "+PNAM+" Lvl:"+LEVEL+" Coins:"+POBJ->query_money()+"\n"+
		   "  "+str+"\n");
              return 1;
           }

LALIGN(str,in)
           {  return extract(str+SPACE+SPACE,0,in-1);}

RALIGN(str,in)
           {
              str=SPACE+SPACE+str;
              return extract(str,strlen(str)-in-1,strlen(str)-1);
           }


