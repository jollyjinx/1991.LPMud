int a;
short() {return "a wlist object";}
id(str) { if(str=="wlo") return 1;}
get()   { return 1;}
init()  { add_action("beg","star");
        }
beg(a)  { if(!a)  a=0;
          else return 1;
          while(a<1000)
          {
          write("B)");
          a+=1;
              call_out("mach",2,a);
          }
          return 1;
        }
mach(a)
        {
          command("e",this_player());
          command("w",this_player());
        }

