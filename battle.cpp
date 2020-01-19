#include "header.h"

bool ship_is_destroyed(Unit** m, const int nn, int i, int j)
{
	int x = 0, y = 0;
	int head = ship_head(m, nn, i, j);
	int a = head / nn;
	int b = head % nn;
	int size = m[i][j].value;

	if (a + 1 < nn)
		if (m[a + 1][b].value > 0)
		{
			x = 0;
			y = 1;
		}
	if (b + 1 < nn)
		if (m[a][b + 1].value > 0)
		{
			x = 1;
			y = 0;
		}
	for (int k = a; k <= a + (size - 1) * y; k++)
		for (int p = b; p <= b + (size - 1) * x; p++)
			if (m[k][p].status == 0)
				return false;
	return true;
}

Unit** area_of_the_destroyed_ship(Unit** m, const int nn, int i, int j)
{
	int x = 0, y = 0;
	int head = ship_head(m, nn, i, j);
	int a = head / nn;
	int b = head % nn;
	int size = m[a][b].value;
	if (a + 1 < nn)
		if (m[a + 1][b].value > 0)
		{
			x = 0;
			y = 1;
		}
	if (b + 1 < nn)
		if (m[a][b + 1].value > 0)
		{
			x = 1;
			y = 0;
		}
	for (int k = a - 1; k <= a + 1 + (size - 1) * y; k++)
		for (int p = b - 1; p <= b + 1 + (size - 1) * x; p++)
		{
			if (!(k >= 0 && k < nn) || !(p >= 0 && p < nn))
				continue;
			if (m[k][p].status == 0)
				m[k][p].status = 1;
		}
	return m;
}

Player battle_shoot(Player p, const int nn, bool demo, int index)
{
	int r;
	int status;
	if (!demo && index == 1)
		r = p.r;
	else
	{
		if (!p.under_attack)
			r = random_shoot(p.m, nn);
		else
			r = aim_shoot(p.m, nn, p.last_hit / nn, p.last_hit % nn);
	}
	p.m[r / nn][r % nn].status = true;

	if (p.m[r / nn][r % nn].value > 0)
	{
		p.last_hit = r;

		if (!ship_is_destroyed(p.m, nn, r / nn, r % nn))
		{
			status = 1;
			p.under_attack = true;
		}
		else
		{
			p.m = area_of_the_destroyed_ship(p.m, nn, r / nn, r % nn);
			status = 2;
			p.under_attack = false;
		}
	}
	else
	{
		status = 0;
	}

	p.r = r;
	p.status = status;

	return p;
}

int count_ships(Unit** m, const int nn, bool flag)
{
	int k = 0;
	for (int i = 0; i < nn; i++)
		for (int j = 0; j < nn; j++)
			if (m[i][j].value > 0 && m[i][j].status && flag || m[i][j].value > 0 && !m[i][j].status && !flag)
				k++;
	return k;
}

void sea_battle(const int nn, int max_size, bool demo)
{
	Player p[2];
	for (int i = 0; i < 2; i++)
	{
		Unit** m = create_squard(nn);
		m = zero_matrix(m, nn, 0, 0);
		m = map_generator(m, nn, max_size);
		p[i].m = m;
		p[i].under_attack = false;
		p[i].r = 0;
		p[i].status = -1;
	}

	int index = rand() % 2;
	system("cls");
	print(p, nn, demo, -1, -1);
	cout << "\n\t";
	print_player(index, demo);
	cout << "is  first!";
	getchar();

	int k = count_ships(p[0].m, nn, 0);
	while (k != count_ships(p[0].m, nn, 1) && k != count_ships(p[1].m, nn, 1))
	{
		if (index == 1 && !demo)
		{
			p[index].r = scan_point(p, nn, index);
		}

		p[index] = battle_shoot(p[index], nn, demo, index);
		print_all(p, nn, index, demo, p[1].r, p[0].r);
		if (p[index].status == 0)
			index = (++index) % 2;
		if (demo)
			_getch();
	}

	for (int i = 0; i < 2; i++)
	{
		delete[] p[i].m;
	}
	cout << "\n\t";
	print_player(index, demo);
	cout << "win!" << endl << "\t\tGame Over";
}