inherit INDOOR_ROOM;

void setup()
{
   set_area("omega_terminal", "landing_term");
   set_brief("Galactic Gadgets");
   set_long("The Galactic Gadgets Electronics store itself is a vibrant hub of innovation, with holographic displays "
            "showcasing the latest wearable tech for cosmic nomads.");
   set_listen("The subtle hum of advanced machinery underscores the atmosphere, reinforcing the station's commitment "
              "to cutting-edge financial services amid the vastness of space.");

   add_item("floor", (["look":"The floor, perhaps made of synthetic materials, maintains a spotless sheen, reflecting "
                              "the ambient light. "]));
   add_item("displays", "hovering holographic displays", "display", "holographic displays",
            (["look":"Hovering holographic displays present the latest in wearable tech. Use 'list' to see what is for "
                     "sale."]));

   set_objects((["../../npc/floor8/zara_nebulon.c":1]));
   set_exits((["east":"s_hall2"]));

   set_enter_msg("east",
                 ({"$N $vslide through the automatic doors the last moment before they close.",
                   "$N $vexit through the automatic doors south.", "$N $vleave east through the automatic doors."}));
   set_exit_msg("east", ({"$N $varrive through the automatic doors.", "$N $vwalk in.",
                          "$N $vare greeted by a robotic assistant named Spark, whose main job is to recommend "
                          "gadgets with an exaggerated electronic buzz and a display of blinking LED lights."}));

   set_combat_forbidden(1);
}

/*
Welcome to "Galactic Gadgets," the premier electronics store for interstellar travelers! Run by Zara Nebulon, a
quirky and enthusiastic space explorer with neon-colored hair that seems to defy gravity, the store is a cosmic
delight. Zara's attire, a mix of futuristic fashion and retro space-themed accessories, includes a pair of levitating
sneakers that she insists are the latest trend in zero-gravity chic.

The store itself is a vibrant hub of innovation, with holographic displays showcasing the latest wearable tech for
cosmic nomads. Zara, known for her infectious energy, often hypes up products with catchphrases like, "Jump to light
speed in style with our Levitation Lenses!" or "Stay connected across galaxies with our Nebula Navigators – the
must-have accessory for any spacefarer!"

Customers are greeted by a robotic assistant named Spark, whose main job is to recommend gadgets with an exaggerated
electronic buzz and a display of blinking LED lights. Amid the high-tech merchandise, you might even spot a humorous
sign that reads, "Our anti-gravity socks: Because tripping in space is still a thing."

Galactic Gadgets is more than a store; it's an experience, where cosmic tech meets a dash of intergalactic humor, and
Zara Nebulon ensures every customer leaves with a smile and the latest in wearable tech fashion for their cosmic
journeys.


*/