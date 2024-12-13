inherit INDOOR_ROOM;

void setup()
{
    set_area("omega_terminal", "trading_term");
    // set_weather(1);
    set_light(1);
    set_brief("South Corridor");
    set_long("A Wide corridor leading to the north and south through trading area.");
    set_state_description("electronic_door_off", "closed");
    set_state_description("electronic_door_on", "open");
    set_exits((["north":"waiting_room",
                "south":"s_corridor1",
                "east" : "omega_pets",
                "west" : "omega_robotics",
    ]));
    // set_objects(([""]));
    set_listen("You hear people running through the terminal. " +
               "It sounds like a busy terminal with people coming and going");
}
