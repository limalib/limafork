#include <menu.h>

inherit OBJ;
inherit M_OPENABLE;
inherit M_INPUT;

string code;
string panel_id;
object body;

void reset_code()
{
   code = (1 + random(9)) + "" + random(10) + "" + random(10) + "" + random(10) + "";
}

string query_code()
{
   return code;
}

string panel_id()
{
   return panel_id;
}

void when_opened()
{
   write("A sleek touchscreen display beneath is revealed. The interface comes to life with a soft hum "
         "and a vibrant holographic display upon exposure.");
}

void mudlib_setup(string pid)
{
   ::mudlib_setup();
   panel_id = pid;
   set_id("communication panel", "panel", "casing");
   set_in_room_desc("A maintenance communication panel");
   set_long("A communication panel that seems to be able to open.");
   set_attached(1);
   add_hook("open", ( : when_opened:));
   reset_code();
}

mixed direct_use_obj()
{
   if (body && !present(body, environment()) || body == this_body())
      body = 0;

   return query_closed() ? "#The panel is closed." : (body ? body->query_name() + " is already using that panel." : 1);
}

mixed direct_close_obj()
{
   if (body && !present(body, environment()) || body == this_body())
      body = 0;

   return body ? body->query_name() + " is using that panel, so you cannot close it." : 1;
}

void stop_using()
{
   body = 0;
}

private
void panel_comms(mixed s)
{
   write("%^DIGITAL_DISPLAY%^[COMM (" + code + ")] - " + upper_case("" + s) + "<res>\n");
}

varargs void panel_info(string s)
{
   if (s)
      panel_comms(s);
   panel_comms("set code");
}

void new_code(string c)
{
   int co = to_int(c);

   if (this_body() != body)
   {
      panel_comms("aborted");
      return;
   }

   if (c == "q" || c == "Q")
   {
      panel_comms("user logout");
      this_body()->do_game_command("close panel");
      stop_using();
      return;
   }
   if (co < 999)
   {
      panel_info("invalid code - press Q to quit");
      modal_simple(( : new_code:));
      return;
   }
   code = "" + co;
   panel_comms("new code accepted");
   panel_comms(co);
   modal_simple(( : new_code:));
}

void do_use()
{
   if (body)
   {
      if (!present(body, environment()))
         body = 0;
   }

   body = this_body();
   body->simple_action("The panel lights up as $n $vpress it.");
   write("Enter new synchronization code:\n");
   call_out("panel_info", 1);
   modal_simple(( : new_code:));
}

string long()
{
   string l = "This communication panel in the maintenance section is a discreet yet essential piece of equipment, its "
              "exterior resembling a sturdy metallic casing. This protective housing, sealed with a set of secure "
              "latches, conceals a valuable touchscreen interface within. To access the communication functions, one "
              "must open the panel to reveal the touchscreen display beneath.";
   string if_opened = "";

   if (!query_closed())
   {
      if_opened = "The touchscreen says:\n\tCurrent synchronization code is: <084>" + code + "<res>";
   }

   return l + "\n\n" + m_openable::extra_long_stuff() + if_opened;
}