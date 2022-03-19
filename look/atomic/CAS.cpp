int compare_and_swap (int* reg, int oldval, int newval)
{
  int old_reg_val = *reg;
  if (old_reg_val == oldval) {
     *reg = newval;
  }
  return old_reg_val;
}

//说明：基于CAS封装的无锁List。
bool CAS(int* addr, int expected, int value)
{
	if (*addr == expected)
	{
		*addr = value;
		return true;
	}
	return false;
}