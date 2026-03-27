
 more tool.c
/*  LAST EDIT: Fri May  3 19:36:31 1991 by Michael Schmidt (michael)  */
/*
 * Data's wizard tool
 * It has features and ideas shamelessly stolen from other great Wizards.
 * Esp. from Deepthought, Marvin, ... And some code from /obj/trace.
 */

string type;
int owner;
int joined;
object confobj;
int lighted;

query_auto_load() {
    return "players/data/obj/tool:normal";
} 

Reset(arg) {
if (arg) return;
lighted = 0;
}

query_weight() {
return 0;
}

id(str) {
    return str == "tool" || str == "datatool";
}

short() {
if (lighted > 0)  
return "Data's wizard tool (glowing)";
else
return "Data's wizard tool";
}

long() {
write("This is Data's special wizard tool.\n");
if (this_player()->query_level()<20) {
    write("What a pity you can't use it...\n");
    return;
    }
write("Mark 'living name' | 'name' | here | up | 'no'\n");
write("Dump [list]\n");
write("Destruct\n");
write("Clean\n");
write("Call 'function' | 'function' 'argument'\n"); 
write("Shout <message>      Tell to all wizards\n");
if (this_player()->query_level() >=25)
  write("SHOUT <message>      Tell to all archwizards\n");
write("Stat <obj_name>      Show stats of an object.\n");
write("Kill                 Destroy the tool.\n");
write("Align <al_string>    Changes your alignment string.\n");
write("Title <title>        Changes your title and announce it.\n");
write("Who                  Displays extended user info.\n");
write("Mail                 Extended user info.\n");
write("Objects <player>     List all objects of a player.\n");
write("On                   Makes the built in light shine brighter.\n");
write("Off                  Makes the built in light shine dimmer.\n");
write("The built-in light is turned ");
if (lighted > 0) write("on.\n"); else write("off.\n");
if (this_player()->query_level() > 20) 
write("Light level is "+lighted+".\n"); 
}

drop() { return 1; }
get(){return 1;}

init() {
owner = this_player();
if (this_player()->query_level()<20) {
    return;
    }
add_action("wanted", "Wanted");
add_action("remove", "Remove");
add_action("i_clean", "Clean");
add_action("i_destruct", "Destruct");
add_action("i_dump", "Dump");
add_action("i_mark", "Mark"); 
add_action("i_call", "Call");
add_action("wshout1"); add_verb("SHOUT");
add_action("wshout"); add_verb("Shout");
add_action("zstat"); add_verb("Stat");
add_action("zkill"); add_verb("Kill");
add_action("chalign"); add_verb("Align");
add_action("chtitle"); add_verb("Title");
add_action("dwho"); add_verb("Who");
add_action("dmail"); add_verb("Mail");
add_action("dobjects"); add_verb("Objects");
add_action("dobjects"); add_verb("Ob");
add_action("don"); add_verb("On");
add_action("doff"); add_verb("Off");
add_action("dcastle");
add_verb("Castle");
} 

wanted(str)
{
  call_other("/players/padrone/inside/sheriffs_office",
     "wanted", find_player(str), "Doing evil things.", 500);
  return 1;
}

remove(str)
{
  call_other("/players/padrone/inside/sheriffs_office",
     "remove_and_reward", str);
  return 1;
}  

zstat(str)  { 
object obj;
if (!str) { write ("Usage: Stat <obj_name>\n"); return 1; }
obj = present(str);
if (!obj) obj = present(str,environment(this_player()));
if (!obj) { write ("This object doesn't exist here.\n"); return 1; }
if (obj->short())
write("short: "+obj->short()+"\n");
write("long:\n");
obj->long();
write("creator: "+creator(obj)+".\n");
if (obj->query_value())
write("value: "+obj->query_value()+"\n");
if (obj->query_weight())
write("weight: "+obj->query_weight()+"\n");
if (obj->query_info())
write("info: "+obj->query_info()+"\n"); 
if (obj->weapon_class())
write("wc: "+obj->weapon_class()+"\n");
if (obj->armour_class())
write("ac: "+obj->armour_class()+"\n");
if (obj->query_type())
write("armour type: "+obj->query_type()+"\n");
return 1;
}
chalign(str) {
    if (this_player()->query_level() < 20) return 0;
    if (!str) {
    write("USAGE: Align <al_string>\n"); return 1;}
    call_other(this_player(),"set_alignment",str);
    write("Alignment changed...\n");
    sshout(capitalize(this_player()->query_name())+" is now "+str+".\n");
    return 1; 
    }

dcastle(str) {
    object castle;
    if (this_player()->query_level() < 50) return 0;
    if (!str) {
    write("USAGE: Castle <name>\n"); return 1;}
    castle = clone_object("room/port_castle");
    move_object(castle,this_player());
    castle->set_owner(str);
    return 1;
    }

chtitle(str) {
    if (this_player()->query_level() < 20) return 0;
    if (!str) { 
    write("USAGE: Title <title>\n"); return 1;}
    call_other(this_player(),"set_title",str);
    write("Title changed...\n");
    sshout(capitalize(this_player()->query_name())+" is now known as "+
capitalize(this_player()->query_name())+" "+str+".\n");
    return 1;
    }

dwho() {
string who;
int i;
who = users();
write("Name            level   str dex int con    wc  ac   hp       money        exp\n");
for (i = 0; i < sizeof(who); i++) {
  if (!who[i]->short())
    write(ladjust("("+who[i]->query_real_name()+")",16)+" "); 
  else
    write(ladjust(who[i]->query_real_name(),16)+" ");
  write(radjust(who[i]->query_level(),4)+" ");
  write(radjust(who[i]->query_str(),5)+" ");
  write(radjust(who[i]->query_dex(),3)+" ");
  write(radjust(who[i]->query_int(),3)+" ");
  write(radjust(who[i]->query_con(),3)+" ");
  write(radjust(who[i]->query_wc(),5)+" ");
  write(radjust(who[i]->query_ac(),3)+" ");
  write(radjust(who[i]->query_hit_point(),4)+" ");
  write(radjust(who[i]->query_money(),11)+" ");
  write(radjust(who[i]->query_exp(),10)+" ");
  write("\n");
}
return 1;
} 

dmail() {
string who;
int i;
who = users();
write("Name             level gender email\n");
for (i = 0; i < sizeof(who); i++) {
        if (!who[i]->short())
  write(ladjust("("+who[i]->query_real_name()+")",16)+" ");
else
  write(ladjust(who[i]->query_real_name(),16)+" ");
write(ladjust(who[i]->query_level(),5)+" ");
write(ladjust(who[i]->query_gender_string(),6)+" ");
write(ladjust(who[i]->query_mailaddr(),47));
write("\n");
} 
return 1;
}

dobjects(str) {
object liv, ob;
if (!str) {
write("Usage: Objects <player>\n");
return 1;
}
if (!(liv = find_living(lower_case(str)))) {
write("Living "+str+" not found.\n");
return 1;
}
write("Name                          creator   value   weight  special\n");
for (ob=first_inventory(liv); ob; ob=next_inventory(ob))
{ 
if (ob->short()) {
write(ladjust(ob->short(),30));
write(ladjust(creator(ob),10));
write(ladjust(ob->query_value(),8));
write(ladjust(ob->query_weight(),8));
if (ob->weapon_class()) {
write("wc: "+ladjust(ob->weapon_class(),4));
}
if (ob->armour_class()) {
write("ac: "+ladjust(ob->armour_class(),4));
write("type: "+ladjust(ob->query_type(),8));
}
write("\n");
if (ob->can_put_and_get()) dobjects1(ob);
}
} 
return 1;
}

dobjects1(thisob)
{
  object ob;
  for (ob=first_inventory(thisob); ob; ob=next_inventory(ob))
    {
      if (ob->short()) {
write(ladjust("in that: "+ob->short(),30));
write(ladjust(creator(ob),10));
write(ladjust(ob->query_value(),8));
write(ladjust(ob->query_weight(),8));
if (ob->weapon_class()) {
  write("wc: "+ladjust(ob->weapon_class(),4));
} 
if (ob->armour_class()) {
  write("ac: "+ladjust(ob->armour_class(),4));
  write("type: "+ladjust(ob->query_type(),8));
}
write("\n");
if (ob->can_put_and_get()) dobjects1(ob);
      }
    }
  return 1;
}
  

don() {
lighted += 1; set_light(1);
write("Light level "+lighted+".\n");
return 1; 
}
doff() {
lighted -= 1; set_light(-1);
write("Light level "+lighted+".\n");
return 1;
}

zkill() {
write("You tear your tool to a thousand pieces. Why? Well... your problem!\n");
write("You lose all commands of Data's tool.\n");
destruct(this_object());
return 1;
    }

sshout(str) {
write("You shout: "+str); 
shout(str);
}

wshout(str)
{ int i;
  object ply;

  if (!str) return 0;
  ply=users();
  for (i=0; i<sizeof(ply); i+=1) {
    if (ply[i] && ply[i]->query_level()>20) {
      if (ply[i]==this_player()) continue;
      write("telling to "+capitalize(ply[i]->query_real_name())+"\n");
      tell_object(ply[i],capitalize(this_player()->query_real_name())+
                        " tells to all wizards: "+ str+"\n");
    } 
  }
  write("Ok.\n");
  return 1;
}

wshout1(str)
{ int i;
  object ply;

  if (!str) return 0;
  ply=users();
  for (i=0; i<sizeof(ply); i+=1) {
    if (ply[i] && ply[i]->query_level()>=25) {
      if (ply[i]==this_player()) continue;
      write("telling to "+capitalize(ply[i]->query_real_name())+"\n");
      tell_object(ply[i],capitalize(this_player()->query_real_name())+ 
                        " tells to all archwizards: "+ str+"\n");
    }
  }
  write("Ok.\n");
  return 1;
}

ladjust(str, n) {
return extract(str+"                             ",0,n-1);
}
radjust(str, n) {
  string out;
  
  if (intp(str))
    {
      int negative; 
      int digits, i;
      
      if (str < 0)
{
  negative = 1;
  str = -str;
}
      else
negative = 0;
      
      i = str; digits=0;
      if (i==0)
digits=1;
      else
while (i >= 1)
  { 
    digits++;
    i = i/10;
  }
      if (negative)
return extract("                             "+"-"+str,digits+30-n);
      else
return extract("                             "+str,digits+29-n);
    }
  else
    return extract("                             "+str,strlen(out)+29-n);
}

init_arg(str) {
type = str;
}
 
/*
extra_look() {
    return call_other(owner,"query_name")+" is a Believer in Zorin";
}
*/

object mark;

i_dump(str) {
    int tmp;
    if (mark == 0) {
write("Nothing marked.\n");
return 1;
    }
    if (str == "list") {
object ob; 
int i;
      
ob = first_inventory(mark);
while(ob) {
    i += 1;
    write(i + ":\t");
    write(ob);
    write("\t" + call_other(ob, "short") + "\n");
    ob = next_inventory(ob);
}
return 1;
    }
    write(call_other(mark, "short") + "\n");
    if (living(mark))
      {
write("(living)"); 
if (call_other(mark, "query_npc"))
  write("(NPC)");
if (tmp = query_ip_number(mark))
#ifdef PADERMUD
  write("(interactive) '" + query_hostname(mark) + "' ");
#else
  write("(interactive) ");
#endif
write("\n");
if (tmp)
  write("idle: " + query_idle(mark) + " seconds\n");
tmp = query_snoop(mark);
if (tmp)
  write("Snooped by " + tmp->query_real_name() + "\n");
      }
     
    tmp = creator(mark);
    if (tmp)
      write("Creator:\t\t" + tmp + "\n");
    tmp = call_other(mark, "query_value");
    if (tmp)
write("Value:\t" + tmp + "\n");
    tmp = call_other(mark, "query_weight");
    if (tmp)
write("Weight:\t" + tmp + "\n");
    return 1;
}

i_clean(str) {
    object ob, o, n;

    if (!mark) 
return 0;
    for (n = first_inventory(mark); n; n = o) {
o = next_inventory(n);
if (query_ip_number(n))
    continue;
destruct(n);
    }
    write("Ok.\n");
    return 1;
}

i_mark(str) {
    string tmp, tmpmark;

    if (str == 0)
      str = "here"; 

    tmpmark = find_living(lower_case(str));
    if (tmpmark)
      {
mark = tmpmark;
write(tmpmark); write("\n");
return 1;
      }
    
    if (str == "up") {
if (mark == 0)
    return 0;
if (environment(mark) == 0)
    return 0;
mark = environment(mark);
write(mark); write("\n"); 
return 1;
    }
    
    if (str == "here") {
mark = environment(this_player());
write(mark); write("\n");
return 1;
    }
    if (sscanf(str, "%d", tmp) == 1) {
object ob;
if (mark == 0)
    return 0;
ob = first_inventory(mark);
while(tmp > 1) {
    tmp -= 1;
    if (ob == 0) 
return 0;
    ob = next_inventory(ob);
}
if (ob == 0)
    return 0;
mark = ob;
write(mark); write("\n");
return 1;
    }
    return 0;
}

i_call(str) {
    string with, what;
    int iwhat, result;
 
    if (!str)
        return 0;
    if (!mark) {
write("No object marked.\n");
return 1;
    }
    if (sscanf(str, "%s %d", with, what) == 2)
        iwhat = 1;
    else if (sscanf(str, "%s %s", with, what) != 2) {
if (sscanf(str, "%s", with) == 1)
    iwhat = 0;
else
    return 0;
    }
    result = call_other(mark, with, what);
    write("Got: "); write(result); write("\n"); 
    say(call_other(this_player(), "query_name") +
" patched the internals of " + call_other(mark, "short") + ".\n");
    return 1;
}

i_destruct() {
    object ob;
    if (!mark)
return 0;
    ob = mark;
    write("Ok. New: ");
    i_mark("up");
    destruct(ob);
    return 1;
} 
EOF 