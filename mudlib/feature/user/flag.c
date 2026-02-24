/*  Package: User permanent flags
 *  Summary: A flag system for user to recognize something.
 */

string explore_flags = "";

varargs int recognize (int flag, int set_if_zero) {
	if (stringp(explore_flags) && test_bit (explore_flags, flag))
		return 1;
	if (set_if_zero)
		explore_flags = set_bit (explore_flags, flag);
	return 0;
}
