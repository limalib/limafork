inherit INDOOR_ROOM;

void setup()
{
   set_area("omega_terminal", "landing_term");
   set_brief("Omega Galactic Bank");
   set_long("The banking office of Omega Galactic stands as a futuristic nexus of finance within the "
            "cosmic expanse. The interior is a symphony of sleek, metallic surfaces and cutting-edge technology. "
            "Luminescent panels embedded in the walls cast a soft, cool glow, creating an atmosphere of modern "
            "sophistication. A series of hovering holographic displays andtransparent counters, constructed from "
            "durable yet aesthetically pleasing materials, facilitate secure and transparent transactions.");
   set_smell("The air carries a faint scent of fake floral perfumes.");
   set_listen("The subtle hum of advanced machinery underscores the atmosphere, reinforcing the station's commitment "
              "to cutting-edge financial services amid the vastness of space.");

   add_item("floor", (["look":"The floor, perhaps made of synthetic materials, maintains a spotless sheen, reflecting "
                              "the ambient light. "]));
   add_item("displays", "hovering holographic displays", "display", "holographic displays",
            (["look":"Hovering holographic displays present real-time financial data and market trends, captivating "
                     "the attention of clients."]));

   set_objects((["../../npc/floor8/alexia_novak.c":1]));
   set_exits((["east":"s_hall3"]));
   set_exit_msg("east",
                ({"$N $vslide through the automatic doors the last moment before they close.",
                  "$N $vexit through the automatic doors south.", "$N $vleave east through the automatic doors."}));
   set_enter_msg("east", ({"$N $varrive through the automatic doors.", "$N $vwalk in."}));
   set_combat_forbidden(1);
}
