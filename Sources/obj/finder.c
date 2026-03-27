/* lars pwd finder by jolly */

#define PWD "vUvhwHSmAsYiM"

int mom;
static string chr;

id(str) { if(str=="finder") return 1;}
short()  { return "A pwdfinder";}

init()   {chr=({"1","2","3","4","5","6","7","8","9","0",
		"a","b","c","d","e","f","g","h","i","j","k","l",
		"m","n","o","p","q","r","s","t","u","v","w","x","y","z",
		"A","B","C","D","E","F","G","H","I","J","K","L",
		"M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"});
	  add_action("find","start");
	  return 1;
	}
	
find()  { restore_object("players/jolly/obj/pswd");
          next();
	  return 1;
	}

next()  { string str;
          str=add_one();
	  
	  if(str=="-") return 1;
	  if(crypt(str,PWD)!=PWD)
	  {
	  next();
          return ;
	  }
	  write("Password found:"+str+".\n");
	  write_file("/players/jolly/passwd.pwd",
	  	     "Password for:"+PWD+"="+str+".\n");
	  return 1;
	}


add_one(){
           string str1,str2,str;
	   int a,b,c;
	   
	   
	   str="";
	   a=sizeof(chr);
	   b=++mom;
	   do
	   {
	      c=b%a;
	      b=b/a;
	      str=str+chr[c];   
	   }
	   while(b>0);
	   
	   if(!0)
	   {
	      tell_object(find_player("jolly"),"Mom: "+str+".\n");
	      save_object("players/jolly/obj/pswd");
	      call_out("next",1);  
	      str="-";  
	   }    
	   return str;
	 }
	   
	   
	   
	   
