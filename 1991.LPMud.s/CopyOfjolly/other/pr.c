

inherit "obj/treasure";

string inst;
init() {
::init();
restore_object(feil());
add_action("show","show");
add_action("inscribe","inscribe");
}
reset() {
set_alias("prize");
set_short("the Poebel prize!");
}
long() { 
          write("a note:"+inst+"\n");
          return 1;
       }

show(arg) {
if(arg=="prize") {
write("You proudly show the prize around.\n");
say(call_other(this_player(),"query_name")+" shows you his Poebel prize.\n");
say("you are impressed");
return 1;
}
}
inscribe(str) { inst=str;
save_object(feil());
                return 1;
              }
feil() { string str;
        string st1,st2;
         str=file_name(this_object());
         sscanf(str,"%s#%s",st1,st2);
         return st1;
      }
