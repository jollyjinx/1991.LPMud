/* Modified for genders: padrone, Oct 22 1990 */
/* Modified for ruffle kiri: kiri, June 26 1991 */

object the_owner;
string cap_name;

get() {
    the_owner = this_player();
    cap_name = call_other(this_player(), "query_name", 0);
    return 1;
}

drop() { return 1; }

id(str) { return str == "butlerseele"; }

long() {
    write("It is transparent.\n");
}

ghost() {
    the_owner = this_player();
    cap_name = call_other(this_player(), "query_name", 0);
    return call_other(this_player(), "query_ghost");
}

init() {
    the_owner = this_player();
    cap_name = call_other(this_player(), "query_name", 0);
    add_action("sag","sag");
}

sag(str){
	say("Jolly's Butler says: "+str+".\n");
}
