/* a joker tool  by jolly
                 alias pat

11.jan 1992

*/

#define plyname call_other(this_player(),"query_name")
#define alname "/players/jolly/obj/jt:al"

object obj,following;
string str;
int inuse ;



id(str)      {
                if(    (str=="jt") || (str=="joker tool")
                    || (str=="tool") ) return 1;
             }

short()      {
                if(!inuse) return "A Joker tool (turned off)";
                           return "A Joker tool (turned on)";
             }

long()       {  jthel("jt");}

get()        {  return 1;}

drop()       {  if(inuse) return 1; }

query_auto_load()
             {  if(inuse) return alname; }

init_arg(str){
                if(str!="al") return;
                  inuse=1;
             }

init()       {
                add_action("jthel",    "help");
                add_action("jtcre", "creator");
                add_action("jtexa","examples");
                add_action("jtupd",  "update");
                add_action("jtlis",    "list");
                add_action("jton" ,    "jton");
                add_action("jtoff",   "jtoff");
                add_action("jtlig",   "light");
                add_action("jtmov",    "move");
                add_action("jtcal",    "call");
                add_action("jtfol",  "follow");
                add_action("jtfwh",    "fwho");
/*                add_action("jtdes",    "dest"); */ 
                add_action("jtcmp",   "cmpjj");
             }

jtcre(str)   {
                if(!inuse) return;
                write("Creator is:"+creator(find_object(str))+"\n");
                return 1;
             }


jthel(str)   {
                if(!id(str)) return;
                if(!inuse)
                {
                   write("This is the Joker tool.\n\n");
                   write("jton turns the tool on.\n");
                }
                else
                {
                   write("Commands the turned on Joker tool.\n");
                   write("  jtoff  turns the tool off.\n");
                   write("  cmpjj  lists nonmembers of jj below lvl20\n");
                   write("   help  tool ( this page )\n");
                   write("   fwho  fast who\n");
                   write("   list  object [-r]\n");
/*                   write("   dest  object [-r]\n"); */
                   write("   move  object to object\n");
                   write("   call  object routine [parameter]\n");
                   write("  light  [number]\n");
                   write("creator  object\n");
                   write(" update  object\n");
                   write(" follow  [object]\n");
                   write("examples shows you a few examples.\n");
                }
                return 1;
             }
                                     
jtexa()      {
                if(!inuse) return;
                write("Joker tool examples (or how i identify inputs)\n\n");
                write(" object= obj[:obj]....\n");
                write("    obj= #number || number || name\n"); 
                write("#number= #73 instead of obj/player#73\n");
                write(" number= me:2 is the 3th item i have (up from 0!)\n");
                write(" name  =   bag || harry || me || env || here ....\n");
                write("\nAnd now a few examples:\n");
                write("list harry:env -r\n");
                write("     lists the environment of harry recursive.\n");
                write("light 3\n");
                write("     adds 3 to the current light level.\n");
                write("move me:bag:2 to sepp\n");
                write("     the 3rd item from your bag is moved to sepp.\n");
                return 1;
             }

jton()       {
                if(this_player()->query_level() <20)
                   inuse=0;
                else
                   inuse=1;
                if(inuse)
                {
                   write("The Joker tool is now ready to work for you.\n");
                   inuse=1;
                }
                else
                {
                   write("The Joker tool dissapears.\n");
                   destruct(this_object());
                }
                return 1;
             }

jtoff()      {
                if(!inuse)
                   write("The tool is already turned off.\n");
                else
                {
                   inuse=0;
                   write("The tool is now turned off.\n");
                }
                return 1;
             }



jtlis(str)   {
                string st1;
                object obj;
                int    rec;

                if(!str || !inuse)  return;
                st1=str;
                rec=0;
                if(sscanf(str,"%s -r",st1)) rec=1;
                if(st1) str=st1;
                obj=finde(obj,str);
                if(obj)
                {  if(!list_obj(obj,rec,1))
                      write("Object contains nothing.\n");
                   return 1;
                }
             }


jtupd(str)   {
                object obj,mom;

                if(!str || !inuse) return;
                obj=finde(obj,str);
                obj=find_object(file_name(obj));
                if(!obj) return;
                str=file_name(obj);
                mom=first_inventory(obj);
                while(mom)
                {
                   if(mom->query_real_name())
                   {
                      if(mom==this_player()) move_object(this_object(),obj);
                      move_object(mom,this_object());
                      mom=first_inventory(obj);
                   }
                   else
                      mom=next_inventory(mom);
                }
                destruct(obj);
                obj=this_object();
                mom=first_inventory(obj);
                while(mom)
                {
                   move_object(mom,str);
                   mom=first_inventory(obj);
                }
                return 1;
             }


jtfwh(str)   {
                string st1;
                int in1;

                if(!inuse || str) return;
                st1=users();
                write(" Name          Lvl  Idle\n");
                for(in1=0;in1<sizeof(st1);in1+=1)
                {
                   if(st1[in1]->short())
                      write(lalign(st1[in1]->query_real_name(),11));
                   else
                      write(lalign("["+st1[in1]->query_real_name()+"]",11));
                      write("  "+ralign(st1[in1]->query_level(),3));
                      write("  "+ntime(query_idle(st1[in1]))+"\n");
                }
                return 1;
             }


jtcmp(str)   {
                string st1;
                int in1;

                if(!inuse || str) return;
                st1=users();
                for(in1=0;in1<sizeof(st1);in1+=1)
                {
                   if(st1[in1]->short())
                   if(!present("jj",st1[in1]))
                   if((st1[in1]->query_level())<20)
                   {
                      write(lalign(st1[in1]->query_real_name(),11));
                      write("  "+ralign(st1[in1]->query_level(),3));
                      write("  "+ntime(query_idle(st1[in1]))+"\n");
                   }
                }
                return 1;
             }

jtlig(str)   {
                int llevel;

                if(!inuse) return;
                if(str) sscanf(str,"%d",llevel);
                if(!llevel)
                {
                   write("Light Level here is : "+set_light(0)+"\n");
                   return 1;
                }
                set_light(llevel);
                write("Light Level now : "+set_light(0)+"\n");
                return 1;
             }

jtmov(str)   {  string st1,st2;
                object ob1,ob2;

                if(!inuse || !str) return;
                if(sscanf(str,"%s to %s",st1,st2)!=2) return;
                ob1=finde(ob1,st1);
                ob2=finde(ob2,st2);
                if(!ob1 || !ob2) return;
                move_object(ob1,ob2);
                return 1;
             }

jtcal(str)   {  object ob1;
                string st1,st2,st3;
                int arg;

                if(!str || !inuse) return;
                sscanf(str,"%s %s %s",st1,st2,st3);
                if(!st3)
                {
                   sscanf(str,"%s %s",st1,st2);
                   st3="";
                }
                if(!st2) return;
                ob1=finde(ob1,st1);
                if(!ob1) return;
                if(sscanf(st3,"%d",arg))
                write("Result :"+call_other(ob1,st2,arg)+"\n");
                else
                write("Result :"+call_other(ob1,st2,st3)+"\n");
                return 1;
             }

jtdes(str)   {
                object ob1;
                string st1;
                int    rec; 

                if(!inuse || !str) return;
                sscanf(str,"%s -r",st1);
                if(st1)
                {
                   str=st1;
                   rec=1;
                }
                ob1=finde(ob1,str);
                if(!ob1) return;
                if(ob1==this_object())
                {
                   destruct(this_object());
                   return;
                }
                dest(ob1,rec);                
                return 1;
             }

jtfol(str)   {
                if(!inuse) return;
                if(!str && following)
                {
                   following=0;
                   write("You stopped the followmode.\n");
                   remove_call_out("heart");
                   return 1;
                }
                if(!str) return;
                following=finde(following,str);
                if(!following) return;
                write("You follow "+following->short()+"\n");
                call_out("heart",1,0);
                return 1;
             }



/************************fuckin routines************/


list_obj(obj,rec,alg)
             {  int      i;
                string str;
                object mom;

                if(!obj) return;
                i=0;
                mom = first_inventory(obj);
                if(mom)
                {
                   write(lalign(" ",alg));
                   write("Object :"+lalign(obj->short(),20)+"  ");
                   write(ralign(filename(obj),30));
                   write(" contains :\n");
                }
                while(mom)
                {
                  write(lalign(" ",alg));
                  if(vis(obj))
                  {
                     write(":"+i);
                     str=mom->short();
                     write("   "+lalign(str,20)+"  ");
                     write(ralign(filename(mom),30));
                     write("\n");
                  }
                  else
                  {
                     write(":"+i);
                     str=mom->query_real_name();
                     if(!str) str=mom->long();
                     write("   ["+lalign(str+"]",19)+"  ");
                     write(ralign(filename(mom),30));
                     write("\n");
                  }
                  i=i+1;
                  if(rec) list_obj(mom,1,alg+3);
                  mom=next_inventory(mom);
                }
                if(i)
                {
                   write(lalign(" ",alg)+"End of inventory\n");
                   return 1;
                }
             }

                

finde(obj,str)
             {
                object ob1,ob2,ob3;
                string st1,st2,st3;
                int    in1,in2,in3;

                if(!strlen(str)) return obj;

                if(find_object(str)) return find_object(str);

                if(sscanf(str,"%s:%s",st1,st2)==2)
                {
                   obj=finde(obj,st1);
                   if(obj) return finde(obj,st2);
                   return;
                }

                if(sscanf(str,"#%s",st1)==1)
                {
                   if(!obj) obj=environment(this_player());
                   ob1=first_inventory(obj);
                   while( ob1 && (!ob2) )
                   {
                      sscanf(file_name(ob1),"%s#%s",st2,st3);
                      if(st3==st1) ob2=ob1;
                      if(!ob2) ob2=finde(ob1,"#"+st1);
                      ob1=next_inventory(ob1);
                   }
                   return ob2;
                }

                if( ( (str=="here")||(str=="env") ) && (obj) )
                   return environment(obj);
                if( (str=="here") || (str=="env") )
                   return environment(this_player());
                if(str=="me")
                   return this_player();
                if(find_living(str))
                   return find_living(str);

                sscanf(str,"%d",in1);
                if( (in1==0) && (str!="0") )
                {
                   if(!obj) obj=environment(this_player());
                   ob1=first_inventory(obj);
                   while( (ob1) && (!ob2))
                   {
                      if( (ob1->id(str)) ) ob2=ob1;
                      if(!ob2) ob2=finde(ob1,str);
                      ob1=next_inventory(ob1);
                   }
                   return ob2;
                }

                if(str=="0") in1=0;
                ob1=first_inventory(obj);
                while(ob1)
                {
                   if(in2==in1) ob2=ob1;
                   in2+=1;
                   ob1=next_inventory(ob1);
                }
                return ob2;
             }




ntime(in1)   {
                string st1;
                int  h,m,s;
          
                if(in1<30)  return "";
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

heart()      {
                if(!following)
                {
                   remove_call_out("heart");
                   return 1;
                }
                if(present(this_player(),following)) return 1;
                move_object(this_player(),environment(following));
                call_out("heart",1,0);
             }


vis(obj)     {  if(obj->short()) return 1;}

lalign(str,in)
             {  return extract(str+"                              ",0,in-1);}

ralign(str,in)
             {
                str="                      "+str;
                return extract(str,strlen(str)-in-1,strlen(str)-1);
             }

filename(obj){
                string str,st1;

                str=file_name(obj);
                sscanf(str,"players/%s",st1);
                if(st1) str="~"+st1;
                return str;
             }


dest(obj,rec){  object mom;

                if(!rec)
                {
                   destruct(obj);
                   return 1;
                }
                mom=first_inventory(obj);
                while(mom)
                {
                   dest(mom,rec);
                   mom=first_inventory(obj);
                }
                destruct(obj);
                return 1;
              }
