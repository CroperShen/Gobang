#pragma once
int NewX(int, int, int);
int NewY(int, int, int);

inline int NewX(int x, int i, int d)
{
	switch (d)
	{
	case 0:
		return x + i;
	case 1:
		return x + i;
	case 2:
		return x + i;
	case 3:
		return x;
	case 4:
		return x - i;
	case 5:
		return x - i;
	case 6:
		return x - i;
	case 7:
		return x;
	}
	return 0;
}

inline int NewY(int y, int i, int d)
{
	switch (d)
	{
	case 0:
		return y - i;
	case 1:
		return y;
	case 2:
		return y + i;
	case 3:
		return y + i;
	case 4:
		return y + i;
	case 5:
		return y;
	case 6:
		return y - i;
	case 7:
		return y - i;
	}
	return 0;
}