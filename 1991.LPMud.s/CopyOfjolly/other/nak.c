/*naked obj for master faroul by jolly*/
string str;

id(str) { if (str=="ticket") return 1;}
get()   { return 1;}
init() { add_action("i","",1);
       }
i(str) {
         string st;
         string st1;
         st1=str;
         str=query_verb();
         if(!str) return;
         st=extract(str,0,1);
         if(st=="i") {write("A ticket.\n"); return 1;}
         st=extract(str,0,3);
         if(st=="look") {write("Oh that hurts ! Chlor makes it impossible"+
                               " to look.\n");return 1;}
       }
long() {write("A ticket which you need to get back\n");
        write("your equipment.\n");
       }
