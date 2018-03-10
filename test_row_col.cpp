#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define N 36

struct pointMatrix{
	Point2f center;
	int r=-1;
	int c=-1;
};

//�����ȡcsv�ļ������ĵ�����ֵ�ĺ���
int readCenterPoints(ifstream &ifs, vector<Point2f> &vecPoints){
	//����csv�ļ�
	string tmp;
	getline(ifs, tmp);//������һ��

	string line;
	while (getline(ifs, line)){
		stringstream iss(line);

		//ֻ��עǰ��������
		Point2f centerPoint;
		for (int j = 0; j < 2; j++){
			string val;
			getline(iss, val, ',');// ָ���ָ���Ϊ��,��
			if (!iss.good()) break;
			
			if (0 == j){
				centerPoint.x = stof(val);
			}
			else if (1 == j){
				centerPoint.y = stof(val);
			}
		}
		vecPoints.push_back(centerPoint);
	}
	return 0;
}

bool cmpY(Point2f p1, Point2f p2){
	return p1.y < p2.y;
}
bool cmpX(Point2f p1, Point2f p2){
	return p1.x < p2.x;
}

int main()
{
	//read the csv file
	ifstream ifs("Points.csv");
	if (!ifs){
		cout << "Open Points.csv failed..." << endl;
		return -1;
	}
	vector<Point2f> vecPoints;
	readCenterPoints(ifs, vecPoints);//��centerPoint����д��vector

	//for (auto iter = vecPoints.begin(); iter != vecPoints.end(); ++iter){
	//	cout << (*iter)<< "  ";
	//}
	cout << "points nums:" << vecPoints.size() << endl;

	//
	vector<Point2f> copyPoints = vecPoints;
	//int i = 0;
	//for (vector<Point2f>::iterator iter = vecPoints.begin(); i < 10; ++iter,++i){
	//	//cout << (*iter) << "  ";
	//	copyPoints.push_back(*iter);

	//}
	//cout << endl;

	
	//for (auto iter = copyPoints.begin(); iter != copyPoints.end(); ++iter){
	//	cout << (*iter)<< "  ";
	//}
	//cout << endl;
	//======================================================================
	sort(copyPoints.begin(), copyPoints.end(), cmpY);//��y����
	
	//��yֵ��С��N��д���µ���������һ�еĵ㣩
	vector<Point2f> firstRowPoints;
	int index = 0;
	for (auto iter = copyPoints.begin(); index < N; index++, iter++){
		firstRowPoints.push_back(*iter);
	}

	sort(firstRowPoints.begin(), firstRowPoints.end(), cmpX);

	//for (auto iter = firstRowPoints.begin(); iter != firstRowPoints.end(); ++iter){
	//	cout << (*iter) << "  ";
	//}
	//cout << endl;
	//���Ƶ�һ��x��������ֵ�Ĳ�ֵ,ȷ��delta_x
	float delta_x = 999;
	for (vector<Point2f>::iterator iter = firstRowPoints.begin(); iter < firstRowPoints.end()-1;++iter){
		float tmp = abs((*(iter + 1)).x - (*iter).x);
		cout << tmp << "  ";
		if (delta_x > tmp){
			delta_x = tmp;
		}
	}
	cout << endl;
	delta_x = delta_x / 3;//x����ֵ���������delta_x
	cout << "delta_x: " << delta_x << endl;

	//==================�����ҳ���һ�е������==============
	sort(copyPoints.begin(), copyPoints.end(), cmpX);//��x����
	cout << "copyPoints size: " << copyPoints.size() << endl;
	//��xֵ��С��N��д���µ���������һ�еĵ㣩
	vector<Point2f> firstColPoints;
	int index_x = 0;
	for (auto iter = copyPoints.begin(); index_x < N; index_x++, iter++){
		firstColPoints.push_back(*iter);
	}

	sort(firstColPoints.begin(), firstColPoints.end(), cmpY);

	//for (auto iter = firstColPoints.begin(); iter != firstColPoints.end(); ++iter){
	//	cout << (*iter) << "  ";
	//}
	//cout << endl;

	//���Ƶ�һ��y��������ֵ�Ĳ�ֵ,ȷ��delta_y
	float delta_y = 999;
	for (vector<Point2f>::iterator iter = firstColPoints.begin(); iter < firstColPoints.end() - 1; ++iter){
		float tmp = abs((*(iter + 1)).y - (*iter).y);
		cout << tmp << "  ";
		if (delta_y> tmp){
			delta_y = tmp;
		}
	}
	cout << endl;
	delta_y = delta_y / 3;//y����ֵ���������delta_y
	cout << "delta_y: " << delta_y << endl;


	//=============================================================
	//������ֵ�����й�ϵ��Ӧ����
	//=============================================================
	vector<pointMatrix> vecPointsMatrix;
	for (vector<Point2f>::iterator iter = vecPoints.begin(); iter != vecPoints.end(); ++iter){
		pointMatrix tmpPoint;
		tmpPoint.center = *iter;
		vecPointsMatrix.push_back(tmpPoint);
	}

	//cout << vecPointsMatrix.size() << endl;

	//ȷ������
	vector<pointMatrix>::iterator it = vecPointsMatrix.begin();
	int i = 0;
	while (it != vecPointsMatrix.end()){
	
		for (int row = 0; row < N; row++){
			float delta1 = abs(firstColPoints.at(row).y - vecPointsMatrix.at(i).center.y);	
			if (delta1 <= delta_y){//���������Χ�ڣ��ҵ������������
				vecPointsMatrix.at(i).r = row; 
			}
		}

		for (int col = 0; col < N; col++){
			float delta2 = abs(firstRowPoints.at(col).x - vecPointsMatrix.at(i).center.x);
			if (delta2 < delta_x){//���������Χ�ڣ��ҵ������������
				vecPointsMatrix.at(i).c = col;
			}
		}
		
		i++;
		it++;
	}
	


	//test
	for (vector<pointMatrix>::iterator iter = vecPointsMatrix.begin(); iter != vecPointsMatrix.end(); iter++){
		cout << (*iter).center << "  " << (*iter).r << "  " << (*iter).c << endl;
	}

	return 0;
}