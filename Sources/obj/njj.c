/* the new joker journal feb 92 
                  by jolly 
		  
*/


#include "/players/jolly/config.h"


#undef VERSION
#define VERSION "Version 2.o.15 ( Mar. o1 1992 )"


/************************ standard ******************************/


reset()      {
                if(!AGENCY) call_other(AGENCYPATH,"???"); /* if it's not loaded */
	     }

id(str)      {
	  	if ( str=="joker journal" || str=="journal" || str=="jj" ) return 1;
                return 0;
             }


short()      {
                return "A Joker Journal";
	     }

long()       { 
		return info();
	     }


taken()      { 
		if(!present("joker journal",POBJ))
                {
		  write("You have to take it first.\n");
                  return 0;
                }
                return 1;
             }


get()	     {  
		return 1;
	     }


drop(silent) {
		if(!drop_sell())
		{
#ifdef JJ_AUTOLOAD
		  if(query_verb()=="quit") return 1;
#else
                  if(query_verb()=="quit") return;
#endif
		  
		  write("Sorry, but members can't drop their last issue.\n"+
		        "If you wanna really drop it : type /quit\n");
		  return 1;
		}
	      }


#ifdef JJ_AUTOLOAD
query_auto_load() 
              {
		if(!drop_sell()) return JJPATHNAME+":"+AGENCY->channel_vars();
              }


init_arg(str) {
		AGENCY->inc_members(1,str);
                return 1;
              }
#endif

   
query_value() {
		if(!drop_sell()) return 0;
		return JJ_VALUE;
	      }




init()        {
                add_action("help"      ,"/help");
		add_action("info"      ,"/info");
		add_action("docs"      ,"/docs");
		add_action("seco"      ,"/second");
		add_action("jjco"      ,"/costs");
		add_action("clon"      ,"/joker");
		add_action("typo"      ,"/typo");

		add_action("subs"      ,"/subscribe");
		add_action("subs"      ,"/sub");
		add_action("quit"      ,"/quit");
	        add_action("memb"      ,"/members");
	        add_action("list"      ,"/list");

	        add_action("news"      ,"/news");
	        add_action("sell"      ,"/adv");
	        add_action("sell"      ,"/advertise");
	        add_action("chif"      ,"/chiffre");

	        add_action("join"      ,"/join");
                add_action("leav"      ,"/leave");
		add_action("reca"      ,"/recap");
		add_action("mesg"      ,"/");
		add_action("msg"       ,"/msg");

	        add_action("topi"      ,"/topic");
		add_action("clea"      ,"/clear");		
		add_action("invi"      ,"/invite");
	        add_action("priv"      ,"/private");
		add_action("oper"      ,"/op");
		add_action("thro"      ,"/throw");

		add_action("snoo"      ,"/snoop");
	      }


help()        {
		cat(JJPLYCMD);
                if(LEVEL>20) cat(JJWIZCMD);
		return 1;
	      }

info()        { 
		say(PNAM+" examines the Joker Journal.\n");
		write("The Joker Journal "+VERSION+"\n");                    
		cat(JJINFO);
		return 1;
	      }

docs()        {
		cat(JJEXPCMD);
		return 1;
	      }

seco()        {
		cat(JJSECOND);
		return 1;
	      }

jjco()	      {
                write("Costs for JJ-actions here in "+GAMENAME+"\n\n");
		write("Chatting        :"+JJ_CHATCOSTS+" Spellpoints\n");
		write("News            :"+JJ_CHATCOSTS+" Spellpoints\n");
		write("Messages        :"+JJ_CHATCOSTS+" Spellpoints\n");
		write("Subscribing     :"+JJ_SUBSCOSTS+" Goldcoins\n");
		write("Cloneing the JJ :"+JJ_CLONECOSTS+" Spellpoints "+JJ_VALUE*2+" Goldcoins\n");
		write("Advertising costs nothing until someone buys the item.\n");
		write(" But then it costs :"+JJ_SELLCOSTS+" Spellpoints ( the advertiser )\n");
		write("                   :"+JJ_SELLCOSTS+" Spellpoints ( the purchaser  )\n");
		return 1;
	      }

typo(str)     {
                write("Jolly thank's you for support.\n");
		ALOG("TYPO :"+str);
		return 1;
	      }


/****************** special routines ********************************/


drop_sell()   {
		int flag;
		object obj;
		if(!test_member()) return 1;
		flag=FALSE;
		obj=first_inventory(POBJ);
		while(obj)
		{
		  if(obj->id("joker journal") && obj!=this_object()) flag=TRUE;
		  if(obj->id("joker journal") && obj==this_object()) flag=FALSE;/*different from*/ 
		  obj=next_inventory(obj);					/*else !        */
		}
		return flag;
	      }
	      

test_member(a){
                if(a && !AGENCY->member(POBJ))
		{
		  write("You are not a subscriber of the Joker Journal.\n");
		  return;
	        }
		return AGENCY->member(POBJ);
	      }


clon(str)     { 
		object obj,whom;
		
		if(!test_member(TRUE)) return 1;
		if(str) whom=present(str,environment(POBJ));
		if(whom && !find_player(str)) whom=0;
		if(!whom)
		{
		  write("Sorry can't find such player here.\n");
		  return 1;
		}
		
		if(present("joker journal",whom))
		{
		  write("Sorry but the player got already one.\n");
		  return 1;
		}
if(!COST(JJ_VALUE*2)) return 1;

#ifdef JJ_CLONECOSTS
		if(POBJ->query_spell_points()<JJ_CLONECOSTS)
		{
		  POBJ->add_money(JJ_VALUE*2);
		  write("You don't have enough Spellpoints.\n");
		  return 1;
		}
		POBJ->restore_spell_points(-JJ_CLONECOSTS);
#endif

		obj=clone_object(JJPATHNAME);
		move_object(obj,whom);
		tell_object(whom,"A brand new Joker Journal appeared in your inventory.\n");
		write("Ok.\n");
		return 1;
	      }



/******************   member actions ********************************/

	      
	      
subs()	      {
		if(test_member()) 
		{
		  write("You are already a member.\n");
		  return 1;
		}
		if(!taken()) 
		{
		  write("You have to take it first.\n");
		  return 1;
		}
		if(COST(1)) AGENCY->inc_members();
		return 1;		 
              }


quit()	      {
		if(!test_member(TRUE)) return 1;
		AGENCY->leave_channel();
		AGENCY->dec_members(POBJ);
		write("You left the membership of the Joker Journal.\n");
		return 1;
              }


memb()	      { 
                if(test_member(TRUE)) AGENCY->list_members(POBJ);
		return 1;
              }


list()	      { 
                if(test_member(TRUE)) AGENCY->list_channels(POBJ);
		return 1;
              }


news(str)     {
		if(!test_member(TRUE)) return 1;
		if(!str)
		{
		  write("Where is your news ?\n");
		  return 1;
		}
		AGENCY->msg_channel(str,NEWS);
		return 1;
              }


sell(str)     {
                if(!test_member(TRUE)) return 1;
		if(!str)
		{
		   write("This is the new JJ you should type: /adv sword\n");
		   return 1;
		}
		AGENCY->adv_sell(str);
		return 1;
	      }


chif(str)     {
	        if(!test_member(TRUE)) return 1;
		if(!str)
		{
		   write("This is the new JJ you should type: /chiffre 32133\n");
		   return 1;
		}
		AGENCY->adv_buy(str);
		return 1;
	      }


join(str)     {
                if(!test_member(TRUE)) return 1;
		if(!str)
		{
		   write("Should I read your thoughts ?  type: /join news\n");
		   return 1;
		}
		AGENCY->join_channel(str);
		return 1;
	      }


leav(str)     {
                if(!test_member(TRUE)) return 1;
		AGENCY->leave_channel(str);
	        return 1;
	      }


reca(str)     {
		if(!test_member(TRUE)) return 1;
		AGENCY->recap_channel(str);
	        return 1;
	      }

mesg(str)     {
                if(!test_member(TRUE)) return 1;
		if(!str)
		{
		   write("Should I read your thoughts ?  type: / blafasel\n");
		   return 1;
		}
		AGENCY->msg_channel(str);
		return 1;
	      }

msg(str)      {
                string a,b;
		
		
		if(!str || sscanf(str,"%s %s",a,b)!=2)
		{
		  write("Syntax error : try /msg jolly blafasel\n");
		  return 1;
		}
		AGENCY->talk_channel(a,b);
		return 1;
	       }
	       
		



/****************** operator actions ********************************/




topi(str)     {
                if(!test_member(TRUE)) return 1;
		if(!str)
		{
		   write("Should I read your thoughts ?  type: /topic blafasel\n");
		   return 1;
		}

		AGENCY->topic_channel(str);
		return 1;
	      }

	      
clea()        {
                if(test_member(TRUE)) AGENCY->clear_channel();
		return 1;	      
	      }


priv()        {
                if(test_member(TRUE)) AGENCY->private_channel();
		return 1;
	      }


invi(str)     {
                if(test_member(TRUE)) AGENCY->invite_channel(str);
		return 1;
	      }


thro(str)     {
		if(test_member(TRUE)) AGENCY->throw_channel(str);
		return 1;
	      }

oper(str)     {
		if(test_member(TRUE)) AGENCY->oper_channel(str);
		return 1;
	      }


/******************   wizard actions ********************************/



snoo()        {
                if(LEVEL<20) return;
		if(test_member(TRUE)) AGENCY->snoop_channel();
		return 1;
	      }





















