// Asteroids.cpp 

//1).For each point we shift the coordinate system in the way that the point should be in the center(i.e.x = 0, y = 0).

//2).Calculate the angles for the points around the new coordinate center.

//3).Arrange the points around the central point by angle(sorting).

//4).Calculate the angle between the first and the last points around the central point.

//5).Do that cyclically, shifting the first and the last point around the central point,
//and calculate the angle span between the first and the last point
//(considering the crossing of X axis), and find the minimal angle span.

//6).Repeat 1) ... 5) for each point and find the minimal angle span for it.
//Then we find the very minimal of minimal angle spans for each points, and the corresponding point will be the point we are looking for.


//#include "stdafx.h" // comment it, if we are not in Windows platform 

#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
#pragma warning (disable:4996)
//######################################################################################################################################
const double pi = 3.1415926;
//######################################################################################################################################
// the class containing x,y coordinates, angle of each point, and the minimal angle for observation of other points
class CAsteroidPoint
{
public:
	int x;
	int y;
	double angle;
	double dMinObservationAngle;

	CAsteroidPoint()
	{
		Reset();
	}

	void Reset()
	{
		x = 0;
		y = 0;
		angle = 0.0;
		dMinObservationAngle = 0.0;
	}

	void FindAngle()
	{
		FindAnglePrivate(angle, x, y);
	}

	CAsteroidPoint(const CAsteroidPoint& source)
	{
		x = source.x;
		y = source.y;
		angle = source.angle;
		dMinObservationAngle = source.dMinObservationAngle;
	}

	CAsteroidPoint& operator = (const CAsteroidPoint& source)
	{
		x = source.x;
		y = source.y;
		angle = source.angle;
		dMinObservationAngle = source.dMinObservationAngle;
		return *this;
	}

    bool operator < (const CAsteroidPoint& compare) // for sorting in the array (vector)
	{
		if (angle < compare.angle)
			return true;
		else
			return false;
    }
	
private:

	void FindAnglePrivate(double& angle, const int& x, const int& y)
	{
		double dX, dY;

		if ((x == 0) && (y == 0))
			angle = 0;
		else
			if ((x == 0) && (y > 0))
				angle = pi / 2;
			else
				if ((x == 0) && (y < 0))
					angle = 3 * pi / 2;
				else
					if ((x > 0) && (y == 0))
						angle = 0;
					else
						if ((x < 0) && (y == 0))
							angle = pi;
						else
						{
							dX = abs(x);
							dY = abs(y);
							angle = atan(dY / dX);

							if ((x < 0) && (y > 0))
								angle = pi-angle;
							else
								if ((x < 0) && (y < 0))
									angle = pi+angle;
								else
									if ((x > 0) && (y < 0))
										angle = 2 * pi - angle;
						}
	} // void FindAnglePrivate(double& angle, const int& x, const int& y)
};
//###################################################################################################################################
bool sortingFunction(CAsteroidPoint PA, CAsteroidPoint PB) // to sort our objects by angle
{
	return (PA < PB);
}
//######################################################################################################################################
double AngleBetweenVectors(CAsteroidPoint ap1, CAsteroidPoint ap2)
{
	double dRes;

	if (ap2.angle < ap1.angle)
		ap2.angle = ap2.angle + 2 * pi; // when we cross zero (X axis)

	dRes = ap2.angle - ap1.angle;

	return dRes;
}
//######################################################################################################################################
vector<CAsteroidPoint> vInput;
vector<CAsteroidPoint> vCoords;
//######################################################################################################################################
CAsteroidPoint FindIdealAsteroid(vector<CAsteroidPoint>& vInput)
{
	CAsteroidPoint ap, apIdeal;
	int i, j, k, l, iMin;
	double dMinAngle, dMinAngleCurrent, dFinalMinimum;

	if ((int)vInput.size() == 0)
		return apIdeal;

	// First, we make a vector with shifted coordinates for each input point:

	for (i = 0; i < (int)vInput.size(); ++i)
	{
				vCoords.clear();
				for (j = 0; j < (int)vInput.size(); ++j)
				{
						if (i != j)
						{
							ap.Reset();
							ap.x = vInput[j].x - vInput[i].x; // now we consider vInput[i].x and vInput[i].y as 0,0
							ap.y = vInput[j].y - vInput[i].y;
							ap.FindAngle();
							vCoords.push_back(ap);
						} // if (i != j)
				} // for (j = 0; j < (int)vInput.size(); ++j)

				if((int)vCoords.size()==0)
				    return apIdeal;

				// Now vCoords represents the shifted coordinates of any other points but vInput[i],
				// in relation to the coordinates of vInput[i] point as coordinates center (0,0).

				sort(vCoords.begin(), vCoords.end(), sortingFunction); // sorting by azimuth, i.e. angle

				l = (int)vCoords.size() - 1;
				dMinAngle = AngleBetweenVectors(vCoords[0], vCoords[l]);

				for (k = 1; k < (int)vCoords.size(); ++k)
				{
					++l;
					if (l >= (int)vCoords.size())
						l = 0;

					dMinAngleCurrent = AngleBetweenVectors(vCoords[k], vCoords[l]);
					if (dMinAngleCurrent < dMinAngle)
						dMinAngle = dMinAngleCurrent;
				} // for (k = 0; k < (int)vCoords.size(); ++k)

				vInput[i].dMinObservationAngle = dMinAngle;
		
	} // for (i = 0; i < (int)vInput.size(); ++i)
		
	// Now we calculate the final minimum:

	dFinalMinimum = vInput[0].dMinObservationAngle;
	iMin = 0;

	for (i = 1; i < (int)vInput.size(); ++i)
	{
		if (dFinalMinimum > vInput[i].dMinObservationAngle)
		{
			dFinalMinimum = vInput[i].dMinObservationAngle;
			iMin = i;
		}
	}

	apIdeal = vInput[iMin];

	return apIdeal;
}
//######################################################################################################################################
int main(int argc, char* argv[], char* envp[])
{
	string sFileName;
	int x, y, iScanRes, iRecordsNumber;
	CAsteroidPoint ap;
	FILE* pf;

	if (argc >= 1)
	{
		sFileName = argv[1];
	}
	else
		sFileName = "";

//......................................................................................................................................
	pf = fopen(sFileName.c_str(), "r");
	if (!pf)
	{
		cout << "Cannot open the input file: " << sFileName << " !\n";
		cout << "Press any key...\n";
		getchar();
		return 0;
	}
//.......................................................................................................................................
	iScanRes = fscanf(pf, "%d", &iRecordsNumber);
	if (iScanRes != 1)
	{
		cout << "Wrong information in input file: " << sFileName << " !\n";
		cout << "Press any key...\n";
		fclose(pf);
		getchar();
		return 0;
	}

	//Read the file:
	vInput.clear();
	while (1)
	{
		iScanRes = fscanf(pf, "%d %d", &x, &y);

		if (iScanRes == EOF)
			break;

		if (iScanRes != 2)
		{
			cout << "Wrong information in input file: " << sFileName << " !\n";
			cout << "Press any key...\n";
			fclose(pf);
			getchar();
			return 0;
		}
        
		ap.Reset();
		ap.x = x;
		ap.y = y;

		vInput.push_back(ap);

	} // while(1)

	fclose(pf);
	ap = FindIdealAsteroid(vInput);

	cout << "Ideal asteroid has coordinates:\n";
	cout << ap.x << " " << ap.y << "\n";
	cout << "Press any key...\n";

	vCoords.clear();
	vInput.clear();

	getchar();
	return 0;
}
//#######################################################################################################################################


