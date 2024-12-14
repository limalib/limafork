/* Do not remove the headers from this file! see /USAGE for more info. */

/*
 * Frame layout module for fancy frames.
 * Tsath 2022/2023. Heavily inspired by frame layouts done by Diavolo@Merentha.
 */

//: MODULE
// This module creates fancy and colourful frames using unicode characters
// and 256 colour based from XTERM256_D.
//
// .. TAGS: RST

#define TRD 0  // ┌ Right down
#define TH 1   // ─ Horisontal
#define THD 2  // ┬ Horisontal down
#define TLD 3  // ┐ Left down
#define TV 4   // │ Vertical
#define TVR 5  // ├ Vertical right
#define TX 6   // ┼ Cross
#define TVL 7  // ┤ Vertical left
#define TRU 8  // └ Right up
#define THU 9  // ┴ Horisontal up
#define TLU 10 // ┘ Left up

#define FALLBACK_STYLE "ascii"
#define COL_GRADIENT 0
#define COL_TITLE 1
#define COL_ACCENT 2
#define COL_WARNING 3

// Frame themes are defined in here, and can be edited using 'admtool'.
#include <frame_themes.h>

private
nosave mapping styles = (["single":({"┌", "─", "┬", "┐", "│", "├", "┼", "┤", "└", "┴", "┘"}),
                          "double":({"╔", "═", "╦", "╗", "║", "╠", "╬", "╣", "╚", "╩", "╝"}),
                           "ascii":({"+", "-", "-", "+", "|", "|", "|", "|", "+", "-", "+"}),
                           "lines":({"-", "-", "-", "-", " ", " ", "-", " ", "-", "-", "-"}),
                            "none":({" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "})]);

private
nosave string *bits;

/* Strings */
private
string title, header_content, footer_content, *content, style;
/* Ints */
private
nosave int width,  // Width of the frame, default user screen width
    title_margin,  // Margin the header takes from left side, default 2
    text_margin,   // Margin around header text
    section_width; // Width of sections in menus.
/* Booleans*/
private
nosave int add_header, add_footer, left_header;

// Colour configuration storage.
private
nosave string *hcolours;

private
nosave string *sections;

private
mapping columns = ([]);
private
mapping column_functions = ([]);
private
string *column_order = ({});
private
mapping column_width = ([]);
private
int max_column_length;

int debug;

//: FUNCTION colour_strlen
// Gives the length of the visible portion of s.  Colour
// codes (e.g. %^GREEN%^ <123>) are ignored.
int colour_strlen(string str)
{
   return strlen(XTERM256_D->substitute_colour(str, "plain"));
}

mapping query_themes()
{
   return colours;
}

//: FUNCTION use_colour
// Returns the colour in the colour array, cols, to use given the position
// out of the width (typically screen width).
string use_colour(string *cols, int position, int width)
{
   int col_index = floor(sizeof(cols) * ((0.0 + position) / (width || 1)));

   col_index = clamp(col_index, 0, sizeof(cols) - 1);
   return cols[col_index] != "" ? "<" + cols[col_index] + ">" : "";
}

//: FUNCTION query_style
// Returns the current frame style.
string query_style()
{
   return style;
}

//: FUNCTION set_style
// Set a specific style if it exists.
void set_style(string t)
{
   style = t;

   if (member_array(style, keys(styles)) != -1)
      bits = styles[style];
   else
      bits = styles[FALLBACK_STYLE];
}

//: FUNCTION query_frame_colour_themes
// Returns a list of the frame colour themes available. The list is sorted alphabetically.
string *query_frame_colour_themes()
{
   return sort_array(keys(colours), 1);
}

//: FUNCTION valid_theme
// Checks if string ``t`` is a valid theme, and returns 1 if it is, otherwise 0.
int valid_theme(string t)
{
   return member_array(t, query_frame_colour_themes()) != -1;
}

//: FUNCTION query_frame_styles
// Returns all the implemented frame styles.
string *query_frame_styles()
{
   return keys(styles);
}

//: FUNCTION set_frame_left_header
// Sets the frame into a mode where the header is not shown at the top,
// but shown at the left side.
void set_frame_left_header()
{
   left_header = 1;
}

//: FUNCTION frame_simplify
// Returns TRUE if the current user (not the object receiving the message!)
// has simplify turned on.
nomask int frame_simplify()
{
   return get_user_variable("simplify") != 0;
}

private
string colour_str(string t, string col)
{
   if (!col || strlen(col) != 3)
      return t;
   return "<" + col + ">" + t + "<res>";
}

//: FUNCTION title
// Sets the title of the frame.
string title(mixed t)
{
   return colour_str("" + t, hcolours[COL_TITLE]);
}

//: FUNCTION accent
// Takes a string and applies the accent colour to it.
string accent(mixed t)
{
   return colour_str("" + t, hcolours[COL_ACCENT]);
}

//: FUNCTION raw_accent
// Returns the raw accent colour, i.e. the XXX number triplet.
string raw_accent()
{
   return hcolours[COL_ACCENT];
}

//: FUNCTION raw_title
// Returns the raw title colour, i.e. the XXX number triplet.
string raw_title()
{
   return hcolours[COL_TITLE];
}

//: FUNCTION raw_warn
// Returns the raw warning colour, i.e. the XXX number triplet.
string raw_warning()
{
   return hcolours[COL_WARNING];
}

//: FUNCTION warning
// Takes a string and applies the warning colour to it.
string warning(mixed t)
{
   return colour_str("" + t, hcolours[COL_WARNING]);
}

//: FUNCTION set_frame_title
// Sets the frame title. Big surprise.
void set_frame_title(string s)
{
   if (XTERM256_D->colourp(s))
      title = s;
   else if (hcolours && hcolours[COL_TITLE] != "")
      title = "<" + hcolours[COL_TITLE] + ">" + s + "<res>";
   else
      title = s;
}

//: FUNCTION set_width
// Sets the width of the frame. Minimum is 10, maximum is 1000. Other values will be capped.
void set_width(int w)
{
   width = clamp(w, 10, 1000);
}

//: FUNCTION query_width
// Returns the width of the frame.
int query_width()
{
   return width;
}

//: FUNCTION set_title_margin
// Sets the margin for the title. It's the space between the frame and the title.
void set_title_margin(int hm)
{
   title_margin = hm;
}

//: FUNCTION set_text_margin
// Set the text margin.
void set_text_margin(int tm)
{
   text_margin = tm;
}

string first_colour()
{
   return "<" + explode(hcolours[0], ",")[0] + ">";
}

string last_colour()
{
   return "<" + explode(hcolours[0], ",")[ < 1] + ">";
}

//: FUNCTION frame_init_user
// Init the frame based on the user settings. This is normally always called before
// the rendering to set the frame up correctly. See most commands using frames.
void frame_init_user()
{
   columns = ([]);
   column_order = ({});
   column_width = ([]);
   max_column_length = 0;
   set_width(this_user()->query_screen_width() ? this_user()->query_screen_width() - 2 : 79);
   hcolours = (this_user()->frames_colour() != "none" ? colours[this_user()->frames_colour()] : colours["none"]);
   set_style(this_user()->frames_style());
   title_margin = 2;
   text_margin = 1;
   add_header = 0;
   add_footer = 0;
   left_header = 0;
}

//: FUNCTION set_frame_header
// Set the frame header, works both vertically and horizontally.
void set_frame_header(string hc)
{
   string *header_lines;
   int i = 0;

   if (!hc)
   {
      add_header = 0;
      header_content = 0;
      return;
   }

   if (!hcolours)
      error("No colours defined, did you call frame_init_user()?");

   header_lines = explode(hc, "\n");
   while (i < sizeof(header_lines))
   {
      string line = header_lines[i];
      if (!XTERM256_D->colourp(line) && hcolours[COL_ACCENT] != "")
         line = "<" + hcolours[COL_ACCENT] + ">" + rtrim(line) + "<res>";
      else
         line = rtrim(line);
      header_lines[i] = line;
      i++;
   }
   header_content = implode(header_lines, "\n");
   add_header = 1;
}

//: FUNCTION set_frame_footer
// Sets the frame footer.
void set_frame_footer(string fc)
{
   footer_content = fc;
   add_footer = 1;
}

//: FUNCTION set_frame_content
// This is the main function to call for the main content of
// the frame. Content must either be an array of strings or a single string.
void set_frame_content(mixed c)
{
   if (arrayp(c))
      content = c;
   else
      content = ({c});
}

//: FUNCTION set_frame_hcolours
// Inject the colour array raw into the frame. Don't call this unless you've read the source code
// for M_FRAME.
void set_frame_hcolours(string *hc)
{
   hcolours = hc;
}

//: FUNCTION set_theme
// Sets the frame theme.
void set_theme(string t)
{
   set_frame_hcolours(colours[t]);
}

private
string simple_header()
{
   string out = "";
   out += bits[TRD] + repeat_string(bits[TH], width - 2) + bits[TLD] + "\n";
   return out;
}

void set_frame_sections(string *s, int width)
{
   sections = s;
   section_width = width;
}

private
string *create_section_header()
{
   string *headers = ({});
   string out;
   int first_header = 1;
   int len;
   foreach (string *data in sections)
   {
      // TBUG("Width: " + width + " Len: " + len + " Out len: " + colour_strlen(out));
      if (colour_strlen(out) + len > width)
      {
         headers += ({out + (first_header ? bits[TLD] : bits[TVL]) + "\n"});
         first_header = 0;
         out = 0;
      }
      if (data[1] == "accent")
         data[1] = "<" + raw_accent() + ">";
      if (data[1] == "title")
         data[1] = "<" + raw_title() + ">";
      if (data[1] == "warning")
         data[1] = "<" + raw_warning() + ">";
      if (!out)
         out = bits[TVR];
      else
         out += bits[TH];
      out += bits[TH] + sprintf("<bld>%s%s<res> ", data[1], data[0]) +
             repeat_string(bits[TH], (section_width - strlen(data[0]) + 3));
      if (!len)
         len = colour_strlen(out + bits[TLD]);
   }

   if (strlen(out))
   {
      if (sizeof(headers) && colour_strlen(out) < colour_strlen(headers[0]))
         headers +=
             ({out + repeat_string(bits[TH], colour_strlen(headers[0]) - colour_strlen(out) - 2) + bits[TVL] + "\n"});
      else
         headers += ({out + (first_header ? bits[TLD] : bits[TVL]) + "\n"});
   }

   return headers;
}

private
string create_menu_header()
{
   string out = "";
   string h_title = colour_strlen(title) > (width - 8) ? title[0..width - 8] : title;
   int i = 0;
   int simple_header = style == "lines" || style == "none";
   int header_width = colour_strlen(h_title) + (text_margin * 2);
   string *headers = explode(header_content || "", "\n");

   out += repeat_string(" ", title_margin) + " " + bits[TRD];
   while (i < header_width)
   {
      out += bits[TH];
      i++;
   }

   out += bits[TLD] + "\n";
   out += bits[TRD] + repeat_string(bits[TH], title_margin) + bits[TVL] + repeat_string(" ", text_margin) + h_title +
          repeat_string(" ", text_margin) + bits[TV] + "\n";

   out += bits[TV] + " " + repeat_string(" ", title_margin - 1) + bits[TRU];

   i = 0;

   while (i < header_width)
   {
      out += bits[TH];
      i++;
   }
   out += bits[TLU] + "\n";
   out += bits[TV] + "\n"; // End of Title box section
   return out;
}

private
string create_header()
{
   string out = "";
   int i = 0;
   int simple_header = style == "lines" || style == "none";
   int header_width = colour_strlen(title) + (text_margin * 2);
   string *headers = explode(header_content || "", "\n");

   if (!title)
      return simple_header();

   if (add_header && title_margin == 1)
      title_margin = 2;

   if (!simple_header)
   {
      out += repeat_string(" ", title_margin) + bits[TRD];
      while (i < header_width)
      {
         out += bits[TH];
         i++;
      }

      out += bits[TLD] + "\n";
   }

   out += bits[TRD] + (add_header ? bits[THD] : "") + repeat_string(bits[TH], title_margin - (add_header ? 2 : 1)) +
          bits[TVL] + repeat_string(" ", text_margin) + title + repeat_string(" ", text_margin) + bits[TVR] +
          repeat_string(bits[TH], width - header_width - title_margin - (add_header ? 4 : 3)) +
          (add_header ? bits[THD] : "") + bits[TLD] + "\n";

   if (!simple_header)
   {
      out +=
          bits[TV] + (add_header ? bits[TV] : "") + repeat_string(" ", title_margin - (add_header ? 2 : 1)) + bits[TRU];

      i = 0;
      while (i < header_width)
      {
         out += bits[TH];
         i++;
      }
      out += bits[TLU] + repeat_string(" ", width - header_width - title_margin - (add_header ? 4 : 3)) +
             (add_header ? bits[TV] : "") + bits[TV] + "\n"; // End of Title box section
   }

   foreach (string h in headers)
   {
      int col_lendiff = strlen(h) - colour_strlen(h);
      int content_width = width - 6 + col_lendiff;
      if (title_margin > 0)
         out += bits[TV] + (add_header && !simple_header ? bits[TV] : " ") + " " +
                sprintf("%-" + content_width + "." + content_width + "s", h) + " " + (add_header ? bits[TV] : "") +
                bits[TV] + "\n";
   }

   if (simple_header)
      out += "";
   else if (add_header)
      out += bits[TV] + (add_header ? bits[TRU] : "") + repeat_string(bits[TH], width - 4) +
             (add_header ? bits[TLU] : "") + bits[TV] + "\n";
   return out;
}

private
string create_footer()
{
   string out = "";
   int i = 0;
   string *footers = explode(footer_content || "", "\n");

   out += bits[TV] + bits[TRD] + repeat_string(bits[TH], (width - 4)) + bits[TLD] + bits[TV] + "\n";

   foreach (string f in footers)
   {
      int col_lendiff = strlen(f) - colour_strlen(f);
      int content_width = width - 6 + col_lendiff;
      out += bits[TV] + bits[TV] + " " + sprintf("%-" + content_width + "." + content_width + "s", f) + " " + bits[TV] +
             bits[TV] + "\n";
   }

   out += bits[TRU] + bits[THU] + repeat_string(bits[TH], width - 4) + bits[THU] + bits[TLU] +
          "\n"; // End of Title box section

   return out;
}

private
string *create_menu_content()
{
   string *lines = ({});
   string out;
   int len;

   foreach (string c in content)
   {
      out = "";
      foreach (string l in explode(c, "\n"))
      {
         if (!len)
            len = colour_strlen(l);
         out += bits[TV] + l[0.. < 2];
         out += bits[TV] + "\n";
      }
      lines += ({out});
   }
   out = bits[TRU] + repeat_string(bits[TH], (len - 1)) + bits[TLU];
   lines += ({out});

   return lines;
}

private
string create_left_header()
{
   string out = "";
   string *headers = explode(header_content || "", "\n");
   int header_width, max_header_width, i = 0;
   string *contents = explode(content[0] || "", "\n");
   int content_length = max(({sizeof(headers), sizeof(contents)}));

   if (title)
      header_width = colour_strlen(title) + (text_margin * 2);

   max_header_width = max(map_array(headers, ( : colour_strlen($1) :)));
   max_header_width = max_header_width + (2 * text_margin);

   // Line 1 - title only
   if (title)
      out += repeat_string(" ", title_margin + max_header_width + 1) + bits[TRD] +
             repeat_string(bits[TH], header_width) + bits[TLD];
   out += "\n";

   // Line 2 - both cases
   if (title)
      out += repeat_string(" ", max_header_width + 1) + bits[TRD] + repeat_string(bits[TH], text_margin) + bits[TVL] +
             repeat_string(" ", text_margin) + title + repeat_string(" ", text_margin) + bits[TVR] +
             repeat_string(bits[TH], width - max_header_width - 6 - header_width) + bits[TLD] + "\n";
   else
      out += repeat_string(" ", max_header_width + 1) + bits[TRD] +
             repeat_string(bits[TH], width - max_header_width - 3) + bits[TLD] + "\n";

   // Line 3
   if (title)
      out += bits[TRD] + repeat_string(bits[TH], max_header_width) + bits[TVL] + repeat_string(" ", title_margin - 1) +
             bits[TRU] + repeat_string(bits[TH], header_width) + bits[TLU] +
             repeat_string(" ", width - max_header_width - header_width - 6) + bits[TV] + "\n";
   else
   {
      string c = contents[0];
      int col_lendiff = strlen(c) - colour_strlen(c);
      int content_width = width - max_header_width + col_lendiff - 5;
      out += bits[TRD] + repeat_string(bits[TH], max_header_width - 2) + repeat_string(bits[TH], title_margin) +
             bits[TVL] + " ";
      out += sprintf("%-" + content_width + "." + content_width + "s", c) + " " + bits[TV] + "\n";
      contents = contents[1..];
      content_length = max(({sizeof(headers), sizeof(contents)}));
   }

   while (i < content_length)
   {
      string head, cont;
      int col_lendiff;
      int content_width;
      head = i < sizeof(headers) ? headers[i] : "";
      cont = i < sizeof(contents) ? contents[i] : "";
      col_lendiff = strlen(cont) - colour_strlen(cont);
      content_width = width - max_header_width + col_lendiff - 5;

      if (i < sizeof(headers)) // Header content | Stuff |
         out += bits[TV] + repeat_string(" ", text_margin) + head + "<res>" +
                repeat_string(" ", max_header_width - text_margin - colour_strlen(head)) + bits[TV] + " ";
      else if (i == sizeof(headers)) // End of header
         out += bits[TRU] + repeat_string(bits[TH], max_header_width) + "<res>" + bits[TVL] + " ";
      else // No header, just spacing.
         out += repeat_string(" ", max_header_width + 1) + bits[TV] + " ";

      // Content
      out += sprintf("%-" + content_width + "." + content_width + "s", cont) + " " + bits[TV] + "\n";
      i++;
   }

   if (i <= sizeof(headers))
      out += bits[TRU] + repeat_string(bits[TH], max_header_width) + "<res>" +
             (i == sizeof(contents) ? bits[THU] + repeat_string(bits[TH], width - max_header_width - 3) + bits[TLU]
                                    : bits[TVL] + repeat_string(" ", width - max_header_width - 3) + bits[TV]) +
             "\n";

   if (i != sizeof(contents) || sizeof(contents) > sizeof(headers))
      out += repeat_string(" ", max_header_width + 1) + bits[TRU] +
             repeat_string(bits[TH], width - max_header_width - 3) + bits[TLU] + "\n";

   return out;
}

private
string create_content()
{
   string out = "";
   string *contents = explode(content[0] || "", "\n");

   foreach (string c in contents)
   {
      int col_lendiff = strlen(c) - colour_strlen(c);
      int content_width = width - 4 + col_lendiff;
      out += bits[TV] + " " + sprintf("%-" + content_width + "." + content_width + "s", c) + " " + bits[TV] + "\n";
   }
   return out;
}

private
string end_frame()
{
   string out = "";
   out += bits[TRU] + repeat_string(bits[TH], width - 2) + bits[TLU] + "\n"; // End of Title box section
   return out;
}

varargs string h_colour_internal(string output, mixed colstring, string frames, int outwidth)
{
   mixed *pieces = pcre_assoc(output, ({"[" + frames + "]"}), ({1}));
   string *bits = pieces[0];
   string *matches = pieces[1];
   string new_out = "";
   string *colours;
   int i = 0;
   int position = 0;

   colours = explode((arrayp(colstring) ? colstring[COL_GRADIENT] : colstring) || hcolours[COL_GRADIENT], ",");

   if (!sizeof(colours))
      return output;
   while (i < sizeof(bits))
   {
      if (strsrch(bits[i], "\n") != -1)
         position = 0;

      new_out += (matches[i] ? use_colour(colours, position, outwidth) + bits[i] + "<res>" : bits[i]);
      position += strlen(bits[i]);
      i++;
   }

   return new_out;
}

string colour_string(string str, string theme)
{
   return h_colour_internal(str, query_themes()[theme], "a-zA-Z ", strlen(str));
}

varargs private string h_colours(string output, mixed colstring)
{
   string frames = implode(bits, "");

   return h_colour_internal(output, colstring, frames, width);
}

//: FUNCTION query_frame_title
// Returns the frame title.
string query_frame_title(string theme)
{
   return colours[theme][COL_TITLE];
}

//: FUNCTION query_frame_accent
// Returns the frame accent colour given ``theme``.
string query_frame_accent(string theme)
{
   return colours[theme][COL_ACCENT];
}

//: FUNCTION query_frame_warning
// Returns the frame warning colour given ``theme``.
string query_frame_warning(string theme)
{
   return colours[theme][COL_WARNING];
}

//: FUNCTION frame_demo_string
// Returns a simple demo string.
string frame_demo_string(string style, int w)
{
   return styles[style][TRD] + styles[style][TH] + styles[style][TH] + styles[style][TH] +
          repeat_string(styles[style][TH], ((w - 10))) + styles[style][THD] + styles[style][TH] + styles[style][TH] +
          styles[style][TH] + repeat_string(styles[style][TH], (w - 10)) + styles[style][TH] + styles[style][TH] +
          styles[style][TH] + styles[style][TLD];
}

//: FUNCTION frame_colour_demo
// Do a frame colour demo using ``style`` using ``colour`` in width ``w``.
string frame_colour_demo(string style, string colour, int w)
{
   if (member_array(style, keys(styles)) == -1)
      style = "single";
   return h_colours(frame_demo_string(style, w), colours[colour]);
}

//: FUNCTION menu_render
// Produces a render of the current menu.
string menu_render()
{
   string out = "";
   string *headers, *contents;
   int content_count = 0;

   out += create_menu_header();
   headers = create_section_header();
   contents = create_menu_content();

   foreach (string h in headers)
   {
      if (!frame_simplify())
         out += h;
      out += contents[content_count];
      content_count++;
   }
   out += contents[content_count];

   if (hcolours && this_user()->terminal_mode() != "plain")
      out = h_colours(out);

   out = replace_string(out, "<>", "");

   return out;
}

//: FUNCTION frame_render
// Renders the final frame into a string for printing.
string frame_render()
{
   string out = "";

   if (!bits)
      error("Need to set frame style before render() using frame->set_style().\n"
            "Current styles: " +
            format_list(query_frame_styles()) +
            ".\n"
            "Did you forget to call frame_init_user() ?");

   if (left_header)
   {
      if (!header_content)
         error("Cannot generate left header frame without content.");
      out += create_left_header();
   }
   else
   {
      out = create_header();

      if (content)
         out += create_content();

      if (footer_content)
         out += create_footer();
      else
         out += end_frame();
   }

   if (hcolours && this_user()->terminal_mode() != "plain")
      out = h_colours(out);
   return out;
}

//: FUNCTION frame_add_column
// Adds a column with a name, and an array of strings or integers to be shown.
// This can only be renderes if all contents are columns. Use ``frame_render_columns()`` to render.
// Call ``frame_init_user()`` before adding columns.
// Important: Frame header and frame content should
// not be called as they are calculated automatically.
varargs void frame_add_column(string name, mixed *data, function colour_function)
{
   column_order += ({name});
   columns[name] = data;
   column_functions[name] = colour_function || ( : "<res>" :);
   column_width[name] = 2 + max(map(data, ( : colour_strlen("" + $1) :)));
   if (column_width[name] < strlen(name) + 5)
      column_width[name] = strlen(name) + 5;

   if (sizeof(data) > max_column_length)
      max_column_length = sizeof(data);
}

void debug()
{
   debug = 1;
}

//: FUNCTION frame_render_columns
// Render the columns added via ``frame_add_column()``.
// Important: Frame header and frame content should
// not be called as they are calculated automatically.
string frame_render_columns()
{
   int index = 0, total_width, width;
   string *output = ({});
   string header = "";
   string *rcols = ({});

   width = query_width() - 16;

   while (total_width < width && index < sizeof(column_order))
   {
      if (total_width + column_width[column_order[index]] < width)
      {
         // TBUG("Max width is: " + width + " Total width isf: " + total_width + " New column " + column_order[index] +
         //     " is " + column_width[column_order[index]]);
         total_width += column_width[column_order[index]];
         rcols += ({column_order[index]});
      }
      index++;
   }

   for (int i = 0; i < max_column_length; i++)
   {
      string row = " ";
      total_width = 0;
      foreach (string col in rcols)
      {
         string value;
         if (!i)
            header += sprintf("%-" + column_width[col] + "." + column_width[col] + "s", col) + "  ";
         if (sizeof(columns[col]) > i)
            value = columns[col][i];
         else
            value = "";
         row += evaluate(column_functions[col], value) +
                sprintf("%-" + column_width[col] + "." + column_width[col] + "s", "" + value) + "  ";
      }
      output += ({row});
   }

   if (debug)
   {
      TBUG("Max column length: " + max_column_length);
      TBUG(columns);
      TBUG(output);
   }
   set_frame_header(header);

   // Cheeky way to strip off last "\n " off the string.
   set_frame_content(implode(output, "\n"));
   return frame_render();
}
