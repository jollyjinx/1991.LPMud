/* Jolly's teststuff JUl 20th 1991 */
 

string NAME;
int schonda;
string str;
string shortname;
string longname;
object ob;
reset(arg) { 
   if (arg) return;
    shortname="Jolly's bodyguardsystem";
   longname="Jolly's new bodyguardsystem.\n"+
	    "  Serialnumber : oo49-89-95o-57-xx JuL91\n"+
	    "  Commands provided : mkguard\n"+
	    "  Attention one use only and undropable.\n";
}

query_weight(){ return 0; }
query_value(){ return 1;}

short() { return shortname; }
long() { write(longname); }

get() {return 1;}
drop(){
	if(schonda==1){
	ob->hit_player(100000);}
	destruct(this_object());
	return 1;
}

but(){
	if(schonda==1) return;
	schonda=1;
	ob = clone_object("/players/jolly/monster/bodyguard");
        move_object(ob,environment(this_player()));
	NAME=this_player();
	ob->but(NAME,this_object());
	set_heart_beat(1);
	say("A bodyguard appeard.\n");
	write("A bodyguard appeard just from  the bgs.\n");
return 1;
}
id(str) {return str == "system";}

   
init() { add_action("but","mkguard"); 
}
 
heart_beat(){
              ob->staff();
}
died() { write("Oh master u'r bodyguard died (what a pity)\n");
         write("The bdgsystem vanished.\n");
         destruct(this_object());
       }
