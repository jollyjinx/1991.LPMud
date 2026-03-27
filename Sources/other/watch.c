/*** Multifunctional watch ***
* By Snake 27.7.1991
* Partly based on the watches of : 
* 11.7.1991 by Magic Computing / Dixie
* 18.3.1991 by Storm
*/

#include "weather.h"

#define THIS_FILE "players/snake/obj/watch"

#define NAME       call_other(this_player(),"query_name"      )
#define POSSESSIVE call_other(this_player(),"query_possessive")

object master;                                                                  
int date, year, hours, minutes, seconds, time_count;                            
string weekday, month;                                                          
int alarm_set, alarm_hour, alarm_minute, delay;                                 
string alarm_string;                                                            

id(str) { return str=="watch"; }


query_auto_load() { return THIS_FILE+":"+delay; }

init_arg(arg) {
  sscanf(arg,"%d",delay);
}

show_time() {
int ntime;
int h,m,s;
  ntime=call_other(WEATHER_OBJ,"query_time");
  h=ntime/3600; m=ntime/60%60; s=ntime%60;
  if (h<10) h="0"+h; if (m<10) m="0"+m; if (s<10) s="0"+s;
  write("Time in Nemesis : "+h+":"+m+":"+s+"\n");
  write("Time in Munich  : "+ctime(time())+"\n");
  if (delay)
  write("Your time       : "+ctime(time()+3600*delay)+"\n");
  if(alarm_set) write("Alarm time: "+alarm_hour+":"+alarm_minute+"\n");
  say(NAME+" looks at "+POSSESSIVE+" watch.\n");
  return 1;
}
                                                                                
get()  { return 1; }

drop(silent) {
    if (query_verb() != "drop") return 1;
    if (!silent)
       write("As you drop the watch it bursts into thousands pieces.\n");
    destruct(this_object());
    return 1;
}

short() { return "a golden watch"; }                                      

long() {                                                                        
    write("*---- The modern Alarm watch for Adventurers ----*\n");
    write("|                                                |\n");
    write("|                    USAGE                       |\n");
    write("|                                                |\n");
    write("|  TIME      shows you the current time          |\n");
    write("|            of Nemesis, of Munich               |\n");
    write("|            and your set time zone              |\n");
    write("|  SET ALARM HH:MM <alarm_string>                |\n");
    write("|            sets the alarm time to HH:MM        |\n");
    write("|            and the alarm-text.                 |\n");
    write("|  CANCEL ALARM                                  |\n");
    write("|            cancels the Alarm-time              |\n");
    write("|  SET ZONE <delay>                              |\n");
    write("|            adjusts the watch to other time     |\n");
    write("|            zones (delay from -12 to +12)       |\n");
    write("|            If set, alarm uses this time.       |\n");
    write("*------------------------------------------------*\n");
}

update_watch() {
int dummy;
     dummy = sscanf(ctime(time()+(delay*3600)),"%s %s %d %d:%d:%d %d",          
     weekday,month,date,hours,minutes,seconds,year);                            
}                                                                               
                                                                                
reset(arg) {                                                                    
     if(arg) return;                                                            
     delay=0;
}                                                                               
                                                                                
init() {                                                                        
     add_action("set_watch","set");                                           
     add_action("time","time");                                                 
     add_action("cancel","cancel");
     master = find_living(lower_case(call_other(this_player(),"query_name")));
     alarm_string = "What was it again ???";
}

time(str) {
     if(str) return;
     update_watch();
     show_time();
     return 1;
}

heart_beat() {                                                                  
     time_count++;                                                              
  /* tell("timecount: "+time_count+"\n"); */                                    
     if( time_count < 25 ) return;                                              
     if(!alarm_set) return;  /* sicherheitshalber */                            
     time_count = 0;                                                            
     update_watch();                                                            
     if( hours == alarm_hour && minutes == alarm_minute) alarm();               
     return;                                                                    
}                                                                               

tell(str) {
    if(!master) return;
    tell_object(master,str);                                                    
    return 1;
}

alarm() {
    tell("WATCH-ALARM: =======> "+alarm_string+" <=======\n");                  
    set_heart_beat(0);                                                          
    alarm_set = 0;                                                              
}                                                                               

set_watch(str) {                                                                
int dummy, hh, mm, newdelay;
                                                                                
    if(!str) {                                                                  
      return 0;                                                                 
    }                                                                           
    if(extract(str,0,4) == "alarm") {
         dummy = sscanf(str,"alarm %d:%d %s",hh,mm,alarm_string);
         if(dummy < 2) {
            write("Illegal arguments.\n");                                      
            return 1;                                                           
         if(!alarm_string) alarm_string = "What should you do at this "+        
                                          "time ?";                             
         }                                                                      
         if(hh < 0 || hh > 23) {                                                
            write("Illegal alarm hour.\n");                                     
            return 1;                                                           
         }                                                                      
         if(mm < 0 || mm > 59) {                                                
            write("Illegal alarm minute.\n");                                   
            return 1;
         }
         write("Alarm set:  "+hh+":"+mm+"\n");
         alarm_hour = hh;
         alarm_minute = mm;
         alarm_set = 1;
         set_heart_beat(1);
         return 1;
    }
    if(extract(str,0,3) == "zone") {
         dummy = sscanf(str,"zone %d",newdelay);
         if(!dummy) {
            write("Illegal time argument.\n");
            return 1;
         }
         if(delay > 12 || delay < -12) {
            write("Time argument out of range.\n");
            return 1;
         }
         delay=newdelay;
         write("Time zone set to "+delay+".\n");
         return 1;
    }
    return 0;
}

cancel(str) {
    if(str == "alarm") {
         if(!alarm_set) {
            write("There was no alarm time set.\n");
            return 1;
         }
         write("Alarm canceled.\n");
         set_heart_beat(0);
         alarm_set = 0;
         return 1;
    }
    return 0;
}
