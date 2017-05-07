/*Author: Aidan Jagger | 130281034
* Class Description:
* Left in some terrain generation code from part of my dissertation... accidentally imported it and not sure if I'll want to use any of it
*/

#pragma once
class HMCoordinate
{
public:
	int x, y;

	HMCoordinate(const int x, const int y) {
		this->x = x;
		this->y = y;
	};
	~HMCoordinate();
};

