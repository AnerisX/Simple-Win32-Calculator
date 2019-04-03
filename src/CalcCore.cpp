#include "stdafx.h"
#include "CalcCore.h"
#include <string>
#include <math.h>


void CalcCore::SetOperands(const float& num)
{
	numbers.push(num);
}

void CalcCore::SetOperators(const Oper& other)
{
	if (operates.size() < numbers.size())
	{
		if (operates.empty() && other.op != equate)
		{
			operates.push(other);
		}
		else
		{
			bool prec = false;
			while (!prec && !operates.empty())
			{
				if (operates.top().Precedence(other))
				{
					operates.push(other);
					prec = true;
				}
				else
				{
					float right = numbers.top();
					numbers.pop();
					float left = numbers.top();
					numbers.pop();

					float result = operates.top().operate(left, right);
					numbers.push(result);
					operates.pop();
				}
			}
			if (other.op != equate && !prec)
			{
				operates.push(other);
			}
		}
	}
}

void CalcCore::clear()
{
	while (numbers.size() != 0)
	{
		numbers.pop();
	}
	while (operates.size() != 0)
	{
		operates.pop();
	}
}

bool CalcCore::CheckOperators()
{
	bool success = true;
	if (operates.size() >= numbers.size() && numbers.size() != 0)
		success = false;
	return success;
}

std::string CalcCore::GetString()
{
	double number, temp;
	number = numbers.top();
	std::string ret;
	ret = std::to_string(number);

	while (ret.back() == '0')
	{
		ret.pop_back();
	}

	if (ret.back() == '.')
	{
		ret.pop_back();
	}
	return ret;
}

bool Oper::Precedence(const Oper& other)
{
	bool preceeds = false;
	if (this->op < 3 && other.op < 3)
		preceeds = false;
	else if (this->op > 3 && other.op > 3)
		preceeds = false;
	else if (this->op > 3 && other.op < 3)
		preceeds = false;
	else if (this->op < 3 && other.op > 3)
		preceeds = true;

	return preceeds;
}

float Oper::operate(const float& lop, const float& rop)
{
	float ret;
	switch (op)
	{
	case add:
		ret = addition(lop, rop);
		break;
	case subtract:
		ret = subtraction(lop, rop);
		break;
	case multiply:
		ret = multiplication(lop, rop);
		break;
	case divide:
		if (rop != 0)
		{
			ret = division(lop, rop);
		}
		else
		{
			ret = 0;
			MessageBox(hWnd, L"Error: Dividing by Zero. Zero was returned.", L"Operation Error", MB_OK);
		}
		break;
	}
	return ret;
}