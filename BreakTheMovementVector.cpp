#include "BreakTheMovementVector.h"

vector<int*> breakTheMovementVector(int i, int j){

	vector<int*> result;

	int signOfI;
	int signOfJ;
	double ii = fabs(i);
	double jj = fabs(j);

	if(i==0 && j==0)
		return result;
	if (i > 0) signOfI = 1; else signOfI = -1;
	if (j > 0) signOfJ = 1; else signOfJ = -1;
	if (j == 0){
		for (int f = 0; f < ii; f++){
			int* point = new int[2];
			point[0] = signOfI;
			point[1] = 0;
			result.push_back(point);
		}
		return result;
	}
	if (i == 0){
		for (int f = 0; f < jj; f++){
			int* point = new int[2];
			point[0] = 0;
			point[1] = signOfJ;
			result.push_back(point);
		}
		return result;
	}
	double a = 1.0;
	double b = 1.0;
	double cross = ((ii/jj) * (a-.5) + .5);
	while (b <= ii || a <= jj){
		if (b > cross){
			int* point = new int[2];
			point[0] = 0;
			point[1] = signOfJ;
			result.push_back(point);
			a++;
			cross = ((ii/jj) * (a-.5) + .5);
		}else if (b == cross){
			int* point = new int[2];
			point[0] = signOfI;
			point[1] = signOfJ;
			result.push_back(point);
			a++;
			b++;
			cross = ((ii/jj) * (a-.5) + .5);
		}else if (b < cross){
			int* point = new int[2];
			point[0] = signOfI;
			point[1] = 0;
			result.push_back(point);
			b++;
		}
	}
	return result;
}


