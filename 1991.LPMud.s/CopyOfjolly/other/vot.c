inherit "obj/treasure";

#define NAME call_other(this_player(),"query_name")
#define LEVEL call_other(this_player(),"query_level")
#define SAVE_NAME "players/alaric/obj/vmachine"

string topic;
int num_entries;
object entries;
object paths;
object votes;
int num_votes;
object points;
object desc;

init() {
  ::init();
  set_id("voting machine");
  set_alias("machine");
  set_short("a voting machine");
  set_long("This is a very sophisticated voting machine.\n"+
"It offers the following commands:\n"+
"    topic               : show the current topic for voting\n"+
"    read entries        : read all the entries you can vote for\n"+
"    show <num>          : show addtional information about entry <num>\n"+
"    vote <num>          : vote for entry <num>\n"+
"    statistics          : show how many points each entry has\n"+
"    settopic <str>      : set the topic to <str> (Level >24 required)\n"+
"    newentry <str>      : make a new entry (Level >20 required)\n"+
"    chentry <num> <str> : change entry <num> (Level >20 required)\n"+
"    newinfo <num> <str> : give a filename <str> to show for entry <num>\n"+
"    help voting         : give a few guidelines for usage of the machine\n"
);
  set_weight(200);
  add_action("do_take","take");
  add_action("do_take","get");
  add_action("set_topic","settopic");
  add_action("write_topic","topic");
  add_action("new_entry","newentry");
  add_action("do_read","read");
  add_action("new_info","newinfo");
  add_action("do_show","show");
  add_action("do_vote","vote");
  add_action("show_stats","statistics");
  add_action("do_help","help");
  add_action("change_entry","chentry");
}

reset(arg)
{
  int i;
  if(arg)
    return;
  restore_object(SAVE_NAME);
  if(!entries)
  {
    entries=allocate(20);
    for(i=0;i<20;i++)
    {
      entries[i]=allocate(2);
      entries[i][0]="";
      entries[i][1]="";
    }
  }
  if(!paths)
  {
    paths=allocate(20);
    for(i=0;i<20;i++)
      paths[i]="";
  }
  if(!num_entries)
    num_entries=0;
  if(!votes)
  {
    votes=allocate(100);
    for(i=0;i<100;i++)
    {
      votes[i]=allocate(2);
      votes[i][0]="";
      votes[i][1]=0;
    }
  }
  if(!num_votes)
    num_votes=0;
  if(!points)
  {
    points=allocate(20);
    for(i=0;i<20;i++)
      points[i]=0;
  }
  if(!desc)
  {
    desc=allocate(20);
    for(i=0;i<20;i++)
      desc[i]=0;
  }
}

do_take(str)
{
  if((str=="machine")||(str=="voting machine"))
  {
    write("You can't take it. It's too heavy !\n");
    return 1;
  }
  else
    return 0;
}

set_topic(str)
{
  if(LEVEL< 50)
  {
    write("You are not high enough of level !\n");
    return 1;
  }
  if(!str)
    write("You must supply a topic !\n");
  else
  {
    do_clean();
    write("You set todays topic to : "+str+"\n");
    say(NAME+" sets todays topic to : "+str+"\n");
    topic=str;
    save_object(SAVE_NAME);
  }
  return 1;
}

string query_topic()
{
  return topic;
}

write_topic()
{
  write("Todays topic is : "+topic+"\n");
  say(NAME+" examines todays topic.\n");
  return 1;
}

set_entry(num,str)
{
  entries[num][0]=str;
  entries[num][1]=NAME;
  save_object(SAVE_NAME);
}

query_entry(num)
{
  return entries[num][0];
}

int query_num_entries()
{
  return num_entries;
}

new_entry(str)
{
  if(LEVEL < 21)
  {
    write("You are not high enough of Level !\n");
    return 1;
  }
  if(!str)
  {
    write("You must supply a string for the entry !\n");
    return 1;
  }
  if(num_entries >= 20)
  {
    write("I'm sorry, the maximum capacity of entries is already reached.\n");
    return 1;
  }
  write("You set the entry nr. "+num_entries+" to "+str+"\n");
  say(NAME+" sets a new entry into the voting machine.\n");
  entries[num_entries][0]=str;
  entries[num_entries][1]=NAME;
  num_entries=num_entries+1;
  save_object(SAVE_NAME);
  return 1;
}

do_read(str)
{
  int i;
  if(str!="entries")
    return 0;
  write("Todays topic is "+topic+"\n");
  if(num_entries==0)
  {
    write("There are no current entries.\n");
    return 1;
  }
  write("You examine the entries:\n");
  for(i=0;i<num_entries;i++)
    write("Entry Nr. "+i+" is "+entries[i][0]+" by "+entries[i][1]+"\n");
  say(NAME+" examines the entries for voting.\n");
  return 1;
}

new_info(str)
{
  int num;
  string fname;
  if(LEVEL < 21)
  {
    write("You are not high enough of Level !\n");
    return 1;
  }
  if(sscanf(str,"%d %s",num,fname)==2)
  {
    fname="/."+fname;
    if(file_size(fname)==-1)
    {
       write("This file does not exist !\n");
       return 1;
    }
    write("You set the infopath for entry Nr. "+num+" to "+fname+".\n");
    say(NAME+" sets a new infofile into the voting machine.\n");
    paths[num]=fname;
    save_object(SAVE_NAME);
    return 1;
  }
  else
    write("Usage: newinfo <num> <pathname>\n");
  return 1;
}

do_show(num)
{
  int inum;
  sscanf(num,"%d",inum);
  if(paths[inum]=="")
  {
    write("There is no info to entry Nr. "+inum+"\n");
    return 1;
  }
  write("Info to entry Nr. "+inum+" :\n");
  cat(paths[inum]);
  say(NAME+" looks at an infofile in the voting machine.\n");
  return 1;
}

do_vote(str)
{
  int i,pln,vnum,found;

/* by jolly */
if(this_player()->query_level()<5)
{
	write("Sorry. Only players above Level 5 are permitted to vote.\n");
	return 1;
}
  found=0;
  if(!str)
  {
    write("You must supply a number to vote for !\n");
    return 1;
  }
  sscanf(str,"%d",vnum);
  if((vnum<0)||(vnum>num_entries))
  {
    write("You can only vote for existing entries !\n");
    return 1;
  }
  for(i=0;i<100;i++)
    if(votes[i][0]==NAME)
    {
      pln=i;
      found=1;
    }
  if(found&&(votes[pln][1]>=3))
  {
     write("You have already given all three votes.\n"); 
     return 1;
  }
  if(!found)
  {
    if(num_votes>=100)
    {
      write("Maximum voters reached. I'm sorry.\n");
      return 1;
    }
    pln=num_votes;
    votes[pln][0]=NAME;
    votes[pln][1]=0;
    num_votes++;
  }
  points[vnum]+=3-votes[pln][1];
  votes[pln][1]++;
  write("You have given "+(4-votes[pln][1])+" points to entry Nr. "+vnum+".\n");
  say(NAME+" has voted for an entry of the voting machine.\n");
  save_object(SAVE_NAME);
  return 1;
}

show_stats()
{
  int i,j,k;
  k=0;
  for(i=600;i>=0;i--)
  {
    for(j=0;j<num_entries;j++)
      if(points[j]==i)
      {
        desc[k]=j;
        k++;
      }
  }
  write("The statistics are :\n");
  for(i=0;i<num_entries;i++)
    write("Entry Nr. "+desc[i]+" "+entries[desc[i]][0]+" by "+entries[desc[i]][1]+" "+points[desc[i]]+" points.\n");
  say(NAME+" looks at the voting statistics.\n");
  save_object(SAVE_NAME);
  return 1;
}

do_help(str)
{
  if(str=="voting")
  {
    write("Here is how you vote:\n\n"+
"You look at the entries and maybe the infofiles and choose the 3 you like\n"+
"best. Then you give the command vote <num> 3 times. Once for every entry\n"+
"you like. The first will get 3 points, the second 2 points and the third\n"+
" 1 point. Then you can look at the statistics to see the entries in\n"+
"descending order of points.\n");
    return 1;
  }
  else
    return 0;
}

do_clean()
{
  int i;

    entries=allocate(20);
    for(i=0;i<20;i++)
    {
      entries[i]=allocate(2);
      entries[i][0]="";
      entries[i][1]="";
    }
    paths=allocate(20);
    for(i=0;i<20;i++)
      paths[i]="";
    num_entries=0;
    votes=allocate(100);
    for(i=0;i<100;i++)
    {
      votes[i]=allocate(2);
      votes[i][0]="";
      votes[i][1]=0;
    }
    num_votes=0;
    points=allocate(20);
    for(i=0;i<20;i++)
      points[i]=0;
    desc=allocate(20);
    for(i=0;i<20;i++)
      desc[i]=0;
}

change_entry(str)
{
  int num;
  string text;
  if(!str)
  {
    write("You must supply an entry number !\n");
    return 1;
  }
  sscanf(str,"%d %s",num,text);
  if((num<0)||(num>num_entries))
  {
    write("Illegal entry number !\n");
    return 1;
  }
  if(entries[num][1]!=NAME)
  {
    write("You can only change your own entries !\n");
    return 1;
  }
  if(points[num]!=0)
  {
    write("Somebody has already voted for this entry.\n");
    write("It cannot be changed anymore !\n");
    return 1;
  }
  entries[num][0]=text;
  write("You have changed entry Nr. "+num+" to "+text+".\n");
  say(NAME+" changes an entry in the voting machine.\n");
  save_object(SAVE_NAME);
  return 1;
}
