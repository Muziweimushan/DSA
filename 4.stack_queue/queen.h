#pragma once

#ifndef __QUEEN_H__
#define __QUEEN_H__

class Queen
{
public:
	Queen(int x = 0, int y = 0)
	{
		m_x = x;
		m_y = y;	
	}

	bool operator == (const Queen &obj) const
	{
		return ((this->m_x == obj.m_x)	/*行相等*/
			|| (this->m_y == obj.m_y) /*列相等*/
			|| (this->m_y - obj.m_y == this->m_x - obj.m_x)	/*正对角线*/
			|| (this->m_y - obj.m_y == obj.m_x - this->m_x));	/*反对角线*/
	}

	bool operator != (const Queen &obj) const
	{
		return !(*this == obj);
	}

	~Queen() {}

private:
	int m_x;	/*行*/
	int m_y;	/*列*/

};


#endif
