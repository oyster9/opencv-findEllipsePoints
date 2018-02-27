#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
	//read the csv file
	ifstream ifs("Points.csv");
	if (!ifs){
		cout << "Open Points.csv failed..." << endl;
		return -1;
	}

	//解析csv文件
	string tmp;
	getline(ifs, tmp);//跳过第一行

	vector<Point2f> vecPoints;

	/*for (int i = 0; i < 10; i++)*/
	string line;
	while(getline(ifs,line)){
	/*	string line;
		getline(ifs, line);
		if (!ifs.good()) break;*/

		stringstream iss(line);
		
		//只关注前两列数据
		Point2f centerPoint;
		for (int j = 0; j < 2; j++){
			string val;
			getline(iss, val, ',');// 指定分隔符为‘,’
			if (!iss.good()) break;
			//cout << val << "   ";
			if (0 == j){
				centerPoint.x = stof(val);
			}
			else if (1 == j){
				centerPoint.y = stof(val);
			}
		}
		//cout << endl;
		vecPoints.push_back(centerPoint);
	}

	for (auto iter = vecPoints.begin(); iter != vecPoints.end(); ++iter){
		cout << *iter << "  ";
	}
	cout << "points nums:" << vecPoints.size() << endl;
	return 0;
}