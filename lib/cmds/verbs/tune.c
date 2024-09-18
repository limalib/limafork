/* Do not remove the headers from this file! see /USAGE for more info. */

inherit VERB_OB;

void do_tune_obj(object ob)
{
   ob->do_tune();
}

void do_tune_obj_to_wrd_wrd(object ob, string wrd1, string wrd2)
{
   ob->do_tune_to(wrd1, wrd2);
}

void create()
{
   add_rules(({
       "OBJ",
       "OBJ to WRD WRD",
   }));
}