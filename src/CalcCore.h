#ifndef CALCCORE_H
#define CALCCORE_H
#include <stack>


enum Prec
{
	equate = 0,
	add = 1,
	subtract = 2,
	multiply = 4, 
	divide = 5
};

struct Oper
{
	~Oper()
	{
		hWnd = nullptr;
	}
	Prec op;
	HWND hWnd = nullptr;
	bool Precedence(const Oper& other);
	bool divisorError = false;
	//lop is the left hand operand, rop is the right hand operand
	float addition(float lop, float rop) { return lop + rop; }
	float subtraction(float lop, float rop) { return lop - rop; }
	float multiplication(float lop, float rop) { return lop * rop; }
	float division(float lop, float rop) { return lop / rop; }
	float operate(const float& lop, const float& rop);
	void SetWindow(HWND& hw) { hWnd = hw; }
};

class CalcCore
{
public:
	CalcCore() = default;
	void SetOperands(const float& num);
	void SetOperators(const Oper& other);
	void clear();
	bool CheckOperators();
	std::string GetString();
private:
	std::stack<float> numbers;
	std::stack<Oper> operates;
};

#endif