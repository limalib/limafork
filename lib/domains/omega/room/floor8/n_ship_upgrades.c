inherit INDOOR_ROOM;

void setup()
{
  set_area("omega_terminal", "landing_term");
  set_brief("Nova Ship Services");
  set_long("As you step into the Nova Ship Services nestled within the bustling corridors of the "
           "terminal, you're greeted by a soft hum of machinery and the sight "
           "of sleek, metallic spaceship components lining the shelves.\n"
           "Above the counter, a large holographic display hovers, showcasing a comprehensive list "
           "of available spaceship upgrades. Each option is accompanied by a detailed description "
           "and the corresponding price, allowing customers to make informed decisions. The list "
           "includes a variety of enhancements to boost a ship's performance, durability, and "
           "functionality, catering to the needs of different spacefarers.");
  set_smell("The air carries a faint scent of lubricants and ionized particles, a testament to the "
            "high-tech nature of the shop's offerings.");
  set_listen("There is a soft hum of machinery in the shop.");

  set_objects((["../../npc/floor8/max_nova.c":1]));
  set_exits((["east":"n_hall3"]));
  set_exit_msg("east",
               ({"$N $vslide through the automatic doors the last moment before they close.",
                 "$N $vexit through the automatic doors south.", "$N $vleave east through the automatic doors."}));
  set_enter_msg("east", ({"$N $varrive through the automatic doors.", "$N $vwalk in."}));
  set_combat_forbidden(1);
}
