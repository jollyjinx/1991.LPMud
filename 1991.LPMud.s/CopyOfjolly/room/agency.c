/*   agency mar. o1 1992
     by Jolly alias Pat
*/
      
#include "room.h"
#include "/players/jolly/config.h"

#undef  EXTRA_RESET
#define EXTRA_RESET extra_reset(arg);

#undef VERSION
#define VERSION "Version 2.o.o3 ( Mar. o1 1992)"


static int     mcounter ;  
static object  membero  ;  /* member object */
static string  memberm  ;  /* member memory */


static string  history  ;
static int     hist_length;
static string  topic    ;
static string  chname   ;
static string  chbusy   ;


static object splayer;
static object sitem;
static int svalue;
static int chiffre;

static object item;
static int    fprice;
static int    ccounter;

/***************** roomverwaltung ******************/


ONE_EXIT(
		AGENCYEXITA,AGENCYEXITB,
		GAMENAME+" Press Agency (entre')",

		GAMENAME+" Press Agency.\n"+
		"A sign reads : Issues of the famous Joker Journal\n"+
		"               will cost you only 20 coins !\n"+
		"               Type : gimme\n"+
		"",1)


extra_reset(arg)
	      { 
		if(arg) return;
		items =({"sign","U should know what it reads !"});
	
		membero=allocate(MAXPLAYERS);
		memberm=allocate(MAXPLAYERS);
		

		splayer=allocate(MAXHISTORY);
		sitem  =allocate(MAXHISTORY);
		svalue =allocate(MAXHISTORY);
		chiffre=allocate(MAXHISTORY);
		ccounter=171;


		chname =allocate(MAXCHANNELS);
		history=allocate(MAXCHANNELS);
		topic  =allocate(MAXCHANNELS);
		
		hist_length= allocate(MAXCHANNELS);
		history[1] = allocate(MAXHISTORY);   			/* Newschannel */
		history[2] = allocate(MAXHISTORY);   			/* Advertising */
		
		chbusy ="";
		chname[0]=LALIGN("Snoop",5);
		chbusy=set_bit(chbusy,0);
		topic[0]="Snoopin all channels is funny!";
		chname[1]=LALIGN("News",5);
		chbusy=set_bit(chbusy,1);
		topic[1]=" Get the News direct from their source";
		chname[2]=LALIGN("Sell",5);
		chbusy=set_bit(chbusy,2);
		topic[2]=" We are all capitalistic...";
		
	      }


init()        {
		::init();
		add_action("give","gimme");
	      }

give()        {
		object ob;
		if( !COST(JJ_VALUE*2) ) return 1;
		ob = clone_object(JJPATHNAME);
		move_object(ob,POBJ);             
		write("You have now the latest issue of the Joker Journal\n");
		say(PNAM+" buys a Joker - Journal\n");
		return 1;
	      }




/***************** memberverwaltung ****************/  



update_members()
	      {
                int a,flag;
		flag=1;
                while(flag && mcounter>0)
                {
                  flag=0;
                  for(a=0;a<mcounter;a++)
                  {
                    if( !membero[a] || !query_ip_number(membero[a]) ) flag=a+1;
                    else
		    if( !present("joker journal",membero[a]) ) flag=a+1;
                  }
                  if(flag) dec_members(0,flag);
                }
              }


dec_members(ply,flag)
              {
                int a;
                
		if(ply)
		{
		  update_members();
		  flag=0;
		  for(a=0;a<mcounter;a++)
                    if(membero[a]==ply) flag=a+1;
		  if(flag) dec_members(0,flag);
		  return 1;
		}
                mcounter--;
                for(a=flag-1;a<mcounter;a++)
		{
                  membero[a]=membero[a+1];
		  memberm[a]=memberm[a+1];
		}  
		return 1;
	      }
	      



inc_members(silent,str)
	      {
	        update_members();
		if(mcounter==MAXPLAYERS)
                {
                  write("Oh sorry : there are MAXPLAYERS Players"+
                        "subscribers at the moment. (to much)\n");
                  ALOG("inc_members wanna inc above maxplayers");
		  COST(-JJ_SUBSCOSTS);
                  return 1;
                }
                if(!silent)
		{
                   write("WoW ! You are now a subscriber "+
                         "of the Joker Journal\n");
                   say(PNAM+" is now member of the Joker Journal\n");
		   memberm[mcounter]="";
		   memberm[mcounter]=set_bit(memberm[mcounter],1);       /* standart channels   */
		   memberm[mcounter]=set_bit(memberm[mcounter],2);       /* news and advertising*/
                }
                else
                {  
		   if(test_bit(str,0)) 
		   {
	             write("The Joker Journal appeared friendly back in your hands.\n");
                     memberm[mcounter]=str;
 		     memberm[mcounter]=clear_bit(memberm[mcounter],0);
		   }
		   else
		   {
		     write("WAIT ! Examine the new Joker Journal ( Second Edition )\n");
		     write("Type /help for further informations.\n");
		     memberm[mcounter]="";
 		     memberm[mcounter]=set_bit(memberm[mcounter],1);   /* standart channels   */
		     memberm[mcounter]=set_bit(memberm[mcounter],2);   /* news and advertising*/
		   }
		}
		membero[mcounter]=POBJ;
                mcounter++;
                return 1;
              }		


member(ply)   {
		int a;

                update_members();
                for(a=0;a<mcounter;a++)
                if(ply==membero[a]) return 1;
                return;
              }


list_members(ply)
              {
                int a,b;

                update_members();
		if(LEVEL>20) write(mcounter+" ");
                write("Members of the Joker Journal and what they read.\n");
                
		if(LEVEL<20)
		  for(a=0;a<mcounter;a++)
                  { 
                    if(membero[a]->short())
                    {
                      write(LALIGN(membero[a]->query_real_name(),13));
		      for(b=1;b<MAXCHANNELS;b++)			       
		        if(test_bit(memberm[a],b)) write(channel_name(b)+" "); 
		        else if(b<3) write(LALIGN("",5)+" ");                  /* players don't */
	              if(test_bit(memberm[a],MAXCHANNELS)) write("*");         /* know the ALL- */
		      write("\n");					       /* channel  B)   */
		    }
		  }
                 else
                   for(a=0;a<mcounter;a++)
                   { 
                     write(LALIGN(membero[a]->query_real_name(),13));
                     for(b=0;b<MAXCHANNELS;b++)				       /* but wizzes B) */
		        if(test_bit(memberm[a],b)) write(channel_name(b)+" ");
			else if(b<3) write(LALIGN("",5)+" ");
		     if(b>2 && test_bit(memberm[a],MAXCHANNELS)) write("*");
		     else write(" ");
		     write(ntime(query_idle(membero[a]))+"\n");
		   }
                return 1;
              }



get_mom(str)  {
		int a;
		
		update_members();
		update_channels();
		
		if(!str)
		{
		  for(a=0;a<mcounter;a++)
		  if (membero[a]==POBJ) return a+1;
		  return;
		}
		if(LALIGN(str,10)==LALIGN("",10)) return;
		for(a=0;a<mcounter;a++)
		if(membero[a]->query_real_name()==str) return a+1;
		return;
	      }


channel_vars(){
		string str;
		int mom;
		
		mom=get_mom();
		if(!mom) mom++; /*that's not right but should never happen */
		mom--;
                
		str="";
		str=set_bit(str,0); /* give chance to see if the old or the new version*/
		if(test_bit(memberm[mom],1)) str=set_bit(str,1);
		if(test_bit(memberm[mom],2)) str=set_bit(str,2);
		return str;
	      }
	      
	      

/****** channel routines  ************************/



update_channels()
              {
	        int a,b;
		string chn,ops,mops;
		chn="";
		ops="";
		mops=allocate(MAXCHANNELS);
		
		for(a=0;a<mcounter;a++)
		{
		  b=channel_number(0,a);
		  chn=set_bit(chn,b);
		  if(b)
		  {
		    if(test_bit(memberm[a],MAXCHANNELS))
		    ops=set_bit(ops,b);
		    else
		    mops=a;
		  }  
		}
		
		for(a=3;a<MAXCHANNELS;a++)
		{
		  if(test_bit(chn,a))
		  {
		    chbusy=set_bit(chbusy,a);
		    if(!test_bit(ops,a))			     /*no oper ? then the first */
		    memberm[mops]=set_bit(memberm[mops],MAXCHANNELS);/*found channellistner is*/
		  }  
		  else
		    chbusy=clear_bit(chbusy,a);
		}
		return 1;
	      }
	      
	      
msg_channel(str,flag)
	      {
	        int a;
		int mom;
		
		mom=get_mom();
		if(!mom) return;
		mom--;
		
		if(!flag) flag=channel_number(0,mom);
                if(!flag)
		{
		  write("U are no member of a special channel.\n");
		  return 1;
		}

#ifdef JJ_CHATCOSTS
                if(flag!=2)
		{
		  if(POBJ->query_spell_points()<JJ_CHATCOSTS)
		  {
		    write("You don't have enough Spellpoints.\n");
		    return 1;
		  }
		  POBJ->restore_spell_points(-JJ_CHATCOSTS);
		}
#endif

		write_history("<"+PNAM+"> "+str,flag);
	        for(a=0;a<mcounter;a++)
		if(membero[a]!=POBJ)
		{
		  if(test_bit(memberm[a],flag) && !test_bit(memberm[a],0) && flag>2 )
	  	  {
		    tell_object(membero[a],"<"+PNAM+"> "+str+"\n");
		  } 
		  if(test_bit(memberm[a],0) || (flag<3 && test_bit(memberm[a],flag)) )
		  {  
		    tell_object(membero[a],"#"+chname[flag]+"<"+PNAM+"> "+str+"\n");
		  }
		} 
		return 1;
	      }
		

channel_name(flag)
	      {
		if(test_bit(chbusy,flag)) return chname[flag];
              }


free_channel(){
		int a;
		
                for(a=1;a<MAXCHANNELS;a++)
		if(!test_bit(chbusy,a))
		{
		  chbusy=set_bit(chbusy,a);
		  return a;
		} 
	      }
	     
		
channel_number(str,mom)
	      {
		int a;
		
		if(!str)
		{
		  for(a=3;a<MAXCHANNELS;a++)
		    if(test_bit(memberm[mom],a)) return a;
		  return;
		}
		else
		{
		  str=lower_case(LALIGN(str,5));
		  for(a=1;a<MAXCHANNELS;a++)
		     if(test_bit(chbusy,a)) 
		        if(lower_case(chname[a])==str) return a;
		  return;
		}
	      }


join_channel(str)
	      {
		int a,b;
		int mom;
                
		mom=get_mom();
		if(!mom) return;	      
		mom--;
		
		a=channel_number(str,mom);
		if(a)
 	        {
		  if(test_bit(memberm[mom],a))
		  {
		    write("You are already a listner of that channel.\n");
		    return 1;
		  }
		  if(a<3)
		  {
		    write("You listen to the "+chname[a]+" channel again.\n");
		    memberm[mom]=set_bit(memberm[mom],a);
		    return 1;
		  }
		  if(LEVEL>20)
		  {
		    memberm[mom]=set_bit(memberm[mom],a+MAXCHANNELS);   /*wizards are always */
		    memberm[mom]=set_bit(memberm[mom],MAXCHANNELS);   /*invited and operators*/
		  }
		  
		  if(test_bit(chbusy,a+MAXCHANNELS) && !(test_bit(memberm[mom],a+MAXCHANNELS)))
		  {
		    write("Sorry you are not invited.\n");
		    return 1;
		  } 
		  b=channel_number(0,mom);
		  if(b) leave_channel();
		  msg_channel(" joined channel "+chname[a],a);
		  write("You are now a listner of the "+chname[a]+" channel.\n");
		  memberm[mom]=set_bit(memberm[mom],a);
		  return 1;
		}		  
		
		a=free_channel();
		if(!a)
		{
		  write("Sorry I can't free the memory for another channel.\n");
		  ALOG("not enough channels");
		  return 1;
		}
	        b=channel_number(0,mom);
		if(b) leave_channel();
		chname[a]=LALIGN(str,5);
		chbusy=set_bit(chbusy,a);
		hist_length[a]=0;
		topic[a]=LALIGN(" ",30);
		history[a]=allocate(MAXHISTORY);
		memberm[mom]=set_bit(memberm[mom],a);
		memberm[mom]=set_bit(memberm[mom],a+MAXCHANNELS);          /* not private but*/
		chbusy=clear_bit(chbusy,a+MAXCHANNELS);                    /* invited B)     */
		memberm[mom]=set_bit(memberm[mom],MAXCHANNELS);           /*      set that bit */
                write("You opened channel "+chname[a]+"\n");             /* and he is operator */
		return 1;
	      }


leave_channel(str)
	      {
		int a,b;
		int mom;
		
	        mom=get_mom();
		if(!mom) return;	      
		mom--;
		
		a=channel_number(str,mom);
		if(!a && !str) return 1;
		if(!a && str)
		{
		  write("Sorry I don't know such channel.\n");
		  return 1;
		}
		if(!test_bit(memberm[mom],a) )
		{ 
		  write("You are no listener of that channel.\n");
		  return 1; 
		}
		  write("You left channel "+chname[a]+"\n");
		  memberm[mom]=clear_bit(memberm[mom],a);               /*no listener*/
		  memberm[mom]=clear_bit(memberm[mom],MAXCHANNELS);     /*no operator*/
		  memberm[mom]=clear_bit(memberm[mom],a+MAXCHANNELS);   /*not invited*/
		  if(a>2) msg_channel(PNAM+" left channel.",a);
                  update_channels();
		  return 1;
              }


recap_channel(str)
              {
		int a;
		int mom;
		
		mom=get_mom();
		if(!mom) return;	      
		mom--;
		
		a=channel_number(str,mom);
		if(!a && !str) return 1;
		if(!a && str)
		{
		  write("Sorry I don't know such channel.\n");
		  return 1;
		}
		if(!test_bit(memberm[mom],a) && LEVEL<20 )
		{ 
		  write("You are no listener of that channel.\n");
		  return 1; 
		}
                read_history(a);
		return 1;
	      }


read_history(flag)
	      {
	      	int a,b;
		
                if(flag==2) update_sells(); 
		b=hist_length[flag];
		if(!b)
		{
		  write("Sorry nothing in history.\n");
		  return 1;
		}
		for(a=0;a<b;a++)
		write(history[flag][a]);
		return 1;
	      }   

write_history(str,flag)
	      {
	        int a,b;
		
		b=hist_length[flag];
		if(b<MAXHISTORY)
		{
		  history[flag][b]=str+"\n";
		  hist_length[flag]=b+1;
		  return 1;
		}
		remove_history(flag,0);
		write_history(str,flag);
		return 1;
	      }
	       
remove_history(chn,flag)
	      {
	        int a,b;
		 
	        b=hist_length[chn];
		if(flag>=b) return 1;
		b--;
		for(a=flag;a<b;a++)
		history[chn][a]=history[chn][a+1];
		hist_length[chn]=b;
		return 1;
	      }

test_operator()
	      {
	      	int a;
		int mom;
		
		mom=get_mom();
		if(!mom) return;	      
		mom--;
		
		a=channel_number(0,mom);
		if(!a)
		{
		  write("Sorry, but you don't listen to any channel.\n");
		  return;
		}
		if(!test_bit(memberm[mom],a+MAXCHANNELS))
		{
		  write("Operators only.\n");
		  return;
		}
	        return a;
	      }


clear_channel()
	      {
	        int a;
		
		a=test_operator();
	        if(!a) return 1;
		
		msg_channel(PNAM+" cleared channel-history.",a);
		hist_length[a]=0;
		return 1;
	      }
	      

private_channel()
              {
	      	int a;
		
		a=test_operator();
		if(!a) return 1;
		
		if(!test_bit(chbusy,a+MAXCHANNELS))
		{
		  msg_channel(PNAM+" set this channel to private",a);
		  chbusy=set_bit(chbusy,a+MAXCHANNELS);
		}
		else
		{
		  msg_channel(PNAM+" set this channel to open (history cleared too)",a);
		  chbusy=clear_bit(chbusy,a+MAXCHANNELS);
		  hist_length[a]=0;
		}
		return 1;
	      }
	
				  
invite_channel(str)
              {
	        int a,b;
		
		a=test_operator();
		if(!a) return 1;
		
		if(!str || LALIGN(str,10)==LALIGN("",10)) return;
		b=get_mom(str);
		if(!b)
		{
		  write("Sorry can't find such member.\n");
		  return 1;
		}
		b--;
		if(!test_bit(memberm[b],a+MAXCHANNELS))
		{
		  tell_object(membero[b],PNAM+" invited you to channel :"+chname[a]+"\n");
		  write("Invited.\n");
		  memberm[b]=set_bit(memberm[b],a+MAXCHANNELS);
		}

		else
		{
/*
		  tell_object(membero[b],PNAM+" canceled your invitation to channel :"+ 
		  			        chname[a]+"\n");
		  memberm[b]=clear_bit(memberm[b],a+MAXCHANNELS);
                  write("You canceled the invitation.\n");
*/
		write("Is already invited.\n");
		}
		
		return 1;
	      }


throw_channel(str)
              {
	        int a,b;
		
		a=test_operator();
		if(!a) return 1;
		
		if(!str || LALIGN(str,10)==LALIGN("",10)) return;
		b=get_mom(str);
		if(!b || !str)
		{
		  write("Sorry can't find such member.\n");
		  return 1;
		}
		b--;
		if(!test_bit(memberm[b],a))
		{
		  write("Sorry but he is not a member of your channel.\n");
		  return 1;
		}
		memberm[b]=clear_bit(memberm[b],a);              /*throw out*/
		memberm[b]=clear_bit(memberm[b],a+MAXCHANNELS);  /*unload*/
                memberm[b]=clear_bit(memberm[b],MAXCHANNELS);
		
		tell_object(membero[b],PNAM+" threw you out of channel :"+chname[a]+"\n");
		write("Thrown.\n");
		return 1;
	      }
		               

oper_channel(str)
              {int a,b;
		
		a=test_operator();
		if(!a) return 1;
		
		if(!str || LALIGN(str,10)==LALIGN("",10)) return;
                b=get_mom(str);
		if(!b)
		{
		  write("Sorry can't find such member.\n");
		  return 1;
		}
		b--;
		if(!test_bit(memberm[b],a))
		{
		  write("Sorry but he is not a member of your channel.\n");
		  return 1;
		}
                memberm[b]=set_bit(memberm[b],MAXCHANNELS);
		
		msg_channel(PNAM+" gives "+str+" operator status.",a);
		write("He is now operator.\n");
		return 1;
	      }


topic_channel(str)
	      {
	        int a;
		
		a=test_operator();
		if(!a) return 1;
		
		topic[a]=LALIGN(str,30);
		msg_channel(PNAM+" set topic to :"+topic[a],a);
		write("Set topic to :"+topic[a]+"\n");
		return 1;
	      }


talk_channel(a,b)
              {
	      	int mom;
		
	        if(LALIGN(a,10)==LALIGN("",10)) return;

	        mom=get_mom(a);
	        if(!mom)
		{
		  write("Sorry can't find member.\n");
		  return 1;
		}
		mom--;
		
#ifdef JJ_CHATCOSTS
		if(POBJ->query_spell_points()<JJ_CHATCOSTS)
		{
		  write("You don't have enough Spellpoints.\n");
		  return 1;
		}
		POBJ->restore_spell_points(-JJ_CHATCOSTS);
#endif
                tell_object(membero[mom],"*"+PNAM+"*->"+b+"\n");
                write("Message sent.\n");
		return 1;
	      }
	        

list_channels()
              {
	        int a;
		for(a=1;a<MAXCHANNELS;a++)
		  if(test_bit(chbusy,a))
		  {
		    write(chname[a]+" "+topic[a]+"  ");
		    if(test_bit(chbusy,a+MAXCHANNELS))
		      write("PRIVATE");
		    write("\n");
		  }
		return 1;
	      }

snoop_channel()
              {
	        int mom;
		
		mom=get_mom();
		if(!mom) return;
		mom--;
		
		if(test_bit(memberm[mom],0))
		{
		  write("Channelsnoop : OFF\n");
		  memberm[mom]=clear_bit(memberm[mom],0);
		}
		else
		{
		  write("Channelsnoop : ON\n");
		  memberm[mom]=set_bit(memberm[mom],0);
		}
		return 1;
	      }


/******standart routines  ************************/


ntime(in1)   {  string st1;
                int  h,m,s;
                h=in1/3600;
                m=(in1-(h*3600))/60;
                s=(in1-(h*3600)-(m*60));
                if(h<10) st1=" "+h+":";
                else     st1=h+":";
                if(m<10) st1=st1+"0"+m+":";
                else     st1=st1+m+":";
                if(s<10) st1=st1+"0"+s;
                else     st1=st1+s;
                return st1;
             }


test_cheat(str) {
                   string legal;
                   int   a,b,flag,number;
		   
                   legal=({"0","1","2","3","4","5","6","7","8","9"});
                   for(a=0;a<strlen(str);a++)
                   { 
                     flag=0;
                     for(b=0;b<10;b++)
                     if(extract(str,a,a)==legal[b]) flag=1;
                     if(!flag) return;
                   }
                   sscanf(str,"%d",number);
                   return number+1;
                 }



#ifdef JJ_NO_DRINKS
has_drink(obj){
		int count;
		object ob;
		
		ob = first_inventory(obj);
		while(ob)
		{
		  if (is_drink(ob) && !ob->is_empty()) count++;
		  if (call_other(ob,"can_put_and_get")) count += has_drink(ob);
		  ob = next_inventory(ob);
		}
		return count;
	      }



is_drink(ob)  {
                return explode(file_name(ob),"#")[0] == "obj/drink" || ob->id("drink");
	      }
#endif





deny_sell(item)
	      {
#ifdef JJ_NO_DRINKS
		if (is_drink(item) || has_drink(item))
                {
		  write("You may not sell drinks.\n");
                  return 1;
                }
#endif
#ifdef JJ_NO_KEYS
                if( item->id("key") )
                {  
		  write("Sorry, maybe this key will help\n");
                  write("       imprisoned people to flee. \n");
                  return 1;
                }
#endif

                if(item->query_value()==0)
                {
	          write("You cannot sell valueless items.\n");
                  return 1;
                }
              }


/**************advertising stuff ************************/


update_sells()
	      { 
		int a,flag;
		
		if(!hist_length[2]) return 1;
	
		
		flag=TRUE;
		while(flag)
		{		  
		  flag=0;
		  a=0;
		  
                  while(a<hist_length[2] && !flag)
                  {
		    if( !splayer[a] ) flag=a+1;
		    else
                    if( !member(splayer[a]) ) flag=a+1;
                    else
		    {
		      if( !present(sitem[a],splayer[a]) ) flag=a+1;
#ifdef JJ_SELLCOSTS
		      else 
		      {
		        if( splayer[a]->query_spell_points()<JJ_SELLCOSTS ) flag=a+1; 
                      }
#endif		
		    }
		    a++;
		  }
                  if(flag) kill_sell(flag);
	        }
	      }
		  
		
		
		
kill_sell(flag) 
          { 
	    int a;	    
	    
	    flag--;                             /* cause the update_sell gives flag+1 */
            remove_history(2,flag);             /* dec's hist_length[2] */ 
	    
	    for(a=flag;a<hist_length[2];a++)
            {
	      splayer[a] = splayer[a+1];
              sitem[a]   = sitem[a+1]  ;
              svalue[a]  = svalue[a+1] ;  
              chiffre[a] = chiffre[a+1];
	    }
            return 1;
          }





adv_sell(str)
	      {
		if(!str) return 1;
	      
#ifdef JJ_SELLCOSTS
	        if(POBJ->query_spell_points()<JJ_SELLCOSTS)
		{
		  write("Sorry but you haven't "+JJ_SELLCOSTS+" Spellpoints.\n");
		  return 1;
		}
#endif
                item = present(str,this_player());
                if(!item) 
                {
	          write("You do not have such an item.\n");
                  return 1;
                }

                if(deny_sell(item)) return 1;
	      
                write("Value of "+str+" : "+item->query_value()+"\n");
                write("How much do you want to get :");
                input_to("adv_price");
                return 1;
              } 



adv_price(str)
	      {
	      	int flag,a;
		
		if(!str) return 1;
	      
		if(str=="") return 1;
		fprice=test_cheat(str);
		if(!fprice)
		{
		  write("Why do u try to cheat ?\n");
		  ALOG("JJPRICE"+str);
		  return 1;
		}
		fprice--;                                      /* this allows zerosells */

#ifdef JJ_NO_VALUELESS
		if(!fprice)
		{
		  write("Sorry. You have to enter a price.\n");
		  write("How much do you want to get :");
                  input_to("adv_price");
                  return 1;
		}
#endif 
		if(fprice>200)
		{
		  if( (fprice/2) > item->query_value() )
                  {
		    write("No extraordinary prices accepted.\n");
                    return 1;
              	  }
              	}           
              	write("OK ! We will try to sell it for you!\n");
      
      		for(a=0;a<hist_length[2];a++)
		if(sitem[a]==item) flag=a+1;
		if(a) kill_sell(a);
		
      	      	if(hist_length[2]==MAXHISTORY) kill_sell(1);

		ccounter+=random(17)+3;
		splayer[hist_length[2]] = POBJ;
              	sitem[hist_length[2]]   = item;
                svalue[hist_length[2]]  = fprice;
                chiffre[hist_length[2]] = ccounter;


		msg_channel(" sells "+item->short()+"\n"+
		            "         Value     :"+RALIGN(item->query_value(),5)+"\n"+
			    "         here only :"+RALIGN(fprice,5)+"  ->Chiffre:"+ccounter,2);
               			return 1;
	      }    
          
     
adv_buy(str)  
	     {
		int i,test;
            	int zaehler;

	    	update_sells();
	    
#ifdef JJ_SELLCOSTS
		if(POBJ->query_spell_points()<JJ_SELLCOSTS)
		{
		  write("Sorry but you haven't "+JJ_SELLCOSTS+" Spellpoints.\n");
		  return 1;
		}
#endif
		
		str=str+" test";    
            	sscanf(str,"%d test",test);
            	zaehler=test;
		
            	i=0;
            	for(test=0;test<hist_length[2];test++) 
                if (zaehler==chiffre[test]) i=test+1;
            
            	if(!i)
	    	{
	     	  write("Sorry this chiffre doesn't exist\n");
                  return 1;
            	}
		i--;

                if( svalue[i] > POBJ->query_money() )
                { 
	           write("Where is your money ?\n");
                   return 1;
                }

                zaehler=transfer(sitem[i],POBJ);
            	if (zaehler > 2)
            	{ 
	    	  kill_sell(i); 
            	  write("Sorry cannot get the item.\n");
            	  return 1;
            	}
		
            	if(zaehler==1)
            	{ 
	    	  write("Too heavy for you .\n");  
            	  return 1;
            	}
		
            	write("You get "+( sitem[i]->short() )+".\n");
            	POBJ->add_money(-svalue[i]);
#ifdef JJ_SELLCOSTS
                POBJ->restore_spell_points(-JJ_SELLCOSTS);
#endif
            	tell_object(splayer[i],"You get "+svalue[i]+" gc from the Joker-Journal\n");
            	splayer[i]->add_money(svalue[i]);
#ifdef JJ_SELLCOSTS
                splayer[i]->restore_spell_points(-JJ_SELLCOSTS);
#endif
            	kill_sell(i+1);
    
              }



