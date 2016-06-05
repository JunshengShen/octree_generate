#include "read_stl.h"
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;

int asc2(char a)
{
	switch (a)
	{
	case '0':
		return 0;
		break;
	case '1':
		return 1;
		break;
	case '2':
		return 2;
		break;
	case '3':
		return 3;
		break;
	case '4':
		return 4;
		break;
	case '5':
		return 5;
		break;
	case '6':
		return 6;
		break;
	case '7':
		return 7;
		break;
	case '8':
		return 8;
		break;
	case '9':
		return 9;
		break;
	}
}
void read_stl::read(string address)
{
	fstream read_in;
	read_in.open(address, ios::in);
	char temp;
	for (;;)
	{
		read_in >> noskipws >> temp;
		if (temp == '\n')
		{
			cout << "a";
			break;
		}
	}
	for (;;)
	{
		triganle temp_tri;
		for (;;)
		{
			read_in >> temp;

			if (temp == 'l')
			{
				vector<char> temp_c;
				int number_of_this = 0;
				int number_of_total = 0;
				read_in >> noskipws >> temp;
				bool if_e = false;
				int location_of_e;
				for (;;)
				{
					read_in >> noskipws >> temp;
					if (temp == ' ' || temp == '\n')
					{
						//根据个数计算并赋值
						for (int k = 0; k < number_of_this; k++)
						{
							if (temp_c[k] == 'e' || temp_c[k] == 'E')
							{
								if_e = true;
								location_of_e = k;
								break;
							}
						}

						int location_of_dot;
						double temp_count = 0;
						
						for (int i = 0; i < number_of_this; i++)
						{
							if (temp_c[i] == '.')
							{
								location_of_dot = i;
								break;
							}
						}
						if (if_e == false)
						{
							if (temp_c[0] == '-')
							{
								for (int i = 1; i < location_of_dot; i++)
								{
									temp_count += asc2(temp_c[i])*pow(10, location_of_dot - i - 1);
								}
								for (int i = location_of_dot + 1; i < number_of_this; i++)
								{
									temp_count += asc2(temp_c[i]) * pow(10, location_of_dot - i);
								}
								temp_count *= -1;
							}
							if (temp_c[0] != '-')
							{
								for (int i = 0; i < location_of_dot; i++)
								{
									temp_count += asc2(temp_c[i])*pow(10, location_of_dot - i - 1);
								}
								for (int i = location_of_dot + 1; i < number_of_this; i++)
								{
									temp_count += asc2(temp_c[i]) * pow(10, location_of_dot - i);
								}
							}
							temp_tri.normal[number_of_total] = temp_count;
							//cout << temp_tri.normal[number_of_total] << " ";
							temp_count = 0;
							number_of_total++;

							temp_c.clear();
							number_of_this = 0;
						}
						else
						{
							if_e = false;
							if (temp_c[0] == '-')
							{
								for (int i = 1; i < location_of_dot; i++)
								{
									temp_count += asc2(temp_c[i])*pow(10, location_of_dot - i - 1);
								}
								for (int i = location_of_dot + 1; i < location_of_e; i++)
								{
									temp_count += asc2(temp_c[i]) * pow(10, location_of_dot - i);
								}
								temp_count *= -1;
							}
							if (temp_c[0] != '-')
							{
								for (int i = 0; i < location_of_dot; i++)
								{
									temp_count += asc2(temp_c[i])*pow(10, location_of_dot - i - 1);
								}
								for (int i = location_of_dot + 1; i < location_of_e; i++)
								{
									temp_count += asc2(temp_c[i]) * pow(10, location_of_dot - i);
								}
							}
							if (temp_c[location_of_e + 1] == '-')
							{							
								temp_count = temp_count*pow(10,-1*( asc2(temp_c[location_of_e + 2]) * 100 
									+asc2(temp_c[location_of_e + 3]) * 10 + asc2(temp_c[location_of_e + 4])));
							}
							else
							{	
								temp_count = temp_count*pow(10, asc2(temp_c[location_of_e + 1]) * 100 
									+ asc2(temp_c[location_of_e + 2]) * 10 + asc2(temp_c[location_of_e + 2]));
							}
							temp_tri.normal[number_of_total] = temp_count;
							//cout << temp_tri.normal[number_of_total] << " ";
							//cin >> temp_count;
							temp_count = 0;
							number_of_total++;

							temp_c.clear();
							number_of_this = 0;
						}
					}
					else
					{
						temp_c.push_back(temp);
						number_of_this++;
					}
					if (number_of_total == 3)
					{
						//cout << endl;
						break;
					}
				}
				break;
			}
		}








		for (;;)
		{
			read_in >> temp;
			
			if (temp == 'p')
			{
				vector<char> temp_r;
				int vertex_number = 0;
				int number_of_vertex = 0;
				bool if_e = false;
				int location_of_e;
				for (int i = 0; i < 3; i++)
				{
					for (;;)
					{
						read_in >> temp;
						if (temp == 'x')
						{
							read_in >> noskipws >> temp;
							break;
						}
					}
					for (;;)
					{
						read_in >> noskipws >> temp;
						if (temp == ' ' || temp == '\n')
						{
							//根据个数计算并赋值
							for (int k = 0; k < number_of_vertex; k++)
							{
								if (temp_r[k] == 'e' || temp_r[k] == 'E')
								{
									if_e = true;
									location_of_e = k;
									break;
								}
							}

							int location_of_dot;
							double temp_count = 0;

							for (int j = 0; j < number_of_vertex; j++)
							{
								if (temp_r[j] == '.')
								{
									location_of_dot = j;
									break;
								}
							}
							if (if_e == false)
							{
								if (temp_r[0] == '-')
								{
									for (int j = 1; j < location_of_dot; j++)
									{
										temp_count += asc2(temp_r[j])*pow(10, location_of_dot - j - 1);
									}
									for (int j = location_of_dot + 1; j < number_of_vertex; j++)
									{
										temp_count += asc2(temp_r[j]) * pow(10, location_of_dot - j);
									}
									temp_count *= -1;
								}
								if (temp_r[0] != '-')
								{
									for (int j = 0; j < location_of_dot; j++)
									{
										temp_count += asc2(temp_r[j])*pow(10, location_of_dot - j - 1);
									}
									for (int i = location_of_dot + 1; i < number_of_vertex; i++)
									{
										temp_count += asc2(temp_r[i]) * pow(10, location_of_dot - i);
									}
								}
								if (vertex_number == 0)
								{
									temp_tri.x[i] = temp_count;
								}
								else if (vertex_number == 1)
								{
									temp_tri.y[i] = temp_count;
								}
								else
								{
									temp_tri.z[i] = temp_count;
								}
								//cout << temp_count << " ";
								temp_count = 0;
								vertex_number++;

								temp_r.clear();
								number_of_vertex = 0;
							}
							else
							{
								if_e = false;
								if (temp_r[0] == '-')
								{
									for (int j = 1; j < location_of_dot; j++)
									{
										temp_count += asc2(temp_r[j])*pow(10, location_of_dot - j - 1);
									}
									for (int j = location_of_dot + 1; j < location_of_e; j++)
									{
										temp_count += asc2(temp_r[j]) * pow(10, location_of_dot - j);
									}
									temp_count *= -1;
								}
								if (temp_r[0] != '-')
								{
									for (int i = 0; i < location_of_dot; i++)
									{
										temp_count += asc2(temp_r[i])*pow(10, location_of_dot - i - 1);
									}
									for (int i = location_of_dot + 1; i < location_of_e; i++)
									{
										temp_count += asc2(temp_r[i]) * pow(10, location_of_dot - i);
									}
								}
								if (temp_r[location_of_e + 1] == '-')
								{
									temp_count = temp_count*pow(10, -1 * (asc2(temp_r[location_of_e + 2]) * 100
										+ asc2(temp_r[location_of_e + 3]) * 10 + asc2(temp_r[location_of_e + 4])));
								}
								else
								{
									temp_count = temp_count*pow(10, asc2(temp_r[location_of_e + 1]) * 100
										+ asc2(temp_r[location_of_e + 2]) * 10 + asc2(temp_r[location_of_e + 2]));
								}
								if (vertex_number == 0)
								{
									temp_tri.x[i] = temp_count;
								}
								else if (vertex_number == 1)
								{
									temp_tri.y[i] = temp_count;
								}
								else
								{
									temp_tri.z[i] = temp_count;
								}
								
								//cout << temp_count << " ";
								//cin >> temp_count;
								temp_count = 0;
								vertex_number++;

								temp_r.clear();
								number_of_vertex = 0;
							}
						}
						else
						{
							temp_r.push_back(temp);
							number_of_vertex++;
						}
						if (vertex_number == 3)
						{
							vertex_number = 0;
							break;
						}
					}
					//cout << endl;
				}
				break;
			}
		}
		for (;;)
		{
			read_in >> noskipws >> temp;
			if (temp == 't')
				break;
		}
		if (triganles.size() == 0)
		{
			minx = temp_tri.x[0];
			maxx = temp_tri.x[0];
			miny = temp_tri.y[0];
			maxy = temp_tri.y[0];
			minz = temp_tri.z[0];
			maxz = temp_tri.z[0];
		}
		
		for (int i = 0; i < 3; i++)
		{
			if (minx > temp_tri.x[i])
			{
				minx = temp_tri.x[i];
			}
			if (maxx < temp_tri.x[i])
			{
				maxx = temp_tri.x[i];
			}
			if (miny > temp_tri.y[i])
			{
				miny = temp_tri.y[i];
			}
			if (maxy < temp_tri.y[i])
			{
				maxy = temp_tri.y[i];
			}
			if (minz > temp_tri.z[i])
			{
				minz = temp_tri.z[i];
			}
			if (maxz < temp_tri.z[i])
			{
				maxz = temp_tri.z[i];
			}
		}
		
		triganles.push_back(temp_tri);
		/*cout << triganles[triganles.size()-1].normal[0] << " " << triganles[triganles.size()-1].normal[1] << " " << triganles[triganles.size()-1].normal[2] << endl;
		cout << triganles[triganles.size()-1].x[0] << " " << triganles[triganles.size()-1].y[0] << " " << triganles[triganles.size()-1].z[0] << endl;
		cout << triganles[triganles.size()-1].x[1] << " " << triganles[triganles.size()-1].y[1] << " " << triganles[triganles.size()-1].z[1] << endl;
		cout << triganles[triganles.size()-1].x[2] << " " << triganles[triganles.size()-1].y[2] << " " << triganles[triganles.size()-1].z[2] << endl;*/
		read_in >> temp;
		if (temp == 'e')
			break;
		read_in >> temp;
		if (temp == 'e')
			break;
	}
			
			//break;
		
	
	read_in.close();
}


/*
void read_stl::read_normal(string address)
{
	fstream read_n;
	char temp;
	read_n.open(address, ios::in);
	
	
}
void read_stl::read_vertexs(string address)
{
	
	fstream read_v;
	read_v.open(address, ios::in);
	char temp;
	
	
}*/