inherit "obj/treasure";

reset(arg) {
   if (arg) return;
   set_id("snooper");
}

init() {
  ::init();
  add_action("snoop_on","snoop");
}

static snoop_on(str)
{
    object ob;
    int ob_level; 

    if (!str) {
        snoop();
        return 1;
    }
    ob = find_player(str);
    if (!ob) {
        write("No such player.\n");
        return 1;
    }
    ob_level = call_other(ob, "query_level");
    if (ob_level >= this_player()->query_level() ) {
        write("You shouldn't be snooping this person.\n");
    }
    snoop(ob);
    return 1; 
}
