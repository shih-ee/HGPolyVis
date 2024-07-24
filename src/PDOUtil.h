#pragma once

inline int pair_hash(int i, int j)
{
	int x, n;
	if (i > j) { x = i; n = j; }
	else { x = j; n = i; }
	return ((x * (x + 1)) / 2) + n;
}