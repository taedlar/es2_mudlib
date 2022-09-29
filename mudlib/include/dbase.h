/*  dbase.h

    Copyright (C) 1994-2000 Annihilator <annihilator@muds.net>

    This program is a part of ES2 mudlib. Permission is granted to use,
    modify, copy or distribute this program provided this copyright notice
    remains intact and subject to the restriction that this program MAY
    NOT be used in any way for monetary gain.

    Details of terms and conditions is available in the Copyright.ES2 file.
    If you don't receive this file along with this program, write to the
    primary author of ES2 mudlib: Annihilator <annihilator@muds.net>
*/

#ifndef DBASE_H
#define DBASE_H

// This is the 'expected' dbase size, which is allocated when initial
#define DEFAULT_DBASE_SIZE		15

mapping query_entire_dbase();
mapping query_entire_temp_dbase();

mixed set(string prop, mixed data);
mixed set_vector(string prop, mixed owner, mixed func);
varargs mixed query(string prop, int raw);
mixed add(string prop, mixed data);
void delete(string prop);

mixed set_temp(string prop, mixed data);
mixed set_temp_vector(string prop, mixed owner, mixed func);
varargs mixed query_temp(string prop, int raw);
mixed add_temp(string prop, mixed data);
void delete_temp(string prop);

#endif	/* DBASE_H */
