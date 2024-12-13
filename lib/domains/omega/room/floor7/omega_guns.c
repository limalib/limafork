inherit INDOOR_ROOM;

void setup()
{
    set_area("omega_terminal", "trading_term");
    // set_weather(1);
    set_light(1);
    set_brief("Omega Gun Trading Company");
    set_long("The Omega Trading Company is a veritable treasure trove of weapons and technology. Rows upon rows of gleaming guns and rifles were on display, each one more advanced and deadly than the last. There are energy weapons, plasma cannons, and even experimental weapons that were still in the prototype stage.\n"
             "The store is ran by a group of technologically advanced alien species, who were always eager to show off their latest creations. They are always happy to answer any questions and give demonstrations of the weapons' capabilities.\n"
             "Customers can also purchase various upgrades and attachments for their weapons, such as scope, silencer, and even a mini-rocket launcher.");
    set_objects((["/domains/common/obj/auto_door":({"east", "n_corridor"}),
    ]));
    set_listen(".");

    set_objects((["^omega/npc/hugo.c":1,
    ]));
}
