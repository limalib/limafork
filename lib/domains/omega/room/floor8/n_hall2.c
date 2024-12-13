inherit INDOOR_ROOM;

void setup()
{
   set_area("omega_terminal", "landing_term");
   set_brief("Stellar Lane near Nova Ship Trading");
   set_long("Bathed in a soft, artificial light that reflects off its sterile, metallic surfaces, the hallway "
            "stretches forward in clean lines. The walls, adorned with occasional access panels, bear subtle signs of "
            "wear and tear, evidence of the bustling activity that characterizes life on the station.\n"
            "Nova Ship Trading is west of here.");
   add_item("floor", "The floor, a smooth, low-gravity surface, allows for effortless movement. ");
   set_exits((["south":"n_hall3", "north":"n_hall1", "west":"n_ships"]));
   set_listen("You hear people running through the terminal. " +
              "It sounds like a busy terminal with people coming and going");
}
