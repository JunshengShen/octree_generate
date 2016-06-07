#include "read_stl.h"
#include "oct.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>
#define levelLow 5;
#define voxelsInOneLowLevel 32768;
using namespace std;
void coor1(float max_x, float max_y, float max_z, int level)//三个包装盒的尺寸  分割次数
{
	double lines = pow(2, level);//计算每一行有多少体素   dxdydz分别是三个方向上每个体素的尺寸
	double dx = max_x / lines;
	double dy = max_y / lines;
	double dz = max_z / lines;

	fstream outfile;
	outfile.open("D:\\oofem\\build2.3\\Debug\\test1.txt", ios::app);
	int number = 1;
	lines += 1;
	for (int i = 0; i < lines; i++)//输出每个节点的坐标
	{
		for (int j = 0; j < lines; j++)
		{
			for (int k = 0; k < lines; k++)
			{

				if (k == 0)
				{
					outfile << "node   " << number++ << "   coords 3     " << j *dx << "  " << k *dy << "  " << i *dz << "       bc 3 2 2 2" << endl;
				}
				else
					outfile << "node   " << number++ << "   coords 3     " << j *dx << "  " << k *dy << "  " << i *dz << endl;
			}
		}
	}
	outfile.close();
}
int main()
{
	read_stl a;
	a.read("c:/1.stl");
	int i = a.triganles.size();
	cout << "triganles=" << i << endl;
	int level;
	cout << "input the lecel:";
	cin >> level;
	cout << a.maxx << ' ' << a.maxy << ' ' << a.maxz << ' ' << a.minx << ' ' << a.miny << ' ' << a.minz << "aaa" << endl;

	//cout << a.triganles[920].x[0] << " " << a.triganles[920].y[0] << " " << a.triganles[920].z[0] << endl;
	//cout << a.triganles[920].x[1] << " " << a.triganles[920].y[1] << " " << a.triganles[920].z[1] << endl;
	//cout << a.triganles[920].x[2] << " " << a.triganles[920].y[2] << " " << a.triganles[920].z[2] << endl;
	/*for (int j = 0; j < i; j++)
	{
	cout << a.triganles[j].normal[0] << " " << a.triganles[j].normal[1] << a.triganles[j].normal[2] << endl;
	cout << a.triganles[j].x[0] << " " << a.triganles[j].y[0] << " " << a.triganles[j].z[0] << endl;
	cout << a.triganles[j].x[1] << " " << a.triganles[j].y[1] << " " << a.triganles[j].z[1] << endl;
	cout << a.triganles[j].x[2] << " " << a.triganles[j].y[2] << " " << a.triganles[j].z[2] << endl;
	}*/

	float l = pow(2, level);
	float dx = abs(a.maxx - a.minx) / l;
	float dy = abs(a.maxy - a.miny) / l;
	float dz = abs(a.maxz - a.minz) / l;
	cout << setprecision(16) << dx << "      " << setprecision(16) << dy << "            " << setprecision(16) << dz;
	cout << endl << endl;
	vector <octree_info> voxels(pow(2, 3 * level));
	cout << "voxels.size()=" << voxels.size();
	int voxel_number = 0;
	clock_t start, end;
	start = clock();

	int levelHigh = level - levelLow;
	int voxelsOneLowLevel = voxelsInOneLowLevel;
	if (levelHigh < 0){
		levelHigh = 0;
		voxelsOneLowLevel = 1<<(3*level);
	}
	for (int i = 0; i < 1<<(3*levelHigh); i++){	//1<<(3*levelHigh)是分割的块数
		vector<octree_info> voxelsToOutput;//存放要输出的非空边界体素
		for (int j = i * voxelsOneLowLevel; j < (i + 1) * voxelsOneLowLevel; j++){//每块内部进行的遍历交叉判断  最后输出到外存
			int *codes = code(j, level);
			float xn = a.minx + codes[0] * dx;
			float xp = a.minx + (codes[0] + 1)*dx;
			float yn = a.miny + codes[1] * dy;
			float yp = a.miny + (codes[1] + 1)*dy;
			float zn = a.minz + codes[2] * dz;
			float zp = a.minz + (codes[2] + 1)*dz;
			octree_info tempForPush;
			for (int k = 0; k < a.triganles.size(); k++)
			{
				//cout << count++<<' ';hh
				coor temp[3];
				for (int l = 0; l < 3; l++)
				{
					temp[l].x = a.triganles[k].x[l];
					temp[l].y = a.triganles[k].y[l];
					temp[l].z = a.triganles[k].z[l];

				}
				//cout << j << " ";
				
				tempForPush.morton = j;
				if (overlap(temp[0], temp[1], temp[2], xn, xp, yn, yp, zn, zp))
				{
					//cout << j<<' ';
					tempForPush.in_out = true;
					tempForPush.inside_boun = true;
					//cout << ' ' << i ;
					voxel_number++;
					voxelsToOutput.push_back(tempForPush);
					break;
				}				
			}
			if (tempForPush.in_out == false) {
				tempForPush.inside_boun = true;
				voxelsToOutput.push_back(tempForPush);
			}
			//cout << j << ' ';
		}
		cout << endl << endl << endl << voxelsToOutput.size();
		fstream outOfCoreOutput;//输出一块到外存 然后清空临时的vector
		stringstream ss;
		string address;
		ss << i;
		ss >> address;	
		address ="d:\\outOfCore"+ address + ".bat";
		outOfCoreOutput.open(address,ios::out);
		outOfCoreOutput.close();
		outOfCoreOutput.open(address, ios::app | ios::binary);
		for (int j = 0; j <voxelsOneLowLevel; j++){
			outOfCoreOutput.write((char *) &voxelsToOutput[j],sizeof(octree_info));
			cout << j<<' ';

		}
		outOfCoreOutput.close();
		voxelsToOutput.clear();
	}

	

	fstream readIn;
	readIn.open("d:\\outOfCore0.bat", ios::in|ios::binary);
	readIn.seekg(0, ios::beg);
	cout << endl;
	octree_info temp;
	while (readIn.read((char *)&temp, sizeof(octree_info))) {
		cout << temp.inside_boun << ' ' << temp.in_out << ' ' << temp.morton << endl;
	}
	readIn.close();
	





	/*
	for (int i = 0; i < voxels.size(); i++)
	{
		//cout << "t";
		int *codes=code(i, level);
		voxels[i].morton = i;
		float xn = a.minx + codes[0] * dx;
		float xp = a.minx + (codes[0] + 1)*dx;
		float yn = a.miny + codes[1] * dy;
		float yp = a.miny + (codes[1] + 1)*dy;
		float zn = a.minz + codes[2] * dz;
		float zp = a.minz + (codes[2] + 1)*dz;
		//cout << "t";
		
		for (int j = 0; j < a.triganles.size(); j++)
		{
			//cout << count++<<' ';
			coor temp[3];
			for (int k = 0; k < 3; k++)
			{
				temp[k].x = a.triganles[j].x[k];
				temp[k].y = a.triganles[j].y[k];
				temp[k].z = a.triganles[j].z[k];
				
			}
			//cout << j << " ";
			if (overlap(temp[0], temp[1], temp[2], xn, xp, yn, yp, zn, zp))
			{
				//cout << j<<' ';
				voxels[i].in_out = true;
				//cout << ' ' << i ;
				voxel_number++;
				break;
			}
			//cout << j;	```````````````````
		}
		
		//cout << "a";
	}
	end = clock();
	
	
	
	clock_t time1, time2;
	time1 = clock();
	fstream outOfCore;
	outOfCore.open("d:\\outOfCore.bat", ios::out);
	outOfCore.close();
	outOfCore.open("d:\\outOfCore.bat", ios::app | ios::binary);
	for (int i = 0; i <pow(pow(2, level),3); i++){
		if (voxels[i].in_out == true)
			outOfCore << voxels[i].morton << ' ';
	}
	outOfCore.close();


	outOfCore.open("d:\\outOfCore.bat", ios::in | ios::binary);
	int readTemp[4096];
	for (int i = 0; i < 4096; i++){
		outOfCore >> readTemp[i];
	}
	time2 = clock();
	cout << time2 - time1;
	for (int i = 0; i < 4096; i++){
		cout << readTemp[i] << ' ';
	}


	fstream outfile;
	cout << endl<<endl;
	cout << (end - start)/CLOCKS_PER_SEC;
	cout << endl<< end<<endl<<start;

	
	
	
	outfile.open("D:\\oofem\\build2.3\\Debug\\test1.txt", ios::out);
	outfile << "Majnun.out" << endl;
	outfile << "test of Brick elements with nlgeo 1(strain is the Green-Lagrangian strain) rotated as a rigid body" << endl;
	outfile << "#NonLinearStatic  nmsteps 1 nsteps 1 " << endl;
	outfile << "#LinearStatic  nmsteps 1 nsteps 1 " << endl;
	outfile << "#nsteps 5 rtolv 1.e-6 stiffMode 1 controlmode 1 maxiter 100" << endl;
	outfile << "#vtkxml tstep_all domain_all primvars 1 1 vars 2 4 1 stype 1" << endl;
	outfile << "#domain 3d" << endl;
	outfile << "#OutputManager tstep_all dofman_all element_all" << endl;
	outfile << "LinearStatic nsteps 3 nmodules 1" << endl;
	outfile << "vtkxml tstep_all domain_all primvars 1 1 vars 2 4 1 stype 1" << endl;
	outfile << "domain 3d" << endl;
	outfile << "OutputManager tstep_all dofman_all element_all" << endl;
	outfile << "ndofman " << pow((pow(2, level) + 1), 3) << " nelem " << voxel_number << " ncrosssect 1 nmat 1 nbc 2 nic 0 nltf 1 " << endl;
	outfile.close();
	coor1(a.maxx - a.minx, a.maxy - a.miny, a.maxz - a.minz, level);



	int temp_count = 0;
	for (int i = 0; i < pow(pow(2, level) , 3); i++)
	{
		if (voxels[i].in_out == true)
		{
			fstream outfile;
			outfile.open("D:\\oofem\\build2.3\\Debug\\test1.txt", ios::app);
			int *asddd = code(i, level);
	
			outfile << "LSpace " << ++temp_count << "	 nodes  8 ";
			//voxel_output* asd = new voxel_output(asddd[0], asddd[1], asddd[2], octree.level_);
			
			//voxel_output asd(pChildNode, level);


			int point_code[8];

			double q = pow(2, level) + 1;
			int x_code = asddd[0]+1;
			int y_code = asddd[1]+1;
			int z_code = asddd[2]+1;
			point_code[0] = (x_code - 1)*q + y_code + z_code *q*q;
			point_code[1] = (x_code - 1)*q + y_code + 1 + z_code *q*q;
			point_code[2] = x_code *q + y_code + 1 + z_code *q*q;
			point_code[3] = x_code *q + y_code + z_code *q*q;

			point_code[4] = (x_code - 1)*q + y_code + (z_code - 1)*q*q;
			point_code[5] = (x_code - 1)*q + y_code + 1 + (z_code - 1)*q*q;
			point_code[6] = x_code *q + y_code + 1 + (z_code - 1)*q*q;
			point_code[7] = x_code *q + y_code + (z_code - 1)*q*q;

			
			for (int i = 0; i <= 7; i++)
			{
				outfile << point_code[i] << "  ";
			}
			//outfile <<"    "<< x_code << "  " << y_code << "  " << z_code;
			outfile << "    	mat 1 crossSect 1	nlgeo 1  " << endl;

			outfile.close();





		}
	}




	cout << "ck";

	outfile.open("D:\\oofem\\build2.3\\Debug\\test1.txt", ios::app);
	outfile << "SimpleCS 1" << endl << "IsoLE 1 d 0. E 15.0 n 0.25 talpha 1.0" << endl << "BoundaryCondition  1 loadTimeFunction 1 prescribedvalue 0.0"
		<< endl << "BoundaryCondition  2 loadTimeFunction 1 prescribedvalue 0.5" << endl << "PiecewiseLinFunction 1 npoints 2 t 2 0. 1000. f(t) 2 0. 1000." << endl;
	outfile.close();






	system("cd /d D:\\oofem\\build2.3\\Debug &oofem.exe&oofem -f test1.txt");

	*/




	return 0;
}