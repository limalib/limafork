/*
** Ship_d for handling player ship.
** Tsath, 2023
**
** This works in conjunction with
**    /std/ship
**    ^common/item/docking_terminal
**    /obj/mudlib/docking_menu
**
*/

inherit M_DAEMON_DATA;
inherit SPACE_CLASSES;

// How often do we check if we should bill people
#define BILL_CYCLE_HOURS 2
// Default bank used if nothing custom set for player
#define DEFAULT_BANK "omega"

// We don't auto bill unless bill is over this amount.
#define BILL_OVER 100.0
// Ship states, don't touch
#define SHIP_PERSISTED 1
#define SHIP_DIRTY 2

/* No save*/
private
nosave mapping ship_sizes = ([]);

/* Saved*/
private
mapping ship_locations = ([]);
private
mapping owners = ([]);
private
mapping bank_connection = ([]);
private
int next_id = 0;
int money_made = 0;
int money_lost = 0;

string ship_name(string type)
{
   return upper_case(type[0..1]) + chr(random(25) + 65) + chr(random(25) + 65) + "-" + random(100000);
}

string random_large_ship()
{
   string *ship_types =
       ({"Starcruiser", "Galactic Carrier", "Void Frigate", "Nebula Dreadnought", "Solar Destroyer",
         "Celestial Battleship", "Interstellar Transporter", "Cosmic Liner", "Deep Space Freighter", "Astrohauler"});
   return choice(ship_types);
}

// This function begins a new lease. The file must the base_name of the file of
// the ship.
varargs int set_owner(object owner, string file, string sname, string ss, string loc, string da, int lt)
{
   string name = lower_case(owner->query_name());
   class ship_info si = new (class ship_info);
   object ship_object = load_object(file);

   if (!owners[name])
      owners[name] = ([]);

   if (!ship_object)
      return 0;

   si->vfile = file + "/" + name + "/" + next_id;
   si->type = ship_object->query_ship_type();
   si->name = sname || ship_name(si->type);
   si->starsystem = ss || "Omega";
   si->location = loc || "Omega Terminal";
   si->docked_at = da;
   si->long_term = lt || time();

   next_id++;
   owners[name][si->name] = si;
   save_me();
   return 1;
}

// Cancels a ship, no questions asked. Any checks if this is allowed must be
// asked before this function.
int cancel_ship(object owner, string ship)
{
   string name = lower_case(owner->query_name());
   string save_file;
   if (!owners[name])
      return 0;
   owners[name] -= ({ship});
   if (!sizeof(!owners[name]))
      map_delete(owners, name);
   save_file = sprintf("/data/ship/%c/%s.o", name[0], replace_string(ship[1..], "/", "_"));
   TBUG(save_file);
   unguarded(1, ( : rm, save_file:));
   save_me();
   return 1;
}

// Sets the connection the player wants to use as a bank.
void set_bank_connection(string bank, object player)
{
   string name = lower_case(player->query_name());
   bank_connection[name] = bank;
   save_me();
}

// A quick notification to the owner via his pager if he has one. If not, tough.
void notify_owner(mixed who, string s)
{
   object body = objectp(who) ? who : find_body(who);
   if (body && present("guild_vdu_ob", body))
   {
      tell(body, "Your VDU says, \"<048>" + upper_case(s) + "<res>\".\n");
   }
}

// Return all the bank connections we have. Mostly for debug purposes.
mapping query_bank_connections()
{
   return bank_connection;
}

// Returns the bank connection of a player. If no connection is set, set it to
// DEFAULT_BANK.
string query_bank(object player)
{
   string name = lower_case(player->query_name());
   if (!bank_connection[name])
      bank_connection[name] = DEFAULT_BANK;
   return bank_connection[name];
}

// Sets the entry point to each ship. This is used by the ship terminals
// to figure out what department resides behind it.
int add_ship_location(string entry, string file)
{
   ship_locations[entry] = file;
   save_me();
}

string ship_filename(string virtual_path)
{
   return "/" + implode(explode(virtual_path, "/")[0.. < 3], "/");
}

// Returns the full name of any ship owned by owner at the location.
varargs class ship_info *query_owned_ship(object owner, string location)
{
   string lcname = lower_case(owner->query_name());
   class ship_info *infos = ({});
   if (!lcname || !owners[lcname])
      return 0;

   foreach (string name, class ship_info si in owners[lcname])
   {
      if (si && strsrch(si->location, location || owner->query_location()) == 0)
         infos += ({si});
   }
   return infos;
}

// Returns all ship location pairs.
mapping query_ship_locations()
{
   return ship_locations;
}

// Returns the ship filename based on the street location. Basically a single
// query version of query_ship_locations().
string query_ship_filename(string entry)
{
   return ship_locations[entry];
}

// This function handles persistence of ships if needed and removal of the
// in-memory ship objects. It tracks the state of the ship so it know if it
// should remove and save or just remove.
void attempt_clean_up(object ship)
{
   if (!ship)
      return;
   if (ship->query_state() == SHIP_DIRTY)
      return;
   if (ship->is_ship_empty() && ship->query_state() == SHIP_PERSISTED)
      ship->remove();
   else
      call_out("attempt_clean", 60, ship);
}

// The actual saving of the ship is begun from here due to permissions on each
// single ship file is not high enough to initiate a save to /data/ship/
// (permission denied).
void save_ship_state(object ship)
{
   string ship_save = ship->save_things_to_string();
   ship->unguarded_save();
   call_out("attempt_clean_up", 60, ship);
}

// When a player reenters their ship, we need to restore the state from disk.
// Much like saving, the ships have no permission to do this, so the SHIP_D
// initiates the restore.
void restore_ship_state(object ship)
{
   string rfile;

   rfile = unguarded(1, ( : read_file, ship->save_to() :));
   if (rfile)
      ship->restore_ship(rfile);
}

mapping query_owners()
{
   return copy(owners);
}

int docking_time(string type)
{
   int sz = ship_sizes[type] ? ship_sizes[type] : 1;
   return time() + ((random(60) + random(60)) * sz);
}

mapping update_ship_sizes()
{
   string *ship_files = get_dir("/domains/common/ship/*.c");

   foreach (string ship in ship_files)
   {
      object ship_ob = load_object("/domains/common/ship/" + ship);
      if (!ship_ob)
         continue;

      ship_sizes[ship_ob->query_ship_type()] = ship_ob->query_ship_size() || 1;
   }
   return copy(ship_sizes);
}

/*
    This function creates a mapping of name : amount pairs for all the money
    owed by the leasers.

    This only works if bills are settled before renting new ships or when
    leaving the lease. It assumes all properties are held since last payment,
    which means property changes must cause a settlement of some kind.
*/
varargs mapping create_bills(int noCut)
{
   mapping bills = ([]);
   update_ship_sizes();

   foreach (string who, mapping ships in owners)
   {
      // Postpone payments if a week didn't pass
      foreach (string name, class ship_info si in ships)
      {
         float cost;
         int ship_size = ship_sizes[si->type] ? ship_sizes[si->type] : 1;
         class spacestation_config sc = SPACESTATION_D->query_config(si->location);
         if (si->long_term)
            cost = (ship_size * sc->storage_fee) * ((time() - si->long_term) / 86400.0);
         bills[who] = bills[who] + cost;
      }

      if (!noCut && bills[who] < BILL_OVER)
         map_delete(bills, who);

      return bills;
   }
}

int not_long_term_parked(string ship_name)
{
   string lcname;
   if (!this_body())
      return;
   lcname = lower_case(this_body()->query_name());
   if (owners[lcname] && owners[lcname][ship_name])
   {
      owners[lcname][ship_name]->long_term = 0;
      save_me();
      return 1;
   }
   return 0;
}

int long_term_parked(string ship_name)
{
   string lcname;
   if (!this_body())
      return;
   lcname = lower_case(this_body()->query_name());
   if (owners[lcname] && owners[lcname][ship_name])
   {
      owners[lcname][ship_name]->long_term = time();
      save_me();
      return 1;
   }
   return 0;
}

int dock_ship(string ship_name, string location)
{
   string lcname = lower_case(this_body()->query_name());
   if (owners[lcname] && owners[lcname][ship_name])
   {
      owners[lcname][ship_name]->docked_at = location;
      save_me();
      return 1;
   }
   return 0;
}

int undock_ship(string ship_name)
{
   string lcname;
   if (!this_body())
      return;
   lcname = lower_case(this_body()->query_name());
   if (owners[lcname] && owners[lcname][ship_name])
   {
      owners[lcname][ship_name]->docked_at = 0;
      save_me();
      return 1;
   }
   return 0;
}

string query_docked(string ship_name)
{
   string lcname;
   if (!this_body())
      return;
   lcname = lower_case(this_body()->query_name());
   if (owners[lcname] && owners[lcname][ship_name])
   {
      return owners[lcname][ship_name]->docked_at;
   }
   return 0;
}

void settle_player(string name)
{
   foreach (string shipname, class ship_info si in owners[name])
   {
      owners[name][shipname]->long_term = time();
   }
}

class ship_info find_ship(string vfile)
{
   string owner = explode(vfile, "/")[ < 2];

   // Handle virtual server ships.
   if (owner == "server")
   {
      object ship = load_object(vfile);
      class ship_info si = new (class ship_info);

      si->vfile = vfile;
      si->name = ship->ship_name();
      si->type = ship->query_ship_type();
      si->starsystem = ship->query_starsystem();
      si->location = ship->query_location();
      return si;
   }

   foreach (string ship_name, class ship_info si in owners[owner])
   {
      if (si->vfile == vfile)
         return si;
   }
   return 0;
}

float outstanding_fees(string name)
{
   string lcname = lower_case(this_body()->query_name());
   float credit;
   class spacestation_config sc;
   class ship_info si = owners[lcname][name];
   int ship_size = ship_sizes[si->type] || 1;
   string location = si->location;
   if (!si->long_term)
      return 0.0;
   sc = SPACESTATION_D->query_config(location);
   credit = (ship_size * sc->storage_fee) * ((time() - si->long_term) / 86400.0);
   return credit;
}

// This function attempts to settle a bill for 'name' of 'amount'. It returns 0
// or 1 depending on the success of this. If the person has no default bank, we
// set it to abacus.
int settle_bill(string name, int amount)
{
   string bank = bank_connection[name];
   int account_value;

   // Set a default bank, if player has none
   if (!bank)
   {
      bank = DEFAULT_BANK;
      bank_connection[name] = bank;
   }

   // How much do we have in the bank
   if (ACCOUNT_D->coverage(bank, name, amount, "credit"))
   {
      ACCOUNT_D->withdraw(bank, name, amount, "credit", "Docking services");
      money_made += amount;
      save_me();
      return 1;
   }
   return 0;
}

void pay_dock_ship(object body, int fee, class ship_info si, string bank)
{
   ACCOUNT_D->withdraw(bank, body, fee, "credit", "Docking at " + this_body()->query_location());
   money_made += fee;
   notify_owner(this_body(), "Docking and storage paid: " + pround(fee, 2) + " ¤.");
   not_long_term_parked(si->name);
}

//  This function runs every BILL_CYCLE_HOURS hours in a call_out. It collects
//  money if people are above 1 week and sends them a pager notification on what
//  happened.
void schedule_bills()
{
   // Simple create_bills(0) so we just get whoever needs to pay after this week.
   mapping bills = create_bills();
   call_out("schedule_bills", (3600 * BILL_CYCLE_HOURS));

   // Iterate through them
   foreach (string who, int amount in bills)
   {
      // If for some reason no amount? Skip
      if (!amount)
         continue;

      // Try to settle the bill
      if (settle_bill(who, amount))
      {
         notify_owner(who, "Ship services paid " + pround(amount, 2) + " crd. Long term services renewed");
         settle_player(who);
      }
      else
         notify_owner(who, "Failed to pay " + pround(amount, 2) + " crd for services. Find a docking terminal.");
   }
}

string stat_me()
{
   string retstr;
   retstr = "Ship daemon stats\n-----------------\n";
   retstr += sprintf("%-25.25s: %-16.16s %-25.25s: %-10.10s\n", "Ships owned",
                     sizeof(flatten_array(values(SHIP_D->query_owners()))) + "", "Customers ever",
                     sizeof(keys(SHIP_D->query_bank_connections())) + "");
   retstr += sprintf("%-25.25s: %-16.16s %-25.25s: %-10.10s\n", "Money made", "¤ " + money_made, "Money lost ",
                     "" + "¤ " + pround(0.0 + money_lost, 2));
   retstr += sprintf("%-25.25s: %-16.16s %-25.25s: %-10.10s\n", "Outstanding money",
                     "¤ " + pround(0.0 + array_sum(values(create_bills(1))), 2), "", "");

   return retstr;
}

// Initial creation of this daemon. Starts the call_out() and sets privilges.
void create()
{
   ::create();
   set_privilege("Mudlib:daemons");
   call_out("schedule_bills", (3600 * BILL_CYCLE_HOURS));
   update_ship_sizes();
}

/*
@filter_array(objects(),(: strsrch(base_name($1),"spacecruiser/")!=-1 :))
ud `SHIP_D`
*/