#define NAME "jolly"
#define DEST "room/forest1"
/*
 * This is just the facade to a castle. If you want to enable the
 * "enter" command, move the player to a hall or something (which
 * you have to design yourself).
 * The predefined string DEST is where a player should come when he
 * leaves the castle.
 *
 * This file is loaded automatically from "init_file". We have to move
 * ourself to where we are supposed to be.
 */

id(str) { return str == "castle"; }

short() {
	return "A Press Agency is to the north";
}


init() {
    add_action("north", "north");
}

north(str) {
     this_player()->move_player("north#players/jolly/room/npa.c");
	return 1;
}

reset(arg) {
    if (arg)
        return;
        call_other("/players/jolly/obj/jj","???");
      call_other("/players/jolly/room/npa.c","???");
    move_object(this_object(), DEST);
}
