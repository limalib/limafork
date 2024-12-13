#include <menu.h>

inherit OBJ;

void mudlib_setup()
{
   ::mudlib_setup();
   set_id("control terminal", "terminal");
   set_in_room_desc("A maintenance control terminal.");
}

mixed direct_use_obj()
{
   return 1;
}

private
string panel_comms(mixed s)
{
   return "%^DIGITAL_DISPLAY%^[MASTER COMMS RELAY] - Subpanel code " + upper_case("" + s) + "<res>\n";
}

string *quest_codes()
{
   string *rooms = ({"maint2", "maint6", "maint7", "maint12"});
   string *codes = ({});

   foreach (string r in rooms)
   {
      object room = load_object("/domains/omega/room/floor1/" + r);
      object panel = present("communication panel", room);
      codes += ({panel->query_code()});
   }
   return codes;
}

string long()
{
   string l = "This control terminal shows a few simple numbers on the screen:\n";
   string *codes = quest_codes();

   foreach (string c in codes)
   {
      l += panel_comms(c);
   }

   return l;
}