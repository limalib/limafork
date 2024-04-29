/* Do not remove the headers from this file! see /USAGE for more info. */

/*
** Tsath 2020
** Favour 
*/

#include <config/guild.h>

private
string name;
private
int tier;
private
string guild;
private
object mission_ctrl;
private
int favour;
private
int length;
private
string description;
private
int ends_at = time();

void set_favour(int f)
{
    favour = f;
}

int query_favour()
{
    return favour;
}

int query_tier()
{
    return tier;
}

void set_length(int l)
{
    length = l;
}

int query_length()
{
    return length;
}

void set_description(string d)
{
    description = d;
}

string query_description()
{
    return description;
}

void set_mission_data(int t, string n)
{
    tier = t;
    name = n;
}

string query_guild()
{
    return guild;
}

void remove()
{
    destruct();
}

int apply_favour()
{
    return 1;
}

void end_favour()
{
    remove();
}

void set_ends_at(int ea)
{
    TBUG("Ends at set to: " + ea);
    ends_at = ea;
}

int query_ends_at()
{
    return ends_at;
}

int ends_in()
{
    int left = ends_at - time();
    return left > 0 ? left : 0;
}

object query_mission_ctrl()
{
    if (!mission_ctrl)
        mission_ctrl = GUILD_D->mission_ctrl_npc(guild);
    mission_ctrl->set_active_mission(this_object());

    return mission_ctrl;
}

void channel_msg(string s)
{
    if (!guild)
        return;
    CHANNEL_D->deliver_channel(guild, query_mission_ctrl()->query_name() + ": " + s);
}

void notify_guild_members(string s)
{
    object *members = filter_array(bodies(), (
                                                 : member_array(guild, $1->guilds_belong()) != -1
                                                 :));
    members = filter_array(members, (
                                        : present("guild_artefact_ob", $1) && member_array(guild, present("guild_artefact_ob", $1)->query_guilds()) != -1
                                        :));
    tell(members, s);
}

void artefact_message(string m)
{
    notify_guild_members("Your "+GUILD_ARTEFACT+" twirples, \"%^GREEN%^" + upper_case(m) + ".%^RESET%^\".\n");
}

void setup(string g)
{
}

void create(string g)
{
    guild = g;
    setup(g);

    if (!clonep())
        return;
    query_mission_ctrl();
    call_out("artefact_message",
             2,
             guild + " new buff \"" + name + "\"");
}