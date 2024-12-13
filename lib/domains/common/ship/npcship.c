inherit SPACESHIP;

string *ship;
string instance_name;
string ship_name;
int x, y;
int setup_done;

void setup()
{
   ::setup();
   set_brief("An NPC ship");
   set_ship_long("A bulky vessel of limited speed but with ample space for hauling goods. ");
   set_ship_cost(50);
   set_ship_type("space barrow");
}

object ship_controller()
{
   return load_object(__FILE__[0.. < 3] + "/" + instance_name);
}

void create(string arg)
{
   int new_size;
   if (!arg)
   {
      ::create();
      return;
   }

   if (sscanf(arg, "%s/%d/%d/%d", instance_name, new_size, x, y) != 4)
      if (sscanf(arg, "%s/%d", instance_name, new_size) != 2)
         return 0;

   set_ship_size(new_size);
   instance_name = instance_name + "/" + new_size;
   ::create();
}

string *ship_design()
{
   if (ship_controller() == this_object())
   {
      if (!ship)
         ship = generate_ship(query_ship_size());
      return ship;
   }
   return ship_controller()->ship_design();
}

int *random_room()
{
   int x = random(sizeof(ship_design()) - 2) + 1;
   int y = random(strlen(ship_design()[0]) - 2) + 1;
   if (ship_design()[x][y] != ' ')
      return ({x, y});
   else
      return random_room();
}

string size_name()
{
   switch (query_ship_size())
   {
   case 0..3:
      return "smaller";
   case 4..6:
      return "larger";
   case 7..9:
      return "quite large";
   case 10..15:
      return "huge";
   default:
      return "monstrously sized";
   }
}

string ship_name()
{
   if (ship_controller() == this_object())
   {
      if (!ship_name)
         ship_name = SHIP_D->ship_name(query_ship_type() + instance_name);
      return ship_name;
   }
   return ship_controller()->ship_name();
}

int *coords()
{
   return ({x, y});
}

string query_instance_name()
{
   return instance_name;
}

varargs int room_type(int *coords)
{
   if (arrayp(coords))
      return ship_design()[coords[0]][coords[1]];
   if (x >= sizeof(ship_design()) || y >= strlen(ship_design()[0]))
      return ' ';
   return ship_design()[x][y];
}

void add_exits()
{
   if (room_type(({x - 1, y})) != ' ')
      add_exit("south", __FILE__[0.. < 3] + "/" + instance_name + "/" + (x - 1) + "/" + y);
   if (room_type(({x + 1, y})) != ' ')
      add_exit("north", __FILE__[0.. < 3] + "/" + instance_name + "/" + (x + 1) + "/" + y);
   if (room_type(({x, y + 1})) != ' ')
      add_exit("east", __FILE__[0.. < 3] + "/" + instance_name + "/" + x + "/" + (y + 1));
   if (room_type(({x, y - 1})) != ' ')
      add_exit("west", __FILE__[0.. < 3] + "/" + instance_name + "/" + x + "/" + (y - 1));
}

int *find_rooms_of_type(int type)
{
}

void generate_room()
{
   string l = "";

   switch (room_type())
   {
   case 'B':
      set_long(
          "The bridge of the spaceship is a dimly lit, claustrophobic space, bathed in a cold, sterile glow from "
          "flickering overhead lights. The walls are lined with aging, yellowed panels covered in switches, levers, "
          "and archaic monitors displaying indecipherable data streams. Wires and cables snake along the floor and "
          "ceiling, occasionally sparking with erratic electricity, casting eerie shadows that dance across the narrow "
          "confines.");
      add_item("overhead lights", "lights",
               "The overhead lights had better days. They seem partially destroyed due to "
               "a previous power surge or other electronic failure.");
      add_item(
          "panels", "switches", "levers", "archaic monitors", "monitors",
          "The yellowed panels are scarred with the passage of time, their surfaces cracked and discolored. Buttons "
          "and switches, some missing or stuck, protrude haphazardly, while faded labels and peeling stickers obscure "
          "their functions. Dust and grime cling to the edges, highlighting years of neglect and makeshift repairs.");
      add_item("wires", "cables", "ceiling",
               "Wires and cables snake chaotically across the bridge, their insulation frayed and patched with hasty "
               "repairs. They hang from the ceiling and trail along the floor, some sparking intermittently. Bundles "
               "are secured with makeshift ties, creating a tangled web that adds to the room's disordered and "
               "hazardous atmosphere.");
      add_item("shadows",
               "The shadows in the bridge shift and flicker with every spark and flicker of the lights, creating a "
               "sense of movement in the corners of the room. They stretch long and jagged across the floor and walls, "
               "merging with the grime and rust, amplifying the oppressive, haunted atmosphere of the space.");
      set_smell("The metallic scent of recycled air hangs heavy, mixed with the faint smell of burnt circuits and "
                "machine oil.");
      break;
   case 'H':
      set_long(
          "The hallway stretches out in a narrow, claustrophobic tunnel, its metallic walls streaked with rust and "
          "grime, casting long, sinister shadows under the dim, flickering lights. Exposed pipes and cables run "
          "haphazardly along the ceiling, occasionally dripping with unidentified fluids that pool in the crevices of "
          "the grated floor. Faint, almost imperceptible scratches and dents "
          "mar the walls, hinting at unseen struggles and the ever-present, lurking menace of the void outside.");
      add_item("rust", "walls", "grime",
               "The rust in the hallway creeps along the walls and ceiling like a spreading infection, its "
               "reddish-brown tendrils etching intricate patterns into the metal. It clusters around seams and bolts, "
               "where moisture has seeped in, forming jagged edges and flaky patches. The corrosion eats away at the "
               "integrity of the structure, giving the hallway an air of decay and abandonment.");
      add_item("lights", "flickering lights",
               "The flickering lights sputter with erratic energy, casting unreliable pools of illumination that pulse "
               "and fade unpredictably. Each sputter creates an uneasy strobe effect, momentarily plunging the room "
               "into near darkness before flaring back to life. The bulbs buzz faintly, their unstable glow struggling "
               "to pierce the pervasive gloom, heightening the sense of instability and impending malfunction.");
      add_item(
          "exposed pipes", "pipes", "exposed cables", "cables",
          "Exposed pipes and cables crisscross the hallway in a chaotic mesh, their surfaces slick with condensation "
          "and grime. The pipes, varying in diameter, show signs of corrosion and hasty repairs, with mismatched "
          "clamps and makeshift patches holding them together. Cables, some with frayed insulation, hang loosely from "
          "the ceiling and snake along the walls, occasionally sparking and emitting a faint hum. This tangle of "
          "neglected infrastructure adds to the room's atmosphere of disrepair and latent danger.");
      set_listen("Every step echoes ominously, the sound swallowed by the oppressive silence that fills the ship, "
                 "broken only by the distant, rhythmic thrum of the engines. ");
      set_smell("The air is thick with the scent of stale sweat and something more acrid, like burning "
                "plastic. ");
      break;
   case 'C':
      set_long(
          "The cargo room is a cavernous, foreboding expanse, filled with towering stacks of battered crates and metal "
          "containers, each marked with fading, cryptic labels and rusted locks. Overhead, dim "
          "emergency lights cast a sickly yellow hue, creating deep, shadowy recesses where the eye struggles to "
          "discern detail. The floor is littered with scattered debris—loose wires, shattered glass, and "
          "unidentifiable scraps of machinery—adding to the room’s oppressive atmosphere.");
      add_item("cargo", "battered crates", "crates", "containers",
               "The battered crates are haphazardly stacked, their surfaces scarred and dented from years of rough "
               "handling. The once-bright paint is chipped and faded, revealing layers of rust and bare metal "
               "underneath. Some crates are secured with makeshift patches of duct tape and metal straps, while others "
               "bear the marks of crude repairs with mismatched bolts and screws.");
      add_item("labels", "label", "faded labels",
               "Faded labels and stenciled warnings are barely legible, obscured by grime and time.");
      add_item(
          "emergency lights", "lights",
          "The emergency lights emit a dim, sickly yellow glow, flickering intermittently and casting long, ominous "
          "shadows. Encased in grimy, cracked covers, their light barely penetrates the surrounding darkness. Some "
          "lights pulse weakly, struggling to stay lit, while others hang loose from their fittings.");
      add_item("loose wires", "wire", "wires",
               "Loose wires sprawl across the floor like tangled serpents, their insulation cracked and frayed. Some "
               "twitch sporadically, emitting faint sparks and low hums. Patches of exposed metal gleam in the dim "
               "light, adding to the hazardous, unkempt ambiance of the room.");
      add_item("debris", "shattered glass", "glass", "scraps",
               "Shattered glass fragments litter the floor like scattered diamonds, glinting in the faint light. "
               "Nearby, metal scraps lie in twisted heaps, their sharp edges catching shadows. Both remnants tell "
               "tales of past collisions and hasty repairs in this forgotten corner of the ship.");

      set_smell("The air is stale and heavy, imbued "
                "with the sharp tang of metal and the faint, unsettling odor of decaying organic matter.");
      set_listen("A distant, incessant drip "
                 "echoes through the silence, amplifying the sense of abandonment and latent threat, as if the "
                 "room itself "
                 "holds memories of forgotten dangers and untold stories.");
      break;
   case 'T':
      set_long(
          "This room is a labyrinthine mess of exposed circuitry and blinking control panels, bathed in the "
          "eerie, intermittent glow of malfunctioning fluorescent lights. The walls are a patchwork of hastily patched "
          "wiring, their insulation fraying and sparking occasionally, casting erratic shadows that dance across the "
          "cluttered space. "
          "Workstations are cluttered with abandoned tools and half-finished repair jobs, the disarray a testament to "
          "the crew's constant battle against the ship's gradual decay.");
      add_item("mess", "exposed circuitry", "control panels", "panels",
               "The mess of exposed circuitry sprawls across the room, a chaotic network of wires and blinking control "
               "panels. Some panels flicker erratically, casting intermittent flashes of light onto the tangled maze "
               "of components. The air hums with the faint buzz of electricity, punctuated by occasional sparks that "
               "illuminate the cluttered workstation.");
      add_item("malfunctioning lights", "lights",
               "The malfunctioning fluorescent lights stutter and flicker with irregular pulses of harsh, pale light. "
               "Their erratic behavior casts unsettling shadows that flicker and dance across the room, contributing "
               "to the oppressive and unstable ambiance of the ship's interior.");
      add_item("walls", "wall", "Clearly not in a good state.");
      add_item("workstation", "workstations", "tool", "tools",
               "The workstations are cluttered with abandoned, broken tools and scattered schematics. Worn consoles "
               "flicker sporadically, surrounded by half-disassembled equipment and discarded components. Piles of "
               "obsolete technology and malfunctioning parts add to the chaotic scene, reflecting the ongoing struggle "
               "to maintain functionality in the ship's technic room.");
      add_item("decay", "It's not pretty. Something bad happened here.");
      set_smell("The air is thick with the acrid "
                "scent of burning electronics and ozone, mixing with the stale musk of enclosed, recycled atmosphere.");
      set_listen("A pervasive hum emanates from the overloaded power conduits, interspersed with the "
                 "crackling of overloaded circuits and the ominous groan of stressed metal.");
      break;
   case '#':
      if (random(10))
      {
         string note;
         set_long("The corridor stretches like a steel artery through the ship, its walls lined with flickering "
                  "emergency lights that cast a dim, eerie glow. Pipes and cables run along the ceiling, occasionally "
                  "dripping with condensation, while scuffed metal panels bear faded markings and hastily scrawled "
                  "maintenance notes.");
         add_item("panels", "metal panels",
                  "The panels along the corridor are weathered, their surfaces scratched and patched with mismatched "
                  "materials.");
         add_item("pipes", "pipe",
                  "Pipes and cables snake along the ceiling, some dripping with condensation, "
                  "creating a labyrinthine network that adds to the ship's industrial, utilitarian aesthetic.");
         set_smell("The air carries a faint tang of recycled atmosphere and faint echoes of distant "
                   "machinery, underscoring the ship's quiet, ominous ambiance.");
         switch (random(5))
         {
         case 0:
            note = "Pipe leak repaired - watch for residual moisture!";
            break;
         case 1:
            note = "Panel 3B malfunctioning - awaiting replacement parts.";
            break;
         case 2:
            note = "Cables inspected and secured - no anomalies detected.";
            break;
         case 3:
            note = "Emergency lights in Sector 7 flickering - investigate ASAP.";
            break;
         case 4:
            note = "Warning: High radiation levels detected in maintenance hatch 12D - proceed with caution!";
            break;
         }
         add_item("note", "notes", note);
      }
      else
      {
         set_long(
             "Medical equipment lies haphazardly on countertops, some instruments still stained from "
             "their last use, and others covered in a fine layer of dust, forgotten in the chaos. The narrow beds, "
             "each "
             "encased in thin, white sheets, are aligned against the walls, their frames creaking ominously with every "
             "vibration of the ship. Monitors flicker sporadically, casting ghostly blue light across the room, while "
             "the "
             "occasional distant beep of a life-support machine adds to the unsettling ambiance. Shadows pool in the "
             "corners, suggesting the presence of unseen threats lurking just out of sight, making the infirmary feel "
             "less like a place of healing and more like a waiting room for the inevitable.");
         add_item("medical equipment", "equipment", "instruments",
                  "The broken medical equipment in the infirmary lies dormant and neglected, some with cracked screens "
                  "and exposed wiring. Tarnished metal frames and shattered glass hint at past emergencies and failed "
                  "attempts at repair, contributing to the room's atmosphere of desolation and decay.");
         add_item(
             "countertops",
             "The countertops in the infirmary are grimy and cluttered, littered with discarded bottles of antiseptic "
             "and "
             "medical tools coated in dried blood. Streaks of unknown substances mar the surface, remnants of hurried "
             "treatments and neglected cleanliness, adding to the room's unsettling atmosphere of neglect.");
         add_item(
             "monitor", "monitors",
             "The monitors in the infirmary flicker intermittently, displaying vital signs with faded, ghostly hues "
             "against their scratched and smudged screens.");
         add_item(
             "bed", "beds",
             "The narrow beds in the infirmary are draped with thin, worn sheets, their metal frames creaking softly "
             "with each movement.");
         set_smell(
             "The air is thick with the antiseptic scent of disinfectant, struggling to mask the underlying smell of "
             "blood and sickness.");
         set_objects((["^std/junk/random_trash":1]));
      }
   }
   add_item("ceiling",
            "The ceiling looms low and oppressive, its surface marred by patches of peeling paint and streaks of rust. "
            "Exposed beams and support struts crisscross above, some showing signs of stress and wear. Interspersed "
            "among them are clusters of dangling wires and conduits, their insulation cracked and brittle.");
   add_item("rust", "It is corroded metal.");
   add_item("streaks", "Lines of rust.");
   add_item("floor", "The floor is a treacherous expanse of metal grating, slick with oil and grime, making every step "
                     "uncertain. Loose screws and scattered debris crunch underfoot, adding to the sense of neglect. "
                     "Below the grating, faint glimmers of flickering lights hint at the labyrinthine depths of the "
                     "ship's lower levels. Pools of unidentified liquid gather in shallow depressions, reflecting the "
                     "dim, erratic lighting above. The uneven, worn surface speaks to years of hurried repairs and "
                     "relentless use, amplifying the air of decay and abandonment that permeates the space.");
}

string room_type_name(int short_code)
{
   string type_name;
   switch (short_code)
   {
   case '#':
      type_name = "corridor";
      break;
   case 'B':
      type_name = "bridge area";
      break;
   case 'H':
      type_name = "hallway";
      break;
   case 'T':
      type_name = "technical area";
      break;
   case 'C':
      type_name = "cargo area";
      break;
   }

   return type_name;
}

// Override the persistance function, since this one is not persisted.
void object_arrived(object ob)
{
   if (!setup_done)
   {
      string long;
      TBUG(ob->short() + " arrived. Setting up new room.");
      add_exits();
      long = room_type_name(room_type());
      generate_room();
      set_brief(ship_name() + " - " + capitalize(long));
      setup_done = 1;
   }
}

// Must be in a spaceship to handle the virtualization
object virtual_create(string arg)
{
   return new (__FILE__, arg);
}

void stat_me()
{
   for (int l = sizeof(ship_design()) - 1; l >= 0; l--)
   {
      string line = ship_design()[l];
      if (x == l)
         line = line[0..y - 1] + "<120>" + line[y..y] + "<res>" + line[y + 1..];
      write(line);
   }
}